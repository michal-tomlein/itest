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

#include "main_window.h"

void MainWindow::setupSessionViewer()
{
	SVSelectedSessionWidget->setEnabled(false); enableSVTools();
	passMarkDetailsTab->setEnabled(false);
    scoringSystemTab->setEnabled(false);
	QObject::connect(SVLSListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentSession(QListWidgetItem *)));
	QObject::connect(SVLASListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentSession(QListWidgetItem *)));
	QObject::connect(SVLCListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentStudent()));
    QObject::connect(SVLCListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showSelectedStudentTab()));
	QObject::connect(actionDelete_log, SIGNAL(triggered()), this, SLOT(deleteLog()));
	QObject::connect(actionArchive_session, SIGNAL(triggered()), this, SLOT(archiveSession()));
	QObject::connect(actionRestore_session, SIGNAL(triggered()), this, SLOT(restoreSession()));
	QObject::connect(actionCopy_to_archive, SIGNAL(triggered()), this, SLOT(copyToArchive()));
	QObject::connect(actionCopy_from_archive, SIGNAL(triggered()), this, SLOT(copyFromArchive()));
	QObject::connect(actionPrint_all, SIGNAL(triggered()), this, SLOT(printAll()));
	QObject::connect(actionPrint_session_summary, SIGNAL(triggered()), this, SLOT(printSessionSummary()));
	QObject::connect(SVLSSearchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchSVLS(const QString &)));
    QObject::connect(SVSearchDetailsLineEdit, SIGNAL(textChanged(QLineEdit *, const QString &)), SVPassMarkTableWidget, SLOT(filterItems(QLineEdit *, const QString &)));
	SVLSListWidget->setSortingEnabled(true);
	SVLASListWidget->setSortingEnabled(true);
    SVPassMarkTableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    SVPassMarkTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    SVPassMarkTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SVScoringSystemTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    SVScoringSystemTableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    SVScoringSystemTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < SVScoringSystemTableWidget->rowCount(); ++i) {
        SVScoringSystemTableWidget->setItem(i, 1, new QTableWidgetItem);
    }
	hideArchive();
}

