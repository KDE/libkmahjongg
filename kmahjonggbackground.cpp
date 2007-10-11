/*
    Copyright (C) 1997 Mathias Mueller   <in5y158@public.uni-hamburg.de>
    Copyright (C) 2006 Mauricio Piacentini   <mauricio@tabuleiro.com>

    Libkmahjongg is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "kmahjonggbackground.h"

#include <kstandarddirs.h>
#include <klocale.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <ksvgrenderer.h>
#include <QImage>
#include <QFile>
#include <QMap>
#include <QPixmap>
#include <QPixmapCache>
#include <QPainter>
#include <KDebug>

class KMahjonggBackgroundPrivate
{
public:
    KMahjonggBackgroundPrivate()
        : w(1), h(1), graphicsLoaded(false), isTiled(true), isSVG(false)
    {
    }

    QMap<QString, QString> authorproperties;
    QString pixmapCacheNameFromElementId(const QString &elementid);
    QPixmap renderBG(short width, short height);

    QPixmap backgroundPixmap;
    QBrush backgroundBrush;
    QString filename;
    QString graphicspath;
    short w;
    short h;

    KSvgRenderer svg;

    bool graphicsLoaded;
    bool isTiled;
    bool isSVG;
};

KMahjonggBackground::KMahjonggBackground()
    : d(new KMahjonggBackgroundPrivate)
{
    static bool _inited = false;
    if (_inited)
        return;
    KGlobal::dirs()->addResourceType("kmahjonggbackground", "data", QString::fromLatin1("kmahjongglib/backgrounds/"));

    KGlobal::locale()->insertCatalog("libkmahjongglib");
    _inited = true;
}

KMahjonggBackground::~KMahjonggBackground() {
    delete d;
}

bool KMahjonggBackground::loadDefault()
{
    QString idx = "default.desktop";

    QString bgPath = KStandardDirs::locate("kmahjonggbackground", idx);
kDebug() << "Inside LoadDefault(), located background at" << bgPath;
    if (bgPath.isEmpty()) {
		return false;
    }
    return load(bgPath, 0, 0);
}

#define kBGVersionFormat 1

bool KMahjonggBackground::load(const QString &file, short width, short height) {
kDebug() << "Background loading";
    d->isSVG = false;

    kDebug() << "Attempting to load .desktop at" << file;

    // verify if it is a valid file first and if we can open it
    QFile bgfile(file);
    if (!bgfile.open(QIODevice::ReadOnly)) {
      return (false);
    }
    bgfile.close();

    KConfig bgconfig(file, KConfig::OnlyLocal);
    KConfigGroup group = bgconfig.group("KMahjonggBackground");

    d->authorproperties.insert("Name", group.readEntry("Name"));// Returns translated data
    d->authorproperties.insert("Author", group.readEntry("Author"));
    d->authorproperties.insert("Description", group.readEntry("Description"));
    d->authorproperties.insert("AuthorEmail", group.readEntry("AuthorEmail"));

    //Version control
    int bgversion = group.readEntry("VersionFormat",0);
    //Format is increased when we have incompatible changes, meaning that older clients are not able to use the remaining information safely
    if (bgversion > kBGVersionFormat) {
        return false;
    }

    QString graphName = group.readEntry("FileName");

    d->graphicspath = KStandardDirs::locate("kmahjonggbackground", graphName);
    kDebug() << "Using background at" << d->graphicspath;

    if (d->graphicspath.isEmpty()) return (false);

    if (group.readEntry("Tiled",0))
    {
        d->w = group.readEntry("Width",0);
        d->h = group.readEntry("Height",0);
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

bool KMahjonggBackground::loadGraphics() {
  if (d->graphicsLoaded == true) return (true) ;
  
  d->svg.load(d->graphicspath);
  if (d->svg.isValid()) {
    d->isSVG = true;
  } else {
    kDebug() << "could not load svg";
    return( false );
  }
  return (true);
}

void KMahjonggBackground::sizeChanged(int newW, int newH) {
        //in tiled mode we do not care about the whole field size
    if (d->isTiled) return;

    if (newW == d->w && newH == d->h)
		return;
    d->w = newW;
    d->h = newH;
}

QString KMahjonggBackgroundPrivate::pixmapCacheNameFromElementId(const QString &elementid) {
	return elementid+QString("W%1H%2").arg(w).arg(h);
}

QPixmap KMahjonggBackgroundPrivate::renderBG(short width, short height) {
    QImage qiRend(QSize(width, height),QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);

    if (svg.isValid()) {
            QPainter p(&qiRend);
	    svg.render(&p);
    }
    return QPixmap::fromImage(qiRend);
}

QBrush & KMahjonggBackground::getBackground() {
    if (!QPixmapCache::find(d->pixmapCacheNameFromElementId(d->filename), d->backgroundPixmap)) {
        d->backgroundPixmap = d->renderBG(d->w, d->h);
        QPixmapCache::insert(d->pixmapCacheNameFromElementId(d->filename), d->backgroundPixmap);
 	}
    d->backgroundBrush = QBrush(d->backgroundPixmap);
    return d->backgroundBrush;
}

QString KMahjonggBackground::path() const {
    return d->filename;
}

QString KMahjonggBackground::authorProperty(const QString &key) const {
    return d->authorproperties[key];
}
