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

#ifndef ANSWERSEDIT_H
#define ANSWERSEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "mttoolbutton.h"
#include "question_item.h"

inline QString removeLineBreaks(QString str) { return str.remove("\n"); }

class AnswersEdit;

class AnswerEdit : public QWidget
{
    Q_OBJECT

public:
    AnswerEdit(int, AnswersEdit *);

public slots:
    void setVisible(bool);
    bool isVisible();

private:
    QLineEdit * ans_text;
    QCheckBox * ans_correct;
    MTToolButton * ans_remove;
    QLabel * ans_label;
    bool ans_visible;
    friend class AnswersEdit;
};

class AnswersEdit : public QWidget
{
    Q_OBJECT

public:
    AnswersEdit(QWidget * = 0);

    void setAnswers(const QStringList &);
    void setAnswers(const QStringList &, Question::Answers, Question::SelectionType = Question::SingleSelection);
    QStringList answers();
    void setCorrectAnswers(Question::Answers);
    Question::Answers correctAnswers();
    void replaceAnswer(int, const QString &);
    QString answer(int);
    void setSelectionType(Question::SelectionType);
    Question::SelectionType selectionType();
    int count();

public slots:
    void addAnswer();
    void removeAnswer(const QString &);
    void removeAnswer(int);
    void clear();

protected:
    void enableAddAnswerButton();

private:
    QList<AnswerEdit *> ae_answers;
    QToolButton * ae_add_button;
    QRadioButton * ae_singleselection;
    QRadioButton * ae_multiselection;
    QLabel * ae_answers_label;
    QLabel * ae_correct_label;
};

#endif // ANSWERSEDIT_H
