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

#include "question_item.h"

QuestionAnswer::QuestionAnswer()
{
    qa_answered = QuestionItem::None;
    qa_correct_answer = QuestionItem::None;
}

QuestionAnswer::QuestionAnswer(QuestionItem::Answer correct, QuestionItem::Answer ans)
{
    qa_answered = ans;
    qa_correct_answer = correct;
}

void QuestionAnswer::setAnswered(QuestionItem::Answer ans) { qa_answered = ans; }

QuestionItem::Answer QuestionAnswer::answered() { return qa_answered; }

void QuestionAnswer::setCorrectAnswer(QuestionItem::Answer ans) { qa_correct_answer = ans; }

QuestionItem::Answer QuestionAnswer::correctAnswer() { return qa_correct_answer; }

bool QuestionAnswer::isAnsweredCorrectly()
{
    if (qa_answered == QuestionItem::None && qa_correct_answer != QuestionItem::None) { return false; }
    if ((qa_answered & qa_correct_answer) == qa_answered) { return true; }
    return false;
}
