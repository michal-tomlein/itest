/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2008 Michal Tomlein (michal.tomlein@gmail.com)

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

#ifndef QUESTION_WIDGET_H
#define QUESTION_WIDGET_H

#include <QSvgWidget>

#include "mtmultisvgwidget.h"
#include "../shared/pass_mark.h"

class QuestionWidget : public QWidget
{
    Q_OBJECT

protected:
    void init(QuestionItem *, bool);

public:
    QuestionWidget(QuestionItem * item, bool highlight_correct_answers = true, QWidget * parent = 0):
    QWidget(parent) { init(item, highlight_correct_answers); };

private:
    QLabel * qw_lbl_icon;
    QLabel * qw_lbl_score;
    QLabel * qw_lbl_question;
    QList<QLabel *> qw_lbl_answers;
    QLabel * qw_lbl_explanation;
    MTMultiSvgWidget * qw_msw_svgs;
};

#endif // QUESTION_WIDGET_H
