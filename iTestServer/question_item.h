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

#ifndef QUESTION_ITEM_H
#define QUESTION_ITEM_H

#include "../shared/question.h"
#include "svg_item.h"

class QuestionAnswer
{
public:
    QuestionAnswer(Question::Answer /* correct */ = Question::None,
                    Question::Answer /* ans */ = Question::None,
                    int /* flag */ = -1,
                    int /* difficulty */ = 0,
                    Question::SelectionType /* type */ = Question::SingleSelection,
                    const QString & /* explanation */ = QString());
    void setAnswered(Question::Answer); Question::Answer answered();
    void setCorrectAnswer(Question::Answer); Question::Answer correctAnswer();
    double score(ScoringSystem);
    double maximumScore(ScoringSystem);
    void setFlag(int); int flag();
    void setDifficulty(int); int difficulty();
    void setSelectionType(Question::SelectionType); Question::SelectionType selectionType();
    void setExplanation(const QString &); QString explanation();

private:
    Question::Answer qa_answered;
    Question::Answer qa_correct_answer;
    int qa_flag;
    int q_difficulty;
    Question::SelectionType q_selectiontype;
    QString qa_explanation;
};

class QuestionItem : public Question
{
public:
    QuestionItem(const QString & /* name */ = QString(),
                int /* flag */ = -1,
                const QString & /* group */ = QString(),
                int /* difficulty */ = 0,
                const QString & /* text */ = QString(),
                const QStringList & /* answers */ = QStringList() << QString() << QString() << QString() << QString(),
                Answers /* correctanswers */ = None,
                SelectionType /* selectiontype */ = SingleSelection,
                const QString & /* explanation */ = QString(),
                unsigned int /* inccount */ = 0,
                unsigned int /* ccount */ = 0,
                bool /* hidden */ = false,
                QList<SvgItem *> /* svgs */ = QList<SvgItem *>(),
                bool /* copysvgs */ = true);
    ~QuestionItem();

public slots:
    bool isHidden(); void setHidden(bool);
    unsigned int incorrectAnsCount(); void setIncorrectAnsCount(unsigned int);
    unsigned int correctAnsCount(); void setCorrectAnsCount(unsigned int);
    void addIncorrectAns(); void addCorrectAns();
    void addSvgItem(SvgItem *);
    bool removeSvgItem(SvgItem *); SvgItem * removeSvgItem(int);
    int numSvgItems(); SvgItem * svgItem(int); QList<SvgItem *> svgItems();
    int recommendedDifficulty();
    QString allProperties(); QString allPublicProperties();

private:
    unsigned int q_incorrectanscount;
    unsigned int q_correctanscount;
    bool q_hidden;
    QList<SvgItem *> q_svgitems;
};

#endif // QUESTION_ITEM_H
