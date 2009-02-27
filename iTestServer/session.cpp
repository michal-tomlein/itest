/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2009 Michal Tomlein (michal.tomlein@gmail.com)

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

#include "archived_session.h"

LogEntry::LogEntry()
{
	le_bgr = 255; le_bgg = 255; le_bgb = 255;
	le_fgr = 0; le_fgg = 0; le_fgb = 0;
	le_text = "";
}

LogEntry::LogEntry(int bgr, int bgg, int bgb, int fgr, int fgg, int fgb, const QString & text)
{
	le_bgr = bgr; le_bgg = bgg; le_bgb = bgb;
	le_fgr = fgr; le_fgg = fgg; le_fgb = fgb;
	le_text = text;
}

QColor LogEntry::entryBackgroundColour() { return QColor::QColor(le_bgr, le_bgg, le_bgb); }

QColor LogEntry::entryForegroundColour() { return QColor::QColor(le_fgr, le_fgg, le_fgb); }

QString LogEntry::entryText() { return le_text; }

QString LogEntry::entryData()
{
	return QString("%1;%2;%3;%4;%5;%6\n%7").arg(le_bgr).arg(le_bgg).arg(le_bgb).arg(le_fgr).arg(le_fgg).arg(le_fgb).arg(le_text);
}

int LogEntry::bgRed() { return le_bgr; }

int LogEntry::bgGreen() { return le_bgg; }

int LogEntry::bgBlue() { return le_bgb; }

int LogEntry::fgRed() { return le_fgr; }

int LogEntry::fgGreen() { return le_fgg; }

int LogEntry::fgBlue() { return le_fgb; }

Session::Session()
{
	s_name = tr("Undefined");
	s_maxscore = 0.0;
	s_score = 0.0;
	s_archived = false;
}

Session::Session(ArchivedSession * archived_session)
{
	s_name = archived_session->s_name;
	s_datetime = archived_session->s_datetime;
	s_maxscore = archived_session->s_maxscore;
	s_score = archived_session->s_score;
	s_log = archived_session->s_log;
	for (int i = 0; i < archived_session->s_students.count(); ++i) {
		s_students << new Student(archived_session->s_students.at(i));
	}
	s_passmark = archived_session->s_passmark;
    s_scoringsystem = archived_session->s_scoringsystem;
	s_archived = false;
}

Session::~Session()
{
	for (int i = 0; i < s_students.count(); ++i) {
		if (s_students.at(i)) delete s_students.at(i);
	}
}

void Session::destruct()
{
	s_students.clear();
}

void Session::setName(const QString & name) { s_name = name; }

QString Session::name() { return s_name; }

void Session::setDateTimeFromString(const QString & date)
{
	s_datetime = QDateTime::fromString(date, "yyyy.MM.dd-hh:mm");
}

void Session::setDateTime(QDateTime datetime) { s_datetime = datetime; }

QString Session::dateTimeToString()
{
	return s_datetime.toString("yyyy.MM.dd-hh:mm");
}

QDateTime Session::dateTime() { return s_datetime; }

void Session::addLogEntry(int bgr, int bgg, int bgb, int fgr, int fgg, int fgb, const QString & text)
{
	s_log << LogEntry::LogEntry(bgr, bgg, bgb, fgr, fgg, fgb, text);
}

int Session::numLogEntries() { return s_log.count(); }

LogEntry Session::logEntry(int i) { return s_log.at(i); }

void Session::deleteLog() { s_log.clear(); }

void Session::addStudent(Student * student) { s_students << student; }

int Session::numStudents() { return s_students.count(); }

Student * Session::student(int i) { return s_students.at(i); }

long double Session::maximumScore() { return s_maxscore; }

long double Session::score() { return s_score; }

int Session::average() { return s_maxscore != 0.0 ? (int)(100.0 * s_score / s_maxscore) : 100; }

void Session::setPassMark(PassMark passmark) { s_passmark = passmark; }

PassMark Session::passMark() { return s_passmark; }

void Session::loadPassMark(const QString & str) { s_passmark.loadData(str); }

void Session::setScoringSystem(ScoringSystem sys)
{
    s_scoringsystem = sys;
    s_maxscore = 0.0; s_score = 0.0;
    for (int i = 0; i < s_students.count(); ++i) {
        s_students.at(i)->updateScore(s_scoringsystem);
        if (s_students.at(i)->isReady()) {
            s_maxscore += s_students.at(i)->maximumScore();
            s_score += s_students.at(i)->score();
        }
    }
}

ScoringSystem Session::scoringSystem() { return s_scoringsystem; }

QString Session::sessionData()
{
	QString out;
	out.append("[SESSION]\n");
	// S_NAME
	out.append(QString("%1\n").arg(s_name));
	// S_DATETIME
	out.append(dateTimeToString());
	// S_PASSMARK
	out.append(QString("\n%1").arg(s_passmark.archiveData()));
    // S_SCORINGSYSTEM
    out.append(QString("\n%1").arg(s_scoringsystem.data()));
	// S_SNUM
	out.append(QString("\n%1").arg(s_students.count()));
	// S_NUMLOGENTRIES
	out.append(QString("\n%1").arg(s_log.count()));
	// S_LOGENTRIES
	LogEntry log_entry;
	for (int i = 0; i < s_log.count(); ++i) {
		log_entry = s_log.at(i);
		out.append(QString("\n%1").arg(log_entry.entryData()));
	}
	return out;
}

bool Session::mostPassed()
{
	int numpassed = 0;
    for (int i = 0; i < s_students.count(); ++i) {
        if (s_students.at(i)->passed()) { numpassed++; }
    }
    return numpassed >= (s_students.count() - numpassed);
}

bool Session::isArchived() { return s_archived; }
