/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// own
#include "kmahjonggbackgroundselector.h"

// Qt
#include <QDir>
#include <QPainter>

// KF
#include <KLocalizedString>

// LibKMahjongg
#include "kmahjonggbackground.h"

KMahjonggBackgroundSelector::KMahjonggBackgroundSelector(QWidget * parent, KConfigSkeleton * aconfig)
    : QWidget(parent)
{
    setupUi(this);
    setupData(aconfig);
}

KMahjonggBackgroundSelector::~KMahjonggBackgroundSelector()
{
    qDeleteAll(backgroundMap);
}

void KMahjonggBackgroundSelector::setupData(KConfigSkeleton * aconfig)
{
    //Get our currently configured background entry
    KConfig * config = aconfig->config();
    KConfigGroup group = config->group("General");
    QString initialGroup = group.readEntry("Background_file");

    //The lineEdit widget holds our bg path, but the user does not manipulate it directly
    kcfg_Background->hide();

    KMahjonggBackground bg;

    //Now get our backgrounds into a list
    QStringList bgsAvailable;
    const QStringList dirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/backgrounds"), QStandardPaths::LocateDirectory);
    for (const QString & dir : dirs) {
        const QStringList fileNames = QDir(dir).entryList(QStringList() << QStringLiteral("*.desktop"));
        for (const QString & file : fileNames) {
            bgsAvailable.append(dir + QLatin1Char('/') + file);
        }
    }

    QLatin1String namestr("Name");
    int numvalidentries = 0;
    for (int i = 0; i < bgsAvailable.size(); ++i) {
        KMahjonggBackground * abg = new KMahjonggBackground();
        QString bgpath = bgsAvailable.at(i);
        if (abg->load(bgpath, backgroundPreview->width(), backgroundPreview->height())) {
            backgroundMap.insert(abg->authorProperty(namestr), abg);
            backgroundList->addItem(abg->authorProperty(namestr));
            //Find if this is our currently configured background
            if (bgpath == initialGroup) {
                //Select current entry
                backgroundList->setCurrentRow(numvalidentries);
                backgroundChanged();
            }
            ++numvalidentries;
        } else {
            delete abg;
        }
    }

    connect(backgroundList, &QListWidget::currentItemChanged, this, &KMahjonggBackgroundSelector::backgroundChanged);
}

void KMahjonggBackgroundSelector::backgroundChanged()
{
    KMahjonggBackground * selBG = backgroundMap.value(backgroundList->currentItem()->text());
    //Sanity checkings. Should not happen.
    if (selBG == nullptr) {
        return;
    }
    if (selBG->path() == kcfg_Background->text()) {
        return;
    }
    QLatin1String authstr("Author");
    QLatin1String contactstr("AuthorEmail");
    QLatin1String descstr("Description");
    kcfg_Background->setText(selBG->path());
    backgroundAuthor->setText(selBG->authorProperty(authstr));
    backgroundContact->setText(selBG->authorProperty(contactstr));
    backgroundDescription->setText(selBG->authorProperty(descstr));

    if (selBG->authorProperty(QStringLiteral("Plain")) == QLatin1String("1")) {
        backgroundPreview->setPixmap(QPixmap());
        return;
    }

    //Make sure SVG is loaded when graphics is selected
    if (!selBG->loadGraphics()) {
        return;
    }

    //Draw the preview
    //TODO here: add code to load and keep proportions for non-tiled content?
    QImage qiRend(backgroundPreview->size(), QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);
    QPainter p(&qiRend);
    p.fillRect(p.viewport(), selBG->getBackground());
    p.end();
    backgroundPreview->setPixmap(QPixmap::fromImage(qiRend));
}