void MainWindow::openArchive()
{
	QString buffer; QuestionItem * q_item;
    QStringList qa_flaglist; bool qa_flaglist_found;
    QStringList qa_anslist; bool qa_anslist_found;
    QStringList qa_correctanslist; bool qa_correctanslist_found;
    QStringList qa_diflist; bool qa_diflist_found;
    QStringList qa_selectiontypelist; bool qa_selectiontypelist_found;
	ArchivedSession * archived_session; bool rearchive;
	QSettings archive(QSettings::IniFormat, QSettings::UserScope, "Michal Tomlein", "iTest");
	QStringList dbs = archive.value("databases").toStringList();
	if (!dbs.contains(current_db_name)) { return; }
	QStringList sns = archive.value(QString("%1/sessions").arg(current_db_name)).toStringList();
	for (int i = 0; i < sns.count(); ++i) {
		buffer = archive.value(QString("%1/%2").arg(current_db_name).arg(sns.at(i))).toString();
		if (buffer.isEmpty()) { continue; }
		archived_session = new ArchivedSession(this, buffer);
		current_db_archivedsessions.insert(archived_session->dateTime(), archived_session);
		QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(archived_session->dateTimeToString()).arg(archived_session->name()));
		SVLASListWidget->insertItem(0, item);
		item->setData(Qt::UserRole, archived_session->dateTime());
		rearchive = false;
		buffer = archive.value(QString("%1/%2/PassMark").arg(current_db_name).arg(sns.at(i))).toString();
		if (!buffer.isEmpty()) {
			archived_session->loadPassMark(buffer);
		} else { rearchive = true; }
		buffer = archive.value(QString("%1/%2/StudentsPassed").arg(current_db_name).arg(sns.at(i))).toString();
		if (buffer.length() == archived_session->numStudents()) {
			for (int i = 0; i < archived_session->numStudents(); ++i) {
				archived_session->student(i)->setPassed(buffer.at(i) == '+');
			}
		} else {
			for (int i = 0; i < archived_session->numStudents(); ++i) {
				archived_session->student(i)->setPassed(archived_session->passMark().check(archived_session->student(i)->results(), &current_db_questions, archived_session->scoringSystem()));
			}
			rearchive = true;
		}
		int numres = 0;
		for (int s = 0; s < archived_session->numStudents(); ++s) {
			numres += archived_session->student(s)->results()->count();
		}
		qa_flaglist = archive.value(QString("%1/%2/QAFlags").arg(current_db_name).arg(sns.at(i))).toString().split(";");
        qa_flaglist_found = qa_flaglist.count() == numres;
        qa_anslist = archive.value(QString("%1/%2/QAAnswers").arg(current_db_name).arg(sns.at(i))).toString().split(";");
		qa_anslist_found = qa_anslist.count() == numres;
		qa_correctanslist = archive.value(QString("%1/%2/QACorrectAnswers").arg(current_db_name).arg(sns.at(i))).toString().split(";");
		qa_correctanslist_found = qa_correctanslist.count() == numres;
        qa_diflist = archive.value(QString("%1/%2/QADifs").arg(current_db_name).arg(sns.at(i))).toString().split(";");
        qa_diflist_found = qa_diflist.count() == numres;
        qa_selectiontypelist = archive.value(QString("%1/%2/QASeletionTypes").arg(current_db_name).arg(sns.at(i))).toString().split(";");
        qa_selectiontypelist_found = qa_selectiontypelist.count() == numres;
		if (!qa_flaglist_found || !qa_anslist_found || !qa_correctanslist_found || !qa_diflist_found || !qa_selectiontypelist_found) { rearchive = true; }
        int x = 0;
		for (int s = 0; s < archived_session->numStudents(); ++s) {
			QMapIterator<QString, QuestionAnswer> qa(*(archived_session->student(s)->results())); QuestionAnswer qans;
			while (qa.hasNext()) { qa.next();
				q_item = NULL; qans = qa.value();
				if (qa_flaglist_found)
				    { qans.setFlag(qa_flaglist.at(x).toInt()); }
                if (qa_anslist_found)
				    { qans.setAnswered((Question::Answer)qa_anslist.at(x).toInt()); }
				if (qa_correctanslist_found)
				    { qans.setCorrectAnswer((Question::Answer)qa_correctanslist.at(x).toInt()); }
                if (qa_diflist_found)
				    { qans.setDifficulty(qa_diflist.at(x).toInt()); }
				if (qa_selectiontypelist_found)
				    { qans.setSelectionType((Question::SelectionType)qa_selectiontypelist.at(x).toInt()); }
				if (!qa_flaglist_found || !qa_correctanslist_found || !qa_diflist_found || !qa_selectiontypelist_found) {
				    QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
					while (q.hasNext()) { q.next();
						if (q.value()->name() == qa.key()) { q_item = q.value(); break; }
					}
					if (q_item == NULL) {
					    if (!qa_flaglist_found) { qans.setFlag(-1); }
					} else {
						if (!qa_flaglist_found) { qans.setFlag(q_item->flag()); }
						if (!qa_correctanslist_found)
						    { qans.setCorrectAnswer(q_item->correctAnswer()); }
                        if (!qa_diflist_found)
                            { qans.setDifficulty(q_item->difficulty()); }
                        if (!qa_selectiontypelist_found)
                            { qans.setSelectionType(q_item->selectionType()); }
					}
				}
				archived_session->student(s)->results()->insert(qa.key(), qans);
				x++;
			}
		}
        buffer = archive.value(QString("%1/%2/ScoringSystem").arg(current_db_name).arg(sns.at(i))).toString();
		if (!buffer.isEmpty()) {
            ScoringSystem sys(buffer);
			archived_session->setScoringSystem(sys);
		} else {
            archived_session->setScoringSystem(ScoringSystem());
            rearchive = true;
        }
		if (rearchive) { archived_session->archive(); }
	}
	hideArchive();
}

void MainWindow::searchSVLS(const QString & keyword)
{
	uint n = SVLSListWidget->filterItems(SVLSSearchLineEdit, keyword);
	n += SVLASListWidget->filterItems(SVLSSearchLineEdit, keyword);
	uint lwcount = SVLSListWidget->count() + SVLASListWidget->count();
	if (keyword.isEmpty()) {
		SVLSSearchLineEdit->setPalette(qApp->palette());
	} else {
		SVLSSearchLineEdit->setPalette(searchLineEditPalettes.search_active_palette);
	}
	if ((!keyword.isEmpty()) && lwcount != 0 && n == 0) {
		SVLSSearchLineEdit->setPalette(searchLineEditPalettes.search_noresults_palette);
	}
}

