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

#include "main_window.h"

#include <cmath>

ArchivedSession::ArchivedSession(MainWindow * parent)
{
	Session();
	as_parent = parent;
	s_archived = true;
	as_status = Default;
}

ArchivedSession::ArchivedSession(MainWindow * parent, Session * session)
{
	s_name = session->s_name;
	s_datetime = session->s_datetime;
	s_qnum = session->s_qnum;
	s_cqnum = session->s_cqnum;
	s_log = session->s_log;
	for (int i = 0; i < session->s_students.count(); ++i) {
		s_students << new Student(session->s_students.at(i));
	}
	s_passmark = session->s_passmark;
	as_parent = parent;
	s_archived = true;
	as_status = Default;
}

ArchivedSession::ArchivedSession(MainWindow * parent, QString input)
{
	Session();
	as_parent = parent;
	s_archived = true;
	restore(input);
	as_status = Default;
}

ArchivedSession::~ArchivedSession() {}

void ArchivedSession::archive()
{
	QSettings archive(QSettings::IniFormat, QSettings::UserScope, "Michal Tomlein", "iTest");
	QStringList dbs = archive.value("databases").toStringList();
	dbs.removeAll(as_parent->current_db_name);
	dbs << as_parent->current_db_name;
	archive.setValue("databases", dbs);
	QString session_title = QString("%1 - %2").arg(dateTimeToString()).arg(s_name);
	QStringList sns = archive.value(QString("%1/sessions").arg(as_parent->current_db_name)).toStringList();
	sns.removeAll(session_title);
	sns << session_title;
	archive.setValue(QString("%1/sessions").arg(as_parent->current_db_name), sns);
	QString data; QTextStream out(&data);
	QString students_passed; QStringList qa_flaglist; QStringList qa_multianslist;
	out << sessionArchiveData() << endl;
	for (int s = 0; s < numStudents(); ++s) {
		out << student(s)->studentArchiveData() << endl;
		students_passed.append(student(s)->passed() ? "+" : "-");
		QMapIterator<QString, QuestionAnswer> i(*(student(s)->results())); QuestionAnswer qans;
		while (i.hasNext()) { i.next();
			qans = i.value(); qa_flaglist << QString("%1").arg(qans.flag());
			qa_multianslist << QString("%1").arg(qans.correctAnswer());
		}
	}
	archive.setValue(QString("%1/%2").arg(as_parent->current_db_name).arg(session_title), data);
	archive.setValue(QString("%1/%2/PassMark").arg(as_parent->current_db_name).arg(session_title), s_passmark.data());
	archive.setValue(QString("%1/%2/StudentsPassed").arg(as_parent->current_db_name).arg(session_title), students_passed);
	archive.setValue(QString("%1/%2/QAFlags").arg(as_parent->current_db_name).arg(session_title), qa_flaglist.join(";"));
	archive.setValue(QString("%1/%2/QACorrectAnswers").arg(as_parent->current_db_name).arg(session_title), qa_multianslist.join(";"));
}

void ArchivedSession::removeFromArchive()
{
	QSettings archive(QSettings::IniFormat, QSettings::UserScope, "Michal Tomlein", "iTest");
	QString session_title = QString("%1 - %2").arg(dateTimeToString()).arg(s_name);
	QStringList sns = archive.value(QString("%1/sessions").arg(as_parent->current_db_name)).toStringList();
	sns.removeAll(session_title);
	archive.setValue(QString("%1/sessions").arg(as_parent->current_db_name), sns);
	archive.remove(QString("%1/%2").arg(as_parent->current_db_name).arg(session_title));
}

void ArchivedSession::restore(QString input)
{
	QTextStream in(&input); QStringList bufferlist; QString buffer;
	QuestionItem::Answer ans; QuestionItem::Answer c_ans;
	if (in.readLine() != "[SESSION]") { return; }
	setName(in.readLine());
	setDateTimeFromString(in.readLine());
	setPassMark(in.readLine().toInt());
	int s_snum = in.readLine().toInt();
	int s_lenum = in.readLine().toInt();
	for (int le = 0; le < s_lenum; ++le) {
		bufferlist.clear();
		bufferlist = in.readLine().split(';');
		if (bufferlist.count() == 6) {
			addLogEntry(bufferlist.at(0).toInt(),
						bufferlist.at(1).toInt(),
						bufferlist.at(2).toInt(),
						bufferlist.at(3).toInt(),
						bufferlist.at(4).toInt(),
						bufferlist.at(5).toInt(),
						in.readLine());
		} else {
			addLogEntry(255, 255, 255, 0, 0, 0, in.readLine());
		}
	}
	for (int s = 0; s < s_snum; ++s) {
		if (in.atEnd()) { break; }
		if (in.readLine() != "[STUDENT]") { continue; }
		Student * student = new Student (in.readLine());
		if (in.readLine() == "true") { student->setReady(true); }
		else { student->setReady(false); }
		student->setNumber(in.readLine().toInt());
		student->setScore(in.readLine().toInt());
		int numresults = in.readLine().toInt();
		QMap<QString, QuestionAnswer> * results = new QMap<QString, QuestionAnswer>;
		for (int a = 0; a < numresults; ++a) {
			buffer = in.readLine();
			ans = QuestionItem::convertOldAnsNumber(in.readLine().toInt());
			c_ans = QuestionItem::convertOldAnsNumber(in.readLine().toInt());
			QuestionAnswer qans(-1, c_ans, ans);
			results->insert(buffer, qans);
		}
		student->setResults(results);
		addStudent(student);
	}
}

QString ArchivedSession::sessionArchiveData()
{
	QString out;
	out.append("[SESSION]\n");
	// S_NAME
	out.append(QString("%1\n").arg(s_name));
	// S_DATETIME
	out.append(dateTimeToString());
	// S_PASSMARK
	out.append(QString("\n%1").arg(s_passmark.passMark()));
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

ArchivedSession::Status ArchivedSession::status() { return as_status; }

void ArchivedSession::setStatus(ArchivedSession::Status status)
{
    as_status = status;
}
