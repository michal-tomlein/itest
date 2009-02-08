#include "main_window.h"

void MainWindow::setupSessionViewer()
{
	VSSCSGroupBox->setEnabled(false); enableVSSTools();
    QObject::connect(mainStackedWidget, SIGNAL(currentChanged(int)), this, SLOT(enableVSSTools()));
    QObject::connect(VSSSplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(updateVSSGeometry()));
	QObject::connect(VSSLSListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentSession(QListWidgetItem *)));
	QObject::connect(VSSLCListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentStudent()));
	QObject::connect(actionDelete_log, SIGNAL(triggered()), this, SLOT(deleteLog()));
	QObject::connect(actionDelete_session, SIGNAL(triggered()), this, SLOT(deleteSession()));
	QObject::connect(actionPrint_all, SIGNAL(triggered()), this, SLOT(printAll()));
	QObject::connect(actionPrint_session_summary, SIGNAL(triggered()), this, SLOT(printSessionSummary()));
	QObject::connect(VSSLSSearchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchVSSLS(const QString &)));
}

void MainWindow::searchVSSLS(const QString & keyword)
{
	searchListWidgetItems(keyword, VSSLSListWidget, VSSLSSearchLineEdit);
}

void MainWindow::setCurrentSession(QListWidgetItem * item)
{
	if (item == NULL) { return; }
	Session * session = current_db_sessions.value(item->data(Qt::UserRole).toDateTime());
	current_db_session = session;
	VSSCSGroupBox->setEnabled(true);
	VSSNameLabel->setText(session->name());
	VSSDateTimeLabel->setText(session->dateTimeToString());
	VSSPassMarkLabel->setText(QString("%1").arg(session->passMark()));
	VSSOverallResultsProgressBar->setMaximum(session->numQuestions());
	VSSOverallResultsProgressBar->setValue(session->numCorrect());
	current_db_students.clear();
	VSSLCListWidget->clear(); VSSSLListWidget->clear();
	for (int i = 0; i < session->numLogEntries(); ++i) {
		QListWidgetItem * log_entry = new QListWidgetItem (session->logEntry(i).entryText(), VSSSLListWidget);
		log_entry->setBackground(QBrush::QBrush(session->logEntry(i).entryBackgroundColour()));
		log_entry->setForeground(QBrush::QBrush(session->logEntry(i).entryForegroundColour()));
	}
	if (session->numLogEntries() <= 0) { VSSLogGroupBox->setVisible(false); }
	else { VSSLogGroupBox->setVisible(true); }
	for (int i = 0; i < session->numStudents(); ++i) {
		QListWidgetItem * item = new QListWidgetItem (session->student(i)->name(), VSSLCListWidget);
		current_db_students.insert(item, session->student(i));
	}
	VSSClientsGroupBox->setEnabled(true); VSSLogGroupBox->setEnabled(true);
	togglePrintEnabled(); enableVSSTools();
	QTimer::singleShot(200, this, SLOT(updateVSSGeometry()));
}

void MainWindow::setCurrentStudent()
{
	if (VSSLCListWidget->currentIndex().isValid()) {
		VSSSelectedClientGroupBox->setEnabled(true); clearVSSSC(); togglePrintEnabled();
		Student * student = current_db_students.value(VSSLCListWidget->currentItem());
		VSSSCNameLabel->setText(student->name());
		if (student->isReady()) {
            if (current_db_session != NULL) {
                VSSSCScoreLabel->setText(tr("%1 out of %2 (%3)").arg(student->score()).arg(student->results()->count()).arg(current_db_session->passMark() <= student->score() ? tr("PASSED") : tr("FAILED")));
            } else {
                VSSSCScoreLabel->setText(tr("%1 out of %2").arg(student->score()).arg(student->results()->count()));
            }
			loadStudentResults(student->results(), VSSSCResultsTableWidget);
		}
	} else {
		VSSSelectedClientGroupBox->setEnabled(false); clearVSSSC(); togglePrintEnabled();
	}
}

void MainWindow::deleteLog()
{
	if (!VSSCSGroupBox->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	switch (QMessageBox::information(this, tr("Delete log"), tr("Are you sure you want to delete the log for session \"%1 - %2\"?").arg(current_db_session->dateTimeToString()).arg(current_db_session->name()), tr("&Delete"), tr("Cancel"), 0, 1)) {
		case 0: // Delete
			break;
		case 1: // Cancel
			return; break;
	}
	VSSSLListWidget->clear(); current_db_session->deleteLog();
	VSSLogGroupBox->setVisible(false); enableVSSTools();
	setDatabaseModified();
	QTimer::singleShot(200, this, SLOT(updateVSSGeometry()));
}

void MainWindow::deleteSession()
{
	if (!VSSCSGroupBox->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	switch (QMessageBox::information(this, tr("Delete session"), tr("Are you sure you want to delete session \"%1 - %2\"?").arg(current_db_session->dateTimeToString()).arg(current_db_session->name()), tr("&Delete"), tr("Cancel"), 0, 1)) {
		case 0: // Delete
			break;
		case 1: // Cancel
			return; break;
	}
	VSSCSGroupBox->setEnabled(false);
	VSSNameLabel->setText("");
	VSSDateTimeLabel->setText("");
	VSSPassMarkLabel->setText("");
	current_db_students.clear();
	VSSLCListWidget->clear(); VSSSLListWidget->clear();
	VSSClientsGroupBox->setEnabled(false); VSSLogGroupBox->setEnabled(false);
	VSSSelectedClientGroupBox->setEnabled(false); clearVSSSC();
	togglePrintEnabled(); enableVSSTools();
	QTimer::singleShot(200, this, SLOT(updateVSSGeometry()));
	for (int i = 0; i < VSSLSListWidget->count(); ++i) {
		if (VSSLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete VSSLSListWidget->item(i); }
	}
	current_db_sessions.remove(current_db_session->dateTime());
	delete current_db_session;
	setDatabaseModified();
}

void MainWindow::loadStudentResults(QMap<QString, QuestionAnswer> * results, QTableWidget * tw)
{
	loadClientResults(results, tw);
}

void MainWindow::clearVSSSC()
{
	VSSSCNameLabel->setText(tr("Not identified yet"));
	VSSSCScoreLabel->setText(tr("Not ready yet"));
	VSSSCResultsTableWidget->setRowCount(0);
}

void MainWindow::clearVSS()
{
	VSSLSListWidget->clear();
	VSSNameLabel->setText("");
	VSSDateTimeLabel->setText("");
	VSSPassMarkLabel->setText("");
	VSSCSGroupBox->setEnabled(false);
	VSSLCListWidget->clear();
	clearVSSSC();
	VSSSLListWidget->clear();
}

void MainWindow::enableVSSTools()
{
    switch (mainStackedWidget->currentIndex()) {
        case 5:
            actionExport_log->setEnabled(true);
            actionDelete_log->setEnabled(false);
            actionDelete_session->setEnabled(false);
            break;
        case 6:
            if (VSSCSGroupBox->isEnabled()) {
                if (VSSSLListWidget->count() > 0) {
                    actionExport_log->setEnabled(true);
                    actionDelete_log->setEnabled(true);
                } else {
                    actionExport_log->setEnabled(false);
                    actionDelete_log->setEnabled(false);
                }
                actionDelete_session->setEnabled(true);
            } else {
                actionExport_log->setEnabled(false);
                actionDelete_log->setEnabled(false);
                actionDelete_session->setEnabled(false);
            }
            break;
        default:
            actionExport_log->setEnabled(false);
            actionDelete_log->setEnabled(false);
            actionDelete_session->setEnabled(false);
            break;
    }
}
