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

#include "pass_mark.h"

PassMark::PassMark(int mark)
{
	pm_mark = mark;
}

void PassMark::setPassMark(int mark)
{
	pm_mark = mark;
}

int PassMark::passMark()
{
	return pm_mark;
}
	
void PassMark::addCondition(int condition, int value, int qnum)
{
	pm_conditions << condition;
	pm_values << value;
    pm_qnums << qnum;
}

int PassMark::count()
{
	return pm_conditions.count();
}

int PassMark::condition(int i)
{
	return pm_conditions.at(i);
}

int PassMark::value(int i)
{
	return pm_values.at(i);
}

int PassMark::conditionIndex(int i)
{
	return pm_conditions.indexOf(i);
}

int PassMark::qnum(int i)
{
    return pm_qnums.at(i);
}

/*bool PassMark::move(int a, int b)
{
	if (!pm_conditions.contains(a)) { return false; }
	int i = pm_conditions.indexOf(a);
	pm_values[i]--;
	if (pm_values.at(i) <= 0) {
		pm_conditions.removeAt(i);
		pm_values.removeAt(i);
	}
	if (pm_conditions.contains(b)) {
		pm_values[pm_conditions.indexOf(b)]++;
	} else {
		pm_conditions << b; pm_values << 1;
	}
	return true;
}*/

#ifdef ITESTSERVER
bool PassMark::check(QMap<QString, QuestionAnswer> * answers, QMap<QListWidgetItem *, QuestionItem *> * questions, ScoringSystem sys)
{
	QuestionItem * item = NULL; QuestionAnswer ans;
	int c_ans_count = 0; int c_ans[20];
	for (int i = 0; i < 20; ++i) { c_ans[i] = 0; }
	QMapIterator<QString, QuestionAnswer> i(*answers);
	while (i.hasNext()) { i.next();
		item = NULL;
		QMapIterator<QListWidgetItem *, QuestionItem *> q(*questions);
		while (q.hasNext()) { q.next();
			if (q.value()->name() == i.key()) {
				item = q.value(); break;
			}
		}
		if (item == NULL) { continue; }
		ans = i.value();
		if (ans.score(sys) > 0.0) {
			c_ans_count++;
			if (item->flag() >= 0 && item->flag() < 20) { c_ans[item->flag()]++; }
		}
	}
	if (c_ans_count < pm_mark) { return false; }
	for (int i = 0; i < pm_conditions.count(); ++i) {
		if (c_ans[pm_conditions.at(i)] < pm_values.at(i)) { return false; }
	}
	return true;
}
#endif

#ifdef ITESTCLIENT
bool PassMark::check(QList<QuestionItem *> questions)
{
	int c_ans_count = 0; int c_ans[20];
	for (int i = 0; i < 20; ++i) { c_ans[i] = 0; }
	for (int i = 0; i < questions.count(); ++i) {
		if (questions.at(i)->score() > 0.0) {
			c_ans_count++;
			if (questions.at(i)->flag() >= 0 && questions.at(i)->flag() < 20) { c_ans[questions.at(i)->flag()]++; }
		}
	}
	if (c_ans_count < pm_mark) { return false; }
	for (int i = 0; i < pm_conditions.count(); ++i) {
		if (c_ans[pm_conditions.at(i)] < pm_values.at(i)) { return false; }
	}
	return true;
}
#endif

#ifdef ITESTSERVER
QString PassMark::data()
{
	QString out;
	out.append("[PASSMARK]\n");
	out.append(QString("%1\n").arg(pm_mark));
	out.append(QString("%1").arg(pm_conditions.count()));
	for (int i = 0; i < pm_conditions.count(); ++i) {
		out.append(QString("\n%1").arg(pm_conditions.at(i)));
		out.append(QString("\n%1").arg(pm_values.at(i)));
        out.append(QString("\n%1").arg(pm_qnums.at(i)));
	}
	return out;
}

QString PassMark::archiveData()
{
	QString out;
	out.append("[PASSMARK]\n");
	out.append(QString("%1\n").arg(pm_mark));
	out.append(QString("%1").arg(pm_conditions.count()));
	for (int i = 0; i < pm_conditions.count(); ++i) {
		out.append(QString("\n%1").arg(pm_conditions.at(i)));
		out.append(QString("\n%1").arg(pm_values.at(i)));
	}
	return out;
}

void PassMark::loadData(QString str)
{
	QTextStream in(&str);
	if (in.readLine() != "[PASSMARK]") { return; }
	clear();
	pm_mark = in.readLine().toInt();
	int c = in.readLine().toInt();
	for (int i = 0; i < c; ++i) {
		pm_conditions << in.readLine().toInt();
		pm_values << in.readLine().toInt();
	}
}
#endif

void PassMark::clear()
{
	pm_conditions.clear();
	pm_values.clear();
	pm_mark = 0;
}
