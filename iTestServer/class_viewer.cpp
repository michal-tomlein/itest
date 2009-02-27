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

void MainWindow::setupClassViewer()
{
    tbtnAdd_class->setAction(actionAdd_class, true);
    tbtnAdd_class->setText(tr("Add"));
    tbtnAdd_class->setIconSize(QSize::QSize(16, 16));
    tbtnDelete_class->setAction(actionDelete_class, true);
    tbtnDelete_class->setText(tr("Delete"));
    tbtnDelete_class->setIconSize(QSize::QSize(16, 16));
    tbtnAdd_student->setAction(actionAdd_student, true);
    tbtnAdd_student->setText(tr("Add"));
    tbtnAdd_student->setIconSize(QSize::QSize(16, 16));
    tbtnDelete_student->setAction(actionDelete_student, true);
    tbtnDelete_student->setText(tr("Delete"));
    tbtnDelete_student->setIconSize(QSize::QSize(16, 16));
    tbtnAdd_session->setAction(actionAdd_session, true);
    tbtnAdd_session->setText(tr("Add"));
    tbtnAdd_session->setIconSize(QSize::QSize(16, 16));
    tbtnDelete_session->setAction(actionDelete_session, true);
    tbtnDelete_session->setText(tr("Delete"));
    tbtnDelete_session->setIconSize(QSize::QSize(16, 16));
    QObject::connect(actionAdd_class, SIGNAL(triggered()), this, SLOT(addClass()));
    QObject::connect(actionDelete_class, SIGNAL(triggered()), this, SLOT(deleteClass()));
    QObject::connect(actionPrint_class_summary, SIGNAL(triggered()), this, SLOT(printClassSummary()));
    QObject::connect(actionAdd_student, SIGNAL(triggered()), this, SLOT(addStudent()));
    QObject::connect(actionDelete_student, SIGNAL(triggered()), this, SLOT(deleteStudent()));
    QObject::connect(actionAdd_session, SIGNAL(triggered()), this, SLOT(addSession()));
    QObject::connect(actionDelete_session, SIGNAL(triggered()), this, SLOT(deleteSession()));
    QObject::connect(CLSCFirstYearSpinBox, SIGNAL(valueChanged(int)), CLSCLastYearSpinBox, SLOT(setMinimum(int)));
    QObject::connect(CLLCListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentClass(QListWidgetItem *)));
    QObject::connect(CLLSListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentClassMember(QListWidgetItem *)));
    QObject::connect(CLLSSListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(viewSession(QListWidgetItem *)));
    QObject::connect(CLLSSListWidget, SIGNAL(currentIndexAvailabilityChanged(bool)), actionDelete_session, SLOT(setEnabled(bool)));
    QObject::connect(CLLSSListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(toggleAddSessionToMemberEnabled()));
    QObject::connect(CLSSResultsTableWidget, SIGNAL(currentIndexAvailabilityChanged(bool)), tbtnRemoveSession, SLOT(setEnabled(bool)));
    QObject::connect(CLSSResultsTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(viewSessionAndStudent(QTableWidgetItem *)));
    QObject::connect(CLLCSearchLineEdit, SIGNAL(textChanged(QLineEdit *, const QString &)), CLLCListWidget, SLOT(filterItems(QLineEdit *, const QString &)));
    QObject::connect(CLLSSearchLineEdit, SIGNAL(textChanged(QLineEdit *, const QString &)), CLLSListWidget, SLOT(filterItems(QLineEdit *, const QString &)));
    QObject::connect(CLLSSSearchLineEdit, SIGNAL(textChanged(QLineEdit *, const QString &)), CLLSSListWidget, SLOT(filterItems(QLineEdit *, const QString &)));
    QObject::connect(tbtnApplyClassChanges, SIGNAL(released()), this, SLOT(updateClassProperties()));
    QObject::connect(tbtnSaveMemberName, SIGNAL(released()), this, SLOT(updateClassMemberName()));
    QObject::connect(tbtnAddSession, SIGNAL(released()), this, SLOT(addSessionToMember()));
    QObject::connect(tbtnRemoveSession, SIGNAL(released()), this, SLOT(removeSessionFromMember()));
    CLLCListWidget->setSortingEnabled(true);
    CLLSSListWidget->setSortingEnabled(true);
    CLSSResultsTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    CLSSResultsTableWidget->verticalHeader()->hide();
    CLSSResultsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setCLSCEnabled(false);
}