void MainWindow::setCurrentSession(QListWidgetItem * item)
{
	if (item == NULL) { return; }
	selectSessionItem(item);
	Session * session;
	if (item->listWidget() == SVLSListWidget) {
		session = current_db_sessions.value(item->data(Qt::UserRole).toDateTime());
	} else if (item->listWidget() == SVLASListWidget) {
		session = current_db_archivedsessions.value(item->data(Qt::UserRole).toDateTime());
	} else { return; }
	current_db_session = session;
	SVSelectedSessionWidget->setEnabled(true);
	SVNameLabel->setText(session->name());
	SVDateTimeLabel->setText(session->dateTimeToString());
	SVPassMarkLabel->setText(QString("%1").arg(session->passMark().passMark()));
	SVOverallResultsProgressBar->setMaximum((int)session->maximumScore());
	SVOverallResultsProgressBar->setValue(session->score() > 0 ? (int)session->score() : 0);
    SVOverallResultsLabel->setText(QString("%1%").arg(session->average()));
	current_db_students.clear();
	SVLCListWidget->clear(); SVLogListWidget->clear();
	for (int i = 0; i < session->numLogEntries(); ++i) {
		QListWidgetItem * log_entry = new QListWidgetItem (session->logEntry(i).entryText(), SVLogListWidget);
		log_entry->setBackground(QBrush::QBrush(session->logEntry(i).entryBackgroundColour()));
		log_entry->setForeground(QBrush::QBrush(session->logEntry(i).entryForegroundColour()));
	}
	SVLogGroupBox->setVisible(session->numLogEntries() > 0);
	for (int i = 0; i < session->numStudents(); ++i) {
		QListWidgetItem * item = new QListWidgetItem(session->student(i)->name(), SVLCListWidget);
		current_db_students.insert(item, session->student(i));
		if (session->student(i)->passed()) {
            item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
        } else {
            item->setBackground(QBrush::QBrush(QColor::QColor(204, 163, 0)));
        }
        item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
	}
    SVPassMarkTableWidget->clearContents();
    SVPassMarkTableWidget->verticalHeader()->hide();
	if (session->passMark().count() > 0) {
		passMarkDetailsTab->setEnabled(true); QTableWidgetItem * item;
		SVPassMarkTableWidget->setRowCount(session->passMark().count());
		for (int i = 0; i < session->passMark().count(); ++i) {
			if (session->passMark().condition(i) < 0 || session->passMark().condition(i) >= 20) { continue; }
		    item = new QTableWidgetItem(QString("%1 - %2").arg(session->passMark().condition(i) + 1).arg(current_db_f[session->passMark().condition(i)]));
		    item->setBackground(QBrush::QBrush(backgroundColourForFlag(session->passMark().condition(i))));
		    item->setForeground(QBrush::QBrush(foregroundColourForFlag(session->passMark().condition(i))));
		    SVPassMarkTableWidget->setItem(i, 0, item);
		    item = new QTableWidgetItem(QString("%1").arg(session->passMark().value(i)));
		    SVPassMarkTableWidget->setItem(i, 1, item);
		    SVPassMarkTableWidget->setRowHeight(i, 16);
		}
		SVPassMarkTableWidget->filterItems(SVSearchDetailsLineEdit, SVSearchDetailsLineEdit->text());
		SVStudentTabWidget->setCurrentIndex(1);
	} else {
		passMarkDetailsTab->setEnabled(false);
		SVStudentTabWidget->setCurrentIndex(0);
	}
    selectedStudentTab->setEnabled(false);
    scoringSystemTab->setEnabled(true);
    SVScoringSystemTableWidget->item(0, 1)->setText(session->scoringSystem().allowIncompleteAnswers ? tr("allowed") : tr("disallowed"));
    for (int i = 0, n = 0; i < 3; ++i, n += 3) {
        SVScoringSystemTableWidget->item(n + 1, 1)->setText(QString("%1").arg(session->scoringSystem().correctAnswer[i]));
        SVScoringSystemTableWidget->item(n + 2, 1)->setText(QString("%1").arg(session->scoringSystem().incorrectAnswer[i]));
        SVScoringSystemTableWidget->item(n + 3, 1)->setText(QString("%1").arg(session->scoringSystem().missingAnswer[i]));
    }
	SVLCGroupBox->setEnabled(true); SVLogGroupBox->setEnabled(true);
	togglePrintEnabled(); enableSVTools();
}

