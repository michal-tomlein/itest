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
