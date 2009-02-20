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

#include "question_widget.h"

void QuestionWidget::init(QuestionItem * item, bool highlight_correct_answers)
{
    QHBoxLayout * mainhlayout = new QHBoxLayout(this);
    mainhlayout->setContentsMargins(6, 6, 6, 6);
    mainhlayout->setSpacing(6);
        QVBoxLayout * vlayout0 = new QVBoxLayout;
        vlayout0->setContentsMargins(0, 0, 0, 0);
        vlayout0->setSpacing(6);
        vlayout0->addStretch();
            qw_lbl_icon = new QLabel(this);
            qw_lbl_icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        vlayout0->addWidget(qw_lbl_icon);
            qw_lbl_score = new QLabel(this);
            qw_lbl_score->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        vlayout0->addWidget(qw_lbl_score);
        vlayout0->addStretch();
    mainhlayout->addLayout(vlayout0);
    mainhlayout->addSpacing(6);
        QVBoxLayout * vlayout = new QVBoxLayout;
        vlayout->setContentsMargins(0, 0, 0, 0);
        vlayout->setSpacing(6);
            qw_lbl_question = new QLabel(this);
        vlayout->addWidget(qw_lbl_question);
            qw_msw_svgs = new MTMultiSvgWidget(this);
        vlayout->addWidget(qw_msw_svgs);
            QHBoxLayout * hlayout = new QHBoxLayout;
            hlayout->setContentsMargins(0, 0, 0, 0);
            hlayout->setSpacing(6);
                QVBoxLayout * vlayout1 = new QVBoxLayout;
                vlayout1->setContentsMargins(0, 0, 0, 0);
                vlayout1->setSpacing(6);
                QVBoxLayout * vlayout2 = new QVBoxLayout;
                vlayout2->setContentsMargins(0, 0, 0, 0);
                vlayout2->setSpacing(6);
                    for (int i = 0; i < item->numAnswers(); ++i) {
                        QLabel * lbl = new QLabel(this);
                        lbl->setWordWrap(true);
                        if (item->numAnswers() % 2 == 0 ? i < item->numAnswers() / 2 : i <= item->numAnswers() / 2) { vlayout1->addWidget(lbl); }
                        else { vlayout2->addWidget(lbl); }
                        qw_lbl_answers << lbl;
                    }
                    if (item->numAnswers() % 2 != 0) { vlayout2->addStretch(); }
            hlayout->addLayout(vlayout1);
            hlayout->addLayout(vlayout2);
        vlayout->addLayout(hlayout);
            qw_lbl_explanation = new QLabel(this);
            qw_lbl_explanation->setWordWrap(true);
        vlayout->addWidget(qw_lbl_explanation);
    mainhlayout->addLayout(vlayout);

    if (!item) { return; }
    qw_lbl_icon->setVisible(highlight_correct_answers);
    qw_lbl_icon->setPixmap(QPixmap(QString::fromUtf8(item->score() > 0.0 ? ":/images/images/button_ok.png" : ":/images/images/button_cancel.png")));
    qw_lbl_score->setVisible(highlight_correct_answers);
    qw_lbl_score->setText(tr("%1 out of %2").arg(item->score()).arg(item->maximumScore()));
    qw_lbl_question->setText(item->text());
    qw_lbl_explanation->setText(item->explanation());
    qw_lbl_explanation->setVisible(!item->explanation().isEmpty());
    QFont font; QList<int> ans_order = item->answerOrder();
    for (int i = 0; i < qw_lbl_answers.count(); ++i) {
        qw_lbl_answers.at(i)->setText(QString("%1 %2").arg(Question::indexToLabel(i + 1)).arg(item->answerAtIndex(ans_order.at(i) + 1)));
        font.setBold(item->isAnswerAtIndexCorrect(ans_order.at(i) + 1) && highlight_correct_answers);
        font.setUnderline(item->answered().testFlag(Question::indexToAnswer(ans_order.at(i) + 1)));
        qw_lbl_answers.at(i)->setFont(font);
    }
    for (int i = 0; i < item->numSvgItems(); ++i) {
        QSvgWidget * svg_widget = new QSvgWidget;
        svg_widget->load(item->svg(i).toUtf8());
        QSize svg_size = svg_widget->sizeHint();
        svg_size.scale(48, 48, Qt::KeepAspectRatioByExpanding);
        svg_widget->setMinimumSize(svg_size);
        svg_widget->setMaximumSize(svg_size);
        qw_msw_svgs->addWidget(svg_widget, item->svgName(i), false);
    }
    if (item->numSvgItems() < 1) { qw_msw_svgs->setVisible(false); }
}
