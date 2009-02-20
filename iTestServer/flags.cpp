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

void MainWindow::addFlagItem(int i)
{
    QLineEdit * le = new QLineEdit(this);
    le->setStatusTip(tr("Type a name for flag %1").arg(i + 1));
    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::Base, backgroundColourForFlag(i));
    palette.setColor(QPalette::Active, QPalette::Text, foregroundColourForFlag(i));
    palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColourForFlag(i));
    palette.setColor(QPalette::Inactive, QPalette::Text, foregroundColourForFlag(i));
    le->setPalette(palette);
    EFFlagLineEdit << le;
    QTreeWidgetItem * item = new QTreeWidgetItem(EFTreeWidget);
    item->setText(0, makeString(i + 1));
    item->setCheckState(0, Qt::Unchecked);
    item->setStatusTip(0, tr("Check or uncheck this checkbox to enable or disable flag %1").arg(i + 1));
    EFTreeWidget->setItemWidget(item, 1, le);
    item->setText(2, "0");
}

void MainWindow::removeFlagItem(int i)
{
    delete EFTreeWidget->takeTopLevelItem(i);
    delete EFFlagLineEdit[i];
    EFFlagLineEdit.resize(EFFlagLineEdit.size() - 1);
}

void MainWindow::setupFlagsPage()
{
    for (int i = 0; i < current_db_f.size(); ++i) {
        addFlagItem(i);
    }
    QObject::connect(EFTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(setFlagEnabled(QTreeWidgetItem *)));
    QObject::connect(EFButtonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(updateFlags(QAbstractButton *)));
}

void MainWindow::setFlagEnabled(QTreeWidgetItem * item)
{
    int i = item->text(0).toInt() - 1;
    if (i >= 0 && i < EFFlagLineEdit.size()) {
        EFFlagLineEdit[i]->setEnabled(item->checkState(0) == Qt::Checked);
        if (i == EFFlagLineEdit.size() - 1 && item->checkState(0) == Qt::Checked) {
            addFlagItem(i + 1);
        }
    }
}

void MainWindow::setFlags()
{
	 int q_flag_i = SQFlagComboBox->itemData(SQFlagComboBox->currentIndex()).toInt();
     SQFlagComboBox->clear(); LQFlagComboBox->clear();
     current_db_flagentries.clear();
     for (int i = 0; i < current_db_f.size(); ++i) {
        if (current_db_fe[i]) {
            SQFlagComboBox->addItem(current_db_f[i], i);
            LQFlagComboBox->addItem(current_db_f[i], i);
            current_db_flagentries.insert(i, SQFlagComboBox->count() - 1);
        }
     }
     SQFlagComboBox->setCurrentIndex(current_db_flagentries.value(q_flag_i));
     checkForUnflaggedQuestions();
}

void MainWindow::loadFlags()
{
	for (int i = 0; i < current_db_f.size(); ++i) {
        if (i >= EFTreeWidget->topLevelItemCount()) { addFlagItem(i); }
        EFTreeWidget->topLevelItem(i)->setCheckState(0, current_db_fe[i] ? Qt::Checked : Qt::Unchecked);
	    EFFlagLineEdit[i]->setText(current_db_f[i]);
    }
    for (int i = EFTreeWidget->topLevelItemCount() - 1; i > current_db_f.size(); --i) {
        removeFlagItem(i);
    }
    if (EFTreeWidget->topLevelItemCount() == current_db_f.size() + 1) {
        EFTreeWidget->topLevelItem(current_db_f.size())->setCheckState(0, Qt::Unchecked);
        EFFlagLineEdit[current_db_f.size()]->clear();
    }
}

void MainWindow::applyFlags()
{
    for (int i = EFTreeWidget->topLevelItemCount() - 1; i > 19; --i) {
        if (EFTreeWidget->topLevelItem(i - 1)->checkState(0) == Qt::Unchecked && EFTreeWidget->topLevelItem(i)->checkState(0) == Qt::Unchecked) {
            removeFlagItem(i);
        } else { break; }
    }
    current_db_f.resize(EFFlagLineEdit.size());
    current_db_fe.resize(EFFlagLineEdit.size());
    for (int i = 0; i < EFFlagLineEdit.size(); ++i) {
        current_db_f[i] = EFFlagLineEdit[i]->text();
        current_db_fe[i] = EFTreeWidget->topLevelItem(i)->checkState(0) == Qt::Checked;
    }
    setFlags();
    updateFlagQnums();
    statusBar()->showMessage(tr("Flags saved"), 10000);
    setDatabaseModified();
}

void MainWindow::discardFlags()
{
    loadFlags(); statusBar()->showMessage(tr("Flags discarded"), 10000);
}

void MainWindow::updateFlags(QAbstractButton * btn)
{
    if (btn == EFButtonBox->button(EFButtonBox->Apply)) {applyFlags();}
    else if (btn == EFButtonBox->button(EFButtonBox->Discard)) {discardFlags();}
}

void MainWindow::updateFlagQnums()
{
    QMap<int, int> flag_qnum;
    QMapIterator<QListWidgetItem *, QuestionItem *> i(current_db_questions);
    while (i.hasNext()) { i.next();
        if (i.value()->flag() >= 0) { flag_qnum[i.value()->flag()]++; }
    }
    for (int i = 0; i < EFTreeWidget->topLevelItemCount(); ++i) {
        EFTreeWidget->topLevelItem(i)->setText(2, makeString(flag_qnum[i]));
        EFTreeWidget->topLevelItem(i)->setDisabled(flag_qnum[i] > 0 && EFTreeWidget->topLevelItem(i)->checkState(0) == Qt::Checked);
        if (i < current_db_f.size()) EFFlagLineEdit[i]->setText(current_db_f[i]);
    }
}