void MainWindow::setCurrentStudent()
{
	if (SVLCListWidget->currentIndex().isValid()) {
		selectedStudentTab->setEnabled(true); clearSVSC(); togglePrintEnabled();
		SVStudentTabWidget->setCurrentIndex(0);
		Student * student = current_db_students.value(SVLCListWidget->currentItem());
		SVStudentNameLabel->setText(student->name());
		if (student->isReady()) {
            if (current_db_session != NULL) {
                SVStudentScoreLabel->setText(tr("%1 out of %2 (%3)").arg(student->score()).arg(student->maximumScore()).arg(student->passed() ? tr("PASSED") : tr("FAILED")));
            } else {
                SVStudentScoreLabel->setText(tr("%1 out of %2").arg(student->score()).arg(student->maximumScore()));
            }
			loadStudentResults(student->results());
		}
	} else {
		selectedStudentTab->setEnabled(false); clearSVSC(); togglePrintEnabled();
	}
}

void MainWindow::showSelectedStudentTab()
{
	SVStudentTabWidget->setCurrentIndex(0);
}

void MainWindow::deleteLog()
{
	if (!SVSelectedSessionWidget->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	switch (QMessageBox::information(this, tr("Delete log"), tr("Are you sure you want to delete the log for session \"%1 - %2\"?").arg(current_db_session->dateTimeToString()).arg(current_db_session->name()), tr("&Delete"), tr("Cancel"), 0, 1)) {
		case 0: // Delete
			break;
		case 1: // Cancel
			return; break;
	}
	SVLogListWidget->clear(); current_db_session->deleteLog();
	if (current_db_session->isArchived()) {
        ArchivedSession * archived_session = current_db_archivedsessions.value(current_db_session->dateTime());
        archived_session->setStatus(ArchivedSession::Archive);
		current_db_queued_sessions.removeAll(archived_session);
		current_db_queued_sessions.enqueue(archived_session);
    }
	SVLogGroupBox->setVisible(false); enableSVTools();
	setDatabaseModified();
}

void MainWindow::archiveSession()
{
	if (!SVSelectedSessionWidget->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = new ArchivedSession(this, current_db_session);
	archived_session->setStatus(ArchivedSession::Archive);
	current_db_queued_sessions.removeAll(archived_session);
	current_db_queued_sessions.enqueue(archived_session);
	for (int i = 0; i < SVLSListWidget->count(); ++i) {
		if (SVLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete SVLSListWidget->item(i); }
	}
	current_db_sessions.remove(current_db_session->dateTime());
	current_db_archivedsessions.insert(archived_session->dateTime(), archived_session);
	delete current_db_session;
	current_db_session = archived_session;
	for (int i = 0; i < SVLASListWidget->count(); ++i) {
		if (SVLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete SVLASListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(archived_session->dateTimeToString()).arg(archived_session->name()));
	SVLASListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, archived_session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

void MainWindow::copyToArchive()
{
	if (!SVSelectedSessionWidget->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = new ArchivedSession(this, current_db_session);
	archived_session->setStatus(ArchivedSession::Archive);
	current_db_queued_sessions.removeAll(archived_session);
	current_db_queued_sessions.enqueue(archived_session);
	current_db_archivedsessions.insert(archived_session->dateTime(), archived_session);
	current_db_session = archived_session;
	for (int i = 0; i < SVLASListWidget->count(); ++i) {
		if (SVLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete SVLASListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(archived_session->dateTimeToString()).arg(archived_session->name()));
	SVLASListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, archived_session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

void MainWindow::restoreSession()
{
	if (!SVSelectedSessionWidget->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (!current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = current_db_archivedsessions.value(current_db_session->dateTime());
	archived_session->setStatus(ArchivedSession::Remove);
	current_db_queued_sessions.removeAll(archived_session);
	current_db_queued_sessions.enqueue(archived_session);
	Session * session = new Session(archived_session);
	for (int i = 0; i < SVLASListWidget->count(); ++i) {
		if (SVLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete SVLASListWidget->item(i); }
	}
	current_db_archivedsessions.remove(current_db_session->dateTime());
	current_db_sessions.insert(session->dateTime(), session);
	current_db_session = session;
	for (int i = 0; i < SVLSListWidget->count(); ++i) {
		if (SVLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete SVLSListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name()));
	SVLSListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

void MainWindow::copyFromArchive()
{
	if (!SVSelectedSessionWidget->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	if (!current_db_session->isArchived()) { return; }
	ArchivedSession * archived_session = current_db_archivedsessions.value(current_db_session->dateTime());
	Session * session = new Session(archived_session);
	current_db_sessions.insert(session->dateTime(), session);
	current_db_session = session;
	for (int i = 0; i < SVLSListWidget->count(); ++i) {
		if (SVLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete SVLSListWidget->item(i); }
	}
	QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name()));
	SVLSListWidget->insertItem(0, item);
	item->setData(Qt::UserRole, session->dateTime());
	selectSessionItem(item);
	setCurrentSession(item);
	hideArchive();
	setDatabaseModified();
}

/*void MainWindow::deleteSession()
{
	if (!SVSelectedSessionWidget->isEnabled()) { return; }
	if (current_db_session == NULL) { return; }
	switch (QMessageBox::information(this, tr("Delete session"), tr("Are you sure you want to delete session \"%1 - %2\"?").arg(current_db_session->dateTimeToString()).arg(current_db_session->name()), tr("&Delete"), tr("Cancel"), 0, 1)) {
		case 0: // Delete
			break;
		case 1: // Cancel
			return; break;
	}
	SVSelectedSessionWidget->setEnabled(false);
	SVNameLabel->setText("");
	SVDateTimeLabel->setText("");
	SVPassMarkLabel->setText("");
	current_db_students.clear();
	SVLCListWidget->clear(); SVSLListWidget->clear();
	SVClientsGroupBox->setEnabled(false); SVLogGroupBox->setEnabled(false);
	SVSelectedClientGroupBox->setEnabled(false); clearSVSC();
	togglePrintEnabled(); enableSVTools();
	for (int i = 0; i < SVLSListWidget->count(); ++i) {
		if (SVLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == current_db_session->dateTime())
			{ delete SVLSListWidget->item(i); }
	}
	current_db_sessions.remove(current_db_session->dateTime());
	delete current_db_session;
	setDatabaseModified();
}*/

void MainWindow::loadStudentResults(QMap<QString, QuestionAnswer> * results)
{
    if (current_db_session == NULL) {
        ScoringSystem sys;
        loadClientResults(results, SVStudentResultsTableWidget, sys);
    } else {
        loadClientResults(results, SVStudentResultsTableWidget, current_db_session->scoringSystem());
    }
}

void MainWindow::selectSessionItem(QListWidgetItem * item)
{
    for (int i = 0; i < SVLSListWidget->count(); ++i) {
		SVLSListWidget->item(i)->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
		SVLSListWidget->item(i)->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
	}
	for (int i = 0; i < SVLASListWidget->count(); ++i) {
		SVLASListWidget->item(i)->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
		SVLASListWidget->item(i)->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
	}
	item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
    item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
	item->listWidget()->clearSelection();
}

void MainWindow::hideArchive()
{
	label_archived_sessions->setVisible(SVLASListWidget->count() != 0);
	SVLASListWidget->setVisible(SVLASListWidget->count() != 0);
}

void MainWindow::clearSVSC()
{
	SVStudentNameLabel->setText(tr("Not identified yet"));
	SVStudentScoreLabel->setText(tr("Not ready yet"));
	SVStudentResultsTableWidget->setRowCount(0);
}

void MainWindow::clearSV()
{
	SVLSListWidget->clear();
	SVLASListWidget->clear();
	SVNameLabel->setText("");
	SVDateTimeLabel->setText("");
	SVPassMarkLabel->setText("");
    SVOverallResultsLabel->setText("");
    SVPassMarkTableWidget->clearContents();
    SVPassMarkTableWidget->verticalHeader()->hide();
    for (int i = 0; i < SVScoringSystemTableWidget->rowCount(); ++i) {
        SVScoringSystemTableWidget->item(i, 1)->setText("");
    }
	SVSelectedSessionWidget->setEnabled(false);
	SVLCListWidget->clear();
	clearSVSC();
	SVLogListWidget->clear();
	hideArchive();
}
