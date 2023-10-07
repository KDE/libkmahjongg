/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGTILESETSELECTOR_H
#define KMAHJONGGTILESETSELECTOR_H

// Qt
#include <QMap>

// KF
#include <KConfigSkeleton>

// LibKMahjongg
#include "ui_kmahjonggtilesetselector.h"

class KMahjonggTileset;

class KMahjonggTilesetSelector : public QWidget, private Ui::KMahjonggTilesetSelector
{
    Q_OBJECT

public:
    explicit KMahjonggTilesetSelector(QWidget *parent, KConfigSkeleton *aconfig);
    ~KMahjonggTilesetSelector() override;

    void setupData(KConfigSkeleton *aconfig);

public Q_SLOTS:
    void tilesetChanged();

private:
    QMap<QString, KMahjonggTileset *> tilesetMap;
};

#endif // KMAHJONGGTILESETSELECTOR_H
