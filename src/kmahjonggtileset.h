/*
    SPDX-FileCopyrightText: 1997 Mathias Mueller <in5y158@public.uni-hamburg.de>
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGTILESET_H
#define KMAHJONGGTILESET_H

// Qt
#include <QString>
#include <QPixmap>
// Std
#include <memory>

// LibKMahjongg
#include <libkmahjongg_export.h>

class KMahjonggTilesetPrivate;

/**
 * @class KMahjonggTileset kmahjonggtileset.h <KMahjonggTileset>
 *
 * A tile set
 */
class LIBKMAHJONGG_EXPORT KMahjonggTileset {
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
    std::unique_ptr<KMahjonggTilesetPrivate> const d;

    Q_DISABLE_COPY(KMahjonggTileset)
};

#endif // KMAHJONGGTILESET_H
