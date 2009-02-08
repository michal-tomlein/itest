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

#include "../shared/about_widget.h"
#include "main_window.h"

void MainWindow::errorInvalidDBFile(QString parentFunction, QString fileName)
{
    QMessageBox::critical(this, parentFunction, tr("Invalid database file %1").arg(fileName));
	this->setEnabled(true);
}

void MainWindow::setProgress(int progress)
{
    if (progress < 0) {
       QTimer::singleShot(2500, this, SLOT(setNullProgress()));
    } else { progressBar->setVisible(true); progressBar->setValue(progress); }
}

void MainWindow::setNullProgress()
{
    progressBar->setValue(0); progressBar->setVisible(false);
}

void MainWindow::clearCurrentValues()
{
    current_db_file.clear();
    current_db_name.clear();
    current_db_date.clear();
    current_db_comments.clear();
    current_db_question.clear();
    for (int i = 0; i < 20; ++i) {current_db_fe[i] = false;}
    for (int i = 0; i < 20; ++i) {current_db_f[i].clear();}
    QMapIterator<QListWidgetItem *, QuestionItem *> i(current_db_questions);
    while (i.hasNext()) { i.next(); delete i.value(); }
    current_db_questions.clear();
    current_db_students.clear();
    QMapIterator<QDateTime, Session *> n(current_db_sessions);
    while (n.hasNext()) { n.next(); if (n.value()) delete n.value(); }
    current_db_sessions.clear();
    QMapIterator<QDateTime, ArchivedSession *> a(current_db_archivedsessions);
    while (a.hasNext()) { a.next(); if (a.value()) delete a.value(); }
    current_db_archivedsessions.clear();
    QMapIterator<QListWidgetItem *, Class *> c(current_db_classes);
    while (c.hasNext()) { c.next(); delete c.value(); }
    current_db_classes.clear();
    current_db_flagentries.clear();
    current_db_queued_sessions.clear();
}

void MainWindow::clearSQ()
{
    SQFlagComboBox->clear();
    LQFlagComboBox->clear();
    clearSQNoFlags();
}

void MainWindow::clearSQNoFlags()
{
    SQQuestionNameLineEdit->clear();
    SQGroupLineEdit->clear();
    SQDifficultyComboBox->setCurrentIndex(0);
    SQQuestionTextEdit->clear();
    for (int i = 0; SQSVGListWidget->count();) {
        delete SQSVGListWidget->takeItem(i);
    }
    SQAnswersEdit->clear();
    SQStatisticsLabel->setVisible(false);
    SQExplanationLineEdit->clear();
}

void MainWindow::clearDBI()
{
    DBIDatabaseNameLineEdit->clear();
    DBIDateEdit->setDateTime(QDateTime::QDateTime(QDate::QDate(2006, 12, 1), QTime::QTime(0, 0)));
    DBIUseLastSaveDateCheckBox->setChecked(true);
}

void MainWindow::setAllEnabled(bool enabled)
{
    actionSave->setEnabled(enabled);
    actionSave_as->setEnabled(enabled);
	actionSave_a_copy->setEnabled(enabled);
	actionSave_a_backup->setEnabled(enabled);
    actionClose->setEnabled(enabled);
//  actionEdit_questions->setEnabled(enabled);
//  actionEdit_test->setEnabled(enabled);
//  actionEdit_flags->setEnabled(enabled);
//  actionEdit_comments->setEnabled(enabled);
//  actionSaved_sessions->setEnabled(enabled);
//  actionEdit_classes->setEnabled(enabled);
    actgrpPage->setEnabled(enabled);
    actionShow_DBI->setEnabled(enabled);
//	actionOverall_statistics->setEnabled(enabled);

    DBIDatabaseNameLineEdit->setEnabled(enabled);
//  DBIDateEdit->setEnabled(enabled);
    DBIUseLastSaveDateCheckBox->setEnabled(enabled);
    LQListWidget->setEnabled(enabled);
    LQFlagComboBox->setEnabled(enabled);
    LQSearchLineEdit->setEnabled(enabled);
    actionAdd->setEnabled(enabled);
//  actionUse_last_save_date->setEnabled(enabled);
//  actionFrom_A_to_Z->setEnabled(enabled);
//  actionFrom_Z_to_A->setEnabled(enabled);
//  actionShow_all->setEnabled(enabled);
//  actionShow_easy->setEnabled(enabled);
//  actionShow_medium->setEnabled(enabled);
//  actionShow_difficult->setEnabled(enabled);
//  actionShow_flag->setEnabled(enabled);

    menuDatabase->setEnabled(enabled);
    if (!enabled) {
        menuQuestion->setEnabled(enabled);
        menuServer->setEnabled(enabled);
        menuSession->setEnabled(enabled);
        menuClass->setEnabled(enabled);
    } else {
        enableTools();
    }

//  setSQEnabled(true);
//  setLQToolsEnabled(true);
    if (!enabled) { setSQEnabled(enabled); setLQToolsEnabled(enabled); }
}

