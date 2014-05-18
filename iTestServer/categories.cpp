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

void MainWindow::addCategoryItem(int i)
{
    QLineEdit *le = new QLineEdit(this);
    le->setStatusTip(tr("Type a name for category %1").arg(i + 1));
    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::Base, backgroundColourForCategory(i));
    palette.setColor(QPalette::Active, QPalette::Text, foregroundColourForCategory(i));
    palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColourForCategory(i));
    palette.setColor(QPalette::Inactive, QPalette::Text, foregroundColourForCategory(i));
    le->setPalette(palette);
    EFCategoryLineEdit << le;
    QTreeWidgetItem *item = new QTreeWidgetItem(EFTreeWidget);
    item->setText(0, QString::number(i + 1));
    item->setCheckState(0, Qt::Unchecked);
    item->setStatusTip(0, tr("Check or uncheck this checkbox to enable or disable category %1").arg(i + 1));
    EFTreeWidget->setItemWidget(item, 1, le);
    item->setText(2, "0");
}

void MainWindow::removeCategoryItem(int i)
{
    delete EFTreeWidget->takeTopLevelItem(i);
    delete EFCategoryLineEdit[i];
    EFCategoryLineEdit.resize(EFCategoryLineEdit.size() - 1);
}

void MainWindow::setupCategoriesPage()
{
    for (int i = 0; i < current_db_categories.size(); ++i) {
        addCategoryItem(i);
    }
    QObject::connect(EFTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(setCategoryEnabled(QTreeWidgetItem *)));
    QObject::connect(EFButtonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(updateCategories(QAbstractButton *)));
}

void MainWindow::setCategoryEnabled(QTreeWidgetItem *item)
{
    int i = item->text(0).toInt() - 1;
    if (i >= 0 && i < EFCategoryLineEdit.size()) {
        EFCategoryLineEdit[i]->setEnabled(item->checkState(0) == Qt::Checked);
        if (i == EFCategoryLineEdit.size() - 1 && item->checkState(0) == Qt::Checked) {
            addCategoryItem(i + 1);
        }
    }
}

void MainWindow::setCategories()
{
     int q_category_i = SQCategoryComboBox->itemData(SQCategoryComboBox->currentIndex()).toInt();
     SQCategoryComboBox->clear(); LQCategoryComboBox->clear();
     current_db_categoryentries.clear();
     for (int i = 0; i < current_db_categories.size(); ++i) {
        if (current_db_categories_enabled[i]) {
            SQCategoryComboBox->addItem(current_db_categories[i], i);
            LQCategoryComboBox->addItem(current_db_categories[i], i);
            current_db_categoryentries.insert(i, SQCategoryComboBox->count() - 1);
        }
     }
     SQCategoryComboBox->setCurrentIndex(current_db_categoryentries.value(q_category_i));
     checkForUncategorisedQuestions();
}

void MainWindow::loadCategories()
{
    for (int i = 0; i < current_db_categories.size(); ++i) {
        if (i >= EFTreeWidget->topLevelItemCount()) {
            addCategoryItem(i);
        }
        EFTreeWidget->topLevelItem(i)->setCheckState(0, current_db_categories_enabled[i] ? Qt::Checked : Qt::Unchecked);
        EFCategoryLineEdit[i]->setText(current_db_categories[i]);
    }
    for (int i = EFTreeWidget->topLevelItemCount() - 1; i > current_db_categories.size(); --i) {
        removeCategoryItem(i);
    }
    if (EFTreeWidget->topLevelItemCount() == current_db_categories.size() + 1) {
        EFTreeWidget->topLevelItem(current_db_categories.size())->setCheckState(0, Qt::Unchecked);
        EFCategoryLineEdit[current_db_categories.size()]->clear();
    }
}

void MainWindow::applyCategories()
{
    for (int i = EFTreeWidget->topLevelItemCount() - 1; i > 19; --i) {
        if (EFTreeWidget->topLevelItem(i - 1)->checkState(0) == Qt::Unchecked && EFTreeWidget->topLevelItem(i)->checkState(0) == Qt::Unchecked) {
            removeCategoryItem(i);
        } else {
            break;
        }
    }
    current_db_categories.resize(EFCategoryLineEdit.size());
    current_db_categories_enabled.resize(EFCategoryLineEdit.size());
    for (int i = 0; i < EFCategoryLineEdit.size(); ++i) {
        current_db_categories[i] = EFCategoryLineEdit[i]->text();
        current_db_categories_enabled[i] = EFTreeWidget->topLevelItem(i)->checkState(0) == Qt::Checked;
    }
    setCategories();
    updateCategoryQnums();
    statusBar()->showMessage(tr("Categories saved"), 10000);
    setDatabaseModified();
}

void MainWindow::discardCategories()
{
    loadCategories(); statusBar()->showMessage(tr("Categories discarded"), 10000);
}

void MainWindow::updateCategories(QAbstractButton *btn)
{
    if (btn == EFButtonBox->button(EFButtonBox->Apply)) {
        applyCategories();
    } else if (btn == EFButtonBox->button(EFButtonBox->Discard)) {
        discardCategories();
    }
}

