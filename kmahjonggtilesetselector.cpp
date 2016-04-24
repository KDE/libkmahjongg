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

// own
#include "kmahjonggtilesetselector.h"

// Qt
#include <QDir>
#include <QPainter>
#include <QStandardPaths>

// KDE
#include <KLocalizedString>

// LibKMahjongg
#include "kmahjonggtileset.h"

KMahjonggTilesetSelector::KMahjonggTilesetSelector(QWidget * parent, KConfigSkeleton * aconfig)
    : QWidget(parent)
{
    setupUi(this);
    setupData(aconfig);
}

KMahjonggTilesetSelector::~KMahjonggTilesetSelector()
{
    tilesetMap.clear();
}

void KMahjonggTilesetSelector::setupData(KConfigSkeleton * aconfig)
{
    //Get our currently configured Tileset entry
    KConfig * config = aconfig->config();
    KConfigGroup group = config->group("General");
    QString initialGroup = group.readEntry("Tileset_file");

    //The lineEdit widget holds our tileset path, but the user does not manipulate it directly
    kcfg_TileSet->hide();

    //This will also load our resourcedir if it is not done already
    KMahjonggTileset tile;

    //Now get our tilesets into a list
    QStringList tilesAvailable;
    const QStringList dirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "kmahjongglib/tilesets", QStandardPaths::LocateDirectory);
    Q_FOREACH (const QString & dir, dirs) {
        const QStringList fileNames = QDir(dir).entryList(QStringList() << QStringLiteral("*.desktop"));
        Q_FOREACH (const QString & file, fileNames) {
            tilesAvailable.append(dir + '/' + file);
        }
    }

    QLatin1String namestr("Name");
    int numvalidentries = 0;
    for (int i = 0; i < tilesAvailable.size(); ++i) {
        KMahjonggTileset * aset = new KMahjonggTileset();
        QString atileset = tilesAvailable.at(i);
        if (aset->loadTileset(atileset)) {
            tilesetMap.insert(aset->authorProperty(namestr), aset);
            tilesetList->addItem(aset->authorProperty(namestr));
            //Find if this is our currently configured Tileset
            if (atileset == initialGroup) {
                //Select current entry
                tilesetList->setCurrentRow(numvalidentries);
                tilesetChanged();
            }
            ++numvalidentries;
        } else {
            delete aset;
        }
    }

    connect(tilesetList, &QListWidget::currentItemChanged, this, &KMahjonggTilesetSelector::tilesetChanged);
}

void KMahjonggTilesetSelector::tilesetChanged()
{
    KMahjonggTileset * selTileset = tilesetMap.value(tilesetList->currentItem()->text());
    //Sanity checkings. Should not happen.
    if (!selTileset) {
        return;
    }
    if (selTileset->path() == kcfg_TileSet->text()) {
        return;
    }
    QLatin1String authstr("Author");
    QLatin1String contactstr("AuthorEmail");
    QLatin1String descstr("Description");
    kcfg_TileSet->setText(selTileset->path());
    tilesetAuthor->setText(selTileset->authorProperty(authstr));
    tilesetContact->setText(selTileset->authorProperty(contactstr));
    tilesetDescription->setText(selTileset->authorProperty(descstr));

    //Make sure SVG is loaded when graphics is selected
    if (!selTileset->loadGraphics()) {
        return;
    }
    //Let the tileset calculate its ideal size for the preview area, but reduce the margins a bit (pass oversized drawing area)
    QSize tilesize = selTileset->preferredTileSize(tilesetPreview->size() * 1.3, 1, 1);
    selTileset->reloadTileset(tilesize);
    //Draw the preview
    QImage qiRend(tilesetPreview->size(), QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);
    QPainter p(&qiRend);
    //Calculate the margins to center the tile
    QSize margin = tilesetPreview->size() - tilesize;
    //Draw unselected tile and first tileface
    p.drawPixmap(margin.width() / 2, margin.height() / 2, selTileset->unselectedTile(1));
    p.drawPixmap(margin.width() / 2, margin.height() / 2, selTileset->tileface(0));
    p.end();
    tilesetPreview->setPixmap(QPixmap::fromImage(qiRend));
}
