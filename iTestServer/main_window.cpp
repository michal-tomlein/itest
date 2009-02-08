#include "about_widget.h"
#include "main_window.h"

void MainWindow::errorInvalidDBFile(QString parentFunction, QString fileName)
{
    QMessageBox::critical(this, parentFunction, tr("Invalid database file %1").arg(fileName));
	this->setEnabled(true);
}

/*void MainWindow::errorInvalidTempFileFormat(QString parentFunction, QString fileName)
{
    QMessageBox::critical(this, parentFunction, tr("Invalid format of temporary file %1").arg(fileName));
}

void MainWindow::errorRWTempFile(QString parentFunction, QString rw, QString q_file_name, QFile& q_file)
{
    QMessageBox::critical(this, parentFunction, tr("Cannot %1 temporary file %2:\n%3.\nPlease do not use any special characters.\nIf this is the case, try again omitting these.").arg(rw).arg(q_file_name).arg(q_file.errorString()));
}

void MainWindow::errorInvalidTempFileFormat_SpecialChars(QString parentFunction, QString fileName)
{
    QMessageBox::critical(this, parentFunction, tr("Invalid format of temporary file %1\nPlease do not use any special characters.\nIf this is the case, try again omitting these.").arg(fileName));
}*/

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
    SQAnswerALineEdit->clear();
    SQAnswerBLineEdit->clear();
    SQAnswerCLineEdit->clear();
    SQAnswerDLineEdit->clear();
    SQCorrectACheckBox->setChecked(false);
    SQCorrectBCheckBox->setChecked(false);
    SQCorrectCCheckBox->setChecked(false);
    SQCorrectDCheckBox->setChecked(false);
    SQStatisticsLabel->setVisible(false);
}

void MainWindow::clearDBI()
{
    DBIDatabaseNameLineEdit->clear();
    DBIDateEdit->setDateTime(QDateTime::QDateTime(QDate::QDate(2006, 12, 1), QTime::QTime(0, 0)));
    DBIUseLastSaveDateCheckBox->setChecked(true);
}

void MainWindow::enableAll()
{
    actionSave->setEnabled(true);
    actionSave_as->setEnabled(true);
	actionSave_a_copy->setEnabled(true);
	actionSave_a_backup->setEnabled(true);
    actionClose->setEnabled(true);
    actionEdit_questions->setEnabled(true);
    actionEdit_test->setEnabled(true);
    actionEdit_flags->setEnabled(true);
    actionEdit_comments->setEnabled(true);
    actionSaved_sessions->setEnabled(true);
    actionShow_DBI->setEnabled(true);
//	actionOverall_statistics->setEnabled(true);

    DBIDatabaseNameLineEdit->setEnabled(true);
//  DBIDateEdit->setEnabled(true);
    DBIUseLastSaveDateCheckBox->setEnabled(true);
    LQListWidget->setEnabled(true);
    LQFlagComboBox->setEnabled(true);
    LQSearchLineEdit->setEnabled(true);
    btnAdd->setEnabled(true);
    actionAdd->setEnabled(true);
//  actionUse_last_save_date->setEnabled(true);
//  actionFrom_A_to_Z->setEnabled(true);
//  actionFrom_Z_to_A->setEnabled(true);
//  actionShow_all->setEnabled(true);
//  actionShow_easy->setEnabled(true);
//  actionShow_medium->setEnabled(true);
//  actionShow_difficult->setEnabled(true);
//  actionShow_flag->setEnabled(true);
    
    menuDatabase->setEnabled(true);
    menuQuestion->setEnabled(true);
    menuServer->setEnabled(true);
    menuSession->setEnabled(true);
    
//  enableSQ();
//  enableLQTools();
}