void MainWindow::setCurrentClass(QListWidgetItem * item)
{
	if (item == NULL) { return; }
    MTListWidget::highlightItem(item);
	clearCLSC(); setCLSCEnabled(true);
	Class * cl = current_db_classes.value(item, NULL);
	if (cl == NULL) { return; }
	current_db_class = cl;
	CLSCGroupBox->setEnabled(true);
	CLSCNameLineEdit->setText(cl->name());
	CLSCFirstYearSpinBox->setValue(cl->firstYear());
	CLSCLastYearSpinBox->setValue(cl->lastYear());
	CLSCNumStudentsLabel->setText(makeString(cl->numMembers()));
	CLSCAverageLabel->setText(QString("%1%").arg(cl->average(&current_db_sessions, &current_db_archivedsessions)));
	CLLSListWidget->clear();
	for (int i = 0; i < cl->numMembers(); ++i) {
		CLLSListWidget->addItem(cl->member(i)->name());
	}
	for (int i = 0; i < cl->numSessions(); ++i) {
	    Session * session = current_db_sessions.value(cl->session(i), current_db_archivedsessions.value(cl->session(i), new ArchivedSession(this)));
	    QListWidgetItem * item = new QListWidgetItem(QString("%1 - %2").arg(cl->sessionToString(i)).arg(session->name()));
	    item->setData(Qt::UserRole, cl->session(i));
        CLLSSListWidget->addItem(item);
	}
    togglePrintEnabled();
}

void MainWindow::setCurrentClassMember(QListWidgetItem * item)
{
    if (item == NULL) { return; }
    if (!current_db_class) { return; }
    MTListWidget::highlightItem(item);
    clearCLSS(); setCLSSEnabled(true);
    ClassMember * mem = current_db_class->member(CLLSListWidget->row(item));
    CLSSNameLineEdit->setText(mem->name());
    CLSSAverageLabel->setText(QString("%1%").arg(mem->average(&current_db_sessions, &current_db_archivedsessions)));
    CLSSResultsTableWidget->setRowCount(mem->numSessionEntries());
    CLSSResultsTableWidget->setSortingEnabled(false);
    for (int i = 0; i < mem->numSessionEntries(); ++i) {
        Session * session = current_db_sessions.value(mem->sessionEntry(i).session, current_db_archivedsessions.value(mem->sessionEntry(i).session, NULL));
        if (session != NULL) {
            CLSSResultsTableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name())));
            CLSSResultsTableWidget->item(i, 0)->setData(Qt::UserRole, session->dateTime());
            CLSSResultsTableWidget->setItem(i, 1, new QTableWidgetItem(session->student(mem->sessionEntry(i).member_num)->name()));
            CLSSResultsTableWidget->item(i, 1)->setData(Qt::UserRole, mem->sessionEntry(i).member_num);
            CLSSResultsTableWidget->item(i, 1)->setBackground(QBrush::QBrush(session->student(mem->sessionEntry(i).member_num)->passed() ? QColor::QColor(197, 255, 120) : QColor::QColor(204, 163, 0)));
            MTProgressBar * bar = new MTProgressBar(this);
            CLSSResultsTableWidget->setCellWidget(i, 2, bar);
            bar->setMaximum((int)(session->student(mem->sessionEntry(i).member_num)->maximumScore() * 100.0));
            bar->setValue((int)(session->student(mem->sessionEntry(i).member_num)->score() * 100.0));
            bar->setTextVisible(true);
        } else {
            CLSSResultsTableWidget->setItem(i, 0, new QTableWidgetItem(mem->sessionToString(i)));
            CLSSResultsTableWidget->item(i, 0)->setData(Qt::UserRole, mem->sessionEntry(i).session);
            CLSSResultsTableWidget->setItem(i, 1, new QTableWidgetItem);
            CLSSResultsTableWidget->item(i, 1)->setData(Qt::UserRole, mem->sessionEntry(i).member_num);
            CLSSResultsTableWidget->setItem(i, 2, new QTableWidgetItem);
        }
    }
    CLSSResultsTableWidget->setSortingEnabled(true);
    CLSSResultsTableWidget->resizeRowsToContents();
    toggleAddSessionToMemberEnabled();
    tbtnRemoveSession->setEnabled(CLSSResultsTableWidget->currentIndex().isValid());
    togglePrintEnabled();
}

