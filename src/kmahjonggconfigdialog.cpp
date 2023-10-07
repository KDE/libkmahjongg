/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// own
#include "kmahjonggconfigdialog.h"

// KF
#include <KConfig>
#include <KLocalizedString>

// LibKMahjongg
#include "kmahjonggbackgroundselector.h"
#include "kmahjonggtilesetselector.h"
#include "libkmahjongg_debug.h"

class KMahjonggConfigDialogPrivate
{
public:
    explicit KMahjonggConfigDialogPrivate(KConfigSkeleton *config)
        : m_config(config)
    {}

public:
    KConfigSkeleton *m_config;
};

KMahjonggConfigDialog::KMahjonggConfigDialog(QWidget *parent, const QString &name, KConfigSkeleton *config)
    : KConfigDialog(parent, name, config)
    , d_ptr(new KMahjonggConfigDialogPrivate(config))
{
    setFaceType(List);
    setModal(true);
}

KMahjonggConfigDialog::~KMahjonggConfigDialog() = default;

void KMahjonggConfigDialog::addTilesetPage()
{
    Q_D(KMahjonggConfigDialog);

    auto *ts = new KMahjonggTilesetSelector(this, d->m_config);
    // TODO: Use the cards icon for our page for now, need to get one for tilesets made
    addPage(ts, i18n("Tiles"), QStringLiteral("games-config-tiles"));
}

void KMahjonggConfigDialog::addBackgroundPage()
{
    Q_D(KMahjonggConfigDialog);

    auto *ts = new KMahjonggBackgroundSelector(this, d->m_config);
    // TODO: need icon
    addPage(ts, i18n("Background"), QStringLiteral("games-config-background"));
}

void KMahjonggConfigDialog::updateWidgetsDefault()
{
    // qCDebug(LIBKMAHJONGG_LOG) << "updateWidgetsDefault";
}

#include "moc_kmahjonggconfigdialog.cpp"
