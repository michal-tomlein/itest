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

#include "main_window.h"

PrintQuestionsDialogue::PrintQuestionsDialogue(MainWindow * parent):
QWidget(parent, Qt::Dialog /*| Qt::WindowMaximizeButtonHint*/)
{
        printq_parent = parent;
	this->setWindowModality(Qt::WindowModal);
	this->setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAC
	this->setWindowTitle(tr("%1 - Print questions").arg(printq_parent->current_db_name));
#else
    this->setWindowTitle(tr("%1 - Print questions - iTest").arg(printq_parent->current_db_name));
#endif
	this->setMinimumSize(QSize(640, 480));
	rbtngrpPrintqSelect = new QButtonGroup(this);
	QObject::connect(rbtngrpPrintqSelect, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(togglePrintSelection(QAbstractButton *)));
	QGridLayout * printq_glayout = new QGridLayout(this);
		QHBoxLayout * printq_hlayout0 = new QHBoxLayout(this);
		printq_hlayout0->setMargin(0); printq_hlayout0->setSpacing(6);
			QLabel * printq_label = new QLabel(this);
			printq_label->setText(tr("Select:"));
		printq_hlayout0->addWidget(printq_label);
			QRadioButton * printq_rbtn_flags = new QRadioButton(this);
			printq_rbtn_flags->setText(tr("Flags"));
			printq_rbtn_flags->setChecked(true);
			rbtngrpPrintqSelect->addButton(printq_rbtn_flags);
		printq_hlayout0->addWidget(printq_rbtn_flags);
			QRadioButton * printq_rbtn_questions = new QRadioButton(this);
			printq_rbtn_questions->setText(tr("Questions"));
			printq_rbtn_flags->setChecked(false);
			rbtngrpPrintqSelect->addButton(printq_rbtn_questions);
		printq_hlayout0->addWidget(printq_rbtn_questions);
		printq_hlayout0->addStretch();
            QLabel * printq_label_hidden = new QLabel(this);
            printq_label_hidden->setForegroundRole(QPalette::Mid);
        printq_hlayout0->addWidget(printq_label_hidden);
	printq_glayout->addLayout(printq_hlayout0, 0, 0);
	    QHBoxLayout * printq_hlayout1 = new QHBoxLayout(this);
	    printq_hlayout1->setMargin(0); printq_hlayout1->setSpacing(6);
	        QVBoxLayout * printq_vlayout1 = new QVBoxLayout(this);
	            QLabel * printq_label_do_not_print = new QLabel(this);
	            printq_label_do_not_print->setText(tr("<b>Do not print:</b>"));
	        printq_vlayout1->addWidget(printq_label_do_not_print);
	            QHBoxLayout * printq_hlayout1_1 = new QHBoxLayout(this);
	                QLabel * printq_label_search_excluded = new QLabel(this);
	                printq_label_search_excluded->setText(tr("Search:"));
	            printq_hlayout1_1->addWidget(printq_label_search_excluded);
	                ExtendedLineEdit * printq_search_excluded = new ExtendedLineEdit(this);
	            printq_hlayout1_1->addWidget(printq_search_excluded);
	        printq_vlayout1->addLayout(printq_hlayout1_1);
	            printq_excludelist = new MTListWidget(this);
                printq_excludelist->setDragDropMode(QAbstractItemView::InternalMove);
	            QObject::connect(printq_excludelist, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(addQuestionToPrint()));
                QObject::connect(printq_search_excluded, SIGNAL(textChanged(QLineEdit *, const QString &)), printq_excludelist, SLOT(filterItems(QLineEdit *, const QString &)));
	        printq_vlayout1->addWidget(printq_excludelist);
	    printq_hlayout1->addLayout(printq_vlayout1);
	        QVBoxLayout * printq_vlayout2 = new QVBoxLayout(this);
	        printq_vlayout2->addStretch();
	            QPushButton * printq_add = new QPushButton(this);
	            printq_add->setEnabled(false);
	            printq_add->setIcon(QIcon(QString::fromUtf8(":/images/images/forward.png")));
	            QObject::connect(printq_add, SIGNAL(released()), this, SLOT(addQuestionToPrint()));
	            QObject::connect(printq_excludelist, SIGNAL(currentIndexAvailabilityChanged(bool)), printq_add, SLOT(setEnabled(bool)));
	        printq_vlayout2->addWidget(printq_add);
	            QPushButton * printq_remove = new QPushButton(this);
	            printq_remove->setEnabled(false);
	            printq_remove->setIcon(QIcon(QString::fromUtf8(":/images/images/back.png")));
	            QObject::connect(printq_remove, SIGNAL(released()), this, SLOT(removeQuestionToPrint()));
	        printq_vlayout2->addWidget(printq_remove);
	        printq_vlayout2->addStretch();
	    printq_hlayout1->addLayout(printq_vlayout2);
	        QVBoxLayout * printq_vlayout3 = new QVBoxLayout(this);
	            QLabel * printq_label_print = new QLabel(this);
	            printq_label_print->setText(tr("<b>Print:</b>"));
	        printq_vlayout3->addWidget(printq_label_print);
	            QHBoxLayout * printq_hlayout1_2 = new QHBoxLayout(this);
	                QLabel * printq_label_search_included = new QLabel(this);
	                printq_label_search_included->setText(tr("Search:"));
	            printq_hlayout1_2->addWidget(printq_label_search_included);
	                ExtendedLineEdit * printq_search_included = new ExtendedLineEdit(this);
	            printq_hlayout1_2->addWidget(printq_search_included);
	        printq_vlayout3->addLayout(printq_hlayout1_2);
	            printq_includelist = new MTTableWidget(this);
                printq_includelist->setShowGrid(false);
                printq_includelist->setColumnCount(2);
                printq_includelist->verticalHeader()->hide();
                printq_includelist->horizontalHeader()->hide();
                printq_includelist->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
                printq_includelist->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
	            QObject::connect(printq_includelist, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(removeQuestionToPrint()));
	            QObject::connect(printq_includelist, SIGNAL(currentIndexAvailabilityChanged(bool)), printq_remove, SLOT(setEnabled(bool)));
                QObject::connect(printq_search_included, SIGNAL(textChanged(QLineEdit *, const QString &)), printq_includelist, SLOT(filterItems(QLineEdit *, const QString &)));
	        printq_vlayout3->addWidget(printq_includelist);
	    printq_hlayout1->addLayout(printq_vlayout3);
	printq_glayout->addLayout(printq_hlayout1, 1, 0);
	    QHBoxLayout * printq_hlayout2 = new QHBoxLayout(this);
	    printq_hlayout2->setMargin(0); printq_hlayout2->setSpacing(6);
			QPushButton * printq_btn_add_all = new QPushButton(this);
			printq_btn_add_all->setText(tr("&Add all"));
			QObject::connect(printq_btn_add_all, SIGNAL(released()), this, SLOT(addAllQuestionsToPrint()));
		printq_hlayout2->addWidget(printq_btn_add_all);
	    printq_hlayout2->addStretch();
	        printq_btn_print = new QPushButton(tr("&Print"), this);
	        QObject::connect(printq_btn_print, SIGNAL(released()), this, SLOT(printQuestions()));
	        printq_btn_print->setEnabled(false);
	    printq_hlayout2->addWidget(printq_btn_print);
	        QPushButton * printq_btn_close = new QPushButton(tr("&Close"), this);
	        QObject::connect(printq_btn_close, SIGNAL(released()), this, SLOT(close()));
	    printq_hlayout2->addWidget(printq_btn_close);
	printq_glayout->addLayout(printq_hlayout2, 2, 0);
	    MTAdvancedGroupBox * printq_advanced = new MTAdvancedGroupBox(this);
	        printq_advanced_statistics = new QCheckBox(tr("Print statistics"), this);
	        printq_advanced_statistics->setChecked(true);
	    printq_advanced->addWidget(printq_advanced_statistics, 0, 0, 1, 2);
	        printq_advanced_formatting = new QCheckBox(tr("Print formatted questions"), this);
	        printq_advanced_formatting->setChecked(true);
	    printq_advanced->addWidget(printq_advanced_formatting, 1, 0, 1, 2);
            printq_advanced_graphics = new QCheckBox(tr("Print graphics"), this);
	        printq_advanced_graphics->setChecked(false);
	    printq_advanced->addWidget(printq_advanced_graphics, 2, 0, 1, 2);
	        printq_advanced_test = new QCheckBox(tr("Print a test (do not highlight the correct answers)"), this);
	        printq_advanced_test->setChecked(false);
            QObject::connect(printq_advanced_test, SIGNAL(toggled(bool)), this, SLOT(resetDefaultValues()));
	        QObject::connect(printq_advanced_test, SIGNAL(toggled(bool)), printq_advanced_statistics, SLOT(setDisabled(bool)));
	        QObject::connect(printq_advanced_test, SIGNAL(toggled(bool)), printq_advanced_formatting, SLOT(setDisabled(bool)));
	    printq_advanced->addWidget(printq_advanced_test, 3, 0, 1, 2);
            printq_advanced_key = new QCheckBox(tr("Print a key to the test (a separate printout with correct answers)"), this);
	        printq_advanced_key->setChecked(false);
            printq_advanced_key->setEnabled(false);
            QObject::connect(printq_advanced_test, SIGNAL(toggled(bool)), printq_advanced_key, SLOT(setEnabled(bool)));
	    printq_advanced->addWidget(printq_advanced_key, 4, 0, 1, 2);
            printq_advanced_usegroups = new QCheckBox(tr("Allow one question per group only"), this);
	        printq_advanced_usegroups->setChecked(false);
            printq_advanced_usegroups->setEnabled(false);
            QObject::connect(printq_advanced_test, SIGNAL(toggled(bool)), printq_advanced_usegroups, SLOT(setEnabled(bool)));
            QObject::connect(printq_advanced_usegroups, SIGNAL(toggled(bool)), this, SLOT(updateTestQnum()));
	    printq_advanced->addWidget(printq_advanced_usegroups, 5, 0, 1, 2);
            printq_advanced_randomise = new QCheckBox(tr("Randomise question order in each printout"), this);
	        printq_advanced_randomise->setChecked(false);
            QObject::connect(printq_advanced_randomise, SIGNAL(toggled(bool)), this, SLOT(resetDefaultValues()));
	    printq_advanced->addWidget(printq_advanced_randomise, 6, 0, 1, 2);
            QLabel * printq_label_numprintouts = new QLabel(this);
            printq_label_numprintouts->setText(tr("Number of different printouts:"));
            printq_label_numprintouts->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            printq_label_numprintouts->setEnabled(false);
            QObject::connect(printq_advanced_randomise, SIGNAL(toggled(bool)), printq_label_numprintouts, SLOT(setEnabled(bool)));
        printq_advanced->addWidget(printq_label_numprintouts, 7, 0, 1, 1);
            printq_advanced_numprintouts = new QSpinBox(this);
            printq_advanced_numprintouts->setMinimum(1);
            printq_advanced_numprintouts->setMaximum(9999);
            printq_advanced_numprintouts->setValue(1);
            printq_advanced_numprintouts->setMaximumSize(printq_advanced_numprintouts->sizeHint());
            printq_advanced_numprintouts->setEnabled(false);
            QObject::connect(printq_advanced_randomise, SIGNAL(toggled(bool)), printq_advanced_numprintouts, SLOT(setEnabled(bool)));
        printq_advanced->addWidget(printq_advanced_numprintouts, 7, 1, 1, 1);
            QLabel * printq_label_numquestions = new QLabel(this);
            printq_label_numquestions->setText(tr("Number of questions:"));
            printq_label_numquestions->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            printq_label_numquestions->setEnabled(false);
            QObject::connect(printq_advanced_randomise, SIGNAL(toggled(bool)), printq_label_numquestions, SLOT(setEnabled(bool)));
        printq_advanced->addWidget(printq_label_numquestions, 8, 0, 1, 1);
            printq_advanced_numquestions = new MTSpinBox(this);
            printq_advanced_numquestions->setMinimum(0);
            printq_advanced_numquestions->setMaximum(9999);
            printq_advanced_numquestions->setValue(0);
            printq_advanced_numquestions->setMaximumSize(printq_advanced_numquestions->sizeHint());
            printq_advanced_numquestions->setEnabled(false);
            QObject::connect(printq_advanced_randomise, SIGNAL(toggled(bool)), printq_advanced_numquestions, SLOT(setEnabled(bool)));
            QObject::connect(printq_advanced_randomise, SIGNAL(toggled(bool)), printq_advanced_numquestions, SLOT(setMaximumValue()));
            QObject::connect(printq_advanced_numquestions, SIGNAL(valueChanged(int)), this, SLOT(togglePrintEnabled()));
        printq_advanced->addWidget(printq_advanced_numquestions, 8, 1, 1, 1);
        printq_advanced->gridLayout()->setColumnStretch(2, 1);
	printq_glayout->addWidget(printq_advanced, 3, 0);
	printq_glayout->setMargin(6); printq_glayout->setSpacing(6);
        int num_hidden = 0;
        for (int i = 0; i < printq_parent->LQListWidget->count(); ++i) {
                if (printq_parent->current_db_questions.value(printq_parent->LQListWidget->item(i))->isHidden()) { num_hidden++; }
        }
        printq_label_hidden->setText(printq_parent->actionShow_hidden->isChecked() ?
                tr("%n hidden question(s) listed", "", num_hidden) : tr("%n hidden question(s) not listed", "", num_hidden));
	togglePrintSelection(printq_rbtn_flags);
        updateTestQnum();
	this->show();
}