void MainWindow::setEQToolsEnabled(bool enabled)
{
    menuQuestion->setEnabled(enabled);
	actionAdd->setEnabled(enabled);
//  actionFrom_A_to_Z->setEnabled(enabled);
//  actionFrom_Z_to_A->setEnabled(enabled);
    menuSort_questions->setEnabled(enabled);
//  actionShow_all->setEnabled(enabled);
//  actionShow_easy->setEnabled(enabled);
//  actionShow_medium->setEnabled(enabled);
//  actionShow_difficult->setEnabled(enabled);
//  actionShow_flag->setEnabled(enabled);
    menuFilter_LQ->setEnabled(enabled);
    if (enabled) {
        if (LQListWidget->currentIndex().isValid())
            { setLQToolsEnabled(enabled); setSQEnabled(enabled); }
    } else {
        setLQToolsEnabled(enabled); setSQEnabled(enabled);
    }
}

void MainWindow::setSQEnabled(bool enabled)
{
/*  SQQuestionNameLineEdit->setEnabled(enabled);
    SQGroupLineEdit->setEnabled(enabled);
    SQFlagComboBox->setEnabled(enabled);
    SQDifficultyComboBox->setEnabled(enabled);
    SQQuestionTextEdit->setEnabled(enabled);
    SQAnswerALineEdit->setEnabled(enabled);
    SQAnswerBLineEdit->setEnabled(enabled);
    SQAnswerCLineEdit->setEnabled(enabled);
    SQAnswerDLineEdit->setEnabled(enabled);
    SQCorrectACheckBox->setEnabled(enabled);
    SQCorrectBCheckBox->setEnabled(enabled);
    SQCorrectCCheckBox->setEnabled(enabled);
    SQCorrectDCheckBox->setEnabled(enabled);
    btnApply->setEnabled(enabled);
    btnDiscard->setEnabled(enabled);

    fontComboBox->setEnabled(enabled);
    sizeComboBox->setEnabled(enabled);
    tbtnBold->setEnabled(enabled);
    tbtnItalic->setEnabled(enabled);
    tbtnUnderlined->setEnabled(enabled);
    tbtnColour->setEnabled(enabled);
    tbtnAlignLeft->setEnabled(enabled);
    tbtnAlignCentre->setEnabled(enabled);
    tbtnAlignRight->setEnabled(enabled);
    tbtnAlignJustify->setEnabled(enabled);*/

	actionApply->setEnabled(enabled);
	actionDiscard->setEnabled(enabled);
    SQGroupBox->setEnabled(enabled);
    menuAttachments->setEnabled(enabled);
}

void MainWindow::setLQToolsEnabled(bool enabled)
{
	actionMove_up->setEnabled(enabled);
	actionMove_down->setEnabled(enabled);
	actionMove_to_top->setEnabled(enabled);
	actionMove_to_bottom->setEnabled(enabled);
	actionDelete->setEnabled(enabled);
	actionDuplicate->setEnabled(enabled);
	actionHide->setEnabled(enabled);
}

const int EQ = 1;
const int EC = 2;
const int EF = 3;
const int ES = 4;
const int SM = 5;
const int SV = 6;
const int CL = 7;

void MainWindow::enableTools()
{
    togglePrintEnabled();
    int i = mainStackedWidget->currentIndex();
    setEQToolsEnabled(i == EQ);
    menuQuestion->setEnabled(i == EQ);
    menuClass->setEnabled(i == CL);
    enableSMTools();
    enableSVTools();
}

void MainWindow::enableSMTools()
{
    int i = mainStackedWidget->currentIndex();
    menuServer->setEnabled(i == ES || i == SM);
    btnStartServer->setEnabled(i == ES && TSQnumSpinBox->value() > 0);
    actionStart_server->setEnabled(i == ES && TSQnumSpinBox->value() > 0);
    actionStop_server->setEnabled(i == SM);
    actionRun_Test_Writer->setEnabled(i == SM);
    actionExport_test->setEnabled(i == SM);
    actionAdd_offline_client->setEnabled(i == SM);
    actionAdd_offline_clients->setEnabled(i == SM);
    if (i == SM) { toggleSaveSessionEnabled(); } else { actionSave_session->setEnabled(false); }
}

void MainWindow::enableSVTools()
{
    int i = mainStackedWidget->currentIndex();
    menuSession->setEnabled(i == SV);
    actionExport_log->setEnabled(i == SM || (i == SV && SVSelectedSessionWidget->isEnabled() && SVLogListWidget->count() > 0));
    actionDelete_log->setEnabled(i == SV && SVSelectedSessionWidget->isEnabled() && SVLogListWidget->count() > 0);
    if (SVSelectedSessionWidget->isEnabled() && current_db_session != NULL) {
        actionArchive_session->setEnabled(i == SV && !current_db_session->isArchived());
        actionCopy_to_archive->setEnabled(i == SV && !current_db_session->isArchived());
        actionRestore_session->setEnabled(i == SV && current_db_session->isArchived());
        actionCopy_from_archive->setEnabled(i == SV && current_db_session->isArchived());
    } else {
        actionArchive_session->setEnabled(false);
        actionRestore_session->setEnabled(false);
        actionCopy_to_archive->setEnabled(false);
        actionCopy_from_archive->setEnabled(false);
    }
}