void MainWindow::updateClassProperties()
{
    if (current_db_class == NULL) { return; }
    if (CLLCListWidget->highlightedRow() < 0) { return; }
    QString name = CLSCNameLineEdit->text();
    current_db_class->setName(name);
    current_db_class->setFirstYear(CLSCFirstYearSpinBox->value());
    current_db_class->setLastYear(CLSCLastYearSpinBox->value());
    CLLCListWidget->highlightedItem()->setText(QString("%1-%2: %3").arg(current_db_class->firstYear()).arg(current_db_class->lastYear()).arg(current_db_class->name()));
    setDatabaseModified();
}

void MainWindow::updateClassMemberName()
{
    if (current_db_class == NULL) { return; }
    if (CLLSListWidget->highlightedRow() < 0) { return; }
    QString name = CLSSNameLineEdit->text();
    current_db_class->member(CLLSListWidget->highlightedRow())->setName(name);
    CLLSListWidget->highlightedItem()->setText(name);
    setDatabaseModified();
}

void MainWindow::addClass()
{
    bool ok; QString c_name; addClass_start:
    c_name = QInputDialog::getText(this, tr("Add class"), tr("Class name:"), QLineEdit::Normal, tr("Class %1").arg(CLLCListWidget->count() + 1), &ok);
    if (!ok || c_name.isEmpty()) { return; }
    for (int i = 0; i < CLLCListWidget->count(); ++i) {
        if (CLLCListWidget->item(i)->text() == c_name) {
            QMessageBox::information(this, tr("Add class"), tr("A class with this name already exists. Please choose a different name."));
            goto addClass_start;
        }
    }
    Class * cl = new Class(c_name);
    QListWidgetItem * cl_item = new QListWidgetItem(QString("%1-%2: %3").arg(cl->firstYear()).arg(cl->lastYear()).arg(cl->name()), CLLCListWidget);
    current_db_classes.insert(cl_item, cl);
    setDatabaseModified();
}

void MainWindow::deleteClass()
{
    if (!current_db_class) { return; }
    QList<QListWidgetItem *> cl_items = current_db_classes.keys(current_db_class);
    if (cl_items.count() <= 0) { return; }
    switch (QMessageBox::information(this, tr("Delete class"), tr("Are you sure you want to delete class \"%1\"?").arg(cl_items.at(0)->text()), tr("&Delete"), tr("Cancel"), 0, 1)) {
        case 0: // Delete
            break;
        case 1: // Cancel
            return; break;
    }
    delete current_db_class;
    for (int i = 0; i < cl_items.count(); ++i) {
        current_db_classes.remove(cl_items.at(i));
        delete cl_items.at(i);
    }
    clearCLSC();
    togglePrintEnabled();
    setDatabaseModified();
}

void MainWindow::addStudent()
{
    if (current_db_class == NULL) { return; }
    bool ok; QString s_name; addStudent_start:
    s_name = QInputDialog::getText(this, tr("Add student"), tr("Student name:"), QLineEdit::Normal, tr("Student %1").arg(CLLSListWidget->count() + 1), &ok);
    if (!ok || s_name.isEmpty()) { return; }
    for (int i = 0; i < CLLSListWidget->count(); ++i) {
        if (CLLSListWidget->item(i)->text() == s_name) {
            switch (QMessageBox::information(this, tr("Add student"), tr("A student with the name \"%1\" already exists in the selected class. Would you like to choose a different name?").arg(s_name), tr("&Use this name anyway"), tr("&Choose a different name"), 0, 1)) {
                case 0: // Use this name anyway
                    break;
                case 1: // Choose a different name
                    goto addStudent_start; break;
            }
        }
    }
    CLLSListWidget->insertItem(current_db_class->addMember(new ClassMember(s_name)), s_name);
    CLSCNumStudentsLabel->setText(makeString(current_db_class->numMembers()));
    setDatabaseModified();
}

