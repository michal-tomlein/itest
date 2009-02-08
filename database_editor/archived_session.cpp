#include "main_window.h"

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
	out << sessionData() << endl;
	for (int s = 0; s < numStudents(); ++s) {
		out << student(s)->studentData() << endl;
	}
	archive.setValue(QString("%1/%2").arg(as_parent->current_db_name).arg(session_title), data);
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
			ans = (QuestionItem::Answer)in.readLine().toInt();
			c_ans = (QuestionItem::Answer)in.readLine().toInt();
			QuestionAnswer qans (c_ans, ans);
			results->insert(buffer, qans);
		}
		student->setResults(results);
		addStudent(student);
	}
}

ArchivedSession::Status ArchivedSession::status() { return as_status; }

void ArchivedSession::setStatus(ArchivedSession::Status status)
{
    as_status = status;
}
