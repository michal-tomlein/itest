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

#include "question_item.h"

QuestionItem::QuestionItem(const QString & name)
{
     q_name = name;
     q_flag = -1;
     q_difficulty = 0;
     q_selectiontype = Question::SingleSelection;
     q_answers << "" << "" << "" << "";
     q_correctanswers = Question::None;
     q_answer = Question::None;
     for (int i = 0; i < 9; ++i) { q_ans_order << i; }
}

void QuestionItem::shuffleAnswers()
{
    q_ans_order.clear();
    int rand;
    for (int i = 0; i < q_answers.count(); ++i) {
        do {
            rand = qrand() % q_answers.count();
        } while (q_ans_order.contains(rand));
        q_ans_order << rand;
    }
}

QList<int> QuestionItem::answerOrder() { return q_ans_order; }

Question::Answers QuestionItem::answered() { return q_answer; }

void QuestionItem::setAnswered(Question::Answers ans) { q_answer = ans; }

void QuestionItem::addSvgItem(const QString & name, const QString & svg)
{
    q_svglist << name;
    q_svgmap.insert(name, svg);
}

int QuestionItem::numSvgItems() { return q_svglist.count(); }

QString QuestionItem::svgName(int i) { return q_svglist.at(i); }

QString QuestionItem::svg(int i) { return q_svgmap.value(q_svglist.at(i), QString()); }

ScoringSystem QuestionItem::q_scoringsystem;

ScoringSystem QuestionItem::scoringSystem() { return q_scoringsystem; }

void QuestionItem::setScoringSystem(ScoringSystem sys) { q_scoringsystem = sys; }