void MainWindow::togglePrintEnabled()
{
    int i = mainStackedWidget->currentIndex();
    actionPrint_questions->setEnabled(i == EQ || i == EC || i == EF || i == ES || i == SV || i == CL);
    actionQuickPrint->setEnabled(i == SM && SMLCListWidget->currentIndex().isValid());
    actionPrint->setEnabled((i == SM && SMLCListWidget->currentIndex().isValid()) || (i == SV && SVLCListWidget->currentIndex().isValid()) || (i == CL && CLLSListWidget->highlightedRow() >= 0));
    actionPrint_session_summary->setEnabled(i == SV && SVSelectedSessionWidget->isEnabled());
    actionPrint_all->setEnabled((i == SV && SVLCListWidget->count() > 0) || (i == CL && CLLSListWidget->count() > 0));
    actionPrint_class_summary->setEnabled(i == CL && CLLCListWidget->highlightedRow() >= 0);
}

void MainWindow::clearAll()
{
	clearCurrentValues();
	clearDBI();
	clearLQ();
	clearSQ();
	ECTextEdit->clear();
	loadFlags();
	updateFlagQnums();
	clearSM();
	clearSV();
	clearCL();
}

void MainWindow::clearLQ()
{
	LQAllRadioButton->setChecked(true);
	LQListWidget->clear();
}

