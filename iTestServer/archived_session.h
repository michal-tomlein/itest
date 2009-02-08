/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2008 Michal Tomlein (michal.tomlein@gmail.com)

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

#ifndef ARCHIVED_SESSION_H
#define ARCHIVED_SESSION_H

#include <QSettings>

#include "session.h"

class ArchivedSession : public Session
{
    Q_OBJECT

public:
    ArchivedSession(MainWindow *);
    ArchivedSession(MainWindow *, Session *);
    ArchivedSession(MainWindow *, QString);
    virtual ~ArchivedSession();
    enum Status { Default, Archive, Remove };

public slots:
    void archive();
    void restore(QString);
    void removeFromArchive();
    Status status(); void setStatus(Status);
    QString sessionArchiveData();

private:
    MainWindow * as_parent;
    Status as_status;

    friend class Session;
};

#endif // ARCHIVED_SESSION_H
