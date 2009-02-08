#include "student.h"

Student::Student()
{
    s_name = QObject::tr("Unknown");
    s_number = 0;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
    s_passed = true;
}

Student::Student(int number)
{
    s_name = QObject::tr("Unknown");
    s_number = number;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
    s_passed = true;
}

Student::Student(QString name)
{
    s_name = name;
    s_number = 0;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
    s_passed = true;
}

Student::Student(int number, QString name)
{
    s_name = name;
    s_number = number;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
    s_passed = true;
}

Student::Student(Client * client)
{
    s_name = client->name();
    s_number = client->number();
    s_score = client->score();
    s_ready = client->isReady();
    s_results = new QMap<QString, QuestionAnswer> (*(client->results()));
    s_passed = client->passed();
}

Student::Student(Student * student)
{
    s_name = student->name();
    s_number = student->number();
    s_score = student->score();
    s_ready = student->isReady();
    s_results = new QMap<QString, QuestionAnswer> (*(student->results()));
    s_passed = student->passed();
}

Student::~Student()
{
    if (s_results) delete s_results;
}

void Student::setName(QString name) { s_name = name; }

QString Student::name() { return s_name; }

void Student::setNumber(int number) { s_number = number; }

int Student::number() { return s_number; }

void Student::setScore(int score) { s_score = score; }

int Student::score() { return s_score; }

void Student::setReady(bool ready) { s_ready = ready; }

bool Student::isReady() { return s_ready; }

void Student::setResults(QMap<QString, QuestionAnswer> * results)
{
    delete s_results;
    s_results = results;
}

QMap<QString, QuestionAnswer> * Student::results() { return s_results; }

void Student::setPassed(bool passed) { s_passed = passed; }

bool Student::passed() { return s_passed; }

/*bool Student::loadStudentData(QString input)
{
	QTextStream in(&input); QString buffer;
	QuestionItem::Answer ans; QuestionItem::Answer c_ans;

	if (in.readLine() != "[STUDENT]") { return false; }
	s_name = in.readLine();
	if (in.readLine() == "true") { s_ready = true; } else { s_ready = false; }
	s_number = in.readLine().toInt();
	s_score = in.readLine().toInt();
	int numresults = in.readLine().toInt();
	s_results->clear();
	for (int i = 0; i < numresults; ++i) {
		buffer = in.readLine();
		ans = (QuestionItem::Answer)in.readLine().toInt();
		c_ans = (QuestionItem::Answer)in.readLine().toInt();
		QuestionAnswer qans (c_ans, ans);
		s_results->insert(buffer, qans);
	}
	return true;
}*/

QString Student::studentData()
{
	QString out;
	out.append("[STUDENT]\n");
	// S_NAME
	out.append(s_name);
	// S_READY
	out.append(s_ready ? "\ntrue" : "\nfalse");
	// S_PASSED
	out.append(s_passed ? "\ntrue" : "\nfalse");
	// S_NUMBER
	out.append(QString("\n%1\n").arg(s_number));
	// S_SCORE
	out.append(QString("%1\n").arg(s_score));
	// S_NUMRESULTS
	out.append(QString("%1").arg(s_results->count()));
	// S_RESULTS
	QMapIterator<QString, QuestionAnswer> i(*s_results); QuestionAnswer qans;
	while (i.hasNext()) { i.next();
		qans = i.value();
		out.append(QString("\n%1").arg(i.key()));
		out.append(QString("\n%1").arg(qans.flag()));
		out.append(QString("\n%1").arg(qans.answered()));
		out.append(QString("\n%1").arg(qans.correctAnswer()));
	}
	return out;
}

QString Student::studentArchiveData()
{
	QString out;
	out.append("[STUDENT]\n");
	// S_NAME
	out.append(s_name);
	// S_READY
	out.append(s_ready ? "\ntrue" : "\nfalse");
	// S_NUMBER
	out.append(QString("\n%1\n").arg(s_number));
	// S_SCORE
	out.append(QString("%1\n").arg(s_score));
	// S_NUMRESULTS
	out.append(QString("%1").arg(s_results->count()));
	// S_RESULTS
	QMapIterator<QString, QuestionAnswer> i(*s_results); QuestionAnswer qans;
	while (i.hasNext()) { i.next();
		qans = i.value();
		out.append(QString("\n%1").arg(i.key()));
		out.append(QString("\n%1").arg(qans.answered()));
		out.append(QString("\n%1").arg(qans.correctAnswer()));
	}
	return out;
}

bool Student::wasAsked(QString qname)
{
	QMapIterator<QString, QuestionAnswer> i(*s_results);
	while (i.hasNext()) { i.next();
		if (i.key() == qname) { return true; }
	}
	return false;
}

uint Student::replaceOccurrences(QString old_qname, QString new_qname)
{
	QuestionAnswer qans = s_results->value(old_qname, QuestionAnswer());
	uint n = s_results->remove(old_qname);
	if (n > 0) { s_results->insert(new_qname, qans); }
	return n;
}
