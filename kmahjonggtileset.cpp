/*
    Copyright (C) 1997 Mathias Mueller   <in5y158@public.uni-hamburg.de>
    Copyright (C) 2006 Mauricio Piacentini  <mauricio@tabuleiro.com>

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

#include "kmahjonggtileset.h"

#include <klocale.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <qimage.h>
#include <kstandarddirs.h>
#include <ksvgrenderer.h>
#include <QPainter>
#include <QPixmapCache>
#include <QFile>
#include <KDebug>
#include <QMap>

#include <stdlib.h>

class KMahjonggTilesetMetricsData
{
public:
    short lvloffx;   // used for 3D indentation, x value
    short lvloffy;   // used for 3D indentation, y value
    short w;    // tile width ( +border +shadow)
    short h;    // tile height ( +border +shadow)
    short fw;   // face width
    short fh;   // face height

    KMahjonggTilesetMetricsData()
     : lvloffx(0), lvloffy(0), w(0), h(0), fw(0), fh(0)
    {}
};

class KMahjonggTilesetPrivate
{
public:
    KMahjonggTilesetPrivate() : isSVG(false), graphicsLoaded(false) {}
    QList<QString> elementIdTable;
    QMap<QString, QString> authorproperties;

    KMahjonggTilesetMetricsData originaldata;
    KMahjonggTilesetMetricsData scaleddata;
    QString filename;  // cache the last file loaded to save reloading it
    QString graphicspath;

    KSvgRenderer svg;
    bool isSVG;
    bool graphicsLoaded;
};

// ---------------------------------------------------------

KMahjonggTileset::KMahjonggTileset()
    : d(new KMahjonggTilesetPrivate)
{
    buildElementIdTable();

    static bool _inited = false;
    if (_inited)
        return;
    KGlobal::dirs()->addResourceType("kmahjonggtileset", "data", QString::fromLatin1("kmahjongglib/tilesets/"));

    KGlobal::locale()->insertCatalog("libkmahjongglib");
    _inited = true;
}

// ---------------------------------------------------------

KMahjonggTileset::~KMahjonggTileset() {
    delete d;
}

void KMahjonggTileset::updateScaleInfo(short tilew, short tileh) 
{
    d->scaleddata.w = tilew;
    d->scaleddata.h = tileh;
    double ratio = ((qreal) d->scaleddata.w) / ((qreal) d->originaldata.w);
    d->scaleddata.lvloffx = (short) (d->originaldata.lvloffx * ratio);
    d->scaleddata.lvloffy = (short) (d->originaldata.lvloffy * ratio);
    d->scaleddata.fw = (short) (d->originaldata.fw * ratio);
    d->scaleddata.fh = (short) (d->originaldata.fh * ratio);
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

    if ((fullw/fullh)>(bw/bh)) {
        //space will be left on height, use width as limit
	aspectratio = bw/fullw;
    } else {
	aspectratio = bh/fullh;
    }
    newtilew = aspectratio * floatw;
    newtileh = aspectratio * floath;
    return QSize((short)newtilew, (short)newtileh);
}

bool KMahjonggTileset::loadDefault()
{
    QString idx = "default.desktop";

    QString tilesetPath = KStandardDirs::locate("kmahjonggtileset", idx);
kDebug() << "Inside LoadDefault(), located path at" << tilesetPath;
    if (tilesetPath.isEmpty()) {
		return false;
    }
    return loadTileset(tilesetPath);
}

QString KMahjonggTileset::authorProperty(const QString &key) const
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
    return (short) (d->scaleddata.fw / 2.0);
}

short KMahjonggTileset::qHeight() const
{
    return (short) (d->scaleddata.fh / 2.0);
}

QString KMahjonggTileset::path() const
{
    return d->filename;
}

#define kTilesetVersionFormat 1

// ---------------------------------------------------------
bool KMahjonggTileset::loadTileset( const QString & tilesetPath)
{

    QImage qiTiles;
    kDebug() << "Attempting to load .desktop at" << tilesetPath;

    //clear our properties map
    d->authorproperties.clear();

    // verify if it is a valid file first and if we can open it
    QFile tilesetfile(tilesetPath);
    if (!tilesetfile.open(QIODevice::ReadOnly)) {
      return (false);
    }
    tilesetfile.close();

    KConfig tileconfig(tilesetPath, KConfig::SimpleConfig);
    KConfigGroup group = tileconfig.group("KMahjonggTileset");

    d->authorproperties.insert("Name", group.readEntry("Name"));// Returns translated data
    d->authorproperties.insert("Author", group.readEntry("Author"));
    d->authorproperties.insert("Description", group.readEntry("Description"));
    d->authorproperties.insert("AuthorEmail", group.readEntry("AuthorEmail"));

    //Version control
    int tileversion = group.readEntry("VersionFormat",0);
    //Format is increased when we have incompatible changes, meaning that older clients are not able to use the remaining information safely
    if (tileversion > kTilesetVersionFormat) {
        return false;
    }

    QString graphName = group.readEntry("FileName");

    d->graphicspath = KStandardDirs::locate("kmahjonggtileset", graphName);
    kDebug() << "Using tileset at" << d->graphicspath;
    //d->filename = graphicsPath;

    //only SVG for now
    d->isSVG = true;
    if (d->graphicspath.isEmpty()) return (false);

    d->originaldata.w = group.readEntry("TileWidth", 30);
    d->originaldata.h = group.readEntry("TileHeight", 50);
    d->originaldata.fw = group.readEntry("TileFaceWidth", 30);
    d->originaldata.fh = group.readEntry("TileFaceHeight", 50);
    d->originaldata.lvloffx = group.readEntry("LevelOffsetX", 10);
    d->originaldata.lvloffy = group.readEntry("LevelOffsetY", 10);
    
    //client application needs to call loadGraphics()
    d->graphicsLoaded = false;
    d->filename = tilesetPath;

 /*   if (d->isSVG) {
	//really?
	d->svg.load(graphicsPath);
	if (d->svg.isValid()) {
		d->filename = tilesetPath;
		//invalidate our global cache
		QPixmapCache::clear();

		d->isSVG = true;
		reloadTileset(QSize(d->originaldata.w, d->originaldata.h));
	    } else {
	        return( false );
	    }
    } else {
    //TODO add support for png??
	return false;
    }*/

    return( true );
}

