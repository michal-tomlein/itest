/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2016 Michal Tomlein

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

#include "answersview.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QRadioButton>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

AnswerView::AnswerView(int i, AnswersView *parent):
QWidget(parent) {
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 0, 0); hlayout->setSpacing(6);
    ans_radiobutton = new QRadioButton(Question::indexToLabel(i), this);
    ans_checkbox = new QCheckBox(Question::indexToLabel(i), this);
    ans_text = new QTextBrowser(this);
    ans_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ans_text->setFontPointSize(15);
    ans_text->setMaximumSize(16777215, 15 * 5);
    hlayout->addWidget(ans_radiobutton);
    hlayout->addWidget(ans_checkbox);
    hlayout->addWidget(ans_text);
}

AnswersView::AnswersView(QWidget *parent):
QWidget(parent) {
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 0, 0, 0);
#ifndef Q_OS_MAC
    vlayout->setSpacing(6);
#else
    vlayout->setSpacing(0);
#endif
    av_inputanswer_label = new QLabel(tr("Enter your answer:"), this);
    av_input_text = new QTextBrowser(this);
    av_input_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    av_input_text->setFontPointSize(15);
    av_inputanswer_label->setFont(av_input_text->font());
    av_input_text->setMaximumSize(16777215, 15 * 5);
    av_input_text->setReadOnly(false);
    vlayout->addSpacerItem(
                new QSpacerItem(20,40, QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
    vlayout->addWidget(av_inputanswer_label);
    vlayout->addWidget(av_input_text);
    av_grp_checkboxes = new QButtonGroup(this);
    av_grp_checkboxes->setExclusive(false);
    av_grp_radiobuttons = new QButtonGroup(this);
    for (int i = 0; i < 9; ++i) {
        AnswerView *ans = new AnswerView(i + 1, this);
        if (i >= 4) {
            ans->setVisible(false);
        }
        av_answers << ans;
        av_grp_checkboxes->addButton(ans->ans_checkbox);
        av_grp_radiobuttons->addButton(ans->ans_radiobutton);
        vlayout->addWidget(ans);
    }
    QObject::connect(av_grp_checkboxes, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(emitButtonReleased(QAbstractButton *)));
    QObject::connect(av_grp_radiobuttons, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(emitButtonReleased(QAbstractButton *)));
}

void AnswersView::setAnswers(const QStringList &answers, Question::Answers selected_answers, Question::SelectionType selectiontype, QList<int> ans_order)
{
    if (selectiontype == Question::OpenQuestion)
    {
        av_inputanswer_label->setVisible(true);
        av_input_text->setVisible(true);
        for (int i = 0; i < 9; ++i)
        {
            AnswerView *ans = av_answers.at(i);
            ans->setVisible(false);
        }
    } else
    {
        av_inputanswer_label->setVisible(false);
        av_input_text->setVisible(false);
    av_ans_order = ans_order;
    av_grp_radiobuttons->setExclusive(false);
    for (int i = 0; i < 9; ++i) {
        AnswerView *ans = av_answers.at(i);
        ans->ans_radiobutton->setVisible(selectiontype == Question::SingleSelection);
        ans->ans_checkbox->setVisible(selectiontype == Question::MultiSelection);
        if (i < answers.count()) {
            ans->ans_text->setText(answers.at(av_ans_order.at(i)));
            ans->ans_text->setFixedHeight(ans->ans_text->document()->size().height());
            ans->ans_checkbox->setChecked(selected_answers.testFlag(Question::indexToAnswer(av_ans_order.at(i) + 1)));
            ans->ans_radiobutton->setChecked(selected_answers.testFlag(Question::indexToAnswer(av_ans_order.at(i) + 1)));
            ans->setVisible(true);
        } else {
            ans->ans_text->clear();
            ans->ans_checkbox->setChecked(false);
            ans->ans_radiobutton->setChecked(false);
            ans->setVisible(false);
        }
    }
    av_grp_radiobuttons->setExclusive(true);
    }
}

Question::Answers AnswersView::selectedAnswers()
{
    Question::Answers selected_answers; AnswerView *ans;
    for (int i = 0; i < 9; ++i) {
        ans = av_answers.at(i);
        if (ans->isVisible() && ((ans->ans_checkbox->isVisible() && ans->ans_checkbox->isChecked()) || (ans->ans_radiobutton->isVisible() && ans->ans_radiobutton->isChecked()))) {
            selected_answers |= Question::indexToAnswer(av_ans_order.at(i) + 1);
        }
    }
    return selected_answers;
}

void AnswersView::clear()
{
    av_grp_radiobuttons->setExclusive(false);
    for (int i = 0; i < 9; ++i) {
        AnswerView *ans = av_answers.at(i);
        ans->ans_text->clear();
        ans->ans_checkbox->setChecked(false);
        ans->ans_radiobutton->setChecked(false);
        ans->setVisible(i < 4);
    }
    av_grp_radiobuttons->setExclusive(true);
}

void AnswersView::emitButtonReleased(QAbstractButton *)
{
    emit buttonReleased(selectedAnswers());
}
