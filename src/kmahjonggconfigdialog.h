/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGCONFIGDIALOG_H
#define KMAHJONGGCONFIGDIALOG_H

// Std
#include <memory>

// Qt
#include <QtClassHelperMacros> // Q_DECLARE_PRIVATE

// KF
#include <KConfigDialog>
#include <KConfigSkeleton>

// LibKMahjongg
#include <libkmahjongg_export.h>

class KMahjonggConfigDialogPrivate;

/**
 * @class KMahjonggConfigDialog kmahjonggconfigdialog.h <KMahjonggConfigDialog>
 *
 * An extended config dialog with convenience methods to add pages for managing tilesets & backgrounds
 */
class LIBKMAHJONGG_EXPORT KMahjonggConfigDialog : public KConfigDialog
{
    Q_OBJECT

public:
    KMahjonggConfigDialog(QWidget *parent, const QString &name, KConfigSkeleton *config);
    ~KMahjonggConfigDialog() override;

    void addTilesetPage();
    void addBackgroundPage();

protected Q_SLOTS:
    void updateWidgetsDefault() override;

private:
    friend class KMahjonggConfigDialogPrivate;
    std::unique_ptr<KMahjonggConfigDialogPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KMahjonggConfigDialog)
    Q_DISABLE_COPY(KMahjonggConfigDialog)
};

#endif // KMAHJONGGCONFIGDIALOG_H
