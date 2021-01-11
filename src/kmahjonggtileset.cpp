/*
    SPDX-FileCopyrightText: 1997 Mathias Mueller <in5y158@public.uni-hamburg.de>
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// own
#include "kmahjonggtileset.h"

// STL
#include <cstdlib>

// Qt
#include <QFile>
#include <QImage>
#include <QMap>
#include <QPainter>
#include <QPixmapCache>
#include <QStandardPaths>
#include <QSvgRenderer>

// KF
#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>

// LibKMahjongg
#include "libkmahjongg_debug.h"

class KMahjonggTilesetMetricsData
{
public:
    short lvloffx; // used for 3D indentation, x value
    short lvloffy; // used for 3D indentation, y value
    short w; // tile width ( +border +shadow)
    short h; // tile height ( +border +shadow)
    short fw; // face width
    short fh; // face height

    KMahjonggTilesetMetricsData()
        : lvloffx(0)
        , lvloffy(0)
        , w(0)
        , h(0)
        , fw(0)
        , fh(0)
    {
    }
};

class KMahjonggTilesetPrivate
{
public:
    KMahjonggTilesetPrivate()
        : isSVG(false)
        , graphicsLoaded(false)
    {
    }
    QList<QString> elementIdTable;
    QMap<QString, QString> authorproperties;

    KMahjonggTilesetMetricsData originaldata;
    KMahjonggTilesetMetricsData scaleddata;
    QString filename; // cache the last file loaded to save reloading it
    QString graphicspath;

    QSvgRenderer svg;
    bool isSVG;
    bool graphicsLoaded;
};

// ---------------------------------------------------------

KMahjonggTileset::KMahjonggTileset()
    : d(new KMahjonggTilesetPrivate)
{
    buildElementIdTable();

    static bool _inited = false;
    if (_inited) {
        return;
    }
    _inited = true;
}

// ---------------------------------------------------------

KMahjonggTileset::~KMahjonggTileset() = default;

void KMahjonggTileset::updateScaleInfo(short tilew, short tileh)
{
    d->scaleddata.w = tilew;
    d->scaleddata.h = tileh;
    double ratio = (static_cast<qreal>(d->scaleddata.w)) / (static_cast<qreal>(d->originaldata.w));
    d->scaleddata.lvloffx = static_cast<short>(d->originaldata.lvloffx * ratio);
    d->scaleddata.lvloffy = static_cast<short>(d->originaldata.lvloffy * ratio);
    d->scaleddata.fw = static_cast<short>(d->originaldata.fw * ratio);
    d->scaleddata.fh = static_cast<short>(d->originaldata.fh * ratio);
}

QSize KMahjonggTileset::preferredTileSize(const QSize & boardsize, int horizontalCells, int verticalCells)
{
    //calculate our best tile size to fit the boardsize passed to us
    qreal newtilew, newtileh, aspectratio;
    qreal bw = boardsize.width();
    qreal bh = boardsize.height();

    //use tileface for calculation, with one complete tile in the sum for extra margin
    qreal fullh = (d->originaldata.fh * verticalCells) + d->originaldata.h;
    qreal fullw = (d->originaldata.fw * horizontalCells) + d->originaldata.w;
    qreal floatw = d->originaldata.w;
    qreal floath = d->originaldata.h;

    if ((fullw / fullh) > (bw / bh)) {
        //space will be left on height, use width as limit
        aspectratio = bw / fullw;
    } else {
        aspectratio = bh / fullh;
    }
    newtilew = aspectratio * floatw;
    newtileh = aspectratio * floath;
    return QSize(static_cast<short>(newtilew), static_cast<short>(newtileh));
}

bool KMahjonggTileset::loadDefault()
{
    QString idx = QStringLiteral("default.desktop");

    QString tilesetPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/tilesets/") + idx);
    qCDebug(LIBKMAHJONGG_LOG) << "Inside LoadDefault(), located path at" << tilesetPath;
    if (tilesetPath.isEmpty()) {
        return false;
    }
    return loadTileset(tilesetPath);
}

QString KMahjonggTileset::authorProperty(const QString & key) const
{
    return d->authorproperties[key];
}

short KMahjonggTileset::width() const
{
    return d->scaleddata.w;
}

short KMahjonggTileset::height() const
{
    return d->scaleddata.h;
}

short KMahjonggTileset::levelOffsetX() const
{
    return d->scaleddata.lvloffx;
}

short KMahjonggTileset::levelOffsetY() const
{
    return d->scaleddata.lvloffy;
}

short KMahjonggTileset::qWidth() const
{
    return static_cast<short>(d->scaleddata.fw / 2.0);
}

short KMahjonggTileset::qHeight() const
{
    return static_cast<short>(d->scaleddata.fh / 2.0);
}

QString KMahjonggTileset::path() const
{
    return d->filename;
}

#define kTilesetVersionFormat 1

// ---------------------------------------------------------
bool KMahjonggTileset::loadTileset(const QString & tilesetPath)
{
    //qCDebug(LIBKMAHJONGG_LOG) << "Attempting to load .desktop at" << tilesetPath;

    //clear our properties map
    d->authorproperties.clear();

    // verify if it is a valid file first and if we can open it
    QFile tilesetfile(tilesetPath);
    if (!tilesetfile.open(QIODevice::ReadOnly)) {
        return false;
    }
    tilesetfile.close();

    KConfig tileconfig(tilesetPath, KConfig::SimpleConfig);
    KConfigGroup group = tileconfig.group("KMahjonggTileset");

    d->authorproperties.insert(QStringLiteral("Name"), group.readEntry("Name")); // Returns translated data
    d->authorproperties.insert(QStringLiteral("Author"), group.readEntry("Author"));
    d->authorproperties.insert(QStringLiteral("Description"), group.readEntry("Description"));
    d->authorproperties.insert(QStringLiteral("AuthorEmail"), group.readEntry("AuthorEmail"));

    //Version control
    int tileversion = group.readEntry("VersionFormat", 0);
    //Format is increased when we have incompatible changes, meaning that older clients are not able to use the remaining information safely
    if (tileversion > kTilesetVersionFormat) {
        return false;
    }

    QString graphName = group.readEntry("FileName");

    d->graphicspath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/tilesets/") + graphName);
    //qCDebug(LIBKMAHJONGG_LOG) << "Using tileset at" << d->graphicspath;

    //only SVG for now
    d->isSVG = true;
    if (d->graphicspath.isEmpty()) {
        return false;
    }

    d->originaldata.w = group.readEntry("TileWidth", 30);
    d->originaldata.h = group.readEntry("TileHeight", 50);
    d->originaldata.fw = group.readEntry("TileFaceWidth", 30);
    d->originaldata.fh = group.readEntry("TileFaceHeight", 50);
    d->originaldata.lvloffx = group.readEntry("LevelOffsetX", 10);
    d->originaldata.lvloffy = group.readEntry("LevelOffsetY", 10);

    //client application needs to call loadGraphics()
    d->graphicsLoaded = false;
    d->filename = tilesetPath;

    return true;
}

// ---------------------------------------------------------
bool KMahjonggTileset::loadGraphics()
{
    if (d->graphicsLoaded) {
        return true;
    }
    if (d->isSVG) {
        //really?
        d->svg.load(d->graphicspath);
        if (d->svg.isValid()) {
            //invalidate our global cache
            QPixmapCache::clear();
            d->graphicsLoaded = true;
            reloadTileset(QSize(d->originaldata.w, d->originaldata.h));
        } else {
            return false;
        }
    } else {
        //TODO add support for png??
        return false;
    }

    return true;
}

// ---------------------------------------------------------
bool KMahjonggTileset::reloadTileset(const QSize & newTilesize)
{
    if (QSize(d->scaleddata.w, d->scaleddata.h) == newTilesize) {
        return false;
    }

    if (d->isSVG) {
        if (d->svg.isValid()) {
            updateScaleInfo(newTilesize.width(), newTilesize.height());
            //rendering will be done when needed, automatically using the global cache
        } else {
            return false;
        }
    } else {
        //TODO add support for png???
        return false;
    }

    return true;
}

void KMahjonggTileset::buildElementIdTable()
{
    //Build a list for faster lookup of element ids, mapped to the enumeration used by GameData and BoardWidget
    //Unselected tiles
    for (short idx = 1; idx <= 4; idx++) {
        d->elementIdTable.append(QStringLiteral("TILE_%1").arg(idx));
    }
    //Selected tiles
    for (short idx = 1; idx <= 4; idx++) {
        d->elementIdTable.append(QStringLiteral("TILE_%1_SEL").arg(idx));
    }
    //now faces
    for (short idx = 1; idx <= 9; idx++) {
        d->elementIdTable.append(QStringLiteral("CHARACTER_%1").arg(idx));
    }
    for (short idx = 1; idx <= 9; idx++) {
        d->elementIdTable.append(QStringLiteral("BAMBOO_%1").arg(idx));
    }
    for (short idx = 1; idx <= 9; idx++) {
        d->elementIdTable.append(QStringLiteral("ROD_%1").arg(idx));
    }
    for (short idx = 1; idx <= 4; idx++) {
        d->elementIdTable.append(QStringLiteral("SEASON_%1").arg(idx));
    }
    for (short idx = 1; idx <= 4; idx++) {
        d->elementIdTable.append(QStringLiteral("WIND_%1").arg(idx));
    }
    for (short idx = 1; idx <= 3; idx++) {
        d->elementIdTable.append(QStringLiteral("DRAGON_%1").arg(idx));
    }
    for (short idx = 1; idx <= 4; idx++) {
        d->elementIdTable.append(QStringLiteral("FLOWER_%1").arg(idx));
    }
}

QString KMahjonggTileset::pixmapCacheNameFromElementId(const QString & elementid)
{
    return authorProperty(QStringLiteral("Name")) + elementid + QStringLiteral("W%1H%2").arg(d->scaleddata.w).arg(d->scaleddata.h);
}

QPixmap KMahjonggTileset::renderElement(short width, short height, const QString & elementid)
{
    //qCDebug(LIBKMAHJONGG_LOG) << "render element" << elementid << width << height;
    QImage qiRend(QSize(width, height), QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);

    if (d->svg.isValid()) {
        QPainter p(&qiRend);
        d->svg.render(&p, elementid);
    }
    return QPixmap::fromImage(qiRend);
}

QPixmap KMahjonggTileset::selectedTile(int num)
{
    QPixmap pm;
    QString elemId = d->elementIdTable.at(num + 4); //selected offset in our idtable;
    if (!QPixmapCache::find(pixmapCacheNameFromElementId(elemId), &pm)) {
        //use tile size
        pm = renderElement(d->scaleddata.w, d->scaleddata.h, elemId);
        QPixmapCache::insert(pixmapCacheNameFromElementId(elemId), pm);
    }
    return pm;
}

QPixmap KMahjonggTileset::unselectedTile(int num)
{
    QPixmap pm;
    QString elemId = d->elementIdTable.at(num);
    if (!QPixmapCache::find(pixmapCacheNameFromElementId(elemId), &pm)) {
        //use tile size
        pm = renderElement(d->scaleddata.w, d->scaleddata.h, elemId);
        QPixmapCache::insert(pixmapCacheNameFromElementId(elemId), pm);
    }
    return pm;
}

QPixmap KMahjonggTileset::tileface(int num)
{
    QPixmap pm;
    if ((num + 8) >= d->elementIdTable.count()) {
        //qCDebug(LIBKMAHJONGG_LOG) << "Client asked for invalid tileface id";
        return pm;
    }

    QString elemId = d->elementIdTable.at(num + 8); //tileface offset in our idtable;
    if (!QPixmapCache::find(pixmapCacheNameFromElementId(elemId), &pm)) {
        //use face size
        pm = renderElement(d->scaleddata.fw, d->scaleddata.fh, elemId);
        QPixmapCache::insert(pixmapCacheNameFromElementId(elemId), pm);
    }
    return pm;
}
