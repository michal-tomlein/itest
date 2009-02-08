/*******************************************************************
 This file is part of iTest
 Copyright (C) 2007 Michal Tomlein (michal.tomlein@gmail.com)

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

void MainWindow::setupFlagsPage()
{
    EFFlagLineEdit[0] = Flag01LineEdit; EFFlagCheckBox[0] = Flag01CheckBox;
    EFFlagLineEdit[1] = Flag02LineEdit; EFFlagCheckBox[1] = Flag02CheckBox;
    EFFlagLineEdit[2] = Flag03LineEdit; EFFlagCheckBox[2] = Flag03CheckBox;
    EFFlagLineEdit[3] = Flag04LineEdit; EFFlagCheckBox[3] = Flag04CheckBox;
    EFFlagLineEdit[4] = Flag05LineEdit; EFFlagCheckBox[4] = Flag05CheckBox;
    EFFlagLineEdit[5] = Flag06LineEdit; EFFlagCheckBox[5] = Flag06CheckBox;
    EFFlagLineEdit[6] = Flag07LineEdit; EFFlagCheckBox[6] = Flag07CheckBox;
    EFFlagLineEdit[7] = Flag08LineEdit; EFFlagCheckBox[7] = Flag08CheckBox;
    EFFlagLineEdit[8] = Flag09LineEdit; EFFlagCheckBox[8] = Flag09CheckBox;
    EFFlagLineEdit[9] = Flag10LineEdit; EFFlagCheckBox[9] = Flag10CheckBox;
    EFFlagLineEdit[10] = Flag11LineEdit; EFFlagCheckBox[10] = Flag11CheckBox;
    EFFlagLineEdit[11] = Flag12LineEdit; EFFlagCheckBox[11] = Flag12CheckBox;
    EFFlagLineEdit[12] = Flag13LineEdit; EFFlagCheckBox[12] = Flag13CheckBox;
    EFFlagLineEdit[13] = Flag14LineEdit; EFFlagCheckBox[13] = Flag14CheckBox;
    EFFlagLineEdit[14] = Flag15LineEdit; EFFlagCheckBox[14] = Flag15CheckBox;
    EFFlagLineEdit[15] = Flag16LineEdit; EFFlagCheckBox[15] = Flag16CheckBox;
    EFFlagLineEdit[16] = Flag17LineEdit; EFFlagCheckBox[16] = Flag17CheckBox;
    EFFlagLineEdit[17] = Flag18LineEdit; EFFlagCheckBox[17] = Flag18CheckBox;
    EFFlagLineEdit[18] = Flag19LineEdit; EFFlagCheckBox[18] = Flag19CheckBox;
    EFFlagLineEdit[19] = Flag20LineEdit; EFFlagCheckBox[19] = Flag20CheckBox;
    EFFlagQnumLabel[0] = FlagQnumLabel_0; EFFlagQnumLabel[1] = FlagQnumLabel_1;
    EFFlagQnumLabel[2] = FlagQnumLabel_2; EFFlagQnumLabel[3] = FlagQnumLabel_3;
    EFFlagQnumLabel[4] = FlagQnumLabel_4; EFFlagQnumLabel[5] = FlagQnumLabel_5;
    EFFlagQnumLabel[6] = FlagQnumLabel_6; EFFlagQnumLabel[7] = FlagQnumLabel_7;
    EFFlagQnumLabel[8] = FlagQnumLabel_8; EFFlagQnumLabel[9] = FlagQnumLabel_9;
    EFFlagQnumLabel[10] = FlagQnumLabel_10; EFFlagQnumLabel[11] = FlagQnumLabel_11;
    EFFlagQnumLabel[12] = FlagQnumLabel_12; EFFlagQnumLabel[13] = FlagQnumLabel_13;
    EFFlagQnumLabel[14] = FlagQnumLabel_14; EFFlagQnumLabel[15] = FlagQnumLabel_15;
    EFFlagQnumLabel[16] = FlagQnumLabel_16; EFFlagQnumLabel[17] = FlagQnumLabel_17;
    EFFlagQnumLabel[18] = FlagQnumLabel_18; EFFlagQnumLabel[19] = FlagQnumLabel_19;
    QLabel * fl[20];
    fl[0] = FlagLabel_1; fl[1] = FlagLabel_2; fl[2] = FlagLabel_3;
    fl[3] = FlagLabel_4; fl[4] = FlagLabel_5; fl[5] = FlagLabel_6;
    fl[6] = FlagLabel_7; fl[7] = FlagLabel_8; fl[8] = FlagLabel_9;
    fl[9] = FlagLabel_10; fl[10] = FlagLabel_11; fl[11] = FlagLabel_12;
    fl[12] = FlagLabel_13; fl[13] = FlagLabel_14; fl[14] = FlagLabel_15;
    fl[15] = FlagLabel_16; fl[16] = FlagLabel_17; fl[17] = FlagLabel_18;
    fl[18] = FlagLabel_19; fl[19] = FlagLabel_20;
    
    for (int i = 0; i < 20; ++i) {
        EFFlagLineEdit[i]->setStatusTip(tr("Type a name for flag %1").arg(i + 1));
        EFFlagCheckBox[i]->setStatusTip(tr("Check or uncheck this checkbox to enable or disable flag %1").arg(i + 1));
        fl[i]->setText(tr("Flag %1:").arg(i + 1));
        QObject::connect(EFFlagCheckBox[i], SIGNAL(toggled(bool)), EFFlagLineEdit[i], SLOT(setEnabled(bool)));
    }
    QObject::connect(EFButtonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(updateFlags(QAbstractButton *)));
}

void MainWindow::setFlags()
{
	 int q_flag_i = SQFlagComboBox->itemData(SQFlagComboBox->currentIndex()).toInt();
     SQFlagComboBox->clear(); LQFlagComboBox->clear();
     current_db_flagentries.clear();
     for (int i = 0; i < 20; ++i) {
        if (current_db_fe[i]) {
            SQFlagComboBox->addItem(QString("%1 - %2").arg(i+1).arg(current_db_f[i]), i);
            LQFlagComboBox->addItem(QString("%1 - %2").arg(i+1).arg(current_db_f[i]), i);
            current_db_flagentries.insert(i, SQFlagComboBox->count()-1);
        }
     }
     SQFlagComboBox->setCurrentIndex(current_db_flagentries.value(q_flag_i));
     checkForUnflaggedQuestions();
}

void MainWindow::loadFlags()
{
	for (int i = 0; i < 20; ++i)
    {
        if (current_db_fe[i]) {EFFlagCheckBox[i]->setChecked( true );}
        else {EFFlagCheckBox[i]->setChecked( false );}
	    EFFlagLineEdit[i]->setText( current_db_f[i] );
    }
}

void MainWindow::applyFlags()
{
    for (int i = 0; i < 20; ++i)
    {
        current_db_f[i] = EFFlagLineEdit[i]->text();
        current_db_fe[i] = EFFlagCheckBox[i]->isChecked();
    }
    setFlags(); updateFlagQnums();
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
    int flag_qnum [20];
    for (int i = 0; i < 20; ++i) { flag_qnum[i] = 0; }
    QMapIterator<QListWidgetItem *, QuestionItem *> i(current_db_questions);
    while (i.hasNext()) {
        i.next();
        if (i.value()->flag() >= 0 && i.value()->flag() < 20) {
            flag_qnum[i.value()->flag()]++;
        }
    }
    QString label_text = tr("Number of questions with this flag: ");
    QString buffer;
    for (int i = 0; i < 20; ++i) {
        buffer = label_text;
        buffer.append(QString("%1").arg(flag_qnum[i]));
        EFFlagQnumLabel[i]->setText(buffer);
        if (flag_qnum[i] > 0) {
            if (EFFlagCheckBox[i]->isChecked()) {
                EFFlagCheckBox[i]->setEnabled(false);
            }
        }
        else {EFFlagCheckBox[i]->setEnabled(true);}
    }
}

void MainWindow::checkForUnflaggedQuestions()
{
    int numflags = 0; QuestionItem * item;
    for (int i = 0; i < 20; ++i) { if (current_db_fe[i]) { numflags++; } }
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

void MainWindow::setFlagLineEditPalettes()
{
    for (int i = 0; i < 20; ++i) {
    	QPalette palette;
    	palette.setColor(QPalette::Active, QPalette::Base, backgroundColourForFlag(i));
    	palette.setColor(QPalette::Active, QPalette::Text, foregroundColourForFlag(i));
    	palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColourForFlag(i));
    	palette.setColor(QPalette::Inactive, QPalette::Text, foregroundColourForFlag(i));
    	EFFlagLineEdit[i]->setPalette(palette);
    }
}

QColor MainWindow::backgroundColourForFlag(int flag)
{
	switch (flag) {
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
		default:
			return QColor::QColor(255, 255, 255); break;
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
