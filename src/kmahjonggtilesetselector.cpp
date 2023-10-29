/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// own
#include "kmahjonggtilesetselector.h"

// Qt
#include <QDir>
#include <QPainter>
#include <QStandardPaths>

// KF
#include <KLocalizedString>

// LibKMahjongg
#include "kmahjonggtileset.h"

KMahjonggTilesetSelector::KMahjonggTilesetSelector(QWidget *parent, KConfigSkeleton *aconfig)
    : QWidget(parent)
{
    setupUi(this);
    setupData(aconfig);
}

KMahjonggTilesetSelector::~KMahjonggTilesetSelector()
{
    qDeleteAll(tilesetMap);
}

void KMahjonggTilesetSelector::setupData(KConfigSkeleton *aconfig)
{
    // Get our currently configured Tileset entry
    KConfig *config = aconfig->config();
    KConfigGroup group = config->group("General");
    QString initialGroup = group.readEntry("Tileset_file");

    // The lineEdit widget holds our tileset path, but the user does not manipulate it directly
    kcfg_TileSet->hide();

    // This will also load our resourcedir if it is not done already
    KMahjonggTileset tile;

    // Now get our tilesets into a list
    QStringList tilesAvailable;
    const QStringList dirs =
        QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("kmahjongglib/tilesets"), QStandardPaths::LocateDirectory);
    for (const QString &dir : dirs) {
        const QStringList fileNames = QDir(dir).entryList({QStringLiteral("*.desktop")});
        tilesAvailable.reserve(tilesAvailable.size() + fileNames.size());
        for (const QString &file : fileNames) {
            tilesAvailable.append(dir + QLatin1Char('/') + file);
        }
    }

    int numvalidentries = 0;
    for (const QString &atileset : std::as_const(tilesAvailable)) {
        auto *aset = new KMahjonggTileset();
        if (aset->loadTileset(atileset)) {
            const QString name = aset->name();
            tilesetMap.insert(name, aset);
            tilesetList->addItem(name);
            // Find if this is our currently configured Tileset
            if (atileset == initialGroup) {
                // Select current entry
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
    KMahjonggTileset *selTileset = tilesetMap.value(tilesetList->currentItem()->text());
    // Sanity checkings. Should not happen.
    if (selTileset == nullptr) {
        return;
    }
    if (selTileset->path() == kcfg_TileSet->text()) {
        return;
    }

    kcfg_TileSet->setText(selTileset->path());
    tilesetAuthor->setText(selTileset->authorName());
    tilesetContact->setText(selTileset->authorEmailAddress());
    tilesetDescription->setText(selTileset->description());

    // Make sure SVG is loaded when graphics is selected
    if (!selTileset->loadGraphics()) {
        return;
    }
    const qreal dpr = qApp->devicePixelRatio();
    // Let the tileset calculate its ideal size for the preview area, but reduce the margins a bit (pass oversized drawing area)
    const QSize previewSize = tilesetPreview->size() * dpr;
    const QSize tilesize = selTileset->preferredTileSize(previewSize * 1.3, 1, 1);
    selTileset->reloadTileset(tilesize);
    // Draw the preview
    QPixmap qiRend(previewSize);
    qiRend.fill(Qt::transparent);
    QPainter p(&qiRend);
    // Calculate the margins to center the tile
    const QSize margin = (previewSize - tilesize) / 2;
    // Draw unselected tile and first tileface
    p.drawPixmap(margin.width(), margin.height(), selTileset->unselectedTile(1));
    p.drawPixmap(margin.width(), margin.height(), selTileset->tileface(0));
    p.end();
    qiRend.setDevicePixelRatio(dpr);
    tilesetPreview->setPixmap(qiRend);
}

#include "moc_kmahjonggtilesetselector.cpp"
