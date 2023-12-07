/*
    SPDX-FileCopyrightText: 1997 Mathias Mueller <in5y158@public.uni-hamburg.de>
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGTILESET_H
#define KMAHJONGGTILESET_H

// Qt
#include <QtClassHelperMacros> // Q_DECLARE_PRIVATE
#include <QPixmap>
#include <QString>
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
class LIBKMAHJONGG_EXPORT KMahjonggTileset
{
public:
    KMahjonggTileset();
    ~KMahjonggTileset();

    bool loadDefault();
    bool loadTileset(const QString &tilesetPath);
    bool loadGraphics();
    bool reloadTileset(QSize newTilesize);
    QSize preferredTileSize(QSize boardsize, int horizontalCells, int verticalCells) const;

    QString name() const;
    QString description() const;
    QString license() const;
    QString copyrightText() const;
    QString version() const;
    QString website() const;
    QString bugReportUrl() const;
    QString authorName() const;
    QString authorEmailAddress() const;

    short width() const;
    short height() const;
    short levelOffsetX() const;
    short levelOffsetY() const;
    short qWidth() const;
    short qHeight() const;
    QString path() const;

    QPixmap selectedTile(int num) const;
    QPixmap unselectedTile(int num) const;
    QPixmap tileface(int num) const;

protected:
    void updateScaleInfo(short tilew, short tileh);
    void buildElementIdTable(void);
    QString pixmapCacheNameFromElementId(const QString &elementid, short width, short height) const;
    QPixmap renderElement(short width, short height, const QString &elementid) const;

private:
    friend class KMahjonggTilesetPrivate;
    std::unique_ptr<KMahjonggTilesetPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KMahjonggTileset)
    Q_DISABLE_COPY(KMahjonggTileset)
};

#endif // KMAHJONGGTILESET_H
