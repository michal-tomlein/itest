#include "student.h"

Student::Student()
{
    s_name = QObject::tr("Unknown");
    s_number = 0;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
}

Student::Student(int number)
{
    s_name = QObject::tr("Unknown");
    s_number = number;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
}

Student::Student(QString name)
{
    s_name = name;
    s_number = 0;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
}

Student::Student(int number, QString name)
{
    s_name = name;
    s_number = number;
    s_score = 0;
    s_ready = false;
    s_results = new QMap<QString, QuestionAnswer>;
}

Student::Student(Client * client)
{
    s_name = client->name();
    s_number = client->number();
    s_score = client->score();
    s_ready = client->isReady();
    s_results = new QMap<QString, QuestionAnswer> (*(client->results()));
}

Student::~Student()
{
    delete s_results;
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
	// S_NUMBER
	out.append(QString("\n%1\n").arg(s_number));
	// S_SCORE
	out.append(QString("%1\n").arg(s_score));
	// S_NUMRESULTS
	out.append(QString("%1").arg(s_results->count()));
	// S_RESULTS
	QMapIterator<QString, QuestionAnswer> i(*s_results); QuestionAnswer qans;
	while (i.hasNext()) {
		i.next(); qans = i.value();
		out.append(QString("\n%1").arg(i.key()));
		out.append(QString("\n%1").arg(qans.answered()));
		out.append(QString("\n%1").arg(qans.correctAnswer()));
	}
	return out;
}