void MainWindow::enableEQTools()
{
    menuQuestion->setEnabled(true);
	actionAdd->setEnabled(true);
//  actionFrom_A_to_Z->setEnabled(true);
//  actionFrom_Z_to_A->setEnabled(true);
    menuSort_questions->setEnabled(true);
//  actionShow_all->setEnabled(true);
//  actionShow_easy->setEnabled(true);
//  actionShow_medium->setEnabled(true);
//  actionShow_difficult->setEnabled(true);
//  actionShow_flag->setEnabled(true);
    menuFilter_LQ->setEnabled(true);
    if (LQListWidget->currentIndex().isValid()) {enableLQTools(); enableSQ();}
}

void MainWindow::enableSQ()
{
/*  SQQuestionNameLineEdit->setEnabled(true);
    SQGroupLineEdit->setEnabled(true);
    SQFlagComboBox->setEnabled(true);
    SQDifficultyComboBox->setEnabled(true);
    SQQuestionTextEdit->setEnabled(true);
    SQAnswerALineEdit->setEnabled(true);
    SQAnswerBLineEdit->setEnabled(true);
    SQAnswerCLineEdit->setEnabled(true);
    SQAnswerDLineEdit->setEnabled(true);
    SQCorrectACheckBox->setEnabled(true);
    SQCorrectBCheckBox->setEnabled(true);
    SQCorrectCCheckBox->setEnabled(true);
    SQCorrectDCheckBox->setEnabled(true);
    btnApply->setEnabled(true);
    btnDiscard->setEnabled(true);

    fontComboBox->setEnabled(true);
    sizeComboBox->setEnabled(true);
    tbtnBold->setEnabled(true);
    tbtnItalic->setEnabled(true);
    tbtnUnderlined->setEnabled(true);
    tbtnColour->setEnabled(true);
    tbtnAlignLeft->setEnabled(true);
    tbtnAlignCentre->setEnabled(true);
    tbtnAlignRight->setEnabled(true);
    tbtnAlignJustify->setEnabled(true);*/

	actionApply->setEnabled(true);
	actionDiscard->setEnabled(true);
    SQGroupBox->setEnabled(true);
}

void MainWindow::disableAll()
{
	actionSave->setEnabled(false);
	actionSave_as->setEnabled(false);
	actionSave_a_copy->setEnabled(false);
	actionSave_a_backup->setEnabled(false);
    actionClose->setEnabled(false);
    actionEdit_questions->setEnabled(false);
    actionEdit_test->setEnabled(false);
    actionEdit_flags->setEnabled(false);
    actionEdit_comments->setEnabled(false);
    actionSaved_sessions->setEnabled(false);
    actionShow_DBI->setEnabled(false);
//	actionOverall_statistics->setEnabled(false);

    DBIDatabaseNameLineEdit->setEnabled(false);
	DBIDateEdit->setEnabled(false);
	DBIUseLastSaveDateCheckBox->setEnabled(false);
	LQListWidget->setEnabled(false);
	LQFlagComboBox->setEnabled(false);
	LQSearchLineEdit->setEnabled(false);
	btnAdd->setEnabled(false);
	actionAdd->setEnabled(false);
//	actionUse_last_save_date->setEnabled(false);
//	actionFrom_A_to_Z->setEnabled(false);
//  actionFrom_Z_to_A->setEnabled(false);
//  actionShow_all->setEnabled(false);
//  actionShow_easy->setEnabled(false);
//  actionShow_medium->setEnabled(false);
//  actionShow_difficult->setEnabled(false);
//  actionShow_flag->setEnabled(false);
	
	menuDatabase->setEnabled(false);
	menuQuestion->setEnabled(false);
	menuServer->setEnabled(false);
	menuSession->setEnabled(false);
	
	disableSQ();
	disableLQTools();
}

void MainWindow::disableEQTools()
{
    menuQuestion->setEnabled(false);
	actionAdd->setEnabled(false);
//  actionFrom_A_to_Z->setEnabled(false);
//  actionFrom_Z_to_A->setEnabled(false);
    menuSort_questions->setEnabled(false);
//  actionShow_all->setEnabled(false);
//  actionShow_easy->setEnabled(false);
//  actionShow_medium->setEnabled(false);
//  actionShow_difficult->setEnabled(false);
//  actionShow_flag->setEnabled(false);
    menuFilter_LQ->setEnabled(false);
    disableLQTools(); disableSQ();
}

