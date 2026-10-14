/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGTILESETSELECTOR_H
#define KMAHJONGGTILESETSELECTOR_H

// Qt
#include <QHash>

// KF
#include <KConfigSkeleton>

// LibKMahjongg
#include "ui_kmahjonggtilesetselector.h"

class KMahjonggTileset;

/*!
 * \class KMahjonggTilesetSelector
 * \inmodule LibKMahjongg
 * \brief The KMahjonggTilesetSelector class provides a widget that allows selection of a tileset.
 * 
 * This class allows the selection of a KMahjonggTileset.
 */
class KMahjonggTilesetSelector : public QWidget, private Ui::KMahjonggTilesetSelector
{
    Q_OBJECT

public:
    /*!
     * Constructs a KMahjonggTilesetSelector with the parent \a parent and config \a aconfig.
     */
    explicit KMahjonggTilesetSelector(QWidget *parent, KConfigSkeleton *aconfig);
    ~KMahjonggTilesetSelector() override;

    /*!
     * Sets this widget's config to \a aconfig.
     */
    void setupData(KConfigSkeleton *aconfig);

public Q_SLOTS:
    /*!
     * Sets the current tileset to the currently selected tileset.
     */
    void tilesetChanged();

private:
    QHash<QString, KMahjonggTileset *> tilesetMap;
};

#endif // KMAHJONGGTILESETSELECTOR_H
