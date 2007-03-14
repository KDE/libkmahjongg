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

#include <klocale.h>
#include <kstandarddirs.h>
#include <krandom.h>
#include <kconfig.h>

#include "kmahjonggtilesetselector.h"
#include "kmahjonggbackgroundselector.h"
#include "kmahjonggconfigdialog.h"

KMahjonggConfigDialog::KMahjonggConfigDialog( QWidget *parent, const QString& name,
                 KConfigSkeleton *config) : KConfigDialog (parent, name, config, 
            List, Ok|Apply|Cancel|Help, Ok, true)
{
    m_config = config;
}

void KMahjonggConfigDialog::addTilesetPage()
{
  KMahjonggTilesetSelector * ts = new KMahjonggTilesetSelector(this, m_config);
  //TODO: Use the cards icon for our page for now, need to get one for tilesets made
  addPage(ts, i18n("Tiles"), "package_games_card");
}

void KMahjonggConfigDialog::addBackgroundPage()
{
  KMahjonggBackgroundSelector * ts = new KMahjonggBackgroundSelector(this, m_config);
  //TODO: need icon
  addPage(ts, i18n("Background"), "background");
}

void KMahjonggConfigDialog::updateWidgetsDefault()
{
    qDebug() << "updateWidgetsDefault";
}
/*void KMahjonggConfigDialog::updateWidgets()
{
    qDebug() << "updateWidgets";
}
void KMahjonggConfigDialog::updateSettings()
{
    qDebug() << "updateSettings";
}*/

#include "kmahjonggconfigdialog.moc"