void MainWindow::disableSQ()
{
/*	SQQuestionNameLineEdit->setEnabled(false);
    SQGroupLineEdit->setEnabled(false);
	SQFlagComboBox->setEnabled(false);
	SQDifficultyComboBox->setEnabled(false);
	SQQuestionTextEdit->setEnabled(false);
	SQAnswerALineEdit->setEnabled(false);
	SQAnswerBLineEdit->setEnabled(false);
	SQAnswerCLineEdit->setEnabled(false);
	SQAnswerDLineEdit->setEnabled(false);
	SQCorrectACheckBox->setEnabled(false);
	SQCorrectBCheckBox->setEnabled(false);
	SQCorrectCCheckBox->setEnabled(false);
	SQCorrectDCheckBox->setEnabled(false);
	btnApply->setEnabled(false);
	btnDiscard->setEnabled(false);

    fontComboBox->setEnabled(false);
    sizeComboBox->setEnabled(false);
    tbtnBold->setEnabled(false);
    tbtnItalic->setEnabled(false);
    tbtnUnderlined->setEnabled(false);
    tbtnColour->setEnabled(false);
    tbtnAlignLeft->setEnabled(false);
    tbtnAlignCentre->setEnabled(false);
    tbtnAlignRight->setEnabled(false);
    tbtnAlignJustify->setEnabled(false);*/

	actionApply->setEnabled(false);
	actionDiscard->setEnabled(false);
    SQGroupBox->setEnabled(false);
}

void MainWindow::enableLQTools()
{
	actionMove_up->setEnabled(true);
	actionMove_down->setEnabled(true);
	btnMoveUp->setEnabled(true);
	btnMoveDown->setEnabled(true);
	actionMove_to_top->setEnabled(true);
	actionMove_to_bottom->setEnabled(true);
	actionDelete->setEnabled(true);
	btnDelete->setEnabled(true);
	actionDuplicate->setEnabled(true);
	btnDuplicate->setEnabled(true);
	actionHide->setEnabled(true);
}

void MainWindow::disableLQTools()
{
	actionMove_up->setEnabled(false);
	actionMove_down->setEnabled(false);
	btnMoveUp->setEnabled(false);
	btnMoveDown->setEnabled(false);
	actionMove_to_top->setEnabled(false);
	actionMove_to_bottom->setEnabled(false);
	actionDelete->setEnabled(false);
	btnDelete->setEnabled(false);
	actionDuplicate->setEnabled(false);
	btnDuplicate->setEnabled(false);
	actionHide->setEnabled(false);
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
	clearVSS();
}

void MainWindow::clearLQ()
{
	LQAllRadioButton->setChecked(true);
	LQListWidget->clear();
}

