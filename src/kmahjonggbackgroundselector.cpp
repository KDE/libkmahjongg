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

KMahjonggBackgroundSelector::KMahjonggBackgroundSelector(QWidget *parent, KConfigSkeleton *aconfig)
    : QWidget(parent)
{
    setupUi(this);
    setupData(aconfig);
}

KMahjonggBackgroundSelector::~KMahjonggBackgroundSelector()
{
    qDeleteAll(backgroundMap);
}

void KMahjonggBackgroundSelector::setupData(KConfigSkeleton *aconfig)
{
    // Get our currently configured background entry
    KConfig *config = aconfig->config();
    KConfigGroup group = config->group("General");
    QString initialGroup = group.readEntry("Background_file");

    // The lineEdit widget holds our bg path, but the user does not manipulate it directly
    kcfg_Background->hide();

    KMahjonggBackground bg;

    // Now get our backgrounds into a list
    QStringList bgsAvailable;
    const QStringList dirs =
        QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/backgrounds"), QStandardPaths::LocateDirectory);
    for (const QString &dir : dirs) {
        const QStringList fileNames = QDir(dir).entryList({QStringLiteral("*.desktop")});
        bgsAvailable.reserve(bgsAvailable.size() + fileNames.size());
        for (const QString &file : fileNames) {
            bgsAvailable.append(dir + QLatin1Char('/') + file);
        }
    }

    const qreal dpr = qApp->devicePixelRatio();
    const QSize previewSize = backgroundPreview->size() * dpr;

    int numvalidentries = 0;
    for (const QString &bgpath : std::as_const(bgsAvailable)) {
        auto *abg = new KMahjonggBackground();
        if (abg->load(bgpath, previewSize.width(), previewSize.height())) {
            const QString name = abg->name();
            backgroundMap.insert(name, abg);
            backgroundList->addItem(name);
            // Find if this is our currently configured background
            if (bgpath == initialGroup) {
                // Select current entry
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
    KMahjonggBackground *selBG = backgroundMap.value(backgroundList->currentItem()->text());
    // Sanity checkings. Should not happen.
    if (selBG == nullptr) {
        return;
    }
    if (selBG->path() == kcfg_Background->text()) {
        return;
    }

    kcfg_Background->setText(selBG->path());
    backgroundAuthor->setText(selBG->authorName());
    backgroundContact->setText(selBG->authorEmailAddress());
    backgroundDescription->setText(selBG->description());

    if (selBG->isPlain()) {
        backgroundPreview->setPixmap(QPixmap());
        return;
    }

    // Make sure SVG is loaded when graphics is selected
    if (!selBG->loadGraphics()) {
        return;
    }

    // Draw the preview
    // TODO here: add code to load and keep proportions for non-tiled content?
    const qreal dpr = qApp->devicePixelRatio();
    QPixmap qiRend(backgroundPreview->size() * dpr);
    qiRend.fill(Qt::transparent);
    QPainter p(&qiRend);
    p.fillRect(p.viewport(), selBG->getBackground());
    p.end();
    qiRend.setDevicePixelRatio(dpr);
    backgroundPreview->setPixmap(qiRend);
}

#include "moc_kmahjonggbackgroundselector.cpp"
