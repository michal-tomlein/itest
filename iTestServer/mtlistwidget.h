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

#ifndef MTLISTWIDGET_H
#define MTLISTWIDGET_H

#include <QListWidget>
#include <QLineEdit>

#include "mtwidgetpalettes.h"

class MTListWidget : public QListWidget
{
    Q_OBJECT

public:
    MTListWidget(QWidget * parent = 0):
    QListWidget(parent) {
        QObject::connect(this, SIGNAL(currentTextChanged(QString)), this, SLOT(emitCurrentIndexAvailabilityChanged()));
    };

public slots:
    int filterItems(QLineEdit * le, const QString & keyword) {
        if (keyword.isEmpty()) {
            le->setPalette(((QWidget *)(parent()))->palette());
        } else {
            le->setPalette(searchLineEditPalettes.search_active_palette);
        }
        int n = 0;
        for (int i = 0; i < count(); ++i) {
            if (this->item(i)->text().contains(keyword, Qt::CaseInsensitive)) {
                this->item(i)->setHidden(false); n++;
            } else { this->item(i)->setHidden(true); }
        }
        if ((!keyword.isEmpty()) && count() != 0 && n == 0) {
            le->setPalette(searchLineEditPalettes.search_noresults_palette);
        }
        return n;
    };
    void highlightItem(int i) { highlightItem(item(i)); };
    static void highlightItem(QListWidgetItem * item)
    {
        if (item == NULL) { return; }
        for (int i = 0; i < item->listWidget()->count(); ++i) {
            item->listWidget()->item(i)->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
            item->listWidget()->item(i)->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        }
        item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
        item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        item->listWidget()->clearSelection();
    };
    bool isItemHighlighted(int i) { return isItemHighlighted(item(i)); };
    static bool isItemHighlighted(QListWidgetItem * item) {
        if (item == NULL) { return false; }
        return (item->background() == QBrush::QBrush(QColor::QColor(197, 255, 120)));
    };
    int highlightedRow() { return row(highlightedItem()); };
    QListWidgetItem * highlightedItem() {
        for (int i = 0; i < count(); ++i) {
            if (isItemHighlighted(i)) { return this->item(i); }
        }
        return NULL;
    };

private slots:
    void emitCurrentIndexAvailabilityChanged() {
        emit currentIndexAvailabilityChanged(currentIndex().isValid());
    };

signals:
    void currentIndexAvailabilityChanged(bool);

private:
    SearchLineEditPalettes searchLineEditPalettes;
};

#endif // MTLISTWIDGET_H
