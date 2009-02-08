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

#include "session_wizard.h"

SessionWizard::SessionWizard(Session * session, Class * cl, QWidget * parent, Qt::WindowFlags f):
QDialog(parent, f)
{
	setupUi(this);
	QObject::connect(btnMatch, SIGNAL(released()), this, SLOT(match()));
	QObject::connect(btnSplit, SIGNAL(released()), this, SLOT(split()));
	twMatchedPairs->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	twMatchedPairs->horizontalHeader()->hide();
	twMatchedPairs->verticalHeader()->hide();
    QObject::connect(lwSessionStudents, SIGNAL(currentTextChanged(QString)), this, SLOT(toggleMatchEnabled()));
    QObject::connect(lwClassStudents, SIGNAL(currentTextChanged(QString)), this, SLOT(toggleMatchEnabled()));
    QObject::connect(twMatchedPairs, SIGNAL(currentItemChanged(QTableWidgetItem *, QTableWidgetItem *)), this, SLOT(toggleSplitEnabled()));
    toggleMatchEnabled(); toggleSplitEnabled();

	lblSessionName->setText(QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name()));
	lblClassName->setText(cl->name());
	for (int i = 0; i < session->numStudents(); ++i) {
	    QListWidgetItem * item = new QListWidgetItem(session->student(i)->name());
	    item->setData(Qt::UserRole, i);
	    lwSessionStudents->addItem(item);
	}
	for (int i = 0; i < cl->numMembers(); ++i) {
	    QListWidgetItem * item = new QListWidgetItem(cl->member(i)->name());
	    item->setData(Qt::UserRole, i);
	    lwClassStudents->addItem(item);
	}
	QString str1; QString str2; bool found = false;
	for (int i = 0; i < lwSessionStudents->count();) {
	    for (int j = 0; j < lwClassStudents->count();) {
	        str1 = lwSessionStudents->item(i)->text().toLower().simplified();
	        str1.remove(" ");
	        str2 = lwClassStudents->item(j)->text().toLower().simplified();
	        str2.remove(" ");
	        if (str1 == str2) {
	            lwSessionStudents->setCurrentRow(i);
	            lwClassStudents->setCurrentRow(j);
	            match();
	            found = true; break;
	        } else {
	            j++;
	        }
	    }
	    if (!found) { i++; } else { found = false; }
	}
}

int SessionWizard::numMatchedPairs() { return twMatchedPairs->rowCount(); }

int SessionWizard::studentNumberInSession(int i) { return twMatchedPairs->item(i, 0)->data(Qt::UserRole).toInt(); }

int SessionWizard::studentNumberInClass(int i) { return twMatchedPairs->item(i, 1)->data(Qt::UserRole).toInt(); }

void SessionWizard::match()
{
    if (!lwSessionStudents->currentIndex().isValid()) { return; }
    if (!lwClassStudents->currentIndex().isValid()) { return; }
    twMatchedPairs->setRowCount(twMatchedPairs->rowCount() + 1);
    QTableWidgetItem * item = new QTableWidgetItem(lwSessionStudents->currentItem()->text());
    item->setData(Qt::UserRole, lwSessionStudents->currentItem()->data(Qt::UserRole));
    twMatchedPairs->setItem(twMatchedPairs->rowCount() - 1, 0, item);
    delete lwSessionStudents->takeItem(lwSessionStudents->currentRow());
    item = new QTableWidgetItem(lwClassStudents->currentItem()->text());
    item->setData(Qt::UserRole, lwClassStudents->currentItem()->data(Qt::UserRole));
    twMatchedPairs->setItem(twMatchedPairs->rowCount() - 1, 1, item);
    delete lwClassStudents->takeItem(lwClassStudents->currentRow());
}

void SessionWizard::split()
{
    if (twMatchedPairs->currentRow() < 0) { return; }
    QListWidgetItem * item = new QListWidgetItem(twMatchedPairs->item(twMatchedPairs->currentRow(), 0)->text());
    item->setData(Qt::UserRole, twMatchedPairs->item(twMatchedPairs->currentRow(), 0)->data(Qt::UserRole));
    lwSessionStudents->addItem(item);
    item = new QListWidgetItem(twMatchedPairs->item(twMatchedPairs->currentRow(), 1)->text());
    item->setData(Qt::UserRole, twMatchedPairs->item(twMatchedPairs->currentRow(), 1)->data(Qt::UserRole));
    lwClassStudents->addItem(item);
    twMatchedPairs->removeRow(twMatchedPairs->currentRow());
}

void SessionWizard::toggleMatchEnabled()
{
    btnMatch->setEnabled(lwSessionStudents->currentIndex().isValid() && lwClassStudents->currentIndex().isValid());
}

void SessionWizard::toggleSplitEnabled()
{
    btnSplit->setEnabled(twMatchedPairs->currentRow() >= 0);
}
