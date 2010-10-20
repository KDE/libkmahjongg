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

#ifndef _KMAHJONGGTILESET_H_
#define _KMAHJONGGTILESET_H_

#include <QtCore/QString>
#include <QtGui/QPixmap>

#include <libkmahjongg_export.h>

class KMahjonggTilesetPrivate;

class KMAHJONGGLIB_EXPORT KMahjonggTileset {
  public:
    KMahjonggTileset();
    ~KMahjonggTileset();

    bool loadDefault();
    bool loadTileset(const QString & tilesetPath);
    bool loadGraphics();
    bool reloadTileset(const QSize & newTilesize);
    QSize preferredTileSize(const QSize & boardsize, int horizontalCells, int verticalCells);
    QString authorProperty(const QString &key) const;

    short width() const;
    short height() const;
    short levelOffsetX() const;
    short levelOffsetY() const;
    short qWidth() const;
    short qHeight() const;
    QString path() const;

    QPixmap selectedTile(int num);
    QPixmap unselectedTile(int num);
    QPixmap tileface(int num);

  protected:
    void updateScaleInfo(short tilew, short tileh);
    void buildElementIdTable(void);
    QString pixmapCacheNameFromElementId(const QString & elementid);
    QPixmap renderElement(short width, short height, const QString & elementid);


  private:
    friend class KMahjonggTilesetPrivate;
    KMahjonggTilesetPrivate *const d;

    Q_DISABLE_COPY(KMahjonggTileset)
};

#endif
