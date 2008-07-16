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

#ifndef __LIB_KMAHJONGGTILESETSELECTOR_H_
#define __LIB_KMAHJONGGTILESETSELECTOR_H_

#include <QMap>
#include <KConfigSkeleton>
#include "ui_kmahjonggtilesetselector.h"

class KMahjonggTileset;

class KMahjonggTilesetSelector : public QWidget, public Ui::KMahjonggTilesetSelector
{
Q_OBJECT
public:
    explicit KMahjonggTilesetSelector( QWidget* parent, KConfigSkeleton * config );
    ~KMahjonggTilesetSelector();
    void setupData(KConfigSkeleton * config);

    QMap<QString, KMahjonggTileset *> tilesetMap;
public slots:
    void tilesetChanged();
};

#endif // __LIB_KMAHJONGGTILESETSELECTOR_H_
