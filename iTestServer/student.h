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

#ifndef STUDENT_H
#define STUDENT_H

#include "client.h"

class Student : public QObject
{
    Q_OBJECT

public:
    Student(const QString & = tr("Unknown"));
    Student(Client *);
    Student(Student *);
    virtual ~Student();

public slots:
    void setName(const QString &); QString name();
    void setNumber(int); int number();
    int numCorrectAnswers(ScoringSystem);
    double score(); double maximumScore();
    void setReady(bool); bool isReady();
    void setResults(QMap<QString, QuestionAnswer> *);
    void updateScore(ScoringSystem);
    QMap<QString, QuestionAnswer> * results();
    void setPassed(bool); bool passed();
    QString studentData(); QString studentArchiveData();
    bool wasAsked(const QString &); uint replaceOccurrences(const QString &, const QString &);

private:
    QString s_name;
    double s_score;
    double s_maxscore;
    bool s_ready;
    QMap<QString, QuestionAnswer> * s_results;
    int s_number;
    bool s_passed;
};

#endif // STUDENT_H
