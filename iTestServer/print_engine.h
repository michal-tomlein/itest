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

#ifndef PRINT_ENGINE_H
#define PRINT_ENGINE_H

#include "session_wizard.h"
#include "mtlistwidget.h"
#include "mttablewidget.h"
#include "mtspinbox.h"

#include <QCheckBox>
#include <QSpinBox>
#include <QHeaderView>

class PrintQuestionsDialogue : public QWidget
{
    Q_OBJECT

public:
    PrintQuestionsDialogue(MainWindow *);

    MTTableWidget * includeTableWidget() { return printq_includelist; };
    bool flagsSelected() { return rbtngrpPrintqSelect->checkedButton()->text() == tr("Flags"); };
    bool questionsSelected() { return rbtngrpPrintqSelect->checkedButton()->text() == tr("Questions"); };
    bool printStatistics() { return printTest() ? false : printq_advanced_statistics->isChecked(); };
    bool printFormatting() { return printTest() ? true : printq_advanced_formatting->isChecked(); };
    bool printTest() { return printq_advanced_test->isChecked(); };
    bool printKey() { return printTest() ? printq_advanced_key->isChecked() : false; };
    bool printGraphics() { return printq_advanced_graphics->isChecked(); };
    bool randomise() { return printq_advanced_randomise->isChecked(); };
    int numPrintouts() { return randomise() ? printq_advanced_numprintouts->value() : 1; };
    bool useGroups() { return printTest() ? printq_advanced_usegroups->isChecked() : false; };
    int numQuestions() { return printq_advanced_numquestions->value(); };

private slots:
    void addQuestionToPrint();
    void removeQuestionToPrint();
    void addAllQuestionsToPrint();
    void togglePrintSelection(QAbstractButton *);
    void togglePrintEnabled();
    void resetDefaultValues();
    void updateTestQnum();
    void printQuestions();

private:
    void addQuestionToPrint(int);

    MTListWidget * printq_excludelist;
	MTTableWidget * printq_includelist;
	QPushButton * printq_btn_print;
	QButtonGroup * rbtngrpPrintqSelect;
	QCheckBox * printq_advanced_statistics;
	QCheckBox * printq_advanced_formatting;
	QCheckBox * printq_advanced_test;
    QCheckBox * printq_advanced_key;
	QCheckBox * printq_advanced_graphics;
    QCheckBox * printq_advanced_randomise;
    QSpinBox * printq_advanced_numprintouts;
    QCheckBox * printq_advanced_usegroups;
    MTSpinBox * printq_advanced_numquestions;
    MainWindow * printq_parent;
};

#endif // PRINT_ENGINE_H
