#include "main_window.h"

void MainWindow::setupSessionViewer()
{
	VSSCSGroupBox->setEnabled(false); enableVSSTools();
	QObject::connect(mainStackedWidget, SIGNAL(currentChanged(int)), this, SLOT(enableVSSTools()));
	QObject::connect(VSSSplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(updateVSSGeometry()));
	QObject::connect(VSSLSListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentSession(QListWidgetItem *)));
	QObject::connect(VSSLASListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentSession(QListWidgetItem *)));
	QObject::connect(VSSLCListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentStudent()));
	QObject::connect(actionDelete_log, SIGNAL(triggered()), this, SLOT(deleteLog()));
	QObject::connect(actionArchive_session, SIGNAL(triggered()), this, SLOT(archiveSession()));
	QObject::connect(actionRestore_session, SIGNAL(triggered()), this, SLOT(restoreSession()));
	QObject::connect(actionCopy_to_archive, SIGNAL(triggered()), this, SLOT(copyToArchive()));
	QObject::connect(actionCopy_from_archive, SIGNAL(triggered()), this, SLOT(copyFromArchive()));
	QObject::connect(actionPrint_all, SIGNAL(triggered()), this, SLOT(printAll()));
	QObject::connect(actionPrint_session_summary, SIGNAL(triggered()), this, SLOT(printSessionSummary()));
	QObject::connect(VSSLSSearchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchVSSLS(const QString &)));
	VSSLSListWidget->setSortingEnabled(true);
	VSSLASListWidget->setSortingEnabled(true);
	hideArchive();
}

void MainWindow::openArchive()
{
	QString buffer; ArchivedSession * archived_session;
	QSettings archive(QSettings::IniFormat, QSettings::UserScope, "Michal Tomlein", "iTest");
	QStringList dbs = archive.value("databases").toStringList();
	if (!dbs.contains(current_db_name)) { return; }
	QStringList sns = archive.value(QString("%1/sessions").arg(current_db_name)).toStringList();
	for (int i = 0; i < sns.count(); ++i) {
		buffer = archive.value(QString("%1/%2").arg(current_db_name).arg(sns.at(i))).toString();
		if (buffer.isEmpty()) { continue; }
		archived_session = new ArchivedSession(this, buffer);
		current_db_archivedsessions.insert(archived_session->dateTime(), archived_session);
		QListWidgetItem * item = new QListWidgetItem (QString("%1 - %2").arg(archived_session->dateTimeToString()).arg(archived_session->name()));
		VSSLASListWidget->insertItem(0, item);
		item->setData(Qt::UserRole, archived_session->dateTime());
	}
	hideArchive();
}

void MainWindow::searchVSSLS(const QString & keyword)
{
	uint n = searchListWidgetItems(keyword, VSSLSListWidget, VSSLSSearchLineEdit);
	n += searchListWidgetItems(keyword, VSSLASListWidget, VSSLSSearchLineEdit);
	uint lwcount = VSSLSListWidget->count() + VSSLASListWidget->count();
	if (keyword.isEmpty()) {
		VSSLSSearchLineEdit->setPalette(qApp->palette());
	} else {
		VSSLSSearchLineEdit->setPalette(search_active_palette);
	}
	if ((!keyword.isEmpty()) && lwcount != 0 && n == 0) {
		VSSLSSearchLineEdit->setPalette(search_noresults_palette);
	}
}

void MainWindow::setCurrentSession(QListWidgetItem * item)
{
	if (item == NULL) { return; }
	selectSessionItem(item);
	Session * session;
	if (item->listWidget() == VSSLSListWidget) {
		session = current_db_sessions.value(item->data(Qt::UserRole).toDateTime());
	} else if (item->listWidget() == VSSLASListWidget) {
		session = current_db_archivedsessions.value(item->data(Qt::UserRole).toDateTime());
	} else { return; }
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
		if (current_db_session->passMark() <= session->student(i)->score()) {
            item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
            item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        } else {
            item->setBackground(QBrush::QBrush(QColor::QColor(204, 163, 0)));
            item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        }
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
	if (current_db_session->isArchived()) {
        ArchivedSession * archived_session = current_db_archivedsessions.value(current_db_session->dateTime());
        archived_session->setStatus(ArchivedSession::Archive);
		current_db_queued_sessions.removeAll(archived_session);
		current_db_queued_sessions.enqueue(archived_session);
    }
	VSSLogGroupBox->setVisible(false); enableVSSTools();
	setDatabaseModified();
	QTimer::singleShot(200, this, SLOT(updateVSSGeometry()));
}

