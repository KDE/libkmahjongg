/*
    SPDX-FileCopyrightText: 1997 Mathias Mueller <in5y158@public.uni-hamburg.de>
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KMAHJONGGBACKGROUND_H
#define KMAHJONGGBACKGROUND_H

// Qt
#include <QBrush>
// Std
#include <memory>

// LibKMahjongg
#include "libkmahjongg_export.h"

class KMahjonggBackgroundPrivate;

/**
 * @class KMahjonggBackground kmahjonggbackground.h <KMahjonggBackground>
 *
 * A background
 */
class LIBKMAHJONGG_EXPORT KMahjonggBackground
{
  public:
    KMahjonggBackground();
    ~KMahjonggBackground();

    bool loadDefault();
    bool load(const QString &file, short width, short height);
    bool loadGraphics();
    void sizeChanged(int newW, int newH);
    QBrush & getBackground();
    QString path() const;
    QString authorProperty(const QString &key) const;

 private:
    std::unique_ptr<KMahjonggBackgroundPrivate> const d;

    Q_DISABLE_COPY(KMahjonggBackground)
};

#endif // KMAHJONGGBACKGROUND_H