void MainWindow::checkForUnflaggedQuestions()
{
    int numflags = 0; QuestionItem * item;
    for (int i = 0; i < current_db_f.size(); ++i) { if (current_db_fe[i]) { numflags++; } }
    if (numflags > 0) {
        for (int i = 0; i < LQListWidget->count(); ++i) {
            item = current_db_questions.value(LQListWidget->item(i));
            if (item->flag() == -1) {
                item->setFlag(SQFlagComboBox->itemData(0).toInt());
                setQuestionItemColour(LQListWidget->item(i), item->flag());
                setDatabaseModified();
                if (LQListWidget->currentRow() == i) {
                    SQFlagComboBox->setCurrentIndex(current_db_flagentries.value(item->flag()));
                    LQFlagComboBox->setCurrentIndex(current_db_flagentries.value(item->flag()));
                }
            }
        }
    }
}

int MainWindow::qnumForFlag(int flag, bool use_groups)
{
    QuestionItem * qi; int qnum = 0; QSet<QString> groups;
    for (int i = 0; i < LQListWidget->count(); ++i) {
        qi = current_db_questions.value(LQListWidget->item(i));
    	if (qi->flag() == flag) {
    		if (use_groups) {
    			if (qi->group().isEmpty()) { qnum++; }
    			else { groups << qi->group(); }
    		} else { qnum++; }
    	}
    }
    qnum += groups.count();
    return qnum;
}

QColor MainWindow::backgroundColourForFlag(int flag)
{
	switch (flag) {
        case -1:
            return QColor::QColor(255, 255, 255); break;
		case 0: // 0: 255 255 255 - 0 0 0
			return QColor::QColor(240, 255, 210); break;
		case 1: // 1: 197 255 120 - 0 0 0
			return QColor::QColor(197, 255, 120); break;
		case 2: // 2: 92 163 0 - 0 0 0
			return QColor::QColor(92, 163, 0); break;
		case 3: // 3: 69 110 14 - 255 255 255
			return QColor::QColor(69, 110, 14); break;
		case 4: // 4: 17 120 122 - 255 255 255
			return QColor::QColor(17, 120, 122); break;
		case 5: // 5: 0 163 136 - 0 0 0
			return QColor::QColor(0, 163, 136); break;
		case 6: // 6: 0 147 163 - 0 0 0
			return QColor::QColor(0, 147, 163); break;
		case 7: // 7: 0 125 163 - 255 255 255
			return QColor::QColor(0, 125, 163); break;
		case 8: // 8: 0 84 163 - 255 255 255
			return QColor::QColor(0, 84, 163); break;
		case 9: // 9: 40 76 110 - 255 255 255
			return QColor::QColor(40, 76, 110); break;
		case 10: // 10: 0 11 163 - 255 255 255
			return QColor::QColor(0, 11, 163); break;
		case 11: // 11: 139 0 163 - 255 255 255
			return QColor::QColor(139, 0, 163); break;
		case 12: // 12: 163 0 79 - 255 255 255
			return QColor::QColor(163, 0, 79); break;
		case 13: // 13: 163 0 0 - 0 0 0
			return QColor::QColor(163, 0, 0); break;
		case 14: // 14: 255 0 0 - 0 0 0
			return QColor::QColor(255, 0, 0); break;
		case 15: // 15: 204 109 0 - 0 0 0
			return QColor::QColor(204, 109, 0); break;
		case 16: // 16: 204 163 0 - 0 0 0
			return QColor::QColor(204, 163, 0); break;
		case 17: // 17: 201 204 0 - 0 0 0
			return QColor::QColor(201, 204, 0); break;
		case 18: // 18: 255 251 0 - 0 0 0
			return QColor::QColor(255, 251, 0); break;
		case 19: // 19: 221 255 0 - 0 0 0
			return QColor::QColor(221, 255, 0); break;
		default: // 20+: 173 217 255 - 0 0 0
			return QColor::QColor(173, 217, 255); break;
	}
	return QColor::QColor(255, 255, 255);
}

QColor MainWindow::foregroundColourForFlag(int flag, bool hidden)
{
	switch (flag) {
		case 0: // 0: 255 255 255 - 0 0 0
		case 1: // 1: 197 255 120 - 0 0 0
		case 2: // 2: 92 163 0 - 0 0 0
		case 5: // 5: 0 163 136 - 0 0 0
		case 6: // 6: 0 147 163 - 0 0 0
		case 13: // 13: 163 0 0 - 0 0 0
		case 14: // 14: 255 0 0 - 0 0 0
		case 15: // 15: 204 109 0 - 0 0 0
		case 16: // 16: 204 163 0 - 0 0 0
		case 17: // 17: 201 204 0 - 0 0 0
		case 18: // 18: 255 251 0 - 0 0 0
		case 19: // 19: 221 255 0 - 0 0 0
			if (hidden) { return QColor::QColor(100, 100, 100); }
			else { return QColor::QColor(0, 0, 0); }
			break;
		case 3: // 3: 69 110 14 - 255 255 255
		case 4: // 4: 17 120 122 - 255 255 255
		case 7: // 7: 0 125 163 - 255 255 255
		case 8: // 8: 0 84 163 - 255 255 255
		case 9: // 9: 40 76 110 - 255 255 255
		case 10: // 10: 0 11 163 - 255 255 255
		case 11: // 11: 139 0 163 - 255 255 255
		case 12: // 12: 163 0 79 - 255 255 255
			if (hidden) { return QColor::QColor(200, 200, 200); }
			else { return QColor::QColor(255, 255, 255); }
			break;
		default:
			if (hidden) { return QColor::QColor(100, 100, 100); }
			else { return QColor::QColor(0, 0, 0); }
			break;
	}
	return QColor::QColor(0, 0, 0);
}
