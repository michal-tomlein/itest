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

#ifndef MTTABLEWIDGET_H
#define MTTABLEWIDGET_H

#include <QHeaderView>
#include <QTableWidget>
#include <QLineEdit>

#include "mtwidgetpalettes.h"

class MTTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    MTTableWidget(QWidget * parent = 0):
    QTableWidget(parent) {
        QObject::connect(this, SIGNAL(currentItemChanged(QTableWidgetItem *, QTableWidgetItem *)), this, SLOT(emitCurrentIndexAvailabilityChanged()));
    };

public slots:
    int filterItems(QLineEdit * le, const QString & keyword) {
        if (keyword.isEmpty()) {
            le->setPalette(((QWidget *)(parent()))->palette());
        } else {
            le->setPalette(searchLineEditPalettes.search_active_palette);
        }
        int n = 0;
        for (int i = 0; i < rowCount(); ++i) {
            if (item(i, 0)->text().contains(keyword, Qt::CaseInsensitive)) {
                showRow(i); n++;
            } else { hideRow(i); }
        }
        if ((!keyword.isEmpty()) && rowCount() != 0 && n == 0) {
            le->setPalette(searchLineEditPalettes.search_noresults_palette);
        }
        return n;
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

#endif // MTTABLEWIDGET_H