void PrintQuestionsDialogue::togglePrintSelection(QAbstractButton * rbtn)
{
    if (rbtn->text() == tr("Flags")) {
        printq_excludelist->clear();
        printq_includelist->clearContents();
        printq_includelist->setRowCount(0);
        printq_includelist->setColumnCount(2);
        printq_includelist->setHorizontalHeaderLabels(QStringList() << tr("Flag name") << tr("Number of questions"));
        printq_includelist->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
        printq_includelist->horizontalHeader()->show();
        QListWidgetItem * item;
        for (int i = 0; i < printq_parent->current_db_f.size(); ++i) {
            if (printq_parent->current_db_fe[i]) {
                item = new QListWidgetItem(QString("%1 - %2").arg(i+1).arg(printq_parent->current_db_f[i]), printq_excludelist);
                item->setData(Qt::UserRole, i);
                MainWindow::setQuestionItemColour(item, i);
            }
        }
    } else if (rbtn->text() == tr("Questions")) {
        printq_excludelist->clear();
        printq_includelist->clearContents();
        printq_includelist->setRowCount(0);
        printq_includelist->setColumnCount(1);
        printq_includelist->horizontalHeader()->hide();
        QListWidgetItem * item;
        for (int i = 0; i < printq_parent->LQListWidget->count(); ++i) {
            if (!printq_parent->current_db_questions.value(printq_parent->LQListWidget->item(i))->isHidden() || printq_parent->actionShow_hidden->isChecked()) {
                item = new QListWidgetItem(*printq_parent->LQListWidget->item(i));
                item->setData(Qt::UserRole, i);
                printq_excludelist->addItem(item);
            }
        }
    }
    updateTestQnum();
}

void PrintQuestionsDialogue::addQuestionToPrint()
{
    if (printq_excludelist->currentIndex().isValid()) {
        addQuestionToPrint(printq_excludelist->currentRow());
        updateTestQnum();
    }
}

void PrintQuestionsDialogue::addQuestionToPrint(int row)
{
    printq_includelist->setRowCount(printq_includelist->rowCount() + 1);
    QTableWidgetItem * item = MainWindow::toTableItem(printq_excludelist->takeItem(row), true);
    printq_includelist->setItem(printq_includelist->rowCount() - 1, 0, item);
    if (rbtngrpPrintqSelect->checkedButton()->text() == tr("Flags")) {
        int max = printq_parent->qnumForFlag(item->data(Qt::UserRole).toInt(), useGroups());
        MTSpinBox * spb_qnum = new MTSpinBox(this);
        printq_includelist->setCellWidget(printq_includelist->rowCount() - 1, 1, spb_qnum);
        spb_qnum->setMaximum(max);
        spb_qnum->setMaximumValue();
        spb_qnum->setFixedHeight(20);
        spb_qnum->setEnabled(randomise());
        QObject::connect(spb_qnum, SIGNAL(valueChanged(int)), this, SLOT(updateTestQnum()));
        printq_includelist->setRowHeight(printq_includelist->rowCount() - 1, 20);
    } else {
        printq_includelist->setRowHeight(printq_includelist->rowCount() - 1, 16);
    }
}

