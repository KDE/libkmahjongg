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
#include <kconfigdialogmanager.h>
#include <kstandardguiitem.h>
#include <QDialog>

#include "kmahjonggconfigdialog.h"

#include <klocale.h>
#include <kstandarddirs.h>
#include <krandom.h>
#include <kconfig.h>
#include <kdebug.h>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "kmahjonggtilesetselector.h"
#include "kmahjonggbackgroundselector.h"

class KMahjonggConfigDialogPrivate
{
public:
    KConfigSkeleton * m_config;
};

KMahjonggConfigDialog::KMahjonggConfigDialog( QWidget *parent, const QString& name,
                 KConfigSkeleton *config)
    : KConfigDialog(parent, name, config),
      d(new KMahjonggConfigDialogPrivate)
{
    setFaceType(List);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Help|QDialogButtonBox::Apply);
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    //PORTING SCRIPT: WARNING mainLayout->addWidget(buttonBox) must be last item in layout. Please move it.
    mainLayout->addWidget(buttonBox);
    buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    setModal(true);
    d->m_config = config;
}

KMahjonggConfigDialog::~KMahjonggConfigDialog()
{
    delete d;
}

void KMahjonggConfigDialog::addTilesetPage()
{
    KMahjonggTilesetSelector * ts = new KMahjonggTilesetSelector(this, d->m_config);
    //TODO: Use the cards icon for our page for now, need to get one for tilesets made
    addPage(ts, i18n("Tiles"), QLatin1String( "games-config-tiles" ));
}

void KMahjonggConfigDialog::addBackgroundPage()
{
    KMahjonggBackgroundSelector * ts = new KMahjonggBackgroundSelector(this, d->m_config);
    //TODO: need icon
    addPage(ts, i18n("Background"), QLatin1String( "games-config-background" ));
}

void KMahjonggConfigDialog::updateWidgetsDefault()
{
    kDebug() << "updateWidgetsDefault";
}
/*void KMahjonggConfigDialog::updateWidgets()
{
    kDebug() << "updateWidgets";
}
void KMahjonggConfigDialog::updateSettings()
{
    kDebug() << "updateSettings";
}*/

#include "kmahjonggconfigdialog.moc"
