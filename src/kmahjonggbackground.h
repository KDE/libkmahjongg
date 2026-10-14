/*
    SPDX-FileCopyrightText: 1997 Mathias Mueller <in5y158@public.uni-hamburg.de>
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGBACKGROUND_H
#define KMAHJONGGBACKGROUND_H

// Qt
#include <QtClassHelperMacros> // Q_DECLARE_PRIVATE
#include <QBrush>
// Std
#include <memory>

// LibKMahjongg
#include "libkmahjongg_export.h"

class KMahjonggBackgroundPrivate;

/*!
 * \class KMahjonggBackground
 * \inmodule LibKMahjongg
 * \brief The KMahjonggBackground class represents a background.
 *
 * This class contains a background and its associated metadata.
 */
class LIBKMAHJONGG_EXPORT KMahjonggBackground
{
public:
    KMahjonggBackground();
    ~KMahjonggBackground();

    /*!
     * Loads the default background,
     * QStandardPaths::GenericDataLocation/kmahjongglib/backgrounds/egyptian.desktop,
     * and returns whether it was successful.
     */
    bool loadDefault();
    /*!
     * Loads the background from the filepath \a file, with \a width and \a height
     * unless the configuration specifies a tiled width and height, and returns whether it was successful.
     */
    bool load(const QString &file, short width, short height);
    /*!
     * Loads the graphics from the configured location unless the background \l isPlain
     * or the graphics were already loaded, and returns whether it was successful.
     */
    bool loadGraphics();
    /*!
     * Sets the width and height to \a newW and \a newH,
     * unless the background is configured to be tiled or \l isPlain.
     */
    void sizeChanged(int newW, int newH);
    /*!
     * Returns a QBrush created with the background image as its pixmap,
     * or an empty QBrush if \l isPlain.
     */
    QBrush &getBackground();
    /*!
     * Returns the file path of this background.
     */
    QString path() const;

    /*!
     * Returns the name of this background.
     */
    QString name() const;
    /*!
     * Returns the description of this background.
     */
    QString description() const;
    /*!
     * Returns the license of this background.
     */
    QString license() const;
    /*!
     * Returns the copyright text of this background.
     */
    QString copyrightText() const;
    /*!
     * Returns the version string of this background.
     */
    QString version() const;
    /*!
     * Returns the website of this background.
     */
    QString website() const;
    /*!
     * Returns the bug report URL of this background.
     */
    QString bugReportUrl() const;
    /*!
     * Returns the name of the author of this background.
     */
    QString authorName() const;
    /*!
     * Returns the email address of the author of this background.
     */
    QString authorEmailAddress() const;
    /*!
     * Returns whether this background is plain, that is, whether it has a background image.
     */
    bool isPlain() const;

private:
    std::unique_ptr<KMahjonggBackgroundPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KMahjonggBackground)
    Q_DISABLE_COPY(KMahjonggBackground)
};

#endif // KMAHJONGGBACKGROUND_H
