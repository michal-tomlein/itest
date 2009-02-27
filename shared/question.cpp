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

#include <cmath>
#include "pass_mark.h"

Question::Question(const QString & name)
{
    q_name = name;
    q_flag = -1;
    q_difficulty = 0;
    q_selectiontype = Question::SingleSelection;
    q_answers << "" << "" << "" << "";
    q_correctanswers = Question::None;
}

QString Question::name() { return q_name; }

void Question::setName(const QString & name) { q_name = name; }

int Question::flag() { return q_flag; }

void Question::setFlag(int flag) { q_flag = flag; }

QString Question::group() { return q_group; }

void Question::setGroup(const QString & group) { q_group = group; }

int Question::difficulty() { return q_difficulty; }

void Question::setDifficulty(int difficulty) { q_difficulty = difficulty; }

QString Question::text() { return q_text; }

void Question::setText(const QString & text)
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

QString Question::explanation() { return q_explanation; }

void Question::setExplanation(const QString & explanation) { q_explanation = explanation; }

Question::SelectionType Question::selectionType() { return q_selectiontype; }

void Question::setSelectionType(Question::SelectionType type) { q_selectiontype = type; }

QString Question::answer(Question::Answer i) { return q_answers.at(answerToIndex(i) - 1); }

void Question::setAnswer(Question::Answer i, const QString & ans)
{
    setAnswerAtIndex(answerToIndex(i), ans);
}

QString Question::answerAtIndex(int i) { return q_answers.at(i - 1); }

void Question::setAnswerAtIndex(int i, const QString & ans)
{
    i--;
    if (i >= 0 && i < q_answers.count()) { q_answers.replace(i, ans); }
    else if (i >= q_answers.count() && i < 9) { q_answers.append(ans); }
}

void Question::addAnswer(const QString & ans)
{
    if (q_answers.count() < 9) { q_answers.append(ans); }
}

int Question::numAnswers() { return q_answers.count(); }

bool Question::hasCorrectAnswer()
{
    return q_correctanswers != Question::None;
}

bool Question::isAnswerAtIndexCorrect(int i)
{
    return q_correctanswers.testFlag(indexToAnswer(i));
}

bool Question::isAnswerCorrect(Question::Answer ans)
{
    return q_correctanswers.testFlag(ans);
}

void Question::setAnswerCorrect(Question::Answers ans, bool correct)
{
    if (correct) { q_correctanswers |= ans; }
    else { q_correctanswers &= ~ans; }
}

QStringList Question::answers() { return q_answers; }

void Question::setAnswers(const QStringList & answers) { q_answers = answers; }

Question::Answer Question::correctAnswer() { return Question::Answer((int)q_correctanswers); }

Question::Answers Question::correctAnswers() { return q_correctanswers; }

void Question::setCorrectAnswers(Question::Answers answers) { q_correctanswers = answers; }

int Question::answerToIndex(Question::Answer ans)
{
    switch (ans) {
        case Question::None: return 0; break;
        case Question::A: return 1; break;
        case Question::B: return 2; break;
        case Question::C: return 3; break;
        case Question::D: return 4; break;
        case Question::E: return 5; break;
        case Question::F: return 6; break;
        case Question::G: return 7; break;
        case Question::H: return 8; break;
        case Question::I: return 9; break;
    }
    return 0;
}

Question::Answer Question::indexToAnswer(int i)
{
    if (i == 0) { return Question::None; }
    return (Question::Answer)pow(2, i - 1);
}

int Question::labelToIndex(const QString & label)
{
    if (label == QObject::tr("a)")) { return 1; }
    else if (label == QObject::tr("b)")) { return 2; }
    else if (label == QObject::tr("c)")) { return 3; }
    else if (label == QObject::tr("d)")) { return 4; }
    else if (label == QObject::tr("e)")) { return 5; }
    else if (label == QObject::tr("f)")) { return 6; }
    else if (label == QObject::tr("g)")) { return 7; }
    else if (label == QObject::tr("h)")) { return 8; }
    else if (label == QObject::tr("i)")) { return 9; }
    return 0;
}

QString Question::indexToLabel(int i)
{
    switch (i) {
        case 0: return QObject::tr("None"); break;
        case 1: return QObject::tr("a)"); break;
        case 2: return QObject::tr("b)"); break;
        case 3: return QObject::tr("c)"); break;
        case 4: return QObject::tr("d)"); break;
        case 5: return QObject::tr("e)"); break;
        case 6: return QObject::tr("f)"); break;
        case 7: return QObject::tr("g)"); break;
        case 8: return QObject::tr("h)"); break;
        case 9: return QObject::tr("i)"); break;
    }
    return QObject::tr("None");
}

