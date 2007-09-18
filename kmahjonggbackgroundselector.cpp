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

#include "kmahjonggbackgroundselector.h"

#include <klocale.h>
#include <kstandarddirs.h>
#include <QPainter>

#include "kmahjonggbackground.h"

KMahjonggBackgroundSelector::KMahjonggBackgroundSelector( QWidget* parent, KConfigSkeleton * aconfig )
        : QWidget( parent )
{
    setupUi(this);
    setupData(aconfig);
}

void KMahjonggBackgroundSelector::setupData(KConfigSkeleton * aconfig)
{
    //Get our currently configured Tileset entry
    KConfig * config = aconfig->config();
    KConfigGroup group = config->group("General");
    QString initialGroup = group.readEntry("Background_file");

    //The lineEdit widget holds our bg path, but the user does not manipulate it directly
    kcfg_Background->hide();

    KMahjonggBackground bg;

    //Now get our tilesets into a list
    QStringList bgsAvailable = KGlobal::dirs()->findAllResources("kmahjonggbackground", QString("*.desktop"), KStandardDirs::Recursive);
    QString namestr("Name");
    int numvalidentries = 0;
    for (int i = 0; i < bgsAvailable.size(); ++i)
    {   
        KMahjonggBackground * abg = new KMahjonggBackground();
        QString bgpath = bgsAvailable.at(i);
        if (abg->load(bgpath,backgroundPreview->width(),backgroundPreview->height())) {
            backgroundMap.insert(abg->authorProperty(namestr), abg);
            backgroundList->addItem(abg->authorProperty(namestr));
            //Find if this is our currently configured Tileset
            if (bgpath==initialGroup) {
                //Select current entry
                backgroundList->setCurrentRow(numvalidentries);
                backgroundChanged();
            }
            ++numvalidentries;
        } else {
            delete abg;
        }
    }
    
    connect(backgroundList, SIGNAL(currentItemChanged ( QListWidgetItem * , QListWidgetItem * )), this, SLOT(backgroundChanged()));
}

void KMahjonggBackgroundSelector::backgroundChanged()
{
    KMahjonggBackground * selBG = backgroundMap.value(backgroundList->currentItem()->text());
        //Sanity checkings. Should not happen.
    if (!selBG) return;
    if (selBG->path()==kcfg_Background->text()) {
        return;
    }
    QString authstr("Author");
    QString contactstr("AuthorEmail");
    QString descstr("Description");
    kcfg_Background->setText(selBG->path());
    backgroundAuthor->setText(selBG->authorProperty(authstr));
    backgroundContact->setText(selBG->authorProperty(contactstr));
    backgroundDescription->setText(selBG->authorProperty(descstr));

    //Make sure SVG is loaded when graphics is selected
    if (!selBG->loadGraphics()) return;

    //Draw the preview
    //TODO here: add code to load and keep proportions for non-tiled content?
    QImage qiRend(backgroundPreview->size(),QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);
    QPainter p(&qiRend);
    p.fillRect(p.viewport(), selBG->getBackground() );
    backgroundPreview->setPixmap(QPixmap::fromImage(qiRend));

}

#include "kmahjonggbackgroundselector.moc"
