/*******************************************************************
 This file is part of iTest
 Copyright (C) 2007 Michal Tomlein (michal.tomlein@gmail.com)

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

#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include <QColor>

#include "pass_mark.h"

class LogEntry
{
public:
    LogEntry();
    LogEntry(int, int, int, int, int, int, QString);
    QColor entryBackgroundColour(); QColor entryForegroundColour();
    QString entryText(); QString entryData();
    int bgRed(); int bgGreen(); int bgBlue();
    int fgRed(); int fgGreen(); int fgBlue();

private:
    int le_bgr; int le_bgg; int le_bgb;
    int le_fgr; int le_fgg; int le_fgb;
    QString le_text;
};

class ArchivedSession;

class Session : public QObject
{
    Q_OBJECT

public:
    Session();
    Session(ArchivedSession *);
    virtual ~Session();

public slots:
    void setName(QString); QString name();
    void setDateTimeFromString(QString); void setDateTime(QDateTime);
    QString dateTimeToString(); QDateTime dateTime();
    void addLogEntry(int, int, int, int, int, int, QString);
    int numLogEntries(); LogEntry logEntry(int); void deleteLog();
    void addStudent(Student *); int numStudents(); Student * student(int);
    uint numQuestions(); uint numCorrect();
    void setPassMark(PassMark); PassMark passMark(); void loadPassMark(QString);
    QString sessionData();
    bool mostPassed(); bool isArchived();
    void destruct();

private:
    QString s_name;
    QDateTime s_datetime;
    uint s_qnum;
    uint s_cqnum;
    QList<LogEntry> s_log;
    QList<Student *> s_students;
    bool s_archived;
    PassMark s_passmark;

    friend class ArchivedSession;
};

#endif // SESSION_H
