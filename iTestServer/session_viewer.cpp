/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2014 Michal Tomlein

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
#include "session.h"
#include "student.h"

#include <QMessageBox>
#include <QSettings>

void MainWindow::setupSessionViewer()
{
    SVSelectedSessionWidget->setEnabled(false); enableSVTools();
    passMarkDetailsTab->setEnabled(false);
    scoringSystemTab->setEnabled(false);
    QObject::connect(SVLSListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentSession(QListWidgetItem *)));
    QObject::connect(SVLCListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentStudent()));
    QObject::connect(SVLCListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showSelectedStudentTab()));
    QObject::connect(actionDelete_log, SIGNAL(triggered()), this, SLOT(deleteLog()));
    QObject::connect(actionPrint_all, SIGNAL(triggered()), this, SLOT(printAll()));
    QObject::connect(actionPrint_session_summary, SIGNAL(triggered()), this, SLOT(printSessionSummary()));
    QObject::connect(SVLSSearchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchSVLS(const QString &)));
    QObject::connect(SVSearchDetailsLineEdit, SIGNAL(textChanged(QLineEdit *, const QString &)), SVPassMarkTableWidget, SLOT(filterItems(QLineEdit *, const QString &)));
    SVLSListWidget->setSortingEnabled(true);
    SVPassMarkTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    SVPassMarkTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    SVPassMarkTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SVScoringSystemTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    SVScoringSystemTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    SVScoringSystemTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < SVScoringSystemTableWidget->rowCount(); ++i) {
        SVScoringSystemTableWidget->setItem(i, 1, new QTableWidgetItem);
    }
}

void MainWindow::searchSVLS(const QString & keyword)
{
    if (keyword.isEmpty()) {
        SVLSSearchLineEdit->setPalette(qApp->palette());
    } else {
        SVLSSearchLineEdit->setPalette(searchLineEditPalettes.search_active_palette);
    }
    if ((!keyword.isEmpty()) && SVLSListWidget->count() != 0 && SVLSListWidget->filterItems(SVLSSearchLineEdit, keyword) == 0) {
        SVLSSearchLineEdit->setPalette(searchLineEditPalettes.search_noresults_palette);
    }
}

void MainWindow::setCurrentSession(QListWidgetItem * item)
{
    if (item == NULL) { return; }
    selectSessionItem(item);
    Session * session = current_db_sessions.value(item->data(Qt::UserRole).toDateTime());
    current_db_session = session;
    SVSelectedSessionWidget->setEnabled(true);
    SVNameLabel->setText(session->name());
    SVDateTimeLabel->setText(session->dateTimeToString());
    SVPassMarkLabel->setText(QString::number(session->passMark().passMark()));
    SVOverallResultsProgressBar->setMaximum((int)session->maximumScore());
    SVOverallResultsProgressBar->setValue(session->score() > 0 ? (int)session->score() : 0);
    SVOverallResultsLabel->setText(QString("%1%").arg(session->average()));
    current_db_students.clear();
    SVLCListWidget->clear(); SVLogListWidget->clear();
    for (int i = 0; i < session->numLogEntries(); ++i) {
        QListWidgetItem * log_entry = new QListWidgetItem (session->logEntry(i).entryText(), SVLogListWidget);
        log_entry->setBackground(QBrush(session->logEntry(i).entryBackgroundColour()));
        log_entry->setForeground(QBrush(session->logEntry(i).entryForegroundColour()));
    }
    SVLogGroupBox->setVisible(session->numLogEntries() > 0);
    for (int i = 0; i < session->numStudents(); ++i) {
        QListWidgetItem * item = new QListWidgetItem(session->student(i)->name(), SVLCListWidget);
        current_db_students.insert(item, session->student(i));
        if (session->student(i)->passed()) {
            item->setBackground(QBrush(QColor(197, 255, 120)));
        } else {
            item->setBackground(QBrush(QColor(204, 163, 0)));
        }
        item->setForeground(QBrush(QColor(0, 0, 0)));
    }
    SVPassMarkTableWidget->clearContents();
    SVPassMarkTableWidget->verticalHeader()->hide();
    if (session->passMark().count() > 0) {
        passMarkDetailsTab->setEnabled(true); QTableWidgetItem * item;
        SVPassMarkTableWidget->setRowCount(session->passMark().count());
        for (int i = 0; i < session->passMark().count(); ++i) {
            if (session->passMark().condition(i) < 0 || session->passMark().condition(i) >= current_db_categories.size()) { continue; }
            item = new QTableWidgetItem(QString("%1 - %2").arg(session->passMark().condition(i) + 1).arg(current_db_categories[session->passMark().condition(i)]));
            item->setBackground(QBrush(backgroundColourForCategory(session->passMark().condition(i))));
            item->setForeground(QBrush(foregroundColourForCategory(session->passMark().condition(i))));
            SVPassMarkTableWidget->setItem(i, 0, item);
            item = new QTableWidgetItem(QString::number(session->passMark().value(i)));
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
        SVScoringSystemTableWidget->item(n + 1, 1)->setText(QString::number(session->scoringSystem().correctAnswer[i]));
        SVScoringSystemTableWidget->item(n + 2, 1)->setText(QString::number(session->scoringSystem().incorrectAnswer[i]));
        SVScoringSystemTableWidget->item(n + 3, 1)->setText(QString::number(session->scoringSystem().missingAnswer[i]));
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
    SVLogGroupBox->setVisible(false); enableSVTools();
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
        SVLSListWidget->item(i)->setBackground(QBrush(QColor(255, 255, 255)));
        SVLSListWidget->item(i)->setForeground(QBrush(QColor(0, 0, 0)));
    }
    item->setBackground(QBrush(QColor(197, 255, 120)));
    item->setForeground(QBrush(QColor(0, 0, 0)));
    item->listWidget()->clearSelection();
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
}
