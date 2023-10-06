/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGBACKGROUNDSELECTOR_H
#define KMAHJONGGBACKGROUNDSELECTOR_H

// Qt
#include <QMap>

// KF
#include <KConfigSkeleton>

// LibKMahjongg
#include "ui_kmahjonggbackgroundselector.h"

class KMahjonggBackground;

class KMahjonggBackgroundSelector : public QWidget, public Ui::KMahjonggBackgroundSelector
{
    Q_OBJECT

public:
    explicit KMahjonggBackgroundSelector(QWidget *parent, KConfigSkeleton *aconfig);
    ~KMahjonggBackgroundSelector() override;

    void setupData(KConfigSkeleton *aconfig);

public Q_SLOTS:
    void backgroundChanged();

public:
    QMap<QString, KMahjonggBackground *> backgroundMap;
};

#endif // KMAHJONGGBACKGROUNDSELECTOR_H
