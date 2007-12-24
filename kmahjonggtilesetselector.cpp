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

#include "kmahjonggtilesetselector.h"

#include <klocale.h>
#include <kstandarddirs.h>
#include <QPainter>

#include "kmahjonggtileset.h"

KMahjonggTilesetSelector::KMahjonggTilesetSelector( QWidget* parent, KConfigSkeleton * aconfig )
        : QWidget( parent )
{
    setupUi(this);
    setupData(aconfig);
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
    QStringList tilesAvailable = KGlobal::dirs()->findAllResources("kmahjonggtileset", QString("*.desktop"), KStandardDirs::Recursive);

    QString namestr("Name");
    int numvalidentries = 0;
    for (int i = 0; i < tilesAvailable.size(); ++i)
    {   
        KMahjonggTileset * aset = new KMahjonggTileset();
        QString atileset = tilesAvailable.at(i);
        if (aset->loadTileset(atileset)) {
            tilesetMap.insert(aset->authorProperty(namestr), aset);
            tilesetList->addItem(aset->authorProperty(namestr));
            //Find if this is our currently configured Tileset
            if (atileset==initialGroup) {
                //Select current entry
                tilesetList->setCurrentRow(numvalidentries);
                tilesetChanged();
            }
            ++numvalidentries;
        } else {
            delete aset;
        }
    }
    
    connect(tilesetList, SIGNAL(currentItemChanged ( QListWidgetItem * , QListWidgetItem * )), this, SLOT(tilesetChanged()));
}

void KMahjonggTilesetSelector::tilesetChanged()
{
    KMahjonggTileset * selTileset = tilesetMap.value(tilesetList->currentItem()->text());
        //Sanity checkings. Should not happen.
    if (!selTileset) return;
    if (selTileset->path()==kcfg_TileSet->text()) {
        return;
    }
    QString authstr("Author");
    QString contactstr("AuthorEmail");
    QString descstr("Description");
    kcfg_TileSet->setText(selTileset->path());
    tilesetAuthor->setText(selTileset->authorProperty(authstr));
    tilesetContact->setText(selTileset->authorProperty(contactstr));
    tilesetDescription->setText(selTileset->authorProperty(descstr));

    //Make sure SVG is loaded when graphics is selected
    if (!selTileset->loadGraphics()) return;
    //Let the tileset calculate its ideal size for the preview area, but reduce the margins a bit (pass oversized drawing area)
    QSize tilesize = selTileset->preferredTileSize(tilesetPreview->size()*1.3, 1, 1);
    selTileset->reloadTileset(tilesize);
    //Draw the preview
    QImage qiRend(tilesetPreview->size(),QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);
    QPainter p(&qiRend);
    //Calculate the margins to center the tile
    QSize margin = tilesetPreview->size() - tilesize;
    //Draw unselected tile and first tileface
    p.drawPixmap(margin.width()/2, margin.height()/2, selTileset->unselectedTile(1));
    p.drawPixmap(margin.width()/2, margin.height()/2, selTileset->tileface(0));
    p.end();
    tilesetPreview->setPixmap(QPixmap::fromImage(qiRend));

}

#include "kmahjonggtilesetselector.moc"