// ---------------------------------------------------------
bool KMahjonggTileset::loadGraphics()
{
  if (d->graphicsLoaded == true) return (true) ;
  if (d->isSVG) {
	//really?
    d->svg.load(d->graphicspath);
    if (d->svg.isValid()) {
      //invalidate our global cache
      QPixmapCache::clear();
      d->graphicsLoaded = true;
      reloadTileset(QSize(d->originaldata.w, d->originaldata.h));
    } else {
      return( false );
    }
  } else {
    //TODO add support for png??
    return false;
  }

  return( true );
}

// ---------------------------------------------------------
bool KMahjonggTileset::reloadTileset( const QSize & newTilesize)
{
    QString tilesetPath = d->filename;

    if (QSize(d->scaleddata.w, d->scaleddata.h) == newTilesize) return false;

    if (d->isSVG) {
	if (d->svg.isValid()) {
		updateScaleInfo(newTilesize.width(), newTilesize.height());
		//rendering will be done when needed, automatically using the global cache
	    } else {
	        return( false );
	}
    } else {
    //TODO add support for png???
	return false;
    }

    return( true );
}

void KMahjonggTileset::buildElementIdTable() {
	//Build a list for faster lookup of element ids, mapped to the enumeration used by GameData and BoardWidget
	//Unselected tiles
	for (short idx=1; idx<=4; idx++) {
		d->elementIdTable.append(QString("TILE_%1").arg(idx));
	}
	//Selected tiles
	for (short idx=1; idx<=4; idx++) {
		d->elementIdTable.append(QString("TILE_%1_SEL").arg(idx));
	}
	//now faces
	for (short idx=1; idx<=9; idx++) {
		d->elementIdTable.append(QString("CHARACTER_%1").arg(idx));
	}
	for (short idx=1; idx<=9; idx++) {
		d->elementIdTable.append(QString("BAMBOO_%1").arg(idx));
	}
	for (short idx=1; idx<=9; idx++) {
		d->elementIdTable.append(QString("ROD_%1").arg(idx));
	}
	for (short idx=1; idx<=4; idx++) {
		d->elementIdTable.append(QString("SEASON_%1").arg(idx));
	}
	for (short idx=1; idx<=4; idx++) {
		d->elementIdTable.append(QString("WIND_%1").arg(idx));
	}
	for (short idx=1; idx<=3; idx++) {
		d->elementIdTable.append(QString("DRAGON_%1").arg(idx));
	}
	for (short idx=1; idx<=4; idx++) {
		d->elementIdTable.append(QString("FLOWER_%1").arg(idx));
	}
}

QString KMahjonggTileset::pixmapCacheNameFromElementId(const QString & elementid) {
	return authorProperty("Name")+ elementid + QString("W%1H%2").arg(d->scaleddata.w).arg(d->scaleddata.h);
}

QPixmap KMahjonggTileset::renderElement(short width, short height, const QString & elementid) {
//kDebug() << "render element" << elementid << width << height;
    QImage qiRend(QSize(width, height),QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);

    if (d->svg.isValid()) {
            QPainter p(&qiRend);
	    d->svg.render(&p, elementid);
    }
    return QPixmap::fromImage(qiRend);
}

QPixmap KMahjonggTileset::selectedTile(int num) {
	QPixmap pm;
	QString elemId = d->elementIdTable.at(num+4);//selected offset in our idtable;
 	if (!QPixmapCache::find(pixmapCacheNameFromElementId(elemId), pm)) {
		//use tile size
     		pm = renderElement(d->scaleddata.w, d->scaleddata.h, elemId);
     		QPixmapCache::insert(pixmapCacheNameFromElementId(elemId), pm);
 	}
	return pm;
}

QPixmap KMahjonggTileset::unselectedTile(int num) {
	QPixmap pm;
	QString elemId = d->elementIdTable.at(num);
 	if (!QPixmapCache::find(pixmapCacheNameFromElementId(elemId), pm)) {
		//use tile size
     		pm = renderElement(d->scaleddata.w, d->scaleddata.h, elemId);
     		QPixmapCache::insert(pixmapCacheNameFromElementId(elemId), pm);
 	}
	return pm;
}

QPixmap KMahjonggTileset::tileface(int num) {
	QPixmap pm;
	QString elemId = d->elementIdTable.at(num + 8);//tileface offset in our idtable;
 	if (!QPixmapCache::find(pixmapCacheNameFromElementId(elemId), pm)) {
		//use face size
     		pm = renderElement(d->scaleddata.fw, d->scaleddata.fh, elemId);
     		QPixmapCache::insert(pixmapCacheNameFromElementId(elemId), pm);
 	}
	return pm;
}


