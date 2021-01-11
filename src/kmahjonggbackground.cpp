/*
    SPDX-FileCopyrightText: 1997 Mathias Mueller <in5y158@public.uni-hamburg.de>
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// own
#include "kmahjonggbackground.h"

// Qt
#include <QFile>
#include <QImage>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QSvgRenderer>

// KF
#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>

// LibKMahjongg
#include "libkmahjongg_debug.h"

class KMahjonggBackgroundPrivate
{
public:
    KMahjonggBackgroundPrivate()
        : w(1)
        , h(1)
        , graphicsLoaded(false)
        , isPlain(false)
        , isTiled(true)
        , isSVG(false)
    {
    }

    QMap<QString, QString> authorproperties;
    QString pixmapCacheNameFromElementId(const QString & elementid);
    QPixmap renderBG(short width, short height);

    QPixmap backgroundPixmap;
    QBrush backgroundBrush;
    QString filename;
    QString graphicspath;
    short w;
    short h;

    QSvgRenderer svg;

    bool graphicsLoaded;
    bool isPlain;
    bool isTiled;
    bool isSVG;
};

KMahjonggBackground::KMahjonggBackground()
    : d(new KMahjonggBackgroundPrivate)
{
    static bool _inited = false;
    if (_inited) {
        return;
    }
    _inited = true;
}

KMahjonggBackground::~KMahjonggBackground() = default;

bool KMahjonggBackground::loadDefault()
{
    // Set default background here.
    QLatin1String idx("egyptian.desktop");

    QString bgPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/backgrounds/") + idx);
    qCDebug(LIBKMAHJONGG_LOG) << "Inside LoadDefault(), located background at" << bgPath;
    if (bgPath.isEmpty()) {
        return false;
    }
    return load(bgPath, 0, 0);
}

#define kBGVersionFormat 1

bool KMahjonggBackground::load(const QString & file, short width, short height)
{
    //qCDebug(LIBKMAHJONGG_LOG) << "Background loading";
    d->isSVG = false;

    //qCDebug(LIBKMAHJONGG_LOG) << "Attempting to load .desktop at" << file;

    // verify if it is a valid file first and if we can open it
    QFile bgfile(file);
    if (!bgfile.open(QIODevice::ReadOnly)) {
        return false;
    }
    bgfile.close();

    KConfig bgconfig(file, KConfig::SimpleConfig);
    KConfigGroup group = bgconfig.group("KMahjonggBackground");

    d->authorproperties.insert(QStringLiteral("Name"), group.readEntry("Name")); // Returns translated data
    d->authorproperties.insert(QStringLiteral("Author"), group.readEntry("Author"));
    d->authorproperties.insert(QStringLiteral("Description"), group.readEntry("Description"));
    d->authorproperties.insert(QStringLiteral("AuthorEmail"), group.readEntry("AuthorEmail"));
    //The "Plain" key is set to 1 by the color_plain background.
    d->isPlain = group.readEntry("Plain", 0) != 0;
    d->authorproperties.insert(QStringLiteral("Plain"), d->isPlain ? QStringLiteral("1") : QStringLiteral("0"));

    //Version control
    int bgversion = group.readEntry("VersionFormat", 0);
    //Format is increased when we have incompatible changes, meaning that older clients are not able to use the remaining information safely
    if (bgversion > kBGVersionFormat) {
        return false;
    }

    if (d->isPlain) {
        //qCDebug(LIBKMAHJONGG_LOG) << "Using plain background";
        d->graphicspath.clear();
        d->filename = file;
        return true;
    }

    QString graphName = group.readEntry("FileName");

    d->graphicspath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/backgrounds/") + graphName);

    qCDebug(LIBKMAHJONGG_LOG) << "Using background at" << d->graphicspath;

    if (d->graphicspath.isEmpty()) {
        return false;
    }

    if (group.readEntry("Tiled", 0) != 0) {
        d->w = group.readEntry("Width", 0);
        d->h = group.readEntry("Height", 0);
        d->isTiled = true;
    } else {
        d->w = width;
        d->h = height;
        d->isTiled = false;
    }
    d->graphicsLoaded = false;
    d->filename = file;
    return true;
}

bool KMahjonggBackground::loadGraphics()
{
    if (d->graphicsLoaded || d->isPlain) {
        return true;
    }

    d->svg.load(d->graphicspath);
    if (d->svg.isValid()) {
        d->isSVG = true;
    } else {
        //qCDebug(LIBKMAHJONGG_LOG) << "could not load svg";
        return false;
    }
    return true;
}

void KMahjonggBackground::sizeChanged(int newW, int newH)
{
    //in tiled mode we do not care about the whole field size
    if (d->isTiled || d->isPlain) {
        return;
    }

    if (newW == d->w && newH == d->h) {
        return;
    }
    d->w = newW;
    d->h = newH;
}

QString KMahjonggBackgroundPrivate::pixmapCacheNameFromElementId(const QString & elementid)
{
    return authorproperties[QStringLiteral("Name")] + elementid + QStringLiteral("W%1H%2").arg(w).arg(h);
}

QPixmap KMahjonggBackgroundPrivate::renderBG(short width, short height)
{
    QImage qiRend(QSize(width, height), QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);

    if (svg.isValid()) {
        QPainter p(&qiRend);
        svg.render(&p);
    }
    return QPixmap::fromImage(qiRend);
}

QBrush & KMahjonggBackground::getBackground()
{
    if (d->isPlain) {
        d->backgroundBrush = QBrush(QPixmap());
    } else {
        if (!QPixmapCache::find(d->pixmapCacheNameFromElementId(d->filename), &d->backgroundPixmap)) {
            d->backgroundPixmap = d->renderBG(d->w, d->h);
            QPixmapCache::insert(d->pixmapCacheNameFromElementId(d->filename), d->backgroundPixmap);
        }
        d->backgroundBrush = QBrush(d->backgroundPixmap);
    }
    return d->backgroundBrush;
}

QString KMahjonggBackground::path() const
{
    return d->filename;
}

QString KMahjonggBackground::authorProperty(const QString & key) const
{
    return d->authorproperties[key];
}