void MainWindow::deleteStudent()
{
    if (current_db_class == NULL) { return; }
    if (CLLSListWidget->highlightedRow() < 0) { return; }
    switch (QMessageBox::information(this, tr("Delete student"), tr("Are you sure you want to delete student \"%1\"?").arg(CLLSListWidget->highlightedItem()->text()), tr("&Delete"), tr("Cancel"), 0, 1)) {
        case 0: // Delete
            break;
        case 1: // Cancel
            return; break;
    }
    current_db_class->removeMember(CLLSListWidget->highlightedRow());
    delete CLLSListWidget->takeItem(CLLSListWidget->highlightedRow());
    CLSCNumStudentsLabel->setText(makeString(current_db_class->numMembers()));
    CLSCAverageLabel->setText(QString("%1%").arg(current_db_class->average(&current_db_sessions, &current_db_archivedsessions)));
    clearCLSS();
    togglePrintEnabled();
    setDatabaseModified();
}

void MainWindow::addSession()
{
    if (!current_db_class) { return; }

    MTListWidget * lw = new MTListWidget;
	QDialog * d = createAddSessionDialogue(tr("Add session"), lw);
    QList<QDateTime> sessions_list;
    for (int i = 0; i < CLLSSListWidget->count(); ++i) {
        sessions_list << CLLSSListWidget->item(i)->data(Qt::UserRole).toDateTime();
    }
    QFont font; font.setBold(true);
    for (int i = 0; i < SVLSListWidget->count(); ++i) {
        if (sessions_list.contains(SVLSListWidget->item(i)->data(Qt::UserRole).toDateTime())) { continue; }
        QListWidgetItem * item = new QListWidgetItem(*(SVLSListWidget->item(i)));
        item->setFont(font);
        item->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
        item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        lw->addItem(item);
    }
    for (int i = 0; i < SVLASListWidget->count(); ++i) {
        if (sessions_list.contains(SVLASListWidget->item(i)->data(Qt::UserRole).toDateTime())) { continue; }
        QListWidgetItem * item = new QListWidgetItem(*(SVLASListWidget->item(i)));
        item->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
        item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        lw->addItem(item);
    }
    lw->setCurrentRow(0);
    if (!d->exec()) { delete d; return; }
    if (lw->currentItem() == NULL) { delete d; return; }
    QDateTime datetime = lw->currentItem()->data(Qt::UserRole).toDateTime();
    current_db_class->addSession(datetime);
    QListWidgetItem * item = new QListWidgetItem(*(lw->currentItem()));
    item->setFont(QFont::QFont());
    CLLSSListWidget->addItem(item);
    delete d;
    setDatabaseModified();
    Session * session = current_db_sessions.value(datetime, current_db_archivedsessions.value(datetime, new ArchivedSession(this)));
    SessionWizard wizard(session, current_db_class, this);
    wizard.setWindowModality(Qt::WindowModal);
    wizard.exec();
    for (int i = 0; i < wizard.numMatchedPairs(); ++i) {
        current_db_class->member(wizard.studentNumberInClass(i))->addSession(datetime, wizard.studentNumberInSession(i));
    }
    setCurrentClassMember(CLLSListWidget->highlightedItem());
    CLSCAverageLabel->setText(QString("%1%").arg(current_db_class->average(&current_db_sessions, &current_db_archivedsessions)));
}

void MainWindow::deleteSession()
{
    if (current_db_class == NULL) { return; }
    if (!CLLSSListWidget->currentIndex().isValid()) { return; }
    switch (QMessageBox::information(this, tr("Delete session"), tr("Are you sure you want to delete session \"%1\" from this class? The session will not be deleted from the database.").arg(CLLSSListWidget->currentItem()->text()), tr("&Delete"), tr("Cancel"), 0, 1)) {
        case 0: // Delete
            break;
        case 1: // Cancel
            return; break;
    }
    QDateTime datetime = CLLSSListWidget->currentItem()->data(Qt::UserRole).toDateTime();
    current_db_class->removeSession(datetime);
    delete CLLSSListWidget->takeItem(CLLSSListWidget->currentRow());
    for (int i = 0; i < current_db_class->numMembers(); ++i) {
        current_db_class->member(i)->removeSession(datetime);
    }
    if (CLLSListWidget->highlightedRow() >= 0) { setCurrentClassMember(CLLSListWidget->highlightedItem()); }
    CLSCAverageLabel->setText(QString("%1%").arg(current_db_class->average(&current_db_sessions, &current_db_archivedsessions)));
    setDatabaseModified();
}

