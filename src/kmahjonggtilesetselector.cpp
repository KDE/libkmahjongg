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
        for (const QString &file : fileNames) {
            tilesAvailable.append(dir + QLatin1Char('/') + file);
        }
    }

    int numvalidentries = 0;
    for (const QString &atileset : std::as_const(tilesAvailable)) {
        auto *aset = new KMahjonggTileset();
        if (aset->loadTileset(atileset)) {
            const QString name = aset->authorProperty(QStringLiteral("Name"));
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
    tilesetAuthor->setText(selTileset->authorProperty(QStringLiteral("Author")));
    tilesetContact->setText(selTileset->authorProperty(QStringLiteral("AuthorEmail")));
    tilesetDescription->setText(selTileset->authorProperty(QStringLiteral("Description")));

    // Make sure SVG is loaded when graphics is selected
    if (!selTileset->loadGraphics()) {
        return;
    }
    // Let the tileset calculate its ideal size for the preview area, but reduce the margins a bit (pass oversized drawing area)
    QSize tilesize = selTileset->preferredTileSize(tilesetPreview->size() * 1.3, 1, 1);
    selTileset->reloadTileset(tilesize);
    // Draw the preview
    QImage qiRend(tilesetPreview->size(), QImage::Format_ARGB32_Premultiplied);
    qiRend.fill(0);
    QPainter p(&qiRend);
    // Calculate the margins to center the tile
    QSize margin = tilesetPreview->size() - tilesize;
    // Draw unselected tile and first tileface
    p.drawPixmap(margin.width() / 2, margin.height() / 2, selTileset->unselectedTile(1));
    p.drawPixmap(margin.width() / 2, margin.height() / 2, selTileset->tileface(0));
    p.end();
    tilesetPreview->setPixmap(QPixmap::fromImage(qiRend));
}

#include "moc_kmahjonggtilesetselector.cpp"