MainWindow::MainWindow()
{
	// User interface ----------------------------------------------------------
    if (tr("LTR") == "RTL") { qApp->setLayoutDirection(Qt::RightToLeft); }
    setupUi(this);
	http = new QHttp(this);
	http_buffer = new QBuffer(this);
	default_printer = NULL;
#ifdef Q_WS_MAC
	this->setUnifiedTitleAndToolBarOnMac(true);
	//this->setAttribute(Qt::WA_MacBrushedMetal);
#endif
	progressBar = new QProgressBar(this);
	progressBar->setTextVisible(false);
    progressBar->resize(QSize(30, 10));
    statusBar()->addPermanentWidget(progressBar, 0);
    statusBar()->setFixedHeight(20);
    progressBar->setFixedWidth(150);
    progressBar->setFixedHeight(15);
    progressBar->setVisible(false);
    LQFlagComboBox->setVisible(false);
    SQStatisticsLabel->setVisible(false);
    currentSvgChanged();
	btnApply = SQButtonBox->button(QDialogButtonBox::Apply);
	btnApply->setText(tr("Apply"));
	btnApply->setStatusTip(tr("Apply any changes you have made to the question"));
	btnApply->setIcon(QIcon(QString::fromUtf8(":/images/images/button_ok.png")));
	btnDiscard = SQButtonBox->button(QDialogButtonBox::Discard);
	btnDiscard->setText(tr("Discard"));
	btnDiscard->setStatusTip(tr("Discard any changes you have made to the question"));
	btnDiscard->setIcon(QIcon(QString::fromUtf8(":/images/images/button_cancel.png")));
	EFButtonBox->button(QDialogButtonBox::Apply)->setText(tr("Apply"));
	EFButtonBox->button(QDialogButtonBox::Apply)->setStatusTip(tr("Apply any changes you have made to the flags"));
	EFButtonBox->button(QDialogButtonBox::Apply)->setIcon(QIcon(QString::fromUtf8(":/images/images/button_ok.png")));
	EFButtonBox->button(QDialogButtonBox::Discard)->setText(tr("Discard"));
	EFButtonBox->button(QDialogButtonBox::Discard)->setStatusTip(tr("Discard any changes you have made to the flags"));
	EFButtonBox->button(QDialogButtonBox::Discard)->setIcon(QIcon(QString::fromUtf8(":/images/images/button_cancel.png")));
    // Initialize variables ----------------------------------------------------
    varinit();
    current_db_session = NULL;
    current_db_class = NULL;
	// Connect slots -----------------------------------------------------------
    tbtnAddQuestion->setDefaultAction(actionAdd);
    tbtnDuplicateQuestion->setDefaultAction(actionDuplicate);
    tbtnDeleteQuestion->setDefaultAction(actionDelete);
    QObject::connect(actionAdd, SIGNAL(triggered()), this, SLOT(addQuestion()));
    QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteQuestion()));
    QObject::connect(actionDuplicate, SIGNAL(triggered()), this, SLOT(duplicateQuestion()));
    QObject::connect(btnApply, SIGNAL(released()), this, SLOT(applyQuestionChanges()));
    QObject::connect(actionApply, SIGNAL(triggered()), this, SLOT(applyQuestionChanges()));
    QObject::connect(btnDiscard, SIGNAL(released()), this, SLOT(discardQuestionChanges()));
    QObject::connect(actionDiscard, SIGNAL(triggered()), this, SLOT(discardQuestionChanges()));

    QObject::connect(actionNew, SIGNAL(triggered()), this, SLOT(newDB()));
    QObject::connect(btnNew, SIGNAL(released()), this, SLOT(newDB()));
    QObject::connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    QObject::connect(btnOpenOther, SIGNAL(released()), this, SLOT(open()));
    QObject::connect(btnOpenSelected, SIGNAL(released()), this, SLOT(openRecent()));
    QObject::connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    QObject::connect(actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));
    QObject::connect(actionSave_a_copy, SIGNAL(triggered()), this, SLOT(saveCopy()));
    QObject::connect(actionSave_a_backup, SIGNAL(triggered()), this, SLOT(saveBackup()));
    QObject::connect(actionClose, SIGNAL(triggered()), this, SLOT(closeDB()));
    QObject::connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
	QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

	QObject::connect(recentDBsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(openRecent(QListWidgetItem *)));
    QObject::connect(LQListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentQuestion()));

    QObject::connect(actionShow_DBI, SIGNAL(toggled(bool)), this, SLOT(onInfoDisplayChange(bool)));
    QObject::connect(actionUse_last_save_date, SIGNAL(toggled(bool)), DBIUseLastSaveDateCheckBox, SLOT(setChecked(bool)));
    QObject::connect(DBIUseLastSaveDateCheckBox, SIGNAL(toggled(bool)), actionUse_last_save_date, SLOT(setChecked(bool)));
    QObject::connect(DBIUseLastSaveDateCheckBox, SIGNAL(toggled(bool)), DBIDateEdit, SLOT(setDisabled(bool)));

    QObject::connect(actionFrom_A_to_Z, SIGNAL(triggered()), this, SLOT(sortQuestionsAscending()));
    QObject::connect(actionFrom_Z_to_A, SIGNAL(triggered()), this, SLOT(sortQuestionsDescending()));

    tbtnAddSVG->setDefaultAction(actionAdd_SVG);
    tbtnRemoveSVG->setDefaultAction(actionRemove_SVG);
    tbtnEditSVG->setDefaultAction(actionEdit_SVG);
    tbtnExportSVG->setDefaultAction(actionExport_SVG);
    QObject::connect(actionAdd_SVG, SIGNAL(triggered()), this, SLOT(addSvg()));
    QObject::connect(actionRemove_SVG, SIGNAL(triggered()), this, SLOT(removeSvg()));
    QObject::connect(actionEdit_SVG, SIGNAL(triggered()), this, SLOT(editSvg()));
    QObject::connect(actionExport_SVG, SIGNAL(triggered()), this, SLOT(exportSvg()));
    QObject::connect(SQSVGListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(previewSvg(QListWidgetItem *)));
    QObject::connect(SQSVGListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(currentSvgChanged()));

    tbtnMoveUp->setDefaultAction(actionMove_up);
    tbtnMoveDown->setDefaultAction(actionMove_down);
    QObject::connect(actionMove_up, SIGNAL(triggered()), this, SLOT(moveUp()));
    QObject::connect(actionMove_down, SIGNAL(triggered()), this, SLOT(moveDown()));
    QObject::connect(actionMove_to_top, SIGNAL(triggered()), this, SLOT(moveToTop()));
    QObject::connect(actionMove_to_bottom, SIGNAL(triggered()), this, SLOT(moveToBottom()));
    QObject::connect(actionHide, SIGNAL(triggered()), this, SLOT(hideQuestion()));
    QObject::connect(actionShow_hidden, SIGNAL(triggered()), this, SLOT(filterLQSearch()));

    rbtngrpFilterLQ = new QButtonGroup(this);
    rbtngrpFilterLQ->addButton(LQAllRadioButton);
    rbtngrpFilterLQ->addButton(LQEasyRadioButton);
    rbtngrpFilterLQ->addButton(LQMediumRadioButton);
    rbtngrpFilterLQ->addButton(LQDifficultRadioButton);
    rbtngrpFilterLQ->addButton(LQFlagRadioButton);

    actgrpFilterLQ = new QActionGroup(this);
    actgrpFilterLQ->addAction(actionShow_all);
    actgrpFilterLQ->addAction(actionShow_easy);
    actgrpFilterLQ->addAction(actionShow_medium);
    actgrpFilterLQ->addAction(actionShow_difficult);
    actgrpFilterLQ->addAction(actionShow_flag);

    QObject::connect(rbtngrpFilterLQ, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(filterLQ(QAbstractButton *)));
    QObject::connect(actgrpFilterLQ, SIGNAL(triggered(QAction *)), this, SLOT(filterLQAction(QAction *)));
    QObject::connect(LQFlagComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterLQFlagChanged()));
    QObject::connect(LQSearchLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(filterLQSearch()));
    QObject::connect(tbtnSearchByGroup, SIGNAL(released()), this, SLOT(searchByGroup()));

    actgrpPage = new QActionGroup(this);
    actgrpPage->addAction(actionEdit_questions);
    actgrpPage->addAction(actionEdit_comments);
    actgrpPage->addAction(actionEdit_flags);
    actgrpPage->addAction(actionEdit_test);
    actgrpPage->addAction(actionSaved_sessions);
    actgrpPage->addAction(actionEdit_classes);

    QObject::connect(actgrpPage, SIGNAL(triggered(QAction *)), this, SLOT(setPage(QAction *)));

    //QObject::connect(btnApply, SIGNAL(released()), this, SLOT(setDatabaseModified()));
    //QObject::connect(actionApply, SIGNAL(triggered()), this, SLOT(setDatabaseModified()));
    QObject::connect(actionUse_last_save_date, SIGNAL(triggered()), this, SLOT(setDatabaseModified()));
    QObject::connect(DBIUseLastSaveDateCheckBox, SIGNAL(released()), this, SLOT(setDatabaseModified()));
    QObject::connect(ECTextEdit, SIGNAL(textChanged()), this, SLOT(setDatabaseModified()));
    QObject::connect(DBIDatabaseNameLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(setDatabaseModified()));
    QObject::connect(DBIDateEdit, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(setDatabaseModified()));

	QObject::connect(actionCheck_for_updates, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
	QObject::connect(http, SIGNAL(done(bool)), this, SLOT(httpRequestFinished(bool)));
	QObject::connect(actionDocumentation, SIGNAL(triggered()), this, SLOT(openDocumentation()));
	QObject::connect(SQStatisticsLabel, SIGNAL(linkActivated(QString)), this, SLOT(adjustQuestionDifficulty()));
	QObject::connect(actionPrint_questions, SIGNAL(triggered()), this, SLOT(showPrintQuestionsDialogue()));
	QObject::connect(actionOverall_statistics, SIGNAL(triggered()), this, SLOT(overallStatistics()));
	QObject::connect(actionChange_language, SIGNAL(triggered()), this, SLOT(changeLanguage()));

	QObject::connect(mainStackedWidget, SIGNAL(currentChanged(int)), this, SLOT(currentPageChanged(int)));
    // Disable all -------------------------------------------------------------
    setAllEnabled(false);
    // Load settings -----------------------------------------------------------
    loadSettings();
    // Text edit ---------------------------------------------------------------
    setupTextEdits();
    // Flag lineEdits & checkBoxes - to arrays of pointers ---------------------
    setupFlagsPage();
    setFlagLineEditPalettes();
    // Server ------------------------------------------------------------------
    setupServer();
    // Session viewer ----------------------------------------------------------
    setupSessionViewer();
    // Class viewer ------------------------------------------------------------
    setupClassViewer();
    // Ready -------------------------------------------------------------------
    statusBar()->showMessage(tr("Ready"), 10000);
    // Check app args ----------------------------------------------------------
    if (qApp->arguments().count() > 1) {
        QFileInfo file_info(qApp->arguments().at(1));
        if (file_info.exists()) {
            addRecent(file_info.absoluteFilePath());
            openDB(file_info.absoluteFilePath());
        }
    }
}

