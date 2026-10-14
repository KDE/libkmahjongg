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

/*!
 * \class KMahjonggConfigDialog
 * \inmodule LibKMahjongg
 * \brief The KMahjonggConfigDialog class provides an extended config dialog for managing tilesets & backgrounds.
 *
 * This class is an extended config dialog with convenience methods to add pages for managing tilesets and backgrounds.
 */
class LIBKMAHJONGG_EXPORT KMahjonggConfigDialog : public KConfigDialog
{
    Q_OBJECT

public:
    /*!
     * Constructs a KMahjonggConfigDialog with the parent \a parent, name \a name, and config \a config.
     */
    KMahjonggConfigDialog(QWidget *parent, const QString &name, KConfigSkeleton *config);
    ~KMahjonggConfigDialog() override;

    /*!
     * Adds a KMahjonggTilesetSelector page to this dialog.
     */
    void addTilesetPage();
    /*!
     * Adds a KMahjonggBackgroundSelector page to this dialog.
     */
    void addBackgroundPage();

protected Q_SLOTS:
    // noop
    void updateWidgetsDefault() override;

private:
    friend class KMahjonggConfigDialogPrivate;
    std::unique_ptr<KMahjonggConfigDialogPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KMahjonggConfigDialog)
    Q_DISABLE_COPY(KMahjonggConfigDialog)
};

#endif // KMAHJONGGCONFIGDIALOG_H
