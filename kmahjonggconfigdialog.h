/*
    Copyright (C) 2006 Mauricio Piacentini  <mauricio@tabuleiro.com>

    Libkmahjongg is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __LIBKMAHJONGGCONFIGDIALOG_H_
#define __LIBKMAHJONGGCONFIGDIALOG_H_

#include <kconfigdialog.h>
#include <libkmahjongg_export.h>

#include <KDE/KConfigSkeleton>

class KMahjonggConfigDialogPrivate;

class KMAHJONGGLIB_EXPORT KMahjonggConfigDialog : public KConfigDialog
{
Q_OBJECT
  public:
    KMahjonggConfigDialog( QWidget *parent, const QString& name,
                 KConfigSkeleton *config);
    ~KMahjonggConfigDialog();
    void addTilesetPage();
    void addBackgroundPage();

  protected Q_SLOTS:
    void updateWidgetsDefault();
    //void updateWidgets();
    //void updateSettings();

  private:
    friend class KMahjonggConfigDialogPrivate;
    KMahjonggConfigDialogPrivate *const d;

    Q_DISABLE_COPY(KMahjonggConfigDialog)
};

#endif
