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

#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QStringList>
#include <QTextDocument>
#include <QTextStream>
#include <QProgressDialog>
#include <QApplication>

class PassMark;

class Question
{
public:
    enum Answer { None = 0, A = 1, B = 2, C = 4, D = 8, E = 16, F = 32, G = 64, H = 128, I = 256 };
    Q_DECLARE_FLAGS(Answers, Answer)
    enum SelectionType { SingleSelection = 0, MultiSelection = 1 };
    Q_DECLARE_FLAGS(SelectionTypeFlag, SelectionType)

    Question(const QString & = QString());

    static int answerToIndex(Answer);
    static Answer indexToAnswer(int);
    static int labelToIndex(const QString &);
    static QString indexToLabel(int);
    static QString answerToString(Answer);
    static Answer convertOldAnsNumber(int);
    static int convertToOldAnsNumber(int);

    static QList<int> randomise(QList<Question *>, PassMark, bool, int, quint32, QProgressDialog * = NULL, QApplication * = NULL);

public slots:
    QString name(); void setName(const QString &);
    int flag(); void setFlag(int);
    QString group(); void setGroup(const QString &);
    int difficulty(); void setDifficulty(int);
    QString text(); void setText(const QString &);
    QString explanation(); void setExplanation(const QString &);
    SelectionType selectionType(); void setSelectionType(SelectionType);
    QString answer(Answer); void setAnswer(Answer, const QString &);
    QString answerAtIndex(int); void setAnswerAtIndex(int, const QString &);
    void addAnswer(const QString &); int numAnswers();
    bool hasCorrectAnswer(); bool isAnswerAtIndexCorrect(int);
    bool isAnswerCorrect(Answer); void setAnswerCorrect(Answers, bool);
    QStringList answers(); void setAnswers(const QStringList &);
    Answer correctAnswer();
    Answers correctAnswers(); void setCorrectAnswers(Answers);

private:
    QString q_name;
    int q_flag;
    QString q_group;
    int q_difficulty;
    QString q_text;
    QString q_explanation;
    SelectionType q_selectiontype;
    QStringList q_answers;
    Answers q_correctanswers;

    friend class QuestionItem;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Question::Answers)
Q_DECLARE_OPERATORS_FOR_FLAGS(Question::SelectionTypeFlag)

class ScoringSystem
{
protected:
    void init();
public:
    ScoringSystem();
    ScoringSystem(const QString &);
    void loadData(QString);
#ifdef ITESTSERVER
    QString data();
#endif
    double correctAnswer[3];
    double incorrectAnswer[3];
    double missingAnswer[3];
    bool allowIncompleteAnswers;
};

#define makeString(a) QString("%1").arg(a)

#endif // QUESTION_H