void MainWindow::updateCategoryQnums()
{
    QMap<int, int> category_qnum;
    QMapIterator<QListWidgetItem *, QuestionItem *> i(current_db_questions);
    while (i.hasNext()) { i.next();
        if (i.value()->category() >= 0) {
            category_qnum[i.value()->category()]++;
        }
    }
    for (int i = 0; i < EFTreeWidget->topLevelItemCount(); ++i) {
        EFTreeWidget->topLevelItem(i)->setText(2, QString::number(category_qnum[i]));
        EFTreeWidget->topLevelItem(i)->setDisabled(category_qnum[i] > 0 && EFTreeWidget->topLevelItem(i)->checkState(0) == Qt::Checked);
        if (i < current_db_categories.size()) EFCategoryLineEdit[i]->setText(current_db_categories[i]);
    }
}

void MainWindow::checkForUncategorisedQuestions()
{
    int numcategories = 0;
    QuestionItem *item;
    for (int i = 0; i < current_db_categories.size(); ++i) {
        if (current_db_categories_enabled[i]) {
            numcategories++;
        }
    }
    if (numcategories > 0) {
        for (int i = 0; i < LQListWidget->count(); ++i) {
            item = current_db_questions.value(LQListWidget->item(i));
            if (item->category() == -1) {
                item->setCategory(SQCategoryComboBox->itemData(0).toInt());
                setQuestionItemColour(LQListWidget->item(i), item->category());
                setDatabaseModified();
                if (LQListWidget->currentRow() == i) {
                    SQCategoryComboBox->setCurrentIndex(current_db_categoryentries.value(item->category()));
                    LQCategoryComboBox->setCurrentIndex(current_db_categoryentries.value(item->category()));
                }
            }
        }
    }
}

int MainWindow::qnumForCategory(int category, bool use_groups)
{
    QuestionItem *qi;
    int qnum = 0;
    QSet<QString> groups;
    for (int i = 0; i < LQListWidget->count(); ++i) {
        qi = current_db_questions.value(LQListWidget->item(i));
        if (qi->category() == category) {
            if (use_groups) {
                if (qi->group().isEmpty()) {
                    qnum++;
                } else {
                    groups << qi->group();
                }
            } else {
                qnum++;
            }
        }
    }
    qnum += groups.count();
    return qnum;
}

QColor MainWindow::backgroundColourForCategory(int category)
{
    switch (category) {
        case -1:
            return QColor(255, 255, 255); break;
        case 0: // 0: 255 255 255 - 0 0 0
            return QColor(240, 255, 210); break;
        case 1: // 1: 197 255 120 - 0 0 0
            return QColor(197, 255, 120); break;
        case 2: // 2: 92 163 0 - 0 0 0
            return QColor(92, 163, 0); break;
        case 3: // 3: 69 110 14 - 255 255 255
            return QColor(69, 110, 14); break;
        case 4: // 4: 17 120 122 - 255 255 255
            return QColor(17, 120, 122); break;
        case 5: // 5: 0 163 136 - 0 0 0
            return QColor(0, 163, 136); break;
        case 6: // 6: 0 147 163 - 0 0 0
            return QColor(0, 147, 163); break;
        case 7: // 7: 0 125 163 - 255 255 255
            return QColor(0, 125, 163); break;
        case 8: // 8: 0 84 163 - 255 255 255
            return QColor(0, 84, 163); break;
        case 9: // 9: 40 76 110 - 255 255 255
            return QColor(40, 76, 110); break;
        case 10: // 10: 0 11 163 - 255 255 255
            return QColor(0, 11, 163); break;
        case 11: // 11: 139 0 163 - 255 255 255
            return QColor(139, 0, 163); break;
        case 12: // 12: 163 0 79 - 255 255 255
            return QColor(163, 0, 79); break;
        case 13: // 13: 163 0 0 - 0 0 0
            return QColor(163, 0, 0); break;
        case 14: // 14: 255 0 0 - 0 0 0
            return QColor(255, 0, 0); break;
        case 15: // 15: 204 109 0 - 0 0 0
            return QColor(204, 109, 0); break;
        case 16: // 16: 204 163 0 - 0 0 0
            return QColor(204, 163, 0); break;
        case 17: // 17: 201 204 0 - 0 0 0
            return QColor(201, 204, 0); break;
        case 18: // 18: 255 251 0 - 0 0 0
            return QColor(255, 251, 0); break;
        case 19: // 19: 221 255 0 - 0 0 0
            return QColor(221, 255, 0); break;
        default: // 20+: 173 217 255 - 0 0 0
            return QColor(173, 217, 255); break;
    }
    return QColor(255, 255, 255);
}

QColor MainWindow::foregroundColourForCategory(int category, bool hidden)
{
    switch (category) {
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
            if (hidden) {
                return QColor(100, 100, 100);
            } else {
                return QColor(0, 0, 0);
            }
            break;
        case 3: // 3: 69 110 14 - 255 255 255
        case 4: // 4: 17 120 122 - 255 255 255
        case 7: // 7: 0 125 163 - 255 255 255
        case 8: // 8: 0 84 163 - 255 255 255
        case 9: // 9: 40 76 110 - 255 255 255
        case 10: // 10: 0 11 163 - 255 255 255
        case 11: // 11: 139 0 163 - 255 255 255
        case 12: // 12: 163 0 79 - 255 255 255
            if (hidden) {
                return QColor(200, 200, 200);
            } else {
                return QColor(255, 255, 255);
            }
            break;
        default:
            if (hidden) {
                return QColor(100, 100, 100);
            } else {
                return QColor(0, 0, 0);
            }
            break;
    }
    return QColor(0, 0, 0);
}