QString Question::answerToString(Question::Answer ans)
{
    QStringList ans_list; Answers a(ans);
    for (int i = 1; i <= 9; ++i) {
        if (a.testFlag(indexToAnswer(i))) { ans_list << indexToLabel(i); }
    }
    if (ans_list.count() == 0) { ans_list << indexToLabel(0); }
    return ans_list.join(", ");
}

Question::Answer Question::convertOldAnsNumber(int num)
{
    switch (num) {
        case 0: return Question::None; break;
        case 1: return Question::A; break;
        case 2: return Question::B; break;
        case 3: return Question::C; break;
        case 4: return Question::D; break;
    }
    return (Question::Answer)num;
}

int Question::convertToOldAnsNumber(int num)
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

void ScoringSystem::init()
{
    for (int i = 0; i < 3; ++i) {
        correctAnswer[i] = 1.0;
        incorrectAnswer[i] = 0.0;
        missingAnswer[i] = 0.0;
    }
    allowIncompleteAnswers = true;
}

ScoringSystem::ScoringSystem() { init(); }

ScoringSystem::ScoringSystem(const QString & str) { loadData(str); }

void ScoringSystem::loadData(QString str)
{
    QTextStream in(&str);
	if (in.readLine() != "[SCORSYS]") { init(); return; }
	allowIncompleteAnswers = in.readLine() == "true";
	QStringList bufferlist = in.readLine().split(";");
    for (int i = 0; i < 3; ++i) {
        correctAnswer[i] = bufferlist.takeFirst().toDouble();
        incorrectAnswer[i] = bufferlist.takeFirst().toDouble();
        missingAnswer[i] = bufferlist.takeFirst().toDouble();
    }
}

#ifdef ITESTSERVER
QString ScoringSystem::data()
{
    QString out;
	out.append("[SCORSYS]\n");
	out.append(allowIncompleteAnswers ? "true\n" : "false\n");
	for (int i = 0; i < 3; ++i) {
		out.append(QString("%1;").arg(correctAnswer[i]));
		out.append(QString("%1;").arg(incorrectAnswer[i]));
        out.append(QString("%1").arg(missingAnswer[i]));
        if (i < 2) { out.append(";"); }
	}
	return out;
}
#endif

#ifdef ITESTSERVER
double QuestionAnswer::score(ScoringSystem q_scoringsystem)
#endif
#ifdef ITESTCLIENT
double QuestionItem::score()
#endif
{
#ifdef ITESTSERVER
    Question::Answers q_answer(qa_answered);
    Question::Answers q_correctanswers(qa_correct_answer);
    int num_answers = 9;
#else
    int num_answers = q_answers.count();
#endif
    if (q_scoringsystem.allowIncompleteAnswers && q_selectiontype == Question::MultiSelection) {
        double score = 0.0;
        if (q_correctanswers == Question::None && q_answer == Question::None) { score = q_scoringsystem.correctAnswer[q_difficulty]; }
        else {
            int numcorrect = 0; int max = 0;
            for (int i = 1; i <= num_answers; ++i) {
                if (q_correctanswers.testFlag(Question::indexToAnswer(i)) && q_answer.testFlag(Question::indexToAnswer(i)))
                    { max++; numcorrect++; }
                else if (q_correctanswers.testFlag(Question::indexToAnswer(i)) && !q_answer.testFlag(Question::indexToAnswer(i)))
                    { max++; score += q_scoringsystem.missingAnswer[q_difficulty]; }
                else if (!q_correctanswers.testFlag(Question::indexToAnswer(i)) && q_answer.testFlag(Question::indexToAnswer(i)))
                    { score += q_scoringsystem.incorrectAnswer[q_difficulty]; }
            }
            if (max != 0) { score += q_scoringsystem.correctAnswer[q_difficulty] * (double)numcorrect / (double)max; }
        }
        return score;
    } else {
        if (q_answer == Question::None) { return q_correctanswers == Question::None ? q_scoringsystem.correctAnswer[q_difficulty] : q_scoringsystem.incorrectAnswer[q_difficulty]; }
        if (q_selectiontype == Question::MultiSelection) { return q_correctanswers == q_answer ? q_scoringsystem.correctAnswer[q_difficulty] : q_scoringsystem.incorrectAnswer[q_difficulty]; }
        return q_correctanswers.testFlag((Question::Answer)(int)q_answer) ? q_scoringsystem.correctAnswer[q_difficulty] : q_scoringsystem.incorrectAnswer[q_difficulty];
    }
    return 0.0;
}

