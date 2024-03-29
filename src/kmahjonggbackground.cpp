/*
    SPDX-FileCopyrightText: 1997 Mathias Mueller <in5y158@public.uni-hamburg.de>
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// own
#include "kmahjonggbackground.h"

// Qt
#include <QFile>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QSvgRenderer>
#include <QGuiApplication>

// KF
#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>

// LibKMahjongg
#include "libkmahjongg_debug.h"

class KMahjonggBackgroundPrivate
{
public:
    KMahjonggBackgroundPrivate() = default;

public:
    QString name;
    QString description;
    QString license;
    QString copyrightText;
    QString version;
    QString website;
    QString bugReportUrl;
    QString authorName;
    QString authorEmailAddress;

    QString pixmapCacheNameFromElementId(const QString &elementid, short width, short height);
    QPixmap renderBG(short width, short height);

    QPixmap backgroundPixmap;
    QBrush backgroundBrush;
    QString filename;
    QString graphicspath;
    short w = 1;
    short h = 1;

    QSvgRenderer svg;

    bool graphicsLoaded = false;
    bool isPlain = false;
    bool isTiled = true;
    bool isSVG = false;
};

KMahjonggBackground::KMahjonggBackground()
    : d_ptr(new KMahjonggBackgroundPrivate)
{
}

KMahjonggBackground::~KMahjonggBackground() = default;

bool KMahjonggBackground::loadDefault()
{
    // Set default background here.
    const QString bgPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/backgrounds/egyptian.desktop"));
    qCDebug(LIBKMAHJONGG_LOG) << "Inside LoadDefault(), located background at" << bgPath;
    if (bgPath.isEmpty()) {
        return false;
    }
    return load(bgPath, 0, 0);
}

#define kBGVersionFormat 1

bool KMahjonggBackground::load(const QString &file, short width, short height)
{
    Q_D(KMahjonggBackground);

    // qCDebug(LIBKMAHJONGG_LOG) << "Background loading";
    d->isSVG = false;

    // qCDebug(LIBKMAHJONGG_LOG) << "Attempting to load .desktop at" << file;

    // verify if it is a valid file first and if we can open it
    QFile bgfile(file);
    if (!bgfile.open(QIODevice::ReadOnly)) {
        return false;
    }
    bgfile.close();

    KConfig bgconfig(file, KConfig::SimpleConfig);
    KConfigGroup group = bgconfig.group(QStringLiteral("KMahjonggBackground"));

    d->isPlain = group.readEntry("Plain", 0) != 0;
    d->name = group.readEntry("Name"); // Returns translated data
    d->description = group.readEntry("Description");
    d->license = group.readEntry("License");
    d->copyrightText = group.readEntry("Copyright");
    d->version = group.readEntry("Version");
    d->website = group.readEntry("Website");
    d->bugReportUrl = group.readEntry("BugReportUrl");
    d->authorName = group.readEntry("Author");
    d->authorEmailAddress = group.readEntry("AuthorEmail");

    // Version control
    int bgversion = group.readEntry("VersionFormat", 0);
    // Format is increased when we have incompatible changes, meaning that older clients are not able to use the remaining information safely
    if (bgversion > kBGVersionFormat) {
        return false;
    }

    if (d->isPlain) {
        // qCDebug(LIBKMAHJONGG_LOG) << "Using plain background";
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
    Q_D(KMahjonggBackground);

    if (d->graphicsLoaded || d->isPlain) {
        return true;
    }

    d->svg.load(d->graphicspath);
    if (d->svg.isValid()) {
        d->isSVG = true;
    } else {
        // qCDebug(LIBKMAHJONGG_LOG) << "could not load svg";
        return false;
    }
    return true;
}

void KMahjonggBackground::sizeChanged(int newW, int newH)
{
    Q_D(KMahjonggBackground);

    // in tiled mode we do not care about the whole field size
    if (d->isTiled || d->isPlain) {
        return;
    }

    if (newW == d->w && newH == d->h) {
        return;
    }
    d->w = newW;
    d->h = newH;
}

QString KMahjonggBackgroundPrivate::pixmapCacheNameFromElementId(const QString &elementid, short width, short height)
{
    return name + elementid + QStringLiteral("W%1H%2").arg(width).arg(height);
}

QPixmap KMahjonggBackgroundPrivate::renderBG(short width, short height)
{
    QPixmap qiRend(width, height);
    qiRend.fill(Qt::transparent);

    if (svg.isValid()) {
        QPainter p(&qiRend);
        svg.render(&p);
    }
    return qiRend;
}

QBrush &KMahjonggBackground::getBackground()
{
    Q_D(KMahjonggBackground);

    if (d->isPlain) {
        d->backgroundBrush = QBrush(QPixmap());
    } else {
        const qreal dpr = qApp->devicePixelRatio();
        const short width = d->w * dpr;
        const short height = d->h * dpr;

        // using raw pixmap size with cache id, as the rendering is done dpr-ignorant
        const QString pixmapCacheName = d->pixmapCacheNameFromElementId(d->filename, width, height);
        if (!QPixmapCache::find(pixmapCacheName, &d->backgroundPixmap)) {
            d->backgroundPixmap = d->renderBG(width, height);
            d->backgroundPixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(pixmapCacheName, d->backgroundPixmap);
        }
        d->backgroundBrush = QBrush(d->backgroundPixmap);
    }
    return d->backgroundBrush;
}

QString KMahjonggBackground::path() const
{
    Q_D(const KMahjonggBackground);

    return d->filename;
}

QString KMahjonggBackground::name() const
{
    Q_D(const KMahjonggBackground);

    return d->name;
}

QString KMahjonggBackground::description() const
{
    Q_D(const KMahjonggBackground);

    return d->description;
}

QString KMahjonggBackground::license() const
{
    Q_D(const KMahjonggBackground);

    return d->license;
}

QString KMahjonggBackground::copyrightText() const
{
    Q_D(const KMahjonggBackground);

    return d->copyrightText;
}

QString KMahjonggBackground::version() const
{
    Q_D(const KMahjonggBackground);

    return d->version;
}

QString KMahjonggBackground::website() const
{
    Q_D(const KMahjonggBackground);

    return d->website;
}

QString KMahjonggBackground::bugReportUrl() const
{
    Q_D(const KMahjonggBackground);

    return d->bugReportUrl;
}

QString KMahjonggBackground::authorName() const
{
    Q_D(const KMahjonggBackground);

    return d->authorName;
}

QString KMahjonggBackground::authorEmailAddress() const
{
    Q_D(const KMahjonggBackground);

    return d->authorEmailAddress;
}

bool KMahjonggBackground::isPlain() const
{
    Q_D(const KMahjonggBackground);

    return d->isPlain;
}
