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
		out.append(QString("\n%1").arg(QuestionItem::convertToOldAnsNumber(qans.answered())));
		out.append(QString("\n%1").arg(QuestionItem::convertToOldAnsNumber(qans.correctAnswer())));
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