void MainWindow::archiveSession()
{
	if (!VSSCSGroupBox->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = new ArchivedSession(this, current_db_session);
	archived_session->setStatus(ArchivedSession::Archive);
	current_db_queued_sessions.removeAll(archived_session);
	current_db_queued_sessions.enqueue(archived_session);
	for (int i = 0; i < VSSLSListWidget->count(); ++i) {
		if (VSSLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete VSSLSListWidget->item(i); }
	}
	current_db_sessions.remove(current_db_session->dateTime());
	current_db_archivedsessions.insert(archived_session->dateTime(), archived_session);
	delete current_db_session;
	current_db_session = archived_session;
	for (int i = 0; i < VSSLASListWidget->count(); ++i) {
		if (VSSLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete VSSLASListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(archived_session->dateTimeToString()).arg(archived_session->name()));
	VSSLASListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, archived_session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

void MainWindow::copyToArchive()
{
	if (!VSSCSGroupBox->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = new ArchivedSession(this, current_db_session);
	archived_session->setStatus(ArchivedSession::Archive);
	current_db_queued_sessions.removeAll(archived_session);
	current_db_queued_sessions.enqueue(archived_session);
	current_db_archivedsessions.insert(archived_session->dateTime(), archived_session);
	current_db_session = archived_session;
	for (int i = 0; i < VSSLASListWidget->count(); ++i) {
		if (VSSLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete VSSLASListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(archived_session->dateTimeToString()).arg(archived_session->name()));
	VSSLASListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, archived_session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

void MainWindow::restoreSession()
{
	if (!VSSCSGroupBox->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (!current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = current_db_archivedsessions.value(current_db_session->dateTime());
	archived_session->setStatus(ArchivedSession::Remove);
	current_db_queued_sessions.removeAll(archived_session);
	current_db_queued_sessions.enqueue(archived_session);
	Session * session = new Session(archived_session);
	for (int i = 0; i < VSSLASListWidget->count(); ++i) {
		if (VSSLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete VSSLASListWidget->item(i); }
	}
	current_db_archivedsessions.remove(current_db_session->dateTime());
	current_db_sessions.insert(session->dateTime(), session);
	current_db_session = session;
	for (int i = 0; i < VSSLSListWidget->count(); ++i) {
		if (VSSLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete VSSLSListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name()));
	VSSLSListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

void MainWindow::copyFromArchive()
{
	if (!VSSCSGroupBox->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (!current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = current_db_archivedsessions.value(current_db_session->dateTime());
	Session * session = new Session(archived_session);
	current_db_sessions.insert(session->dateTime(), session);
	current_db_session = session;
	for (int i = 0; i < VSSLSListWidget->count(); ++i) {
		if (VSSLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete VSSLSListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name()));
	VSSLSListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

/*void MainWindow::deleteSession()
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
}*/

void MainWindow::loadStudentResults(QMap<QString, QuestionAnswer> * results, QTableWidget * tw)
{
	loadClientResults(results, tw);
}

void MainWindow::selectSessionItem(QListWidgetItem * item)
{
    for (int i = 0; i < VSSLSListWidget->count(); ++i) {
		VSSLSListWidget->item(i)->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
		VSSLSListWidget->item(i)->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
	}
	for (int i = 0; i < VSSLASListWidget->count(); ++i) {
		VSSLASListWidget->item(i)->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
		VSSLASListWidget->item(i)->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
	}
	item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
    item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
	item->listWidget()->clearSelection();
}

void MainWindow::hideArchive()
{
	if (VSSLASListWidget->count() == 0) {
		label_archived_sessions->setVisible(false);
		VSSLASListWidget->setVisible(false);
	} else {
		label_archived_sessions->setVisible(true);
		VSSLASListWidget->setVisible(true);
	}
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
	VSSLASListWidget->clear();
	VSSNameLabel->setText("");
	VSSDateTimeLabel->setText("");
	VSSPassMarkLabel->setText("");
	VSSCSGroupBox->setEnabled(false);
	VSSLCListWidget->clear();
	clearVSSSC();
	VSSSLListWidget->clear();
	hideArchive();
}

void MainWindow::enableVSSTools()
{
    switch (mainStackedWidget->currentIndex()) {
        case 5:
            actionExport_log->setEnabled(true);
            actionDelete_log->setEnabled(false);
            actionArchive_session->setEnabled(false);
            actionRestore_session->setEnabled(false);
            actionCopy_to_archive->setEnabled(false);
            actionCopy_from_archive->setEnabled(false);
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
                if (current_db_session != NULL) {
                    if (current_db_session->isArchived()) {
                        actionArchive_session->setEnabled(false);
                        actionCopy_to_archive->setEnabled(false);
                        actionRestore_session->setEnabled(true);
                        actionCopy_from_archive->setEnabled(true);
                    } else {
                        actionArchive_session->setEnabled(true);
                        actionCopy_to_archive->setEnabled(true);
                        actionRestore_session->setEnabled(false);
                        actionCopy_from_archive->setEnabled(false);
                    }
                } else {
                    actionArchive_session->setEnabled(false);
                    actionRestore_session->setEnabled(false);
                    actionCopy_to_archive->setEnabled(false);
                    actionCopy_from_archive->setEnabled(false);
                }
            } else {
                actionExport_log->setEnabled(false);
                actionDelete_log->setEnabled(false);
                actionArchive_session->setEnabled(false);
                actionRestore_session->setEnabled(false);
                actionCopy_to_archive->setEnabled(false);
                actionCopy_from_archive->setEnabled(false);
            }
            break;
        default:
            actionExport_log->setEnabled(false);
            actionDelete_log->setEnabled(false);
            actionArchive_session->setEnabled(false);
            actionRestore_session->setEnabled(false);
            actionCopy_to_archive->setEnabled(false);
            actionCopy_from_archive->setEnabled(false);
            break;
    }
}
