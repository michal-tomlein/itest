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

#ifndef ANSWERSVIEW_H
#define ANSWERSVIEW_H

#include <QWidget>
#include <QTextBrowser>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "question_item.h"

class AnswersView;

class AnswerView : public QWidget
{
    Q_OBJECT

public:
    AnswerView(int, AnswersView *);

private:
    QTextBrowser * ans_text;
    QCheckBox * ans_checkbox;
    QRadioButton * ans_radiobutton;
    friend class AnswersView;
};

class AnswersView : public QWidget
{
    Q_OBJECT

public:
    AnswersView(QWidget * = 0);

    void setAnswers(const QStringList &, Question::Answers, Question::SelectionType, QList<int>);
    Question::Answers selectedAnswers();

public slots:
    void clear();

signals:
    void buttonReleased(Question::Answers);

protected slots:
    void emitButtonReleased(QAbstractButton *);

private:
    QList<AnswerView *> av_answers;
    QList<int> av_ans_order;
    QButtonGroup * av_grp_checkboxes;
    QButtonGroup * av_grp_radiobuttons;
};

#endif // ANSWERSVIEW_H
