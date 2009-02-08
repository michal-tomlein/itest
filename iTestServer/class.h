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

#ifndef CLASS_H
#define CLASS_H

#include "archived_session.h"

struct SessionEntry
{
    QDateTime session;
    int member_num;
};

class ClassMember
{
public:
    ClassMember();
    ClassMember(QString);

    void setName(QString); QString name();
    void addSession(QDateTime, int);
    void addSession(QString, int);
    void removeSession(QDateTime);
    void removeSession(QString);
    SessionEntry sessionEntry(int);
    QDateTime session(int);
    QString sessionToString(int);
    int memberNumForSession(int);
    int numSessionEntries();
    bool hasSession(QDateTime);
    int average(QMap<QDateTime, Session *> *, QMap<QDateTime, ArchivedSession *> *);
    QString memberData();

private:
    QString ms_name;
    QList<SessionEntry> ms_sessions;
};

class Class
{
public:
    Class();
    Class(QString);
    virtual ~Class();

    void setName(QString); QString name();
    void setFirstYear(int); int firstYear();
    void setLastYear(int); int lastYear();
    int addMember(ClassMember *); int addMember(QString);
    int removeMember(ClassMember *); void removeMember(int);
    int numMembers(); int memberPosition(ClassMember *);
    ClassMember * member(int);
    ClassMember * takeMember(int);
    void addSession(QDateTime); void addSession(QString);
    int removeSession(QDateTime); int removeSession(QString);
    void removeSession(int);
    int numSessions();
    QDateTime session(int); QString sessionToString(int);
    int average(QMap<QDateTime, Session *> *, QMap<QDateTime, ArchivedSession *> *);
    QString classData();

private:
    int updateAddedMemberPosition();

    QString cl_name;
    int cl_firstyear; int cl_lastyear;
    QList<ClassMember *> cl_members;
    QList<QDateTime> cl_sessions;
};

#endif // CLASS_H
