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
     q_incorrectanscount = 0;
     q_correctanscount = 0;
     q_hidden = false;
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
     q_incorrectanscount = 0;
     q_correctanscount = 0;
     q_hidden = false;
}

QuestionItem::QuestionItem(QString name, int flag, QString group, int difficulty, QString text, QStringList answers, QuestionItem::Answers correctanswers, unsigned int inccount, unsigned int ccount, bool hidden)
{
     q_name = name;
     q_flag = flag;
     q_group = group;
     q_difficulty = difficulty;
     setText(text);
     q_answers = answers;
     q_correctanswers = correctanswers;
     q_incorrectanscount = inccount;
     q_correctanscount = ccount;
     q_hidden = hidden;
}

QuestionItem::~QuestionItem()
{
	for (int i = 0; i < q_svgitems.count(); ++i) {
		if (q_svgitems.at(i)) delete q_svgitems.at(i);
	}
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

bool QuestionItem::isHidden() { return q_hidden; }

QStringList QuestionItem::answers() { return q_answers; }

QuestionItem::Answer QuestionItem::correctAnswer() { return QuestionItem::Answer((int)q_correctanswers); }

QuestionItem::Answers QuestionItem::correctAnswers() { return q_correctanswers; }

QString QuestionItem::allProperties()
{
    QString out;
    // Q_NAME
    out.append("[Q_NAME]\n");
    out.append(q_name);
    // Q_FLAG
    out.append("\n[Q_FLAG]\n");
    out.append(QString("%1").arg(q_flag));
    // Q_GROUP
    out.append("\n[Q_GRP]\n");
    out.append(q_group);
    // Q_DIFFICULTY
    out.append("\n[Q_DIF]\n");
    out.append(QString("%1").arg(q_difficulty));
    // Q_TEXT
    out.append("\n[Q_TEXT]\n");
    out.append(q_text);
    // Q_ANSA
    out.append("\n[Q_ANSA]\n");
    out.append(q_answers.at(0));
    out.append("\n");
    out.append(isAnsACorrect() ? "true" : "false");
    // Q_ANSB
    out.append("\n[Q_ANSB]\n");
    out.append(q_answers.at(1));
    out.append("\n");
    out.append(isAnsBCorrect() ? "true" : "false");
    // Q_ANSC
    out.append("\n[Q_ANSC]\n");
    out.append(q_answers.at(2));
    out.append("\n");
    out.append(isAnsCCorrect() ? "true" : "false");
    // Q_ANSD
    out.append("\n[Q_ANSD]\n");
    out.append(q_answers.at(3));
    out.append("\n");
    out.append(isAnsDCorrect() ? "true" : "false");
    // STATISTICS
    out.append("\n[Q_ICCNT]\n");
    out.append(QString("%1\n%2").arg(q_incorrectanscount).arg(q_correctanscount));
    // Q_HIDDEN
    out.append("\n[Q_HID]\n");
    out.append(q_hidden ? "true" : "false");
    // Q_SVG
    out.append("\n[Q_SVG]\n");
    out.append(QString("%1").arg(q_svgitems.count()));
    for (int i = 0; i < q_svgitems.count(); ++i) {
        out.append("\n");
        out.append(q_svgitems.at(i)->text());
        out.append("\n");
        out.append(q_svgitems.at(i)->svg());
    }
    return out;
}

QString QuestionItem::allPublicProperties()
{
    QString out;
    // Q_NAME
    out.append("[Q_NAME]\n");
    out.append(q_name);
    // Q_FLAG
    out.append("\n[Q_FLAG]\n");
    out.append(QString("%1").arg(q_flag));
    // Q_GROUP
    out.append("\n[Q_GRP]\n");
    out.append(q_group);
    // Q_DIFFICULTY
    out.append("\n[Q_DIF]\n");
    out.append(QString("%1").arg(q_difficulty));
    // Q_TEXT
    out.append("\n[Q_TEXT]\n");
    out.append(q_text);
    // Q_ANSA
    out.append("\n[Q_ANSA]\n");
    out.append(q_answers.at(0));
    // Q_ANSB
    out.append("\n[Q_ANSB]\n");
    out.append(q_answers.at(1));
    // Q_ANSC
    out.append("\n[Q_ANSC]\n");
    out.append(q_answers.at(2));
    // Q_ANSD
    out.append("\n[Q_ANSD]\n");
    out.append(q_answers.at(3));
    // Q_SVG
    out.append("\n[Q_SVG]\n");
    out.append(QString("%1").arg(q_svgitems.count()));
    return out;
}

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

void QuestionItem::setIncorrectAnsCount(unsigned int count) { q_incorrectanscount = count; }

void QuestionItem::setCorrectAnsCount(unsigned int count) { q_correctanscount = count; }

void QuestionItem::setHidden(bool hidden) { q_hidden = hidden; }

unsigned int QuestionItem::incorrectAnsCount() { return q_incorrectanscount; }

unsigned int QuestionItem::correctAnsCount() { return q_correctanscount; }

void QuestionItem::addIncorrectAns() { q_incorrectanscount++; }

void QuestionItem::addCorrectAns() { q_correctanscount++; }

int QuestionItem::recommendedDifficulty()
{
    if (q_correctanscount == 0 && q_incorrectanscount == 0) { return -1; }
    else if (q_correctanscount == 0 && q_incorrectanscount > 0) { return 2; }
    long double ansratio = (long double)q_incorrectanscount / (long double)q_correctanscount;
    if (ansratio <= 0.5) { return 0; }
    else if (ansratio > 0.5 && ansratio < 2.0) { return 1; }
    else if (ansratio >= 2.0) { return 2; }
    return -1;
}

void QuestionItem::addSvgItem(SvgItem * svg) { q_svgitems << svg; }

bool QuestionItem::removeSvgItem(SvgItem * svg) { return q_svgitems.removeAll(svg) > 0; }

SvgItem * QuestionItem::removeSvgItem(int i) { return q_svgitems.takeAt(i); }

int QuestionItem::numSvgItems() { return q_svgitems.count(); }

SvgItem * QuestionItem::svgItem(int i) { return q_svgitems.at(i); }

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
    return 0;
}