void PrintQuestionsDialogue::removeQuestionToPrint()
{
    if (printq_includelist->currentRow() >= 0) {
        printq_excludelist->addItem(MainWindow::toListItem(printq_includelist->item(printq_includelist->currentRow(), 0)));
        printq_includelist->removeRow(printq_includelist->currentRow());
        updateTestQnum();
    }
}

void PrintQuestionsDialogue::addAllQuestionsToPrint()
{
    for (int i = 0; i < printq_excludelist->count();) {
        addQuestionToPrint(i);
    }
    updateTestQnum();
}

void PrintQuestionsDialogue::togglePrintEnabled()
{
    printq_btn_print->setEnabled(printq_advanced_numquestions->value() > 0);
}

void PrintQuestionsDialogue::resetDefaultValues()
{
    if (printTest()) {
        printq_advanced_statistics->setChecked(false);
        printq_advanced_formatting->setChecked(true);
    } else {
        printq_advanced_key->setChecked(false);
        printq_advanced_usegroups->setChecked(false);
    }
    if (!randomise()) {
        printq_advanced_numprintouts->setValue(1);
    }
    if (rbtngrpPrintqSelect->checkedButton()->text() == tr("Flags")) {
        for (int i = 0; i < printq_includelist->rowCount(); ++i) {
            printq_includelist->cellWidget(i, 1)->setEnabled(randomise());
            if (!randomise()) { ((MTSpinBox *)printq_includelist->cellWidget(i, 1))->setMaximumValue(); }
        }
    }
}

void PrintQuestionsDialogue::updateTestQnum()
{
    printq_parent->updateTestQnum(true, useGroups(), flagsSelected(), printq_advanced_numquestions, printq_includelist);
}

void PrintQuestionsDialogue::printQuestions()
{
    printq_parent->printQuestions(this);
}

int percentageFromValues(double, double, double);
QString htmlForProgressBar(int, bool);

bool MainWindow::loadPrinterSettings()
{
    QSettings settings("Michal Tomlein", "iTest");
    printer_configured = settings.value("printer/configured", false).toBool();
    printer_colorMode = settings.value("printer/colorMode").toInt();
    printer_doubleSidedPrinting = settings.value("printer/doubleSidedPrinting").toBool();
    printer_fontEmbeddingEnabled = settings.value("printer/fontEmbeddingEnabled").toBool();
    printer_numCopies = settings.value("printer/numCopies").toInt();
    printer_orientation = settings.value("printer/orientation").toInt();
    printer_outputFileName = settings.value("printer/outputFileName").toString();
    printer_outputFormat = settings.value("printer/outputFormat").toInt();
    printer_pageOrder = settings.value("printer/pageOrder").toInt();
    printer_pageSize = settings.value("printer/pageSize").toInt();
    printer_paperSource = settings.value("printer/paperSource").toInt();
    printer_printRange = settings.value("printer/printRange").toInt();
    printer_printerName = settings.value("printer/printerName").toString();
    printer_resolution = settings.value("printer/resolution").toInt();
    return printer_configured;
}

void MainWindow::savePrinterSettings()
{
    QSettings settings("Michal Tomlein", "iTest");
    settings.remove("printer");
    settings.setValue("printer/configured", printer_configured);
    if (!printer_configured) return;
    settings.setValue("printer/colorMode", printer_colorMode);
    settings.setValue("printer/doubleSidedPrinting", printer_doubleSidedPrinting);
    settings.setValue("printer/fontEmbeddingEnabled", printer_fontEmbeddingEnabled);
    settings.setValue("printer/numCopies", printer_numCopies);
    settings.setValue("printer/orientation", printer_orientation);
    settings.setValue("printer/outputFileName", printer_outputFileName);
    settings.setValue("printer/outputFormat", printer_outputFormat);
    settings.setValue("printer/pageOrder", printer_pageOrder);
    settings.setValue("printer/pageSize", printer_pageSize);
    settings.setValue("printer/paperSource", printer_paperSource);
    settings.setValue("printer/printRange", printer_printRange);
    settings.setValue("printer/printerName", printer_printerName);
    settings.setValue("printer/resolution", printer_resolution);
}

