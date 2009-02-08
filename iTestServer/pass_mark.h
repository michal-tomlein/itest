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

#ifndef PASS_MARK_H
#define PASS_MARK_H

#include "student.h"

#include <QTextStream>

class PassMark
{
public:
	PassMark();
	PassMark(int);
	
	void setPassMark(int);
	int passMark();
	void addCondition(int, int);
	int count();
	int condition(int);
	int value(int);
	int conditionIndex(int);
	bool check(QMap<QString, QuestionAnswer> *, QMap<QListWidgetItem *, QuestionItem *> *);
	QString data(); void loadData(QString);
	void clear();
	
private:
	QList<int> pm_conditions;
	QList<int> pm_values;
	int pm_mark;
};

#endif // PASS_MARK_H
