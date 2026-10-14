/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGBACKGROUNDSELECTOR_H
#define KMAHJONGGBACKGROUNDSELECTOR_H

// Qt
#include <QHash>

// KF
#include <KConfigSkeleton>

// LibKMahjongg
#include "ui_kmahjonggbackgroundselector.h"

class KMahjonggBackground;

/*!
 * \class KMahjonggBackgroundSelector
 * \inmodule LibKMahjongg
 * \brief The KMahjonggBackgroundSelector class provides a widget that allows selection of a background.
 * 
 * This class allows the selection of a KMahjonggBackground.
 */
class KMahjonggBackgroundSelector : public QWidget, private Ui::KMahjonggBackgroundSelector
{
    Q_OBJECT

public:
    /*!
     * Constructs a KMahjonggBackgroundSelector with the parent \a parent and config \a aconfig.
     */
    explicit KMahjonggBackgroundSelector(QWidget *parent, KConfigSkeleton *aconfig);
    ~KMahjonggBackgroundSelector() override;

    /*!
     * Sets this widget's config to \a aconfig.
     */
    void setupData(KConfigSkeleton *aconfig);

public Q_SLOTS:
    /*!
     * Sets the current background to the currently selected background.
     */
    void backgroundChanged();

private:
    QHash<QString, KMahjonggBackground *> backgroundMap;
};

#endif // KMAHJONGGBACKGROUNDSELECTOR_H
