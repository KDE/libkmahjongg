/*
    Copyright (C) 1997 Mathias Mueller   <in5y158@public.uni-hamburg.de>
    Copyright (C) 2006 Mauricio Piacentini   <mauricio@tabuleiro.com>

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

#ifndef _KMAHJONGGBACKGROUND_H_
#define _KMAHJONGGBACKGROUND_H_

#include <QtGui/QBrush>

#include "libkmahjongg_export.h"

class KMahjonggBackgroundPrivate;

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
    friend class KMahjonggBackgroundPrivate;
    KMahjonggBackgroundPrivate *const d;

    Q_DISABLE_COPY(KMahjonggBackground)
};

#endif
