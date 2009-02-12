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

#ifndef PASS_MARK_H
#define PASS_MARK_H

#ifdef ITESTSERVER
#include "../iTestServer/student.h"
#endif
#ifdef ITESTCLIENT
#include "../iTestClient/question_item.h"
#endif

#include <QTextStream>

class PassMark
{
public:
	PassMark(int = 0);

	void setPassMark(int);
	int passMark();
	void addCondition(int, int, int);
	int count();
	int condition(int);
	int value(int);
	int conditionIndex(int);
    int qnum(int);
#ifdef ITESTSERVER
	bool check(QMap<QString, QuestionAnswer> *, QMap<QListWidgetItem *, QuestionItem *> *, ScoringSystem);
#endif
#ifdef ITESTCLIENT
    bool check(QList<QuestionItem *>);
#endif
#ifdef ITESTSERVER
	QString data(); QString archiveData(); void loadData(QString);
#endif
	void clear();

private:
	QList<int> pm_conditions;
	QList<int> pm_values;
    QList<int> pm_qnums;
	int pm_mark;
};

#endif // PASS_MARK_H