void MainWindow::viewSession(QListWidgetItem * item)
{
    if (item == NULL) { return; }
    QDateTime sdt = item->data(Qt::UserRole).toDateTime();
    for (int i = 0; i < SVLSListWidget->count(); ++i) {
        if (SVLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == sdt) {
            setCurrentSession(SVLSListWidget->item(i));
            actionSaved_sessions->setChecked(true);
            mainStackedWidget->setCurrentIndex(6);
            return;
        }
    }
    for (int i = 0; i < SVLASListWidget->count(); ++i) {
        if (SVLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == sdt) {
            setCurrentSession(SVLASListWidget->item(i));
            actionSaved_sessions->setChecked(true);
            mainStackedWidget->setCurrentIndex(6);
            return;
        }
    }
    QMessageBox::information(this, tr("iTestServer"), tr("Session not found. This session might have been archived on a different computer."));
}

void MainWindow::viewSessionAndStudent(QTableWidgetItem *)
{
    if (CLSSResultsTableWidget->currentRow() < 0) { return; }
    QDateTime sdt = CLSSResultsTableWidget->item(CLSSResultsTableWidget->currentRow(), 0)->data(Qt::UserRole).toDateTime();
    int num = CLSSResultsTableWidget->item(CLSSResultsTableWidget->currentRow(), 1)->data(Qt::UserRole).toInt();
    for (int i = 0; i < SVLSListWidget->count(); ++i) {
        if (SVLSListWidget->item(i)->data(Qt::UserRole).toDateTime() == sdt) {
            setCurrentSession(SVLSListWidget->item(i));
            SVLCListWidget->setCurrentRow(num);
            actionSaved_sessions->setChecked(true);
            mainStackedWidget->setCurrentIndex(6);
            return;
        }
    }
    for (int i = 0; i < SVLASListWidget->count(); ++i) {
        if (SVLASListWidget->item(i)->data(Qt::UserRole).toDateTime() == sdt) {
            setCurrentSession(SVLASListWidget->item(i));
            SVLCListWidget->setCurrentRow(num);
            actionSaved_sessions->setChecked(true);
            mainStackedWidget->setCurrentIndex(6);
            return;
        }
    }
    QMessageBox::information(this, tr("iTestServer"), tr("Session not found. This session might have been archived on a different computer."));
}

void MainWindow::addSessionToMember()
{
    if (!CLLSSListWidget->currentIndex().isValid()) { return; }
    if (current_db_class == NULL) { return; }
    if (CLLSListWidget->highlightedRow() < 0) { return; }

    QDateTime datetime = CLLSSListWidget->currentItem()->data(Qt::UserRole).toDateTime();
    Session * session = current_db_sessions.value(datetime, current_db_archivedsessions.value(datetime, new ArchivedSession(this)));
    if (session->numStudents() < 1) { return; }
    MTListWidget * lw = new MTListWidget;
	QDialog * d = createAddSessionDialogue(tr("Add selected session"), lw);
    for (int i = 0; i < session->numStudents(); ++i) {
        lw->addItem(session->student(i)->name());
    }
    lw->setCurrentRow(0);
    if (!d->exec()) { delete d; return; }
    if (lw->currentItem() == NULL) { delete d; return; }
    current_db_class->member(CLLSListWidget->highlightedRow())->addSession(datetime, lw->currentRow());
    setCurrentClassMember(CLLSListWidget->highlightedItem());
    CLSCAverageLabel->setText(QString("%1%").arg(current_db_class->average(&current_db_sessions, &current_db_archivedsessions)));
    delete d;
    setDatabaseModified();
}

