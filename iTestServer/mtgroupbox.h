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

#ifndef MTGROUPBOX_H
#define MTGROUPBOX_H

#include <QGroupBox>
#include <QLayout>

class MTGroupBox : public QGroupBox
{
    Q_OBJECT

protected:
    void init() {
#ifdef Q_WS_MAC
        this->setContentsMargins(20, 0, 20, 20);
#else
        this->setContentsMargins(10, 0, 10, 10);
#endif
    };

public:
    MTGroupBox(QWidget * parent = 0):
    QGroupBox(parent) { init(); };
    MTGroupBox(QString title, QWidget * parent = 0):
    QGroupBox(title, parent) { init(); };

    /*void setLayout(QLayout * layout) {
        this->QWidget::setLayout(layout);
#ifdef Q_WS_MAC
        layout->setContentsMargins(9, 6, 9, 9);
#else
        layout->setContentsMargins(9, 6, 9, 9);
#endif
    };*/
};

#endif // MTGROUPBOX_H
