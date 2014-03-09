/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2014 Michal Tomlein

 iTest is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 iTest is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with iTest; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************/

#ifndef DEFS_H
#define DEFS_H

#define ITEST_VERSION "1.4.2"
#define F_ITEST_VERSION 1.42
#define ITDB_VERSION "1.4"
#define F_ITDB_VERSION 1.4
#define ITOS_VERSION "1.4.2"
#define F_ITOS_VERSION 1.42

#define ITEST_URL "itest.sourceforge.net"

#define DATE_FORMAT "yyyy.MM.dd"
#define TIME_FORMAT "hh:mm"
#define DATE_TIME_FORMAT DATE_FORMAT "-" TIME_FORMAT

class QString;
class QVariant;
template<class Key, class T>
class QMap;
template<class Key, class T>
class QMultiMap;
template<class T>
class QList;

typedef QMap<QString, QVariant> QVariantMap;

#include <QtGlobal>
#include <QString>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

#define setSectionResizeMode setResizeMode

#else

namespace Qt {
    inline QString escape(const QString &s) {
        return s.toHtmlEscaped();
    }
}

#endif

#endif // DEFS_H
