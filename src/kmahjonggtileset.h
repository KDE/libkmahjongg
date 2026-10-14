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

/*!
 * \class KMahjonggTileset
 * \inmodule LibKMahjongg
 * \brief KMahjonggTileset represents a tileset.
 *
 * This class contains a tileset and its associated metadata.
 */
class LIBKMAHJONGG_EXPORT KMahjonggTileset
{
public:
    KMahjonggTileset();
    ~KMahjonggTileset();

    /*!
     * Loads the default tileset,
     * QStandardPaths::GenericDataLocation/kmahjongglib/tilesets/default.desktop,
     * and returns whether it was successful.
     */
    bool loadDefault();
    /*!
     * Loads the tileset from the file path \a tilesetPath, and returns whether it was successful.
     */
    bool loadTileset(const QString &tilesetPath);
    /*!
     * Loads the graphics from the configured location unless the graphics were already loaded,
     * and returns whether it was successful.
     */
    bool loadGraphics();
    /*!
     * Sets the tile size to \a newTilesize, and returns whether it was successful.
     * \sa width, height
     */
    bool reloadTileset(QSize newTilesize);
    /*!
     * Returns the best size calculated to fit the board with the size \a boardsize,
     * number of columns \a horizontalCells, and number of rows \a verticalCells.
     */
    QSize preferredTileSize(QSize boardsize, int horizontalCells, int verticalCells) const;

    /*!
     * Returns the name of this tileset.
     */
    QString name() const;
    /*!
     * Returns the description of this tileset.
     */
    QString description() const;
    /*!
     * Returns the license of this tileset.
     */
    QString license() const;
    /*!
     * Returns the copyright text of this tileset.
     */
    QString copyrightText() const;
    /*!
     * Returns the version string of this tileset.
     */
    QString version() const;
    /*!
     * Returns the website of this tileset.
     */
    QString website() const;
    /*!
     * Returns the bug report URL of this tileset.
     */
    QString bugReportUrl() const;
    /*!
     * Returns the name of the author of this tileset.
     */
    QString authorName() const;
    /*!
     * Returns the email address of the author of this tileset.
     */
    QString authorEmailAddress() const;

    /*!
     * Returns the width of a tile.
     * \sa reloadTileset, height, qWidth
     */
    short width() const;
    /*!
     * Returns the height of a tile.
     * \sa reloadTileset, width, qHeight
     */
    short height() const;
    /*!
     * Returns the x offset used for 3D indentation of a tile.
     * \sa levelOffsetY
     */
    short levelOffsetX() const;
    /*!
     * Returns the y offset used for 3D indentation of a tile.
     * \sa levelOffsetX
     */
    short levelOffsetY() const;
    /*!
     * Returns the width of a tile's face / 2.
     * \sa qHeight, width
     */
    short qWidth() const;
    /*!
     * Returns the height of a tile's face / 2.
     * \sa qWidth, height
     */
    short qHeight() const;
    /*!
     * Returns the file path of this tileset.
     */
    QString path() const;

    /*!
     * Returns the selected variant pixmap of the tile with the element ID \a num
     * scaled according to \l width and \l height.
     * \sa unselectedTile, tileface
     */
    QPixmap selectedTile(int num) const;
    /*!
     * Returns the unselected variant pixmap of the tile with the element ID \a num
     * scaled according to \l width and \l height.
     * \sa selectedTile, tileface
     */
    QPixmap unselectedTile(int num) const;
    /*!
     * Returns the pixmap of the tile face with the element ID \a num
     * scaled according to the config's face width and height times the ratio of \l width / \l height.
     * \sa selectedTile, unselectedTile
     */
    QPixmap tileface(int num) const;

private:
    friend class KMahjonggTilesetPrivate;
    std::unique_ptr<KMahjonggTilesetPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KMahjonggTileset)
    Q_DISABLE_COPY(KMahjonggTileset)
};

#endif // KMAHJONGGTILESET_H
