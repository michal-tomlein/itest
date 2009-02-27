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

#include "answersedit.h"

AnswerEdit::AnswerEdit(int i, AnswersEdit * parent):
QWidget(parent) {
    ans_visible = this->QWidget::isVisible();
    QHBoxLayout * hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 0, 0); hlayout->setSpacing(6);
    ans_text = new QLineEdit(this);
    ans_text->setStatusTip(tr("Answer %1 of the selected question").arg(Question::indexToLabel(i + 1)));
    ans_correct = new QCheckBox(this);
    ans_correct->setStatusTip(tr("A checked checkbox indicates a correct answer"));
    ans_correct->setMaximumSize(ans_text->sizeHint().height(), ans_text->sizeHint().height());
    ans_remove = new MTToolButton(this, makeString(i));
    ans_remove->setIcon(QIcon(QString::fromUtf8(":/images/images/list-remove.png")));
    ans_remove->setStatusTip(tr("Remove this answer"));
    ans_remove->setMaximumSize(ans_text->sizeHint().height(), ans_text->sizeHint().height());
    QObject::connect(ans_remove, SIGNAL(released(QString)), parent, SLOT(removeAnswer(QString)));
    ans_label = new QLabel(Question::indexToLabel(i + 1));
    ans_label->setMinimumSize(15, 0);
    hlayout->addWidget(ans_remove);
    hlayout->addWidget(ans_label);
    hlayout->addWidget(ans_text);
    hlayout->addWidget(ans_correct);
}

void AnswerEdit::setVisible(bool visible)
{
    ans_visible = visible;
    this->QWidget::setVisible(visible);
}

bool AnswerEdit::isVisible() { return ans_visible; }

AnswersEdit::AnswersEdit(QWidget * parent):
QWidget(parent) {
    QVBoxLayout * vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 0, 0, 0);
#ifndef Q_WS_MAC
    vlayout->setSpacing(6);
#else
    vlayout->setSpacing(3);
#endif
    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0); hlayout->setSpacing(6);
    ae_add_button = new QToolButton(this);
    ae_add_button->setText(tr("Add answer"));
    ae_add_button->setIcon(QIcon(QString::fromUtf8(":/images/images/list-add.png")));
    QObject::connect(ae_add_button, SIGNAL(released()), this, SLOT(addAnswer()));
    hlayout->addWidget(ae_add_button);
    hlayout->addStretch();
    ae_answers_label = new QLabel(tr("Answers:"), this);
    hlayout->addWidget(ae_answers_label);
    hlayout->addStretch();
    ae_singleselection = new QRadioButton(tr("Single choice"), this);
    ae_singleselection->setStatusTip(tr("Single choice questions allow selecting one answer only, even if the question has more correct answers"));
    ae_singleselection->setChecked(true);
    hlayout->addWidget(ae_singleselection);
#ifdef Q_WS_MAC
#if QT_VERSION < 0x040400
    hlayout->addSpacing(6);
#endif
#endif
    ae_multiselection = new QRadioButton(tr("Multiple choice"), this);
    ae_multiselection->setStatusTip(tr("Multiple choice questions allow selecting more answers"));
    hlayout->addWidget(ae_multiselection);
    hlayout->addStretch();
    ae_correct_label = new QLabel(tr("Correct:"), this);
    hlayout->addWidget(ae_correct_label);
    vlayout->addLayout(hlayout);
#ifdef Q_WS_MAC
    QVBoxLayout * vlayout2 = new QVBoxLayout;
    vlayout2->setContentsMargins(0, 0, 0, 0); vlayout2->setSpacing(0);
    vlayout->addLayout(vlayout2);
#endif
    for (int i = 0; i < 9; ++i) {
        AnswerEdit * ans = new AnswerEdit(i, this);
        if (i >= 4) { ans->setVisible(false); }
        ae_answers << ans;
#ifndef Q_WS_MAC
        vlayout->addWidget(ans);
#else
        vlayout2->addWidget(ans);
#endif
    }
    ae_add_button->setMaximumSize(16777215, ae_answers.at(0)->ans_text->sizeHint().height());
    ae_add_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void AnswersEdit::setAnswers(const QStringList & answers)
{
    for (int i = 0; i < 9; ++i) {
        AnswerEdit * ans = ae_answers.at(i);
        if (i < answers.count()) {
            ans->ans_text->setText(answers.at(i));
            ans->setVisible(true);
        } else {
            ans->ans_text->clear();
            ans->setVisible(false);
        }
    }
    enableAddAnswerButton();
}