MainWindow::MainWindow()
{
	// User interface ----------------------------------------------------------
    setupUi(this);
	http = new QHttp(this);
	http_buffer = new QBuffer(this);
	default_printer = NULL;
#ifdef Q_WS_MAC
	this->setUnifiedTitleAndToolBarOnMac(true);
	//this->setAttribute(Qt::WA_MacBrushedMetal);
	DBIGroupBox->setMinimumSize(649, 60);
	DBIGroupBox->setMaximumSize(16777215, 60);
	VSSCSGroupBox->setMinimumSize(0, 80);
	VSSCSGroupBox->setMaximumSize(16777215, 80);
	SMInfoGroupBox->setMinimumSize(0, 60);
	SMInfoGroupBox->setMaximumSize(16777215, 60);
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
	btnApply = SQButtonBox->button(QDialogButtonBox::Apply);
	btnApply->setStatusTip(tr("Apply any changes you have made to the question"));
	btnApply->setIcon(QIcon(QString::fromUtf8(":/images/images/button_ok.png")));
	btnDiscard = SQButtonBox->button(QDialogButtonBox::Discard);
	btnDiscard->setStatusTip(tr("Discard any changes you have made to the question"));
	btnDiscard->setIcon(QIcon(QString::fromUtf8(":/images/images/button_cancel.png")));
	EFButtonBox->button(QDialogButtonBox::Apply)->setStatusTip(tr("Apply any changes you have made to the flags"));
	EFButtonBox->button(QDialogButtonBox::Apply)->setIcon(QIcon(QString::fromUtf8(":/images/images/button_ok.png")));
	EFButtonBox->button(QDialogButtonBox::Discard)->setStatusTip(tr("Discard any changes you have made to the flags"));
	EFButtonBox->button(QDialogButtonBox::Discard)->setIcon(QIcon(QString::fromUtf8(":/images/images/button_cancel.png")));
    // Initialize variables ----------------------------------------------------
    varinit();
	// Connect slots -----------------------------------------------------------
    QObject::connect(btnAdd, SIGNAL(released()), this, SLOT(addQuestion()));
    QObject::connect(actionAdd, SIGNAL(triggered()), this, SLOT(addQuestion()));
    QObject::connect(btnDelete, SIGNAL(released()), this, SLOT(deleteQuestion()));
    QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteQuestion()));
    QObject::connect(btnDuplicate, SIGNAL(released()), this, SLOT(duplicateQuestion()));
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
    QObject::connect(btnClose, SIGNAL(released()), this, SLOT(closeDB()));
    QObject::connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
	QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	
	QObject::connect(recentDBsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(openRecent(QListWidgetItem *)));
    QObject::connect(LQListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentQuestion()));
    
    QObject::connect(showInfoCheckBox, SIGNAL(toggled(bool)), this, SLOT(onInfoDisplayChange(bool)));
    QObject::connect(actionShow_DBI, SIGNAL(toggled(bool)), this, SLOT(onInfoDisplayChange(bool)));
    QObject::connect(actionUse_last_save_date, SIGNAL(toggled(bool)), DBIUseLastSaveDateCheckBox, SLOT(setChecked(bool)));
    QObject::connect(DBIUseLastSaveDateCheckBox, SIGNAL(toggled(bool)), actionUse_last_save_date, SLOT(setChecked(bool)));
    QObject::connect(DBIUseLastSaveDateCheckBox, SIGNAL(toggled(bool)), DBIDateEdit, SLOT(setDisabled(bool)));
    
    QObject::connect(actionFrom_A_to_Z, SIGNAL(triggered()), this, SLOT(sortQuestionsAscending()));
    QObject::connect(actionFrom_Z_to_A, SIGNAL(triggered()), this, SLOT(sortQuestionsDescending()));
    
    QObject::connect(actionMove_up, SIGNAL(triggered()), this, SLOT(moveUp()));
    QObject::connect(actionMove_down, SIGNAL(triggered()), this, SLOT(moveDown()));
    QObject::connect(btnMoveUp, SIGNAL(released()), this, SLOT(moveUp()));
    QObject::connect(btnMoveDown, SIGNAL(released()), this, SLOT(moveDown()));
    QObject::connect(actionMove_to_top, SIGNAL(triggered()), this, SLOT(moveToTop()));
    QObject::connect(actionMove_to_bottom, SIGNAL(triggered()), this, SLOT(moveToBottom()));
    QObject::connect(actionHide, SIGNAL(triggered()), this, SLOT(hideQuestion()));
    QObject::connect(actionShow_hidden, SIGNAL(triggered()), this, SLOT(filterLQSearch()));
    
    rbtngrpFilterLQ = new QButtonGroup (this);
    rbtngrpFilterLQ->addButton(LQAllRadioButton);
    rbtngrpFilterLQ->addButton(LQEasyRadioButton);
    rbtngrpFilterLQ->addButton(LQMediumRadioButton);
    rbtngrpFilterLQ->addButton(LQDifficultRadioButton);
    rbtngrpFilterLQ->addButton(LQFlagRadioButton);
    
    actgrpFilterLQ = new QActionGroup (this);
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
    
    actgrpPage = new QActionGroup (this);
    actgrpPage->addAction(actionEdit_questions);
    actgrpPage->addAction(actionEdit_comments);
    actgrpPage->addAction(actionEdit_flags);
    actgrpPage->addAction(actionEdit_test);
    actgrpPage->addAction(actionSaved_sessions);
    
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
	QObject::connect(actionOverall_statistics, SIGNAL(triggered()), this, SLOT(overallStatistics()));
	QObject::connect(actionChange_language, SIGNAL(triggered()), this, SLOT(changeLanguage()));
    // Disable all -------------------------------------------------------------
    disableAll();
    // Geometry ----------------------------------------------------------------
	updateGeometry();
    // Load settings -----------------------------------------------------------
    loadSettings();
    // Text edit ---------------------------------------------------------------
    setupTextEdits();
    // Flag lineEdits & checkBoxes - to arrays of pointers ---------------------
    setupFlagsPage();
    setFlagLineEditPalettes();
    // Palettes ----------------------------------------------------------------
    search_active_palette.setColor(QPalette::Active, QPalette::Base, QColor::QColor(255, 251, 0));
    search_active_palette.setColor(QPalette::Active, QPalette::Text, QColor::QColor(0, 0, 0));
    search_active_palette.setColor(QPalette::Inactive, QPalette::Base, QColor::QColor(255, 251, 0));
    search_active_palette.setColor(QPalette::Inactive, QPalette::Text, QColor::QColor(0, 0, 0));
    search_noresults_palette.setColor(QPalette::Active, QPalette::Base, QColor::QColor(255, 0, 0));
    search_noresults_palette.setColor(QPalette::Active, QPalette::Text, QColor::QColor(0, 0, 0));
    search_noresults_palette.setColor(QPalette::Inactive, QPalette::Base, QColor::QColor(255, 0, 0));
    search_noresults_palette.setColor(QPalette::Inactive, QPalette::Text, QColor::QColor(0, 0, 0));
    // Server ------------------------------------------------------------------
    setupServer();
    // Session viewer ----------------------------------------------------------
    setupSessionViewer();
    // Ready -------------------------------------------------------------------
    statusBar()->showMessage(tr("Ready"), 10000);
    // Check app args ----------------------------------------------------------
    if (qApp->arguments().count() > 1) {
        QFileInfo file_info (qApp->arguments().at(1));
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
    settings.setValue("editor/showDBI", showInfoCheckBox->isChecked());
    settings.setValue("editor/showHidden", actionShow_hidden->isChecked());
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

void MainWindow::setPage(QAction * act)
{
    if (act == actionEdit_questions) {
        mainStackedWidget->setCurrentIndex(1); togglePrintEnabled();
        updateGeometry(); enableEQTools();
    } else if (act == actionEdit_comments) {
        mainStackedWidget->setCurrentIndex(2); togglePrintEnabled();
        updateGeometry(); disableEQTools();
    } else if (act == actionEdit_flags) {
        mainStackedWidget->setCurrentIndex(3); togglePrintEnabled();
        updateGeometry(); disableEQTools(); updateFlagQnums();
    } else if (act == actionEdit_test) {
        mainStackedWidget->setCurrentIndex(4); togglePrintEnabled();
        updateGeometry(); disableEQTools(); reloadAvailableItems();
    } else if (act == actionSaved_sessions) {
        mainStackedWidget->setCurrentIndex(6); togglePrintEnabled();
        updateGeometry(); disableEQTools();
    }
}

void MainWindow::onInfoDisplayChange(bool show)
{
    DBIGroupBox->setVisible(show);
    actionShow_DBI->setChecked(show); showInfoCheckBox->setChecked(show);
    QTimer::singleShot(200, this, SLOT(updateGeometry()));
}

void MainWindow::updateGeometry()
{
#ifdef Q_WS_MAC
    int h = statusbar->height(); int i = 20;
#else
    int h = menubar->height() + toolBar->height() + statusbar->height(); int i = 10;
#endif
    mainStackedWidget->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	welcomeGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	mainGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	LQGridLayout->setGeometry(0, i, LQGroupBox->geometry().width(), LQGroupBox->geometry().height()-i);
	SQGridLayout->setGeometry(0, i, SQGroupBox->geometry().width(), SQGroupBox->geometry().height()-i);
	DBIHorizontalLayout->setGeometry(0, i, DBIGroupBox->geometry().width(), DBIGroupBox->geometry().height()-i);
	editCommentsGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	editFlagsGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	testSettingsGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	TSAdvancedSetupGridLayout->setGeometry(0, i, TSAdvancedSetupGroupBox->geometry().width(), TSAdvancedSetupGroupBox->geometry().height()-i);
	TSCommonSettingsGridLayout->setGeometry(0, 0, TSCommonSettingsGroupBox->geometry().width(), TSCommonSettingsGroupBox->geometry().height());
	serverModeGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	SMInfoGridLayout->setGeometry(0, i, SMInfoGroupBox->geometry().width(), SMInfoGroupBox->geometry().height()-i);
	SMClientsGridLayout->setGeometry(0, i, SMClientsGroupBox->geometry().width(), SMClientsGroupBox->geometry().height()-i);
	SMSelectedClientGridLayout->setGeometry(0, i, SMSelectedClientGroupBox->geometry().width(), SMSelectedClientGroupBox->geometry().height()-i);
	SMLogGridLayout->setGeometry(0, i, SMLogGroupBox->geometry().width(), SMLogGroupBox->geometry().height()-i);
	savedSessionsGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
	VSSLSGridLayout->setGeometry(0, i, VSSLSGroupBox->geometry().width(), VSSLSGroupBox->geometry().height()-i);
	VSSCSGridLayout->setGeometry(0, i, VSSCSGroupBox->geometry().width(), VSSCSGroupBox->geometry().height()-i);
	updateVSSGeometry(); updateTSTWGeometry(); updatePMDTWGeometry();
}

void MainWindow::updateVSSGeometry()
{
#ifdef Q_WS_MAC
    int i = 20;
#else
    int i = 10;
#endif
	VSSClientsGridLayout->setGeometry(0, i, VSSClientsGroupBox->geometry().width(), VSSClientsGroupBox->geometry().height()-i);
	VSSSelectedClientGroupBox->setGeometry(0, 0, VSSStackedWidget->geometry().width(), VSSStackedWidget->geometry().height());
	VSSSelectedClientGridLayout->setGeometry(0, i, VSSSelectedClientGroupBox->geometry().width(), VSSSelectedClientGroupBox->geometry().height()-i);
	VSSPassMarkDetailsGroupBox->setGeometry(0, 0, VSSStackedWidget->geometry().width(), VSSStackedWidget->geometry().height());
	VSSPassMarkDetailsGridLayout->setGeometry(0, i, VSSPassMarkDetailsGroupBox->geometry().width(), VSSPassMarkDetailsGroupBox->geometry().height()-i);
	VSSLogGridLayout->setGeometry(0, i, VSSLogGroupBox->geometry().width(), VSSLogGroupBox->geometry().height()-i);
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
	updateGeometry();
	event->accept();
}

void MainWindow::setDatabaseModified() { this->setWindowModified(true); }

QString MainWindow::removeLineBreaks(QString str) { return str.remove("\n"); }

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

uint MainWindow::searchListWidgetItems(QString keyword, QListWidget * lw, QLineEdit * le)
{
	if (keyword.isEmpty()) {
		le->setPalette(qApp->palette());
	} else {
		le->setPalette(search_active_palette);
	}
	uint n = 0;
	for (int i = 0; i < lw->count(); ++i) {
		if (lw->item(i)->text().contains(keyword, Qt::CaseInsensitive)) {
			lw->item(i)->setHidden(false); n++;
		} else { lw->item(i)->setHidden(true); }
	}
	if ((!keyword.isEmpty()) && lw->count() != 0 && n == 0) {
		le->setPalette(search_noresults_palette);
	}
	return n;
}

uint MainWindow::searchTableWidgetItems(QString keyword, QTableWidget * tw, QLineEdit * le)
{
	if (keyword.isEmpty()) {
		le->setPalette(qApp->palette());
	} else {
		le->setPalette(search_active_palette);
	}
	uint n = 0;
	for (int i = 0; i < tw->rowCount(); ++i) {
		if (tw->item(i, 0)->text().contains(keyword, Qt::CaseInsensitive)) {
			tw->showRow(i); n++;
		} else { tw->hideRow(i); }
	}
	if ((!keyword.isEmpty()) && tw->rowCount() != 0 && n == 0) {
		le->setPalette(search_noresults_palette);
	}
	return n;
}

void MainWindow::overallStatistics()
{
	QuestionItem * q_item; QTableWidgetItem * tw_item;
	QWidget * stats_widget = new QWidget(this, Qt::Dialog /*| Qt::WindowMaximizeButtonHint*/);
	stats_widget->setWindowModality(Qt::WindowModal);
	stats_widget->setAttribute(Qt::WA_DeleteOnClose);
	stats_widget->setWindowTitle(tr("%1 - Overall statistics - iTest").arg(current_db_name));
	stats_widget->setMinimumSize(QSize(300, 200));
	QGridLayout * stats_glayout = new QGridLayout(stats_widget);
	QLabel * stats_label = new QLabel(stats_widget);
	stats_glayout->addWidget(stats_label, 0, 0);
	stats_tw = new QTableWidget(stats_widget);
	stats_glayout->addWidget(stats_tw, 1, 0);
	QHBoxLayout * stats_hlayout = new QHBoxLayout(stats_widget);
	stats_hlayout->setMargin(0); stats_glayout->setSpacing(6);
	stats_hlayout->addStretch();
	stats_btn_adjustall = new QPushButton (tr("Adjust all"), stats_widget);
	QObject::connect(stats_btn_adjustall, SIGNAL(released()), this, SLOT(statsAdjustAll()));
	stats_btn_adjustall->setEnabled(false);
	stats_hlayout->addWidget(stats_btn_adjustall);
	QPushButton * stats_btn_close = new QPushButton (tr("Close"), stats_widget);
	QObject::connect(stats_btn_close, SIGNAL(released()), stats_widget, SLOT(close()));
	QObject::connect(stats_widget, SIGNAL(destroyed()), this, SLOT(statsWidgetClosed()));
	stats_hlayout->addWidget(stats_btn_close);
	stats_glayout->addLayout(stats_hlayout, 2, 0);
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
		stats_btn_adjust->setEnabled(false);
		stats_tw->setCellWidget(row, 5, stats_btn_adjust);
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

void MainWindow::changeLanguage()
{
	QWidget * lang_widget = new QWidget(this, Qt::Dialog);
	lang_widget->setWindowModality(Qt::WindowModal);
	lang_widget->setAttribute(Qt::WA_DeleteOnClose);
	lang_widget->setWindowTitle(tr("Change language - iTest"));
	QGridLayout * lang_glayout = new QGridLayout(lang_widget);
	lang_glayout->setMargin(6); lang_glayout->setSpacing(6);
	QLabel * lang_label = new QLabel(lang_widget);
	lang_label->setText(tr("Select your preferred language"));
	lang_glayout->addWidget(lang_label, 0, 0);
	langComboBox = new QComboBox(lang_widget);
	QStringList langs(itest_i18n.values()); langs.sort();
	for (int i = 0; i < langs.count(); ++i) {
		langComboBox->addItem(tr(QByteArray().append(langs.at(i)).constData()), lang_widget);
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

void MainWindow::about()
{
    AboutWidget * itest_about = new AboutWidget(ver, QString("4.3.1"), QString("2007"));
    itest_about->setWindowFlags(Qt::Dialog /*| Qt::WindowMaximizeButtonHint*/ | Qt::WindowStaysOnTopHint);
	itest_about->show();
}