QDialog * MainWindow::createAddSessionDialogue(const QString & title, MTListWidget * lw)
{
    QDialog * d = new QDialog(this);
#ifdef Q_WS_MAC
	d->setWindowTitle(QString("%1 - %2").arg(current_db_name).arg(title));
#else
    d->setWindowTitle(QString("%1 - %2 - iTest").arg(current_db_name).arg(title));
#endif
    d->setMinimumSize(QSize(300, 100));
        QVBoxLayout * vl = new QVBoxLayout(d);
        vl->setMargin(6); vl->setSpacing(6);
            QHBoxLayout * hl = new QHBoxLayout(d);
            hl->setMargin(0); hl->setSpacing(6);
                QLabel * lbl = new QLabel(tr("Search:"), d);
                ExtendedLineEdit * sle = new ExtendedLineEdit(d);
            hl->addWidget(lbl);
            hl->addWidget(sle);
        vl->addLayout(hl);
            if (lw == NULL) { lw = new MTListWidget(d); } else { lw->setParent(d); }
            QObject::connect(lw, SIGNAL(itemDoubleClicked(QListWidgetItem *)), d, SLOT(accept()));
            QObject::connect(sle, SIGNAL(textChanged(QLineEdit *, const QString &)), lw, SLOT(filterItems(QLineEdit *, const QString &)));
        vl->addWidget(lw);
            QDialogButtonBox * bb = new QDialogButtonBox(d);
            bb->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QObject::connect(bb, SIGNAL(accepted()), d, SLOT(accept()));
            QObject::connect(bb, SIGNAL(rejected()), d, SLOT(reject()));
        vl->addWidget(bb);
    return d;
}

void MainWindow::removeSessionFromMember()
{
    if (CLSSResultsTableWidget->currentRow() < 0) { return; }
    if (current_db_class == NULL) { return; }
    if (CLLSListWidget->highlightedRow() < 0) { return; }
    switch (QMessageBox::information(this, tr("Remove session"), tr("Are you sure you want to remove session \"%1\" from the table of results of the selected student? The session will not be deleted from the database.").arg(CLSSResultsTableWidget->item(CLSSResultsTableWidget->currentRow(), 0)->text()), tr("&Remove"), tr("Cancel"), 0, 1)) {
        case 0: // Remove
            break;
        case 1: // Cancel
            return; break;
    }
    current_db_class->member(CLLSListWidget->highlightedRow())->removeSession(CLSSResultsTableWidget->item(CLSSResultsTableWidget->currentRow(), 0)->data(Qt::UserRole).toDateTime());
    setCurrentClassMember(CLLSListWidget->highlightedItem());
    CLSCAverageLabel->setText(QString("%1%").arg(current_db_class->average(&current_db_sessions, &current_db_archivedsessions)));
    setDatabaseModified();
}

void MainWindow::toggleAddSessionToMemberEnabled()
{
    if (CLLSSListWidget->currentIndex().isValid()) {
        if (current_db_class == NULL) { tbtnAddSession->setEnabled(false); return; }
        if (CLLSListWidget->highlightedRow() < 0) { tbtnAddSession->setEnabled(false); return; }
        tbtnAddSession->setEnabled(!current_db_class->member(CLLSListWidget->highlightedRow())->hasSession(CLLSSListWidget->currentItem()->data(Qt::UserRole).toDateTime()));
    } else {
        tbtnAddSession->setEnabled(false);
    }
}

void MainWindow::clearCL()
{
    clearCLSC();
    CLLCListWidget->clear();
}

void MainWindow::clearCLSC()
{
	clearCLSS();
    CLSCNameLineEdit->clear();
	CLSCNumStudentsLabel->setText("0");
	CLSCAverageLabel->setText("0%");
	CLLSListWidget->clear();
	CLLSSListWidget->clear();
    setCLSCEnabled(false);
}

void MainWindow::clearCLSS()
{
	CLSSNameLineEdit->clear();
	CLSSAverageLabel->setText("0%");
	CLSSResultsTableWidget->clearContents();
	CLSSResultsTableWidget->setRowCount(0);
    setCLSSEnabled(false);
}

void MainWindow::setCLSCEnabled(bool enable)
{
    CLSCGroupBox->setEnabled(enable);
    CLLSGroupBox->setEnabled(enable);
    CLLSSGroupBox->setEnabled(enable);
    actionDelete_class->setEnabled(enable);
    actionAdd_session->setEnabled(enable);
    actionDelete_session->setEnabled(enable && CLLSSListWidget->currentIndex().isValid());
    actionAdd_student->setEnabled(enable);
    if (!enable) { setCLSSEnabled(enable); }
}

void MainWindow::setCLSSEnabled(bool enable)
{
    CLSSGroupBox->setEnabled(enable);
    actionDelete_student->setEnabled(enable);
}
