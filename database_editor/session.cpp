#include "archived_session.h"

LogEntry::LogEntry()
{
	le_bgr = 255; le_bgg = 255; le_bgb = 255;
	le_fgr = 0; le_fgg = 0; le_fgb = 0;
	le_text = "";
}

LogEntry::LogEntry(int bgr, int bgg, int bgb, int fgr, int fgg, int fgb, QString text)
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
	s_qnum = 0;
	s_cqnum = 0;
	s_passmark = 0;
	s_archived = false;
}

Session::Session(ArchivedSession * archived_session)
{
	s_name = archived_session->s_name;
	s_datetime = archived_session->s_datetime;
	s_qnum = archived_session->s_qnum;
	s_cqnum = archived_session->s_cqnum;
	s_log = archived_session->s_log;
	for (int i = 0; i < archived_session->s_students.count(); ++i) {
		s_students << new Student(archived_session->s_students.at(i));
	}
	s_passmark = archived_session->s_passmark;
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

void Session::setName(QString name) { s_name = name; }

QString Session::name() { return s_name; }

void Session::setDateTimeFromString(QString date)
{
	s_datetime = QDateTime::fromString(date, "yyyy.MM.dd-hh:mm");
}

void Session::setDateTime(QDateTime datetime) { s_datetime = datetime; }

QString Session::dateTimeToString()
{
	return s_datetime.toString("yyyy.MM.dd-hh:mm");
}

QDateTime Session::dateTime() { return s_datetime; }

void Session::addLogEntry(int bgr, int bgg, int bgb, int fgr, int fgg, int fgb, QString text)
{
	s_log << LogEntry::LogEntry(bgr, bgg, bgb, fgr, fgg, fgb, text);
}

int Session::numLogEntries() { return s_log.count(); }

LogEntry Session::logEntry(int i) { return s_log.at(i); }

void Session::deleteLog() { s_log.clear(); }

void Session::addStudent(Student * student)
{
	s_students << student;
	if (student->isReady()) {
		s_qnum += student->results()->count();
		s_cqnum += student->score();
	}
}

int Session::numStudents() { return s_students.count(); }

Student * Session::student(int i) { return s_students.at(i); }

uint Session::numQuestions() { return s_qnum; }

uint Session::numCorrect() { return s_cqnum; }

void Session::setPassMark(int passmark) { s_passmark = passmark; }

int Session::passMark() { return s_passmark; }

QString Session::sessionData()
{
	QString out;
	out.append("[SESSION]\n");
	// S_NAME
	out.append(QString("%1\n").arg(s_name));
	// S_DATETIME
	out.append(dateTimeToString());
	// S_PASSMARK
	out.append(QString("\n%1").arg(s_passmark));
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
	return (s_cqnum >= (s_passmark * s_students.count()));
}

bool Session::isArchived() { return s_archived; }
