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

#include "class.h"

ClassMember::ClassMember()
{
    ms_name = "";
}

ClassMember::ClassMember(QString name)
{
    ms_name = name;
}

void ClassMember::setName(QString name) { ms_name = name; }

QString ClassMember::name() { return ms_name; }

void ClassMember::addSession(QDateTime date, int num)
{
    SessionEntry se;
    se.session = date;
    se.member_num = num;
    ms_sessions << se;
}

void ClassMember::addSession(QString date, int num)
{
    SessionEntry se;
    se.session = QDateTime::fromString(date, "yyyy.MM.dd-hh:mm");
    se.member_num = num;
    ms_sessions << se;
}

void ClassMember::removeSession(QDateTime date)
{
    for (int i = 0; i < ms_sessions.count(); ++i) {
        if (ms_sessions.at(i).session == date) { ms_sessions.removeAt(i); i--; }
    }
}

void ClassMember::removeSession(QString date)
{
    removeSession(QDateTime::fromString(date, "yyyy.MM.dd-hh:mm"));
}

SessionEntry ClassMember::sessionEntry(int i) { return ms_sessions.at(i); }

QDateTime ClassMember::session(int i) { return ms_sessions.at(i).session; }

QString ClassMember::sessionToString(int i) { return ms_sessions.at(i).session.toString("yyyy.MM.dd-hh:mm"); }

int ClassMember::memberNumForSession(int i) { return ms_sessions.at(i).member_num; }

int ClassMember::numSessionEntries() { return ms_sessions.count(); }

bool ClassMember::hasSession(QDateTime session)
{
    for (int i = 0; i < ms_sessions.count(); ++i) {
        if (ms_sessions.at(i).session == session) { return true; }
    }
    return false;
}

int ClassMember::average(QMap<QDateTime, Session *> * sessions, QMap<QDateTime, ArchivedSession *> * archivedsessions)
{
    long double score = 0.0; long double maxscore = 0.0;
    for (int i = 0; i < ms_sessions.count(); ++i) {
        Session * session = sessions->value(ms_sessions.at(i).session, archivedsessions->value(ms_sessions.at(i).session, NULL));
        if (session == NULL) { continue; }
        if (ms_sessions.at(i).member_num < 0 || ms_sessions.at(i).member_num >= session->numStudents()) { continue; }
        Student * student = session->student(ms_sessions.at(i).member_num);
        score += student->score();
        maxscore += student->maximumScore();
    }
    if (score == 0.0) { return 0; }
    if (maxscore == 0.0) { return 100; }
    return (int)(100.0 * score / maxscore);
}

QString ClassMember::memberData()
{
    QString out;
	out.append("[MEM]\n");
	// MEM_NAME
	out.append(ms_name);
	// MEM_SNUM
	out.append(QString("\n%1").arg(ms_sessions.count()));
	for (int i = 0; i < ms_sessions.count(); ++i) {
		// MEM_SESSION
	    out.append(QString("\n%1\n").arg(sessionToString(i)));
	    // MEM_NAME
	    out.append(QString("%1").arg(memberNumForSession(i)));
	}
	return out;
}

Class::Class()
{
	cl_name = "";
	cl_firstyear = QDate::currentDate().year();
	cl_lastyear = QDate::currentDate().year() + 4;
}

Class::Class(QString name)
{
	cl_name = name;
	cl_firstyear = QDate::currentDate().year();
	cl_lastyear = QDate::currentDate().year() + 4;
}

Class::~Class()
{
    for (int i = 0; i < cl_members.count(); ++i) {
        if (cl_members.at(i)) delete cl_members.at(i);
    }
}

void Class::setName(QString name) { cl_name = name; }

QString Class::name() { return cl_name; }

void Class::setFirstYear(int year) { cl_firstyear = year; }

int Class::firstYear() { return cl_firstyear; }

void Class::setLastYear(int year) { cl_lastyear = year; }

int Class::lastYear() { return cl_lastyear; }

int Class::addMember(ClassMember * mem) { cl_members << mem; return updateAddedMemberPosition(); }

int Class::addMember(QString name) { cl_members << new ClassMember(name); return updateAddedMemberPosition(); }

int Class::removeMember(ClassMember * mem) { delete mem; return cl_members.removeAll(mem); }

void Class::removeMember(int i) { delete cl_members.takeAt(i); }

int Class::numMembers() { return cl_members.count(); }

int Class::memberPosition(ClassMember * mem) { return cl_members.indexOf(mem); }

ClassMember * Class::member(int i) { return cl_members.at(i); }

ClassMember * Class::takeMember(int i) { return cl_members.takeAt(i); }

void Class::addSession(QDateTime session) { cl_sessions << session; }

void Class::addSession(QString session) { cl_sessions << QDateTime::fromString(session, "yyyy.MM.dd-hh:mm"); }

int Class::removeSession(QDateTime session) { return cl_sessions.removeAll(session); }

int Class::removeSession(QString session) { return cl_sessions.removeAll(QDateTime::fromString(session, "yyyy.MM.dd-hh:mm")); }

void Class::removeSession(int i) { cl_sessions.removeAt(i); }

int Class::numSessions() { return cl_sessions.count(); }

QDateTime Class::session(int i) { return cl_sessions.at(i); }

QString Class::sessionToString(int i) { return cl_sessions.at(i).toString("yyyy.MM.dd-hh:mm"); }

int Class::average(QMap<QDateTime, Session *> * sessions, QMap<QDateTime, ArchivedSession *> * archivedsessions)
{
    long double score = 0.0; int n = 0;
    for (int i = 0; i < cl_members.count(); ++i) {
        if (cl_members.at(i)->numSessionEntries() > 0) {
            score += cl_members.at(i)->average(sessions, archivedsessions);
            n++;
        }
    }
    if (n != 0) { return (int)(score / (long double)n); }
    return 100;
}

QString Class::classData()
{
	QString out;
	out.append("[CLASS]\n");
	// CL_NAME
	out.append(QString("%1\n").arg(cl_name));
    // CL_YEAR
    out.append(QString("%1-%2\n").arg(cl_firstyear).arg(cl_lastyear));
	// CL_SESSIONS
    QStringList sessions;
	for (int i = 0; i < cl_sessions.count(); ++i) {
	    sessions << sessionToString(i);
	}
	out.append(sessions.join(";"));
	// CL_MNUM
	out.append(QString("\n%1").arg(cl_members.count()));
	for (int i = 0; i < cl_members.count(); ++i) {
	    out.append(QString("\n%1").arg(cl_members.at(i)->memberData()));
	}
	return out;
}

int Class::updateAddedMemberPosition()
{
    for (int i = 0; i < cl_members.count() - 1; ++i) {
        if (cl_members.at(i)->name().toLower() > cl_members.last()->name().toLower())
            { cl_members.move(cl_members.count() - 1, i); return i; }
    }
    return cl_members.count() - 1;
}