void MainWindow::quit()
{
	// Save changes before proceeding?
    bool cancelled = saveChangesBeforeProceeding(tr("Quit iTest"), true);
    if (!cancelled) {
    // Quit
       saveSettings();
	   this->close();
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("Michal Tomlein", "iTest");
    QStringList recent = settings.value("editor/recentDatabases").toStringList();
    if (!recent.isEmpty()) { recentDBsListWidget->addItems(recent); }
    this->move(settings.value("editor/pos", this->pos()).toPoint());
    this->resize(settings.value("editor/size", this->size()).toSize());
    onInfoDisplayChange(settings.value("editor/showDBI", false).toBool());
    actionShow_hidden->setChecked(settings.value("editor/showHidden", false).toBool());
    SQSplitter->restoreState(settings.value("editor/SQSplitterState").toByteArray());
    SVSplitter->restoreState(settings.value("editor/VSSSplitterState").toByteArray());
    TSCustomServerPortSpinBox->setValue(settings.value("editor/customServerPort", 7777).toInt());
}

void MainWindow::saveSettings()
{
    QSettings settings("Michal Tomlein", "iTest");
    QStringList recent;
    for (int i = 0; i < recentDBsListWidget->count(); ++i)
    { recent << recentDBsListWidget->item(i)->text(); }
    settings.setValue("editor/recentDatabases", recent);
    settings.setValue("editor/pos", this->pos());
    settings.setValue("editor/size", this->size());
    settings.setValue("editor/showDBI", actionShow_DBI->isChecked());
    settings.setValue("editor/showHidden", actionShow_hidden->isChecked());
    settings.setValue("editor/SQSplitterState", SQSplitter->saveState());
    settings.setValue("editor/VSSSplitterState", SVSplitter->saveState());
    settings.setValue("editor/customServerPort", TSCustomServerPortSpinBox->value());
}

void MainWindow::addRecent(QString name)
{
    for (int i = 0; i < recentDBsListWidget->count();) {
        if (recentDBsListWidget->item(i)->text() == name) {
            delete recentDBsListWidget->item(i);
        } else {i++;}
    }
    recentDBsListWidget->insertItem(0, name);
    recentDBsListWidget->setCurrentRow(0);
}

void MainWindow::currentPageChanged(int i)
{
    enableTools();
    switch (i) {
        case 3: updateFlagQnums(); break;
        case 4: reloadAvailableItems(); break;
    }
}

void MainWindow::setPage(QAction * act)
{
    if (act == actionEdit_questions) {
        mainStackedWidget->setCurrentIndex(1);
    } else if (act == actionEdit_comments) {
        mainStackedWidget->setCurrentIndex(2);
    } else if (act == actionEdit_flags) {
        mainStackedWidget->setCurrentIndex(3);
    } else if (act == actionEdit_test) {
        mainStackedWidget->setCurrentIndex(4);
    } else if (act == actionSaved_sessions) {
        mainStackedWidget->setCurrentIndex(6);
    } else if (act == actionEdit_classes) {
        mainStackedWidget->setCurrentIndex(7);
    }
}

void MainWindow::onInfoDisplayChange(bool show)
{
    DBIGroupBox->setVisible(show);
    actionShow_DBI->setChecked(show);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (mainStackedWidget->currentIndex() == 5) {event->ignore(); return;}
    
    // Save changes before proceeding?
    bool cancelled = saveChangesBeforeProceeding(tr("Quit iTest"), true);
    if (!cancelled) {
    // Quit
       saveSettings();
	   event->accept();
    } else {event->ignore();}
}

void MainWindow::setDatabaseModified() { this->setWindowModified(true); }

void MainWindow::checkForUpdates()
{
	delete http_buffer; http_buffer = new QBuffer(this);
    http->setHost(itest_url);
	http->get("/current-version", http_buffer);
}

void MainWindow::httpRequestFinished(bool error)
{
	httpRequestFinished_start:
	if (error) {
		switch (QMessageBox::critical(this, tr("iTest"), tr("Failed to check for updates."), tr("&Try again"), tr("Cancel"), 0, 1)) {
			case 0: // Try again
				checkForUpdates(); return; break;
			case 1: // Cancel
				return; break;
		}
	}
	QString str(http_buffer->data()); QTextStream in(&str);
	if (in.readLine() != "[iTest.current-version]") { error = true; goto httpRequestFinished_start; }
	QString current_ver = in.readLine();
	if (in.readLine() != "[iTest.current-version.float]") { error = true; goto httpRequestFinished_start; }
	float f_current_ver = in.readLine().toFloat();
	if (in.readLine() != "[iTest.release-notes]") { error = true; goto httpRequestFinished_start; }
	QString release_notes;
	while (!in.atEnd()) { release_notes.append(in.readLine()); }
	if (f_current_ver <= f_ver) {
		QMessageBox::information(this, tr("iTest"), tr("Your iTest is up-to-date."));
	} else {
		QString info; QTextStream out(&info);
		out << "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body><p>" << endl;
		out << "<b>" << tr("iTest %1 is available now.").arg(current_ver) << "</b><br><br>" << endl;
		out << release_notes << endl << "</p></body></html>";
		QMessageBox::information(this, tr("iTest"), info);
	}
}

void MainWindow::openDocumentation()
{
    QDesktopServices::openUrl(docs_url);
}

void MainWindow::overallStatistics()
{
	QuestionItem * q_item; QTableWidgetItem * tw_item;
	QWidget * stats_widget = new QWidget(this, Qt::Dialog /*| Qt::WindowMaximizeButtonHint*/);
	stats_widget->setWindowModality(Qt::WindowModal);
	stats_widget->setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAC
	stats_widget->setWindowTitle(tr("%1 - Overall statistics").arg(current_db_name));
#else
    stats_widget->setWindowTitle(tr("%1 - Overall statistics - iTest").arg(current_db_name));
#endif
	stats_widget->setMinimumSize(QSize(300, 200));
	QGridLayout * stats_glayout = new QGridLayout(stats_widget);
	QLabel * stats_label = new QLabel(stats_widget);
	stats_glayout->addWidget(stats_label, 0, 0);
	QHBoxLayout * stats_hlayout_search = new QHBoxLayout(stats_widget);
	QLabel * stats_label_search = new QLabel(stats_widget);
    stats_label_search->setText(tr("Search:"));
	stats_hlayout_search->addWidget(stats_label_search);
	ExtendedLineEdit * stats_search = new ExtendedLineEdit(stats_widget);
	stats_hlayout_search->addWidget(stats_search);
	stats_glayout->addLayout(stats_hlayout_search, 1, 0);
	stats_tw = new MTTableWidget(stats_widget);
    QObject::connect(stats_search, SIGNAL(textChanged(QLineEdit *, const QString &)), stats_tw, SLOT(filterItems(QLineEdit *, const QString &)));
	stats_glayout->addWidget(stats_tw, 2, 0);
	QHBoxLayout * stats_hlayout = new QHBoxLayout(stats_widget);
	stats_hlayout->setMargin(0); stats_hlayout->setSpacing(6);
	stats_hlayout->addStretch();
	stats_btn_adjustall = new QPushButton (tr("Adjust all"), stats_widget);
	QObject::connect(stats_btn_adjustall, SIGNAL(released()), this, SLOT(statsAdjustAll()));
	stats_btn_adjustall->setEnabled(false);
	stats_hlayout->addWidget(stats_btn_adjustall);
	QPushButton * stats_btn_close = new QPushButton (tr("Close"), stats_widget);
	QObject::connect(stats_btn_close, SIGNAL(released()), stats_widget, SLOT(close()));
	QObject::connect(stats_widget, SIGNAL(destroyed()), this, SLOT(statsWidgetClosed()));
	stats_hlayout->addWidget(stats_btn_close);
	stats_glayout->addLayout(stats_hlayout, 3, 0);
	stats_glayout->setMargin(6); stats_glayout->setSpacing(6);
	btngrpStatsAdjust = new QButtonGroup(stats_widget);
	QObject::connect(btngrpStatsAdjust, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(statsAdjust(QAbstractButton *)));
	int rows = 0; int row = 0;
	for (int i = 0; i < LQListWidget->count(); ++i) {
		if (current_db_questions.value(LQListWidget->item(i))->recommendedDifficulty() != -1) { rows++; }
	}
	stats_label->setText(tr("<b>%1 questions with statistics found</b>").arg(rows));
	if (rows == 0) { stats_widget->show(); return; }
	stats_tw->setRowCount(rows);
	stats_tw->setColumnCount(6);
	tw_item = new QTableWidgetItem(tr("Question name"));
	stats_tw->setHorizontalHeaderItem(0, tw_item);
	tw_item = new QTableWidgetItem(tr("Difficulty"));
	stats_tw->setHorizontalHeaderItem(1, tw_item);
	tw_item = new QTableWidgetItem(tr("Calculated difficulty"));
	stats_tw->setHorizontalHeaderItem(2, tw_item);
	tw_item = new QTableWidgetItem(tr("Number of correct answers"));
	stats_tw->setHorizontalHeaderItem(3, tw_item);
	tw_item = new QTableWidgetItem(tr("Number of incorrect answers"));
	stats_tw->setHorizontalHeaderItem(4, tw_item);
	tw_item = new QTableWidgetItem(tr("Adjust difficulty"));
	stats_tw->setHorizontalHeaderItem(5, tw_item);
	stats_tw->verticalHeader()->hide();
	QFont font; font.setBold(true);
	for (int i = 0; i < LQListWidget->count(); ++i) {
		q_item = current_db_questions.value(LQListWidget->item(i));
		if (q_item->recommendedDifficulty() == -1) { continue; }
		tw_item = new QTableWidgetItem(q_item->group().isEmpty() ? q_item->name() : QString("[%1] %2").arg(q_item->group()).arg(q_item->name()));
		tw_item->setBackground(QBrush::QBrush(backgroundColourForFlag(q_item->flag())));
		tw_item->setForeground(QBrush::QBrush(foregroundColourForFlag(q_item->flag())));
		tw_item->setFont(font);
		stats_tw->setItem(row, 0, tw_item);
		tw_item = new QTableWidgetItem;
		switch (q_item->difficulty()) {
			case -1: tw_item->setText(tr("Unknown")); break;
			case 0: tw_item->setText(tr("Easy"));
				tw_item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
				tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
				break;
			case 1: tw_item->setText(tr("Medium"));
				tw_item->setBackground(QBrush::QBrush(QColor::QColor(255, 251, 0)));
				tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
				break;
			case 2: tw_item->setText(tr("Difficult"));
				tw_item->setBackground(QBrush::QBrush(QColor::QColor(204, 109, 0)));
				tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
				break;
			default: tw_item->setText(tr("Unknown")); break;
		}
		stats_tw->setItem(row, 1, tw_item);
		tw_item = new QTableWidgetItem;
		switch (q_item->recommendedDifficulty()) {
			case -1: tw_item->setText(tr("Unknown")); break;
			case 0: tw_item->setText(tr("Easy"));
				tw_item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
				tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
				break;
			case 1: tw_item->setText(tr("Medium"));
				tw_item->setBackground(QBrush::QBrush(QColor::QColor(255, 251, 0)));
				tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
				break;
			case 2: tw_item->setText(tr("Difficult"));
				tw_item->setBackground(QBrush::QBrush(QColor::QColor(204, 109, 0)));
				tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
				break;
			default: tw_item->setText(tr("Unknown")); break;
		}
		stats_tw->setItem(row, 2, tw_item);
		tw_item = new QTableWidgetItem(QString("%1").arg(q_item->correctAnsCount()));
		tw_item->setFont(font); tw_item->setForeground(QBrush::QBrush(QColor::QColor(92, 163, 0)));
		stats_tw->setItem(row, 3, tw_item);
		tw_item = new QTableWidgetItem(QString("%1").arg(q_item->incorrectAnsCount()));
		tw_item->setFont(font); tw_item->setForeground(QBrush::QBrush(QColor::QColor(204, 109, 0)));
		stats_tw->setItem(row, 4, tw_item);
		tw_item = new QTableWidgetItem;
		QPushButton * stats_btn_adjust = new QPushButton(tr("Adjust difficulty"), stats_tw);
        stats_tw->setCellWidget(row, 5, stats_btn_adjust);
		stats_btn_adjust->setEnabled(false);
		stats_qmap.insert(stats_btn_adjust, q_item);
		stats_twmap.insert(stats_btn_adjust, row);
		stats_lwmap.insert(stats_btn_adjust, i);
		btngrpStatsAdjust->addButton(stats_btn_adjust);
		if (q_item->difficulty() != q_item->recommendedDifficulty()) {
			stats_btn_adjustall->setEnabled(true);
			stats_btn_adjust->setEnabled(true);
		}
		row++;
	}
	stats_tw->resizeColumnsToContents(); stats_tw->resizeRowsToContents();
	stats_widget->resize(stats_tw->columnWidth(0) + stats_tw->columnWidth(1) + stats_tw->columnWidth(2) + stats_tw->columnWidth(3) + stats_tw->columnWidth(4) + stats_tw->columnWidth(5) + 40, 400);
	stats_widget->show();
}

void MainWindow::statsAdjustAll()
{
	if (stats_tw == NULL) { return; }
	QAbstractButton * btn;
	for (int i = 0; i < stats_tw->rowCount(); ++i) {
		btn = (QAbstractButton *)stats_tw->cellWidget(i, 5);
		if (btn == NULL) { continue; }
		if (!btn->isEnabled()) { continue; }
		statsAdjust(btn);
	}
	if (stats_btn_adjustall == NULL) { return; }
	stats_btn_adjustall->setEnabled(false);
}

void MainWindow::statsAdjust(QAbstractButton * btn)
{
	QuestionItem * q_item = stats_qmap.value(btn);
	if (q_item == NULL) { return; }
	if (stats_tw == NULL) { return; }
	int row = stats_twmap.value(btn, -1);
	if (row == -1) { return; }
	QTableWidgetItem * tw_item = stats_tw->item(row, 1);
	if (tw_item == NULL) { return; }
	int rdif = q_item->recommendedDifficulty();
	switch (rdif) {
		case -1: break;
		case 0: tw_item->setText(tr("Easy"));
			tw_item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
			tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
			break;
		case 1: tw_item->setText(tr("Medium"));
			tw_item->setBackground(QBrush::QBrush(QColor::QColor(255, 251, 0)));
			tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
			break;
		case 2: tw_item->setText(tr("Difficult"));
			tw_item->setBackground(QBrush::QBrush(QColor::QColor(204, 109, 0)));
			tw_item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
			break;
		default: break;
	}
	if (rdif >= 0 && rdif <= 2) {
		q_item->setDifficulty(rdif);
		if (current_db_questions.value(LQListWidget->currentItem()) == q_item)
			{ SQDifficultyComboBox->setCurrentIndex(rdif); }
		setDatabaseModified(); btn->setEnabled(false);
		int i = stats_lwmap.value(btn, -1);
		if (i == -1) { return; }
		QListWidgetItem * lw_item = LQListWidget->item(i);
		if (lw_item == NULL) { return; }
		setQuestionItemIcon(lw_item, rdif);
	}
}

void MainWindow::statsWidgetClosed()
{
	stats_qmap.clear();
	stats_twmap.clear();
	stats_lwmap.clear();
}

void MainWindow::showPrintQuestionsDialogue()
{
	new PrintQuestionsDialogue(this);
}

void MainWindow::changeLanguage()
{
	QWidget * lang_widget = new QWidget(this, Qt::Dialog);
	lang_widget->setWindowModality(Qt::WindowModal);
	lang_widget->setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAC
	lang_widget->setWindowTitle(tr("Change language"));
#else
    lang_widget->setWindowTitle(tr("Change language - iTest"));
#endif
	QGridLayout * lang_glayout = new QGridLayout(lang_widget);
	lang_glayout->setMargin(6); lang_glayout->setSpacing(6);
	QLabel * lang_label = new QLabel(lang_widget);
	lang_label->setText(tr("Select your preferred language"));
	lang_glayout->addWidget(lang_label, 0, 0);
	langComboBox = new QComboBox(lang_widget);
	QStringList langs(itest_i18n.keys()); langs.sort();
	for (int i = 0; i < langs.count(); ++i) {
		langComboBox->addItem(langs.at(i));
		if (langs.at(i) == "English") { langComboBox->setCurrentIndex(i); }
	}
	lang_glayout->addWidget(langComboBox, 1, 0);
	QDialogButtonBox * lang_buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, lang_widget);
	QObject::connect(lang_buttonbox, SIGNAL(accepted()), this, SLOT(langChanged()));
	QObject::connect(lang_buttonbox, SIGNAL(rejected()), lang_widget, SLOT(close()));
	lang_glayout->addWidget(lang_buttonbox, 2, 0);
	lang_widget->show();
}