void AnswersEdit::setAnswers(const QStringList & answers, Question::Answers correct_answers, Question::SelectionType selectiontype)
{
    for (int i = 0; i < 9; ++i) {
        AnswerEdit * ans = ae_answers.at(i);
        if (i < answers.count()) {
            ans->ans_text->setText(answers.at(i));
            ans->ans_correct->setChecked(correct_answers.testFlag(Question::indexToAnswer(i + 1)));
            ans->setVisible(true);
        } else {
            ans->ans_text->clear();
            ans->ans_correct->setChecked(false);
            ans->setVisible(false);
        }
    }
    setSelectionType(selectiontype);
    enableAddAnswerButton();
}

QStringList AnswersEdit::answers()
{
    QStringList answers;
    for (int i = 0; i < 9; ++i) {
        if (ae_answers.at(i)->isVisible()) { answers << removeLineBreaks(ae_answers.at(i)->ans_text->text()); }
    }
    return answers;
}

void AnswersEdit::setCorrectAnswers(Question::Answers correct_answers)
{
    for (int i = 0; i < 9; ++i) {
        ae_answers.at(i)->ans_correct->setChecked(correct_answers.testFlag(Question::indexToAnswer(i + 1)));
    }
}

Question::Answers AnswersEdit::correctAnswers()
{
    Question::Answers correct_answers;
    for (int i = 0; i < 9; ++i) {
        if (ae_answers.at(i)->isVisible() && ae_answers.at(i)->ans_correct->isChecked()) {
            correct_answers |= Question::indexToAnswer(i + 1);
        }
    }
    return correct_answers;
}

void AnswersEdit::replaceAnswer(int i, const QString & text)
{
    if (i < 0 || i > 8) { return; }
    ae_answers.at(i)->ans_text->setText(text);
    ae_answers.at(i)->setVisible(true);
}

QString AnswersEdit::answer(int i)
{
    if (i < 0 || i > 8) { return QString(); }
    return removeLineBreaks(ae_answers.at(i)->ans_text->text());
}

void AnswersEdit::setSelectionType(Question::SelectionType type)
{
    switch (type) {
        case Question::SingleSelection: ae_singleselection->setChecked(true); break;
        case Question::MultiSelection: ae_multiselection->setChecked(true); break;
        default: ae_singleselection->setChecked(true); break;
    }
}

Question::SelectionType AnswersEdit::selectionType()
{
    return ae_multiselection->isChecked() ? Question::MultiSelection : Question::SingleSelection;
}

int AnswersEdit::count()
{
    int n = 0;
    for (int i = 0; i < 9; ++i) {
        if (ae_answers.at(i)->isVisible()) { n++; }
    }
    return n;
}

void AnswersEdit::addAnswer()
{
    for (int i = 0; i < 9; ++i) {
        if (!ae_answers.at(i)->isVisible()) {
            ae_answers.at(i)->ans_text->clear();
            ae_answers.at(i)->ans_correct->setChecked(false);
            ae_answers.at(i)->setVisible(true);
            enableAddAnswerButton();
            break;
        }
    }
}

void AnswersEdit::removeAnswer(const QString & id)
{
    bool ok; int i = id.toInt(&ok);
    if (ok) { removeAnswer(i); }
}

void AnswersEdit::removeAnswer(int i)
{
    if (i < 0 || i > 8) { return; }
    AnswerEdit * prev_ans = ae_answers.at(i);
    for (int n = i + 1; n < 9; ++n) {
        if (ae_answers.at(n)->isVisible()) {
            prev_ans->ans_text->setText(ae_answers.at(n)->ans_text->text());
            prev_ans->ans_correct->setChecked(ae_answers.at(n)->ans_correct->isChecked());
            prev_ans = ae_answers.at(n);
        }
    }
    prev_ans->ans_text->clear();
    prev_ans->ans_correct->setChecked(false);
    prev_ans->setVisible(false);
    enableAddAnswerButton();
}

void AnswersEdit::clear()
{
    for (int i = 0; i < 9; ++i) {
        AnswerEdit * ans = ae_answers.at(i);
        ans->ans_text->clear();
        ans->ans_correct->setChecked(false);
        ans->setVisible(i < 4);
    }
    enableAddAnswerButton();
}

void AnswersEdit::enableAddAnswerButton()
{
    int n = 0; for (int i = 0; i < 9; ++i) { if (ae_answers.at(i)->isVisible()) { n++; } }
    ae_add_button->setEnabled(n < 9);
}
