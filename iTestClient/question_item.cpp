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

QuestionItem::QuestionItem()
{
     q_name = "";
     q_flag = -1;
     q_group = "";
     q_difficulty = 0;
     q_text = "";
     q_answers << "" << "" << "" << "";
     q_correctanswers = QuestionItem::None;
     q_answer = None;
}

QuestionItem::QuestionItem(QString name)
{
     q_name = name;
     q_flag = -1;
     q_group = "";
     q_difficulty = 0;
     q_text = "";
     q_answers << "" << "" << "" << "";
     q_correctanswers = QuestionItem::None;
     q_answer = None;
}

QuestionItem::QuestionItem(QString name, int flag, QString group, int difficulty, QString text, QStringList answers)
{
     q_name = name;
     q_flag = flag;
     q_group = group;
     q_difficulty = difficulty;
     setText(text);
     q_answers = answers;
     q_correctanswers = QuestionItem::None;
     q_answer = None;
}

QString QuestionItem::name() { return q_name; }

int QuestionItem::flag() { return q_flag; }

QString QuestionItem::group() { return q_group; }

int QuestionItem::difficulty() { return q_difficulty; }

QString QuestionItem::text() { return q_text; }

QString QuestionItem::ansA() { return q_answers.at(0); }

QString QuestionItem::ansB() { return q_answers.at(1); }

QString QuestionItem::ansC() { return q_answers.at(2); }

QString QuestionItem::ansD() { return q_answers.at(3); }

bool QuestionItem::isAnsCorrect(QuestionItem::Answer ans)
{
    return q_correctanswers.testFlag(ans);
}

bool QuestionItem::isAnsACorrect()
{
    return q_correctanswers.testFlag(QuestionItem::A);
}

bool QuestionItem::isAnsBCorrect()
{
    return q_correctanswers.testFlag(QuestionItem::B);
}

bool QuestionItem::isAnsCCorrect()
{
    return q_correctanswers.testFlag(QuestionItem::C);
}

bool QuestionItem::isAnsDCorrect()
{
    return q_correctanswers.testFlag(QuestionItem::D);
}

QStringList QuestionItem::answers() { return q_answers; }

QuestionItem::Answer QuestionItem::correctAnswer() { return QuestionItem::Answer((int)q_correctanswers); }

QuestionItem::Answers QuestionItem::correctAnswers() { return q_correctanswers; }

void QuestionItem::setName(QString name) { q_name = name; }

void QuestionItem::setFlag(int flag) { q_flag = flag; }

void QuestionItem::setGroup(QString group) { q_group = group; }

void QuestionItem::setDifficulty(int difficulty) { q_difficulty = difficulty; }

void QuestionItem::setText(QString text)
{
	QTextDocument doc; doc.setHtml(text); QString final = text; QString lastgood; QTextDocument testdoc;
	QStringList before; QString after = "font-size:10pt;";
	before << "font-size:8.25pt;" << "font-size:8pt;" << "font-size:9pt;";
	for (int b = 0; b < before.count(); ++b) {
		int skip = 0; int c = text.count(before.at(b), Qt::CaseInsensitive);
		for (int i = 0; i < c; ++i) {
			lastgood = final;
			if (final.contains(before.at(b), Qt::CaseInsensitive)) {
				final.replace(final.indexOf(before.at(b), skip), before.at(b).count(), after);
				testdoc.setHtml(final);
				if (doc.toPlainText() != testdoc.toPlainText()) {
					skip = final.indexOf(before.at(b), skip) + 10;
					final = lastgood;
				}
			}
		}
	}
	q_text = final;
}

void QuestionItem::setAnsA(QString ans) { q_answers.replace(0, ans); }

void QuestionItem::setAnsB(QString ans) { q_answers.replace(1, ans); }

void QuestionItem::setAnsC(QString ans) { q_answers.replace(2, ans); }

void QuestionItem::setAnsD(QString ans) { q_answers.replace(3, ans); }

void QuestionItem::setAnsCorrect(QuestionItem::Answers ans, bool correct)
{
    if (correct) { q_correctanswers |= ans; }
    else { q_correctanswers &= ~ans; }
}

void QuestionItem::setAnsACorrect(bool correct)
{
    if (correct) { q_correctanswers |= QuestionItem::A; }
    else { q_correctanswers &= ~QuestionItem::A; }
}

void QuestionItem::setAnsBCorrect(bool correct)
{
    if (correct) { q_correctanswers |= QuestionItem::B; }
    else { q_correctanswers &= ~QuestionItem::B; }
}

void QuestionItem::setAnsCCorrect(bool correct)
{
    if (correct) { q_correctanswers |= QuestionItem::C; }
    else { q_correctanswers &= ~QuestionItem::C; }
}

void QuestionItem::setAnsDCorrect(bool correct)
{
    if (correct) { q_correctanswers |= QuestionItem::D; }
    else { q_correctanswers &= ~QuestionItem::D; }
}

void QuestionItem::setAnswers(QStringList answers) { q_answers = answers; }

void QuestionItem::setCorrectAnswers(QuestionItem::Answers answers) { q_correctanswers = answers; }

bool QuestionItem::hasCorrectAnswer()
{
    return q_correctanswers != QuestionItem::None;
}

QuestionItem::Answer QuestionItem::answered() { return q_answer; }

void QuestionItem::setAnswered(Answer ans) { q_answer = ans; }

bool QuestionItem::isAnsweredCorrectly()
{
    switch (q_answer) {
        case None: return (!hasCorrectAnswer()); break;
        case A: return (isAnsACorrect()); break;
        case B: return (isAnsBCorrect()); break;
        case C: return (isAnsCCorrect()); break;
        case D: return (isAnsDCorrect()); break;
    }
    return false;
}

void QuestionItem::addSvgItem(QString name, QString svg)
{
    q_svglist << name;
    q_svgmap.insert(name, svg);
}

int QuestionItem::numSvgItems() { return q_svglist.count(); }

QString QuestionItem::svgName(int i ) { return q_svglist.at(i); }

QString QuestionItem::svg(int i) { return q_svgmap.value(q_svglist.at(i), QString()); }

QuestionItem::Answer QuestionItem::convertOldAnsNumber(int num)
{
    switch (num) {
        case 0: return QuestionItem::None; break;
        case 1: return QuestionItem::A; break;
        case 2: return QuestionItem::B; break;
        case 3: return QuestionItem::C; break;
        case 4: return QuestionItem::D; break;
    }
    return (QuestionItem::Answer)num;
}

int QuestionItem::convertToOldAnsNumber(int num)
{
    switch (num) {
        case 0: return 0; break;
        case 1: return 1; break;
        case 2: return 2; break;
        case 4: return 3; break;
        case 8: return 4; break;
    }
    return num;
}