void MainWindow::langChanged()
{
	if (langComboBox == NULL) { return; }
	QString lang = itest_i18n.value(langComboBox->currentText(), langComboBox->currentText());
	QSettings settings("Michal Tomlein", "iTest");
	QString current_lang = settings.value("lang", "English").toString();
	if (current_lang != lang) {
		settings.setValue("lang", lang);
		QMessageBox::information(this, tr("iTest"), tr("You need to restart iTest for the changes to apply."));
	}
	if (langComboBox->parent() == NULL) { return; }
	QWidget * lang_widget = (QWidget *)langComboBox->parent();
	lang_widget->close();
}

void MainWindow::previewSvg(QListWidgetItem * item)
{
    SvgItem * svg_item = (SvgItem *)item;
    if (!svg_item->isValid()) { return; }
    QSvgWidget * svg_widget = new QSvgWidget;
	svg_widget->setAttribute(Qt::WA_DeleteOnClose);
	svg_widget->setWindowTitle(svg_item->text());
	svg_widget->load(svg_item->svg().toUtf8());
    QSize minimum_size = svg_widget->renderer()->defaultSize();
	minimum_size.scale(128, 128, Qt::KeepAspectRatioByExpanding);
    svg_widget->setMinimumSize(minimum_size);
    minimum_size.scale(256, 256, Qt::KeepAspectRatioByExpanding);
    svg_widget->resize(minimum_size);
	svg_widget->show();
}

void MainWindow::about()
{
    AboutWidget * itest_about = new AboutWidget(ver, QString("2008"));
	itest_about->setParent(this);
    itest_about->setWindowFlags(Qt::Dialog /*| Qt::WindowMaximizeButtonHint*/ | Qt::WindowStaysOnTopHint);
	itest_about->show();
}