#ifdef ITESTSERVER
double QuestionAnswer::maximumScore(ScoringSystem q_scoringsystem)
#endif
#ifdef ITESTCLIENT
double QuestionItem::maximumScore()
#endif
{
    return q_scoringsystem.correctAnswer[q_difficulty];
}

QList<int> Question::randomise(QList<Question *> questions, PassMark passmark, bool use_groups, int qnum, quint32 id, QProgressDialog * progress, QApplication * app)
{
    QList<int> final_randlist; QList<QString> groups;
    if (passmark.count() <= 0) {
    	if (progress != NULL) { progress->setMaximum(qnum); }
    	int rand;
    	for (int i = 0; i < qnum; ++i) {
    		random_0:
    		do {
    			rand = (qrand() + id) % questions.size();
    		} while (final_randlist.contains(rand));
    		if (use_groups) {
    		    if (!questions.at(rand)->group().isEmpty()) {
    		    	if (groups.contains(questions.at(rand)->group())) { goto random_0; }
    		    	else { groups << questions.at(rand)->group(); }
    		    }
    		}
    		final_randlist << rand;
    		if (progress != NULL) { progress->setValue(i + 1); } // PROGRESS >>>>>>>
    		if (app != NULL) { app->processEvents(); }
    	}
    } else {
    	if (progress != NULL) { progress->setMaximum(questions.size() + passmark.count()); }
    	QList<Question *> qflist[passmark.count()];
    	QList<Question *> unusedqlist; int x;
    	for (int i = 0; i < questions.size(); ++i) {
    		if (passmark.conditionIndex(questions.at(i)->flag()) != -1) {
    			qflist[passmark.conditionIndex(questions.at(i)->flag())] << questions.at(i);
    		} else {
    			unusedqlist << questions.at(i);
    		}
    		if (progress != NULL) { progress->setValue(i + 1); } // PROGRESS >>>>>>>
    		if (app != NULL) { app->processEvents(); }
    	}
        int y = qnum;
    	for (int c = 0; c < passmark.count(); ++c) {
            y -= passmark.qnum(c);
    		int rand; QList<int> randlist;
    		x = passmark.conditionIndex(passmark.condition(c));
    		for (int i = 0; i < passmark.qnum(c); ++i) {
    			random_1:
    			do {
    			    rand = (qrand() + id) % qflist[x].size();
    			} while (randlist.contains(rand));
    			if (use_groups) {
    			    if (!qflist[x].at(rand)->group().isEmpty()) {
    			    	if (groups.contains(qflist[x].at(rand)->group())) { goto random_1; }
    			    	else { groups << qflist[x].at(rand)->group(); }
    			    }
    			}
    			randlist << rand;
    			if (app != NULL) { app->processEvents(); }
    		}
    		for (int i = 0; i < qflist[x].size(); ++i) {
    			if (!randlist.contains(i)) { unusedqlist << qflist[x].at(i); }
    		}
            for (int i = 0; i < randlist.count(); ++i) {
                final_randlist << questions.indexOf(qflist[x].at(randlist.at(i)));
            }
    		if (progress != NULL) { progress->setValue(c + questions.size() + 1); } // PROGRESS
    		if (app != NULL) { app->processEvents(); }
    	}
    	int rand; QList<int> randlist;
    	int z = questions.size() + passmark.count();
    	for (int i = 0; i < y; ++i) {
    		random_2:
    	    do {
    	    	rand = (qrand() + id) % unusedqlist.size();
    	    } while (randlist.contains(rand));
    	    if (use_groups) {
    	    	if (!unusedqlist.at(rand)->group().isEmpty()) {
    	    		if (groups.contains(unusedqlist.at(rand)->group())) { goto random_2; }
    	        	else { groups << unusedqlist.at(rand)->group(); }
    	        }
    	    }
    	    randlist << rand;
    	    if (progress != NULL) { progress->setValue(i + z + 1); } // PROGRESS >>>
    	    if (app != NULL) { app->processEvents(); }
    	}
        for (int i = 0; i < randlist.count(); ++i) {
            final_randlist << questions.indexOf(unusedqlist.at(randlist.at(i)));
        }
    }
    return final_randlist;
}
