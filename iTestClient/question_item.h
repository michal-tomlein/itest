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

#ifndef QUESTION_ITEM_H
#define QUESTION_ITEM_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QTextDocument>

class QuestionItem
{
public:
    enum Answer { None = 0, A = 1, B = 2, C = 4, D = 8 };
    Q_DECLARE_FLAGS(Answers, Answer)

    QuestionItem();
    QuestionItem(QString);
    QuestionItem(QString, int, QString, int, QString, QStringList);
    
    static QuestionItem::Answer convertOldAnsNumber(int);
    static int convertToOldAnsNumber(int);

public slots:
    QString name();
    int flag();
    QString group();
    int difficulty();
    QString text();
    QString ansA(); QString ansB(); QString ansC(); QString ansD();
    bool isAnsCorrect(Answer);
    bool isAnsACorrect(); bool isAnsBCorrect();
    bool isAnsCCorrect(); bool isAnsDCorrect();
    QStringList answers();
    Answer correctAnswer();
    Answers correctAnswers();
    void setName(QString);
    void setFlag(int);
    void setGroup(QString);
    void setDifficulty(int);
    void setText(QString);
    void setAnsA(QString); void setAnsB(QString);
    void setAnsC(QString); void setAnsD(QString);
    void setAnsCorrect(Answers, bool);
    void setAnsACorrect(bool); void setAnsBCorrect(bool);
    void setAnsCCorrect(bool); void setAnsDCorrect(bool);
    void setAnswers(QStringList);
    void setCorrectAnswers(Answers);
    bool hasCorrectAnswer();
    Answer answered(); void setAnswered(Answer);
    bool isAnsweredCorrectly();
    void addSvgItem(QString, QString); int numSvgItems();
    QString svgName(int); QString svg(int);

private:
    QString q_name;
    int q_flag;
    QString q_group;
    int q_difficulty;
    QString q_text;
    QStringList q_answers;
    Answers q_correctanswers;
    Answer q_answer;
    QStringList q_svglist;
    QMap<QString, QString> q_svgmap;
};

class QuestionAnswer
{
public:
    QuestionAnswer();
    QuestionAnswer(QuestionItem::Answer, QuestionItem::Answer);
    void setAnswered(QuestionItem::Answer); QuestionItem::Answer answered();
    void setCorrectAnswer(QuestionItem::Answer); QuestionItem::Answer correctAnswer();
    bool isAnsweredCorrectly();

private:
    QuestionItem::Answer qa_answered;
    QuestionItem::Answer qa_correct_answer;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QuestionItem::Answers)

#endif // QUESTION_ITEM_H