bool MainWindow::printerConfiguration(QString & printer_config)
{
    QTextStream conf(&printer_config);
    conf << "<br><br>\n<b>" << tr("Colour mode:") << "</b>" << " ";
    switch (printer_colorMode) {
        case QPrinter::Color: conf << tr("Colour") << endl; break;
        case QPrinter::GrayScale: conf << tr("GrayScale") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Double-sided printing:") << "</b>" << " ";
    if (printer_doubleSidedPrinting) { conf << tr("Yes") << endl; }
    else { conf << tr("No") << endl; }
    conf << "<br><b>" << tr("Font embedding:") << "</b>" << " ";
    if (printer_fontEmbeddingEnabled) { conf << tr("Yes") << endl; }
    else { conf << tr("No") << endl; }
    conf << "<br><b>" << tr("Number of copies:") << "</b>" << " ";
    conf << printer_numCopies << endl;
#ifndef Q_WS_X11
    conf << "<br>" << tr("Note: On Windows and Mac OS X, the number above is always 1<br>\nas these operating systems handle the number of copies internally.<br>\nTo print more copies, click 'Review configuration' and set the<br>\nnumber of copies each time you start the server.") << endl;
#endif
    conf << "<br><b>" << tr("Orientation:") << "</b>" << " ";
    switch (printer_orientation) {
        case QPrinter::Portrait: conf << tr("Portrait") << endl; break;
        case QPrinter::Landscape: conf << tr("Landscape") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    if (!printer_outputFileName.isEmpty()) {
        conf << "<br><b>" << tr("Output file name:") << "</b>" << " ";
        conf << printer_outputFileName << endl;
    }
    conf << "<br><b>" << tr("Output format:") << "</b>" << " ";
    switch (printer_outputFormat) {
        case QPrinter::NativeFormat: conf << tr("Native") << endl; break;
        case QPrinter::PdfFormat: conf << tr("PDF") << endl; break;
        case QPrinter::PostScriptFormat: conf << tr("PostScript") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Page order:") << "</b>" << " ";
    switch (printer_pageOrder) {
        case QPrinter::FirstPageFirst: conf << tr("First page first") << endl; break;
        case QPrinter::LastPageFirst: conf << tr("Last page first") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Paper format:") << "</b>" << " ";
    switch (printer_pageSize) {
        case QPrinter::A0: conf << "A0" << endl; break;
        case QPrinter::A1: conf << "A1" << endl; break;
        case QPrinter::A2: conf << "A2" << endl; break;
        case QPrinter::A3: conf << "A3" << endl; break;
        case QPrinter::A4: conf << "A4" << endl; break;
        case QPrinter::A5: conf << "A5" << endl; break;
        case QPrinter::A6: conf << "A6" << endl; break;
        case QPrinter::A7: conf << "A7" << endl; break;
        case QPrinter::A8: conf << "A8" << endl; break;
        case QPrinter::A9: conf << "A9" << endl; break;
        case QPrinter::B0: conf << "B0" << endl; break;
        case QPrinter::B1: conf << "B1" << endl; break;
        case QPrinter::B10: conf << "B10" << endl; break;
        case QPrinter::B2: conf << "B2" << endl; break;
        case QPrinter::B3: conf << "B3" << endl; break;
        case QPrinter::B4: conf << "B4" << endl; break;
        case QPrinter::B5: conf << "B5" << endl; break;
        case QPrinter::B6: conf << "B6" << endl; break;
        case QPrinter::B7: conf << "B7" << endl; break;
        case QPrinter::B8: conf << "B8" << endl; break;
        case QPrinter::B9: conf << "B9" << endl; break;
        case QPrinter::C5E: conf << "C5E" << endl; break;
        case QPrinter::Comm10E: conf << "U.S. Common 10 Envelope" << endl; break;
        case QPrinter::DLE: conf << "DLE" << endl; break;
        case QPrinter::Executive: conf << "Executive" << endl; break;
        case QPrinter::Folio: conf << "Folio" << endl; break;
        case QPrinter::Ledger: conf << "Ledger" << endl; break;
        case QPrinter::Legal: conf << "Legal" << endl; break;
        case QPrinter::Letter: conf << "Letter" << endl; break;
        case QPrinter::Tabloid: conf << "Tabloid" << endl; break;
        case QPrinter::Custom: conf << tr("Custom") << endl; break;
        default: conf << tr("Custom") << endl; break;
    }
    conf << "<br><b>" << tr("Paper source:") << "</b>" << " ";
    switch (printer_paperSource) {
        case QPrinter::OnlyOne: conf << tr("Only one") << endl; break;
        case QPrinter::Lower: conf << tr("Lower") << endl; break;
        case QPrinter::Middle: conf << tr("Middle") << endl; break;
        case QPrinter::Manual: conf << tr("Manual") << endl; break;
        case QPrinter::Envelope: conf << tr("Envelope") << endl; break;
        case QPrinter::EnvelopeManual: conf << tr("Envelope (manual)") << endl; break;
        case QPrinter::Auto: conf << tr("Auto") << endl; break;
        case QPrinter::Tractor: conf << tr("Tractor") << endl; break;
        case QPrinter::SmallFormat: conf << tr("Small format") << endl; break;
        case QPrinter::LargeFormat: conf << tr("Large format") << endl; break;
        case QPrinter::LargeCapacity: conf << tr("Large capacity") << endl; break;
        case QPrinter::Cassette: conf << tr("Cassette") << endl; break;
        case QPrinter::FormSource: conf << tr("Form source") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Print range:") << "</b>" << " ";
    switch (printer_printRange) {
        case QPrinter::AllPages: conf << tr("All pages") << endl; break;
        case QPrinter::Selection: conf << tr("Selection") << endl; break;
        case QPrinter::PageRange: conf << tr("Page range") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    if (printer_outputFileName.isEmpty()) {
        conf << "<br><b>" << tr("Printer name:") << "</b>" << " ";
        conf << printer_printerName << endl;
    }
    conf << "<br><b>" << tr("Resolution:") << "</b>" << " ";
    conf << printer_resolution << endl;
    conf << "<br><br>" << endl;
    return true;
}

bool MainWindow::configurePrinter(bool review_config)
{
    if (!review_config) {
        if (default_printer != NULL) delete default_printer;
        default_printer = new QPrinter(QPrinter::HighResolution);
    } else {
        if (!loadPrinterConfiguration()) return false;
        default_printer->setOutputFileName("");
    }
    QPrintDialog * dialogue = new QPrintDialog(default_printer, this);
    dialogue->setWindowTitle(tr("Configure printer"));
    if (dialogue->exec() != QDialog::Accepted) return false;

    printer_configured = true;
    printer_colorMode = default_printer->colorMode();
    printer_doubleSidedPrinting = default_printer->doubleSidedPrinting();
    printer_fontEmbeddingEnabled = default_printer->fontEmbeddingEnabled();
    printer_numCopies = default_printer->numCopies();
    printer_orientation = default_printer->orientation();
    printer_outputFileName = default_printer->outputFileName();
    printer_outputFormat = default_printer->outputFormat();
    printer_pageOrder = default_printer->pageOrder();
    printer_pageSize = default_printer->pageSize();
    printer_paperSource = default_printer->paperSource();
    printer_printRange = default_printer->printRange();
    printer_printerName = default_printer->printerName();
    printer_resolution = default_printer->resolution();
    savePrinterSettings();
    return true;
}

bool MainWindow::loadPrinterConfiguration()
{
    if (default_printer != NULL) delete default_printer;
    default_printer = new QPrinter(QPrinter::HighResolution);
    default_printer->setColorMode((QPrinter::ColorMode)printer_colorMode);
    default_printer->setDoubleSidedPrinting(printer_doubleSidedPrinting);
    default_printer->setFontEmbeddingEnabled(printer_fontEmbeddingEnabled);
    default_printer->setNumCopies(printer_numCopies);
    default_printer->setOrientation((QPrinter::Orientation)printer_orientation);
    default_printer->setOutputFileName(printer_outputFileName);
    default_printer->setOutputFormat((QPrinter::OutputFormat)printer_outputFormat);
    default_printer->setPageOrder((QPrinter::PageOrder)printer_pageOrder);
    default_printer->setPageSize((QPrinter::PageSize)printer_pageSize);
    default_printer->setPaperSource((QPrinter::PaperSource)printer_paperSource);
    default_printer->setPrintRange((QPrinter::PrintRange)printer_printRange);
    default_printer->setPrinterName(printer_printerName);
    default_printer->setResolution(printer_resolution);
    return true;
}

bool MainWindow::printClientResults(Client * client, QPrinter * printer)
{
    Student * student = new Student(client);
    bool ok = printStudentResults(student, printer, current_db_testname, current_db_scoringsystem);
    delete student;
    return ok;
}

bool MainWindow::printStudentResults(Student * student, QPrinter * printer, const QString & session_name, ScoringSystem sys)
{
    if (!student->isReady()) return false;
    QTextDocument doc; QString html; QTextStream out(&html);
    QuestionItem * item; QTextDocument qdoc;
    QString header = tr("Exam results"); header.append(QString(" - %1 - %2 - %3").arg(Qt::escape(session_name)).arg(Qt::escape(student->name())).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm")));
    out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
    out << header << endl << "</title><style type=\"text/css\">" << endl;
    out << ".heading { font-family: sans-serif; font-size: medium; font-weight: bold; color: black; }" << endl;
    out << ".default_text { font-family: sans-serif; font-size: 7pt; color: black; }" << endl;
    out << ".bold_text { font-family: sans-serif; font-size: 7pt; color: black; font-weight: bold; }" << endl;
    out << ".correct_qname { font-family: sans-serif; font-size: 7pt; font-weight: bold; color: rgb(69, 110, 14); }" << endl;
    out << ".incorrect_qname { font-family: sans-serif; font-size: 7pt; font-weight: bold; color: rgb(204, 109, 0); }" << endl;
    out << ".question { font-family: sans-serif; font-size: 7pt; color: black; }" << endl;
    out << ".answer { font-family: sans-serif; font-size: 7pt; font-style: italic; color: black; }" << endl;
    out << ".answered { text-decoration: underline; }" << endl;
    out << ".correct { font-weight: bold; }" << endl;
    out << "</style></head><body>" << endl;
    out << "<p class=\"heading\" align=\"center\">" << endl << header << endl << "</p>" << endl;
    QMapIterator<QString, QuestionAnswer> i(*(student->results())); QuestionAnswer qans;
    while (i.hasNext()) {
        i.next(); qans = i.value();
        out << "<p><table border=\"0\" width=\"100%\" padding=\"0\" spacing=\"0\"><tr><td>" << endl;
        if (qans.score(sys) > 0.0) {
            out << "<div class=\"correct_qname\">" << endl;
        } else {
            out << "<div class=\"incorrect_qname\">" << endl;
        }
        item = NULL;
        QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
        while (q.hasNext()) { q.next();
            if (q.value()->name() == i.key()) { item = q.value(); break; }
        }
        if (item != NULL) {
            if (item->flag() >= 0 && item->flag() < current_db_f.size()) {
                out << "<div class=\"bold_text\">";
                out << Qt::escape(current_db_f[item->flag()]) << ": </div>";
            }
            if (!item->group().isEmpty()) {
                out << "[" << Qt::escape(item->group()) << "] ";
            }
            out << Qt::escape(item->name()) << endl << "</div></td><td align=\"right\" class=\"default_text\">" << endl;
            out << tr("%1 out of %2").arg(qans.score(sys)).arg(qans.maximumScore(sys)) << "</td></table>" << endl;
            out << "<div class=\"question\">" << endl;
            qdoc.setHtml(item->text());
            out << Qt::escape(qdoc.toPlainText()) << endl << "</div>" << endl;
            for (int a = 1; a <= item->numAnswers(); ++a) {
                out << "<div class=\"answer";
                if ((qans.answered() & Question::indexToAnswer(a)) == Question::indexToAnswer(a)) { out << " answered"; }
                if (item->isAnswerAtIndexCorrect(a)) { out << " correct"; }
                out << "\">\n" << Question::indexToLabel(a) << " " << Qt::escape(item->answerAtIndex(a)) << endl << "</div>" << endl;
            }
        } else {
            out << Qt::escape(i.key()) << endl << "</div></td><td align=\"right\" class=\"default_text\">" << endl;
            out << tr("%1 out of %2").arg(qans.score(sys)).arg(qans.maximumScore(sys)) << "</td></table>" << endl;
            out << "<div class=\"question\">" << endl;
            out << tr("Not available") << endl << "</div>" << endl;
            out << "<div class=\"answer\">" << endl;
            out << tr("Answered:") << " " << Question::answerToString(qans.answered()) << "; ";
            out << tr("Correct answer:") << " " << Question::answerToString(qans.correctAnswer());
            out << endl << "</div>" << endl;
        }
        out << "</p>" << endl;
    }
    out << "<p class=\"default_text\" align=\"right\">" << endl;
    out << "<b>" << tr("Total score:") << "</b> ";
    out << tr("%1 out of %2").arg(student->score()).arg(student->maximumScore());
    out << QString(" (%3)").arg(student->passed() ? tr("PASSED") : tr("FAILED"));
    out << endl << "</p></body></html>" << endl;
    doc.setHtml(html); printer->setDocName(header); doc.print(printer);
    return true;
}

void MainWindow::quickPrint()
{
    if (!SMLCListWidget->currentIndex().isValid()) return;
    Client * client = current_db_clients.value(SMLCListWidget->currentItem());
    if (client == NULL) return;
    if (!printClientResults(client, default_printer)) {
        QListWidgetItem * log_entry = new QListWidgetItem(tr("%1 > Client #%2 (%3) > failed to print the client's results (Server QuickPrint call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(client->number()).arg(client->name()));
        SMSLListWidget->insertItem(0, log_entry);
        log_entry->setBackground(QBrush::QBrush(QColor::QColor(200, 0, 0)));
        log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
    } else {
        QListWidgetItem * log_entry = new QListWidgetItem(tr("%1 > Client #%2 (%3) > results printed successfully (Server QuickPrint call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(client->number()).arg(client->name()));
        SMSLListWidget->insertItem(0, log_entry);
        log_entry->setBackground(QBrush::QBrush(QColor::QColor(180, 255, 0)));
        log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
    }
}

void MainWindow::print()
{
    if (mainStackedWidget->currentIndex() == 5 || mainStackedWidget->currentIndex() == 6) {
        Student * student; QString printdialogue_caption; QString session_name; ScoringSystem sys;
        if (mainStackedWidget->currentIndex() == 5) {
            if (!SMLCListWidget->currentIndex().isValid()) return;
            student = new Student(current_db_clients.value(SMLCListWidget->currentItem()));
            printdialogue_caption = tr("Print client's results");
            session_name = current_db_testname;
            sys = current_db_scoringsystem;
        } else if (mainStackedWidget->currentIndex() == 6) {
            if (!SVLCListWidget->currentIndex().isValid()) return;
            student = current_db_students.value(SVLCListWidget->currentItem());
            printdialogue_caption = tr("Print student's results");
            if (current_db_session != NULL) {
                session_name = current_db_session->name();
                sys = current_db_session->scoringSystem();
            } else { session_name = current_db_name; }
        } else { return; }
        if (student == NULL) return;

        QPrinter * printer = new QPrinter(QPrinter::HighResolution);
        QPrintDialog * dialogue = new QPrintDialog(printer, this);
        dialogue->setWindowTitle(printdialogue_caption);
        if (dialogue->exec() != QDialog::Accepted) return;

        if (!printStudentResults(student, printer, session_name, sys)) {
            if (mainStackedWidget->currentIndex() == 5) {
                QListWidgetItem * log_entry = new QListWidgetItem(tr("%1 > Client #%2 (%3) > failed to print the client's results (Server Print call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(student->number()).arg(student->name()));
                SMSLListWidget->insertItem(0, log_entry);
                log_entry->setBackground(QBrush::QBrush(QColor::QColor(200, 0, 0)));
                log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
            } else {
                statusBar()->showMessage(tr("Failed to print the results"), 10000);
            }
        } else {
            if (mainStackedWidget->currentIndex() == 5) {
                QListWidgetItem * log_entry = new QListWidgetItem(tr("%1 > Client #%2 (%3) > results printed successfully (Server Print call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(student->number()).arg(student->name()));
                SMSLListWidget->insertItem(0, log_entry);
                log_entry->setBackground(QBrush::QBrush(QColor::QColor(180, 255, 0)));
                log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
            } else {
                statusBar()->showMessage(tr("Results printed successfully"), 10000);
            }
        }

        delete printer;
        if (mainStackedWidget->currentIndex() == 5) { delete student; }
    } else if (mainStackedWidget->currentIndex() == 7) {
        if (current_db_class == NULL) { return; }
        if (CLLSListWidget->highlightedRow() < 0) { return; }
        ClassMember * mem = current_db_class->member(CLLSListWidget->highlightedRow());
        if (mem == NULL) { return; }

        QPrinter * printer = new QPrinter(QPrinter::HighResolution);
        QPrintDialog * dialogue = new QPrintDialog(printer, this);
        dialogue->setWindowTitle(tr("Print statistics for the selected student"));
        if (dialogue->exec() != QDialog::Accepted) return;

        QTextDocument doc; doc.setHtml(htmlForClassMember(mem));
        printer->setDocName(tr("Statistics for %1").arg(mem->name())); doc.print(printer);

        statusBar()->showMessage(tr("Statistics printed successfully"), 10000);
        delete printer;
    }
}

void MainWindow::printAll()
{
    if (mainStackedWidget->currentIndex() == 5 || mainStackedWidget->currentIndex() == 6) {
        if (current_db_session == NULL) { return; }

        QPrinter * printer = new QPrinter(QPrinter::HighResolution);
        QPrintDialog * dialogue = new QPrintDialog(printer, this);
        dialogue->setWindowTitle(tr("Print the results of all students"));
        if (dialogue->exec() != QDialog::Accepted) return;

        if (printSessionSummary(current_db_session, printer)) {
            statusBar()->showMessage(tr("Summary printed successfully"), 10000);
        } else {
            statusBar()->showMessage(tr("Failed to print the summary"), 10000);
        }
        int numfailed = 0, numsuccessful = 0;
        for (int i = 0; i < current_db_session->numStudents(); ++i) {
            if (printStudentResults(current_db_session->student(i), printer, current_db_session->name(), current_db_session->scoringSystem()))
            { numsuccessful++; } else { numfailed++; }
        }
        statusBar()->showMessage(tr("%1 results printed successfully; %2 failed").arg(numsuccessful).arg(numfailed), 10000);

        delete printer;
    } else if (mainStackedWidget->currentIndex() == 7) {
        if (current_db_class == NULL) { return; }

        QPrinter * printer = new QPrinter(QPrinter::HighResolution);
        QPrintDialog * dialogue = new QPrintDialog(printer, this);
        dialogue->setWindowTitle(tr("Print the results of all students"));
        if (dialogue->exec() != QDialog::Accepted) return;

        if (printClassSummary(current_db_class, printer)) {
            statusBar()->showMessage(tr("Summary printed successfully"), 10000);
        } else {
            statusBar()->showMessage(tr("Failed to print the summary"), 10000);
        }
        for (int i = 0; i < current_db_class->numMembers(); ++i) {
            QTextDocument doc; doc.setHtml(htmlForClassMember(current_db_class->member(i)));
            printer->setDocName(tr("Statistics for %1").arg(current_db_class->member(i)->name())); doc.print(printer);
        }
        statusBar()->showMessage(tr("Results printed successfully"), 10000);

        delete printer;
    }
}

void MainWindow::printSessionSummary()
{
    if (current_db_session == NULL) { return; }

    QPrinter * printer = new QPrinter(QPrinter::HighResolution);
    QPrintDialog * dialogue = new QPrintDialog(printer, this);
    dialogue->setWindowTitle(tr("Print session summary"));
    if (dialogue->exec() != QDialog::Accepted) return;

    if (printSessionSummary(current_db_session, printer)) {
        statusBar()->showMessage(tr("Summary printed successfully"), 10000);
    } else {
        statusBar()->showMessage(tr("Failed to print the summary"), 10000);
    }

    delete printer;
}

bool MainWindow::printSessionSummary(Session * session, QPrinter * printer)
{
    if (session == NULL) { return false; }
    if (printer == NULL) { return false; }
    QTextDocument doc; QString html; QTextStream out(&html);
    QString header = tr("Session statistics and summary");
    header.append(QString(" - %1 - %2").arg(Qt::escape(session->name())).arg(session->dateTimeToString()));
    out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
    out << header << endl << "</title><style type=\"text/css\">" << endl;
    out << ".heading { font-family: sans-serif; font-size: medium; font-weight: bold; color: black; }" << endl;
    out << ".default_text { font-family: sans-serif; font-size: small; color: black; }" << endl;
    out << ".bold_text { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
    out << ".student_passed { font-family: sans-serif; font-size: small; font-weight: bold; color: rgb(69, 110, 14); }" << endl;
    out << ".student_failed { font-family: sans-serif; font-size: small; font-weight: bold; color: rgb(204, 109, 0); }" << endl;
    out << "</style></head><body>" << endl;
    out << "<p class=\"heading\" align=\"center\">" << endl << header << endl << "</p>" << endl;
    out << "<p>\n<div class=\"bold_text\">" << endl << tr("Pass mark:");
    out << "</div><br><table border=\"0\" width=\"100%\">" << endl;
    out << "<tr><td width=\"30%\"><div class=\"bold_text\">" << endl;
    out << tr("Total") << endl << "</div></td><td><div class=\"bold_text\">";
    out << session->passMark().passMark() << endl << "</div></td></tr>" << endl;
    for (int i = 0; i < session->passMark().count(); ++i) {
        out << "<tr><td width=\"30%\"><div class=\"default_text\">" << endl;
        out << current_db_f[session->passMark().condition(i)] << endl;
        out << "</div></td><td><div class=\"default_text\">" << endl;
        out << session->passMark().value(i) << endl << "</div></td></tr>" << endl;
    }
    out << "</table><br><table border=\"0\" width=\"100%\"><tr><td width=\"60%\" valign=\"middle\"><div class=\"bold_text\">" << endl;
    out << tr("Average:") << endl;
    out << "</div></td><td>" << htmlForProgressBar(session->average(), session->mostPassed());
    out << "</td><td width=\"7%\" align=\"right\" valign=\"middle\"><div class=\"default_text\">" << endl;
    out << session->average() << "%" << endl;
    out << "</div></td></tr></table><br><div class=\"bold_text\">" << endl;
    out << tr("Students:") << endl << "</div><br>" << endl;
    for (int i = 0; i < session->numStudents(); ++i) {
        out << "<table border=\"0\" width=\"100%\"><tr><td width=\"30%\" valign=\"middle\"><div class=\"" << (session->student(i)->passed() ? "student_passed" : "student_failed") << "\">" << endl;
        out << Qt::escape(session->student(i)->name()) << endl;
        out << "</div></td><td width=\"30%\" valign=\"middle\"><div class=\"default_text\">" << endl;
        if (session->student(i)->isReady()) {
            out << tr("%1 out of %2 (%3)").arg(session->student(i)->score()).arg(session->student(i)->maximumScore()).arg(session->student(i)->passed() ? tr("PASSED") : tr("FAILED")) << endl;
        } else {
            out << tr("NOT AVAILABLE") << endl;
        }
        out << "</div></td><td>" << endl;
        if (session->student(i)->isReady()) {
            int percentage = percentageFromValues(0.0, session->student(i)->maximumScore(), session->student(i)->score());
            out << htmlForProgressBar(percentage, session->student(i)->passed());
            out << "</td><td width=\"7%\" align=\"right\" valign=\"middle\"><div class=\"default_text\">" << endl;
            out << percentage << "%" << endl;
        } else {
            out << htmlForProgressBar(0, false);
            out << "</td><td width=\"7%\" align=\"right\" valign=\"middle\"><div class=\"default_text\">" << endl << tr("N/A") << endl;
        }
        out << "</div></td></tr></table>" << endl;
    }
    out << "</p></body></html>" << endl;
    doc.setHtml(html); printer->setDocName(header); doc.print(printer);
    return true;
}

void MainWindow::printClassSummary()
{
    if (current_db_class == NULL) { return; }

    QPrinter * printer = new QPrinter(QPrinter::HighResolution);
    QPrintDialog * dialogue = new QPrintDialog(printer, this);
    dialogue->setWindowTitle(tr("Print class summary"));
    if (dialogue->exec() != QDialog::Accepted) return;

    if (printClassSummary(current_db_class, printer)) {
        statusBar()->showMessage(tr("Summary printed successfully"), 10000);
    } else {
        statusBar()->showMessage(tr("Failed to print the summary"), 10000);
    }

    delete printer;
}

bool MainWindow::printClassSummary(Class * cl, QPrinter * printer)
{
    if (cl == NULL) { return false; }
    if (printer == NULL) { return false; }
    QTextDocument doc; QString html; QTextStream out(&html);
    QString header = tr("Class statistics and summary");
    header.append(QString(" - %1").arg(Qt::escape(cl->name())));
    int average = cl->average(&current_db_sessions, &current_db_archivedsessions);
    out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
    out << header << endl << "</title><style type=\"text/css\">" << endl;
    out << ".heading { font-family: sans-serif; font-size: medium; font-weight: bold; color: black; }" << endl;
    out << ".default_text { font-family: sans-serif; font-size: small; color: black; }" << endl;
    out << ".bold_text { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
    out << ".student_passed { font-family: sans-serif; font-size: small; color: rgb(69, 110, 14); }" << endl;
    out << ".student_failed { font-family: sans-serif; font-size: small; color: rgb(204, 109, 0); }" << endl;
    out << "</style></head><body>" << endl;
    out << "<p align=\"center\"><div class=\"heading\">" << endl;
    out << header << endl << "</div><div align=\"center\" class=\"bold_text\">" << endl;
    out << cl->firstYear() << " - " << cl->lastYear() << "</div></p><p>" << endl;
    out << "<table border=\"0\" width=\"100%\"><tr><td width=\"60%\" valign=\"middle\"><div class=\"bold_text\">" << endl;
    out << tr("Class average:") << endl;
    out << "</div></td><td>" << htmlForProgressBar(average, average >= 50);
    out << "</td><td width=\"7%\" align=\"right\" valign=\"middle\"><div class=\"default_text\">" << endl;
    out << average << "%" << endl << "</div></td></tr></table>" << endl;
    out << "<table border=\"0\" width=\"100%\"><tr><td width=\"60%\"><div class=\"bold_text\">" << endl;
    out << tr("Students:") << endl << "</div></td><td><div class=\"bold_text\">" << endl;
    out << tr("Average:") << endl << "</div></td><td></td></tr></table>" << endl;
    for (int i = 0; i < cl->numMembers(); ++i) {
        out << "<table border=\"0\" width=\"100%\"><tr><td width=\"60%\" valign=\"middle\"><div class=\"default_text\">" << endl;
        out << Qt::escape(cl->member(i)->name());
        average = cl->member(i)->average(&current_db_sessions, &current_db_archivedsessions);
        out << "</div></td><td>" << htmlForProgressBar(average, average >= 50);
        out << "</td><td width=\"7%\" align=\"right\" valign=\"middle\"><div class=\"default_text\">" << endl;
        out << average << "%" << endl << "</div></td></tr></table>" << endl;
    }
    out << "</p></body></html>" << endl;
    doc.setHtml(html); printer->setDocName(header); doc.print(printer);
    return true;
}

void MainWindow::printQuestions(PrintQuestionsDialogue * printq_widget)
{
    QPrinter * printer = new QPrinter(QPrinter::HighResolution);
    QPrintDialog * dialogue = new QPrintDialog(printer, printq_widget);
    dialogue->setWindowTitle(tr("Print questions"));
    if (dialogue->exec() != QDialog::Accepted) return;

    bool flags_selected = printq_widget->flagsSelected();
    bool questions_selected = printq_widget->questionsSelected();
    bool formatted = printq_widget->printFormatting();
    bool print_statistics = printq_widget->printStatistics();
    bool test = printq_widget->printTest();
    bool print_graphics = printq_widget->printGraphics();
    bool randomise = printq_widget->randomise();
    int numprintouts = printq_widget->numPrintouts();
    bool use_groups = printq_widget->useGroups();
    int numquestions = printq_widget->numQuestions();
    QList<int> used_items; PassMark passmark; int pm_v = 0;
    for (int i = 0; i < printq_widget->includeTableWidget()->rowCount(); ++i) {
        used_items << printq_widget->includeTableWidget()->item(i, 0)->data(Qt::UserRole).toInt();
        if (flags_selected) {
            pm_v = ((QSpinBox *)printq_widget->includeTableWidget()->cellWidget(i, 1))->value();
            if (flags_selected && pm_v > 0) {
                passmark.addCondition(printq_widget->includeTableWidget()->item(i, 0)->data(Qt::UserRole).toInt(), 0, pm_v);
            }
        }
    }
    QList<Question *> questions;
    for (int i = 0; i < LQListWidget->count(); ++i) {
        if (flags_selected) {
            QuestionItem * item = current_db_questions.value(LQListWidget->item(i));
            if (used_items.contains(item->flag()) && (!item->isHidden() || actionShow_hidden->isChecked())) {
                questions << item;
            }
        } else if (questions_selected) {
            if (used_items.contains(i)) {
                questions << current_db_questions.value(LQListWidget->item(i));
            }
        }
    }
    QFileInfo file_info; bool print_to_file = false; bool native_format = false;
    int len = makeString(numprintouts).length();
    if (!printer->outputFileName().isEmpty() && (numprintouts > 1 || printq_widget->printKey())) {
        file_info.setFile(printer->outputFileName());
        print_to_file = true;
        native_format = printer->outputFormat() == QPrinter::NativeFormat;
    }
    QList<int> randlist[numprintouts]; QString timestamp = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm");
    for (int i = 0; i < numprintouts; ++i) {
        if (randomise) { randlist[i] = Question::randomise(questions, passmark, use_groups, numquestions, i); }
        else { for (int q = 0; q < questions.count(); ++q) { randlist[i] << q; } }
        QTextDocument doc; QString html; QTextStream out(&html);
        out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
        out << tr("Questions") << endl << "</title><style type=\"text/css\">" << endl;
        out << ".heading { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
        out << ".default_text { font-family: sans-serif; font-size: small; color: black; }" << endl;
        out << ".bold_text { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
        out << ".answer { font-family: sans-serif; font-size: small; font-style: italic; color: black; }" << endl;
        out << ".correct { font-weight: bold; }" << endl;
        out << "</style></head><body>" << endl;
        if (test) { out << QString("<div class=\"heading\" align=\"center\">%1#%2</div><hr noshade=\"noshade\" size=\"1\">").arg(timestamp).arg(i + 1, len, 10, QChar('0')) << endl; }
        for (int r = 0; r < randlist[i].count(); ++r) {
            out << htmlForQuestion((QuestionItem *)questions.at(randlist[i].at(r)), test ? r + 1 : 0, doc, test, formatted, print_statistics, print_graphics);
        }
        out << "</body></html>" << endl;
        if (print_to_file) {
            printer->setOutputFileName(file_info.dir().absoluteFilePath(file_info.baseName() + QString("_%1.").arg(i + 1, len, 10, QChar('0')) + file_info.completeSuffix()));
            if (native_format) { printer->setOutputFormat(QPrinter::NativeFormat); }
        }
        doc.setHtml(html); printer->setDocName(tr("Questions")); doc.print(printer);
    }
    if (printq_widget->printKey()) {
        QTextDocument doc; QString html; QTextStream out(&html);
        out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
        out << tr("Key") << endl << "</title><style type=\"text/css\">" << endl;
        out << ".heading { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
        out << ".default_text { font-family: sans-serif; font-size: small; color: black; }" << endl;
        out << ".bold_text { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
        out << ".odd_col { background-color: lightgrey; }" << endl;
        out << "table { border-style: solid; border-color: lightgrey; }" << endl;
        out << "</style></head><body>" << endl;
        out << QString("<div class=\"heading\" align=\"center\">%1</div><hr noshade=\"noshade\" size=\"1\">").arg(timestamp) << endl;
        int cols = 12;
        int n = numprintouts / cols; if (numprintouts % cols > 0) { n++; }
        for (int t = 0; t < n; ++t) {
            out << "<table border=\"1\" width=\"100%\" cellspacing=\"0\" cellpadding=\"1\">" << endl;
            out << "<tr><th></th>" << endl;
            for (int i = t * cols; i < qMin(numprintouts, (t + 1) * cols); ++i) {
                out << QString("<th class=\"bold_text%1\" align=\"left\">#%2</th>").arg(i % 2 == 0 && numprintouts > 1 ? " odd_col" : "").arg(i + 1, len, 10, QChar('0')) << endl;
            }
            out << "</tr>" << endl;
            for (int i = 0; i < numquestions; ++i) {
                out << "<tr><td class=\"bold_text\">(" << i + 1 << ")</td>";
                for (int j = t * cols; j < qMin(numprintouts, (t + 1) * cols); ++j) {
                    out << QString("<td class=\"default_text%1\">").arg(j % 2 == 0 && numprintouts > 1 ? " odd_col" : "");
                    out << Question::answerToString(questions.at(randlist[j].at(i))->correctAnswer());
                    out << "</td>" << endl;
                }
                out << "</tr>" << endl;
            }
            out << "</table><br>" << endl;
        }
        out << "</body></html>" << endl;
        if (print_to_file) {
            printer->setOutputFileName(file_info.dir().absoluteFilePath(file_info.baseName() + QString("_%1.").arg(tr("key")) + file_info.completeSuffix()));
            if (native_format) { printer->setOutputFormat(QPrinter::NativeFormat); }
        }
        doc.setHtml(html); printer->setDocName(tr("Key")); doc.print(printer);
    }
    delete printer;
    return;
}

QString MainWindow::htmlForQuestion(QuestionItem * item, int n, QTextDocument & doc, bool test, bool formatted, bool print_statistics, bool print_graphics)
{
    if (item == NULL) { return ""; }
    QString html; QTextStream out(&html);
    out << "<div class=\"heading\" align=\"center\">" << endl;
    if (n != 0) { out << "(" << n << ") "; }
    if (item->flag() >= 0 && item->flag() < current_db_f.size()) {
        out << Qt::escape(current_db_f[item->flag()]) << ": ";
    }
    if (!item->group().isEmpty()) {
        out << "[" << Qt::escape(item->group()) << "] ";
    }
    out << Qt::escape(item->name()) << endl << "</div>" << endl;
    if (!test) {
        out << "<table border=\"0\" width=\"100%\">" << endl;
        out << "<tr><td width=\"40%\"><div class=\"bold_text\">" << endl;
        out << tr("Name:") << "</div></td><td><div class=\"default_text\">" << endl;
        out << Qt::escape(item->name()) << "</div></td></tr>" << endl;
        if (item->flag() >= 0 && item->flag() < current_db_f.size()) {
            out << "<tr><td width=\"40%\"><div class=\"bold_text\">" << endl;
            out << tr("Flag:") << "</div></td><td><div class=\"default_text\">" << endl;
            out << Qt::escape(current_db_f[item->flag()]) << "</div></td></tr>" << endl;
        }
        if (!item->group().isEmpty()) {
            out << "<tr><td width=\"40%\"><div class=\"bold_text\">" << endl;
            out << tr("Group:") << "</div></td><td><div class=\"default_text\">" << endl;
            out << Qt::escape(item->group()) << "</div></td></tr>" << endl;
        }
        out << "<tr><td width=\"40%\"><div class=\"bold_text\">" << endl;
        out << tr("Difficulty:") << "</div></td><td><div class=\"default_text\">" << endl;
        switch (item->difficulty()) {
            case 0: out << tr("Easy"); break;
            case 1: out << tr("Medium"); break;
            case 2: out << tr("Difficult"); break;
            default: out << tr("Unknown"); break;
        }
        out << "</div></td></tr>" << endl;
        if (item->numSvgItems() > 0 && !print_graphics) {
            out << "<tr><td width=\"40%\"><div class=\"bold_text\">" << endl;
            out << tr("Attachments (SVG):") << "</div></td><td><div class=\"default_text\">" << endl;
            for (int i = 0; i < item->numSvgItems(); ++i) {
                out << Qt::escape(item->svgItem(i)->text()) << "<b>; </b>";
            }
            out << "</div></td></tr>" << endl;
        }
        out << "</table>" << endl;
    }
    if (formatted) {
        out << item->text() << endl;
    } else {
        QTextDocument doc; doc.setHtml(item->text());
        out << Qt::escape(doc.toPlainText()) << endl;
    }
    if (item->numSvgItems() > 0 && print_graphics) {
        out << "<table border=\"0\" width=\"100%\"><tr>" << endl;
        for (int i = 0; i < item->numSvgItems(); ++i) {
            QSvgRenderer svgrenderer(item->svgItem(i)->svg().toUtf8());
            if (!svgrenderer.isValid()) { out << "<td></td>" << endl; continue; }
            QSize svg_size = svgrenderer.defaultSize();
            svg_size.scale(128, 128, Qt::KeepAspectRatio);
            QPixmap pixmap(svg_size);
            QPainter painter(&pixmap);
            painter.save();
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(QRect(0, 0, svg_size.width(), svg_size.height()));
            painter.restore();
            svgrenderer.render(&painter);
            QUrl resource_url(QString("%1-%2-%3.qpixmap").arg(item->name()).arg(i).arg(qrand()));
            doc.addResource(QTextDocument::ImageResource, resource_url, pixmap);
            out << "<td align=\"center\"><img src=\"" << resource_url.toString(QUrl::None) << "\"></td>" << endl;
        }
        out << "</tr><tr>" << endl;
        for (int i = 0; i < item->numSvgItems(); ++i) {
            out << "<td align=\"center\"><div class=\"default_text\">";
            out << Qt::escape(item->svgItem(i)->text()) << "</div></td>" << endl;
        }
        out << "</tr></table>" << endl;
    }
    for (int a = 1; a <= item->numAnswers(); ++a) {
        if (!item->answerAtIndex(a).isEmpty() || item->isAnswerAtIndexCorrect(a)) {
            out << "<div class=\"answer";
            if (item->isAnswerAtIndexCorrect(a) && !test) { out << " correct"; }
            out << "\">\n" << Question::indexToLabel(a) << " " << Qt::escape(item->answerAtIndex(a)) << endl << "</div>" << endl;
        }
    }
    if (print_statistics) {
        if (item->incorrectAnsCount() != 0 || item->correctAnsCount() != 0) {
            out << "<br><div class=\"bold_text\">" << tr("Statistics:") << "</div>" << endl;
            out << "<table border=\"0\" width=\"100%\">" << endl;
            out << "<tr><td width=\"40%\"><div class=\"default_text\">" << endl;
            out << tr("Number of correct answers:") << "</div></td><td><div class=\"default_text\">" << endl;
            out << item->correctAnsCount() << "</div></td></tr>" << endl;
            out << "<tr><td width=\"40%\"><div class=\"default_text\">" << endl;
            out << tr("Number of incorrect answers:") << "</div></td><td><div class=\"default_text\">" << endl;
            out << item->incorrectAnsCount() << "</div></td></tr>" << endl;
            out << "<tr><td width=\"40%\"><div class=\"default_text\">" << endl;
            out << tr("Calculated difficulty:") << "</div></td><td><div class=\"default_text\">" << endl;
            switch (item->recommendedDifficulty()) {
                case -1: out << tr("Unavailable"); break;
                case 0: out << tr("Easy"); break;
                case 1: out << tr("Medium"); break;
                case 2: out << tr("Difficult"); break;
                default: out << tr("Unavailable"); break;
            }
            out << "</div></td></tr></table>" << endl;
        }
    }
    out << "</p><hr noshade=\"noshade\" size=\"1\">" << endl;
    return html;
}

QString MainWindow::htmlForClassMember(ClassMember * mem)
{
    QString html; QTextStream out(&html); int average = mem->average(&current_db_sessions, &current_db_archivedsessions);
    out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
    out << tr("Statistics for %1").arg(Qt::escape(mem->name())) << endl << "</title><style type=\"text/css\">" << endl;
    out << ".heading { font-family: sans-serif; font-size: medium; font-weight: bold; color: black; }" << endl;
    out << ".default_text { font-family: sans-serif; font-size: small; color: black; }" << endl;
    out << ".bold_text { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
    out << ".student_passed { font-family: sans-serif; font-size: small; color: rgb(69, 110, 14); }" << endl;
    out << ".student_failed { font-family: sans-serif; font-size: small; color: rgb(204, 109, 0); }" << endl;
    out << "</style></head><body>" << endl;
    out << "<p align=\"center\"><div class=\"heading\">" << endl;
    out << tr("Statistics for %1").arg(Qt::escape(mem->name())) << endl << "</div><div align=\"center\" class=\"bold_text\">" << endl;
    out << current_db_class->firstYear() << " - " << current_db_class->lastYear() << ": " << Qt::escape(current_db_class->name()) << "</div></p><p>" << endl;
    out << "<table border=\"0\" width=\"100%\"><tr><td width=\"60%\" valign=\"middle\"><div class=\"bold_text\">" << endl;
    out << tr("Average:") << endl;
    out << "</div></td><td>" << htmlForProgressBar(average, average >= 50);
    out << "</td><td width=\"7%\" align=\"right\" valign=\"middle\"><div class=\"default_text\">" << endl;
    out << average << "%" << endl << "</div></td></tr></table>" << endl;
    out << "<table border=\"0\" width=\"100%\"><tr><td width=\"35%\"><div class=\"bold_text\">" << endl;
    out << tr("Test sessions attended:") << endl << "</div></td><td width=\"25%\"><div class=\"bold_text\">" << endl;
    out << tr("Under the name:") << endl << "</div></td><td><div class=\"bold_text\">" << endl;
    out << tr("With a score of:") << endl << "</div></td><td></td></tr></table>" << endl;
    QMap<QDateTime, int> sessions_map;
    for (int i = 0; i < mem->numSessionEntries(); ++i) {
        sessions_map.insert(mem->sessionEntry(i).session, i);
    }
    QMapIterator<QDateTime, int> i(sessions_map);
    while (i.hasNext()) { i.next();
        Session * session = current_db_sessions.value(i.key(), current_db_archivedsessions.value(i.key(), NULL));
        out << "<table border=\"0\" width=\"100%\"><tr><td width=\"35%\" valign=\"middle\"><div class=\"default_text\">" << endl;
        out << mem->sessionToString(i.value());
        if (session != NULL) { out << " - " << Qt::escape(session->name()); }
        out << "</div></td><td width=\"25%\" valign=\"middle\"><div class=\"";
        out << (session != NULL ? (session->student(mem->sessionEntry(i.value()).member_num)->passed() ? "student_passed" : "student_failed") : "student_passed");
        out << "\">" << endl;
        if (session != NULL) { out << Qt::escape(session->student(mem->sessionEntry(i.value()).member_num)->name()); }
        out << "</div></td><td>";
        int percentage = 0;
        if (session != NULL) {
            percentage = percentageFromValues(0.0, session->student(mem->sessionEntry(i.value()).member_num)->maximumScore(), session->student(mem->sessionEntry(i.value()).member_num)->score());
            out << htmlForProgressBar(percentage, session->student(mem->sessionEntry(i.value()).member_num)->passed());
        }
        else { out << htmlForProgressBar(0, true); }
        out << "</td><td width=\"7%\" align=\"right\" valign=\"middle\"><div class=\"default_text\">" << endl;
        out << percentage << "%" << endl << "</div></td></tr></table>" << endl;
    }
    out << "</p></body></html>" << endl;
    return html;
}

int percentageFromValues(double minimum, double maximum, double value)
{
    return (maximum - minimum) != 0.0 ? (int)(100 * (value - minimum) / (maximum - minimum)) : 100;
}

QString htmlForProgressBar(int percentage, bool green)
{
    QString html; QTextStream out(&html);
    out << "<table border=\"2\" width=\"100%\" height=\"20\"><tr>";
    if (percentage > 0) {
        out << "<td bgcolor=\"";
        out << (green ? "yellowgreen" : "#FF9419"); // "#456E0E" : "#CC6D00"
        out << "\" width=\"" << percentage << "%\"></td>" << endl;
    }
    out << (percentage >= 100 ? "</tr>" : "<td></td></tr>") << "</table>" << endl;
    return html;
}
