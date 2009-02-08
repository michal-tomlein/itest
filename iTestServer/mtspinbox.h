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

#ifndef MTSPINBOX_H
#define MTSPINBOX_H

#include <QSpinBox>
#include <QDoubleSpinBox>

class MTSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    MTSpinBox(QWidget * parent = 0):
    QSpinBox(parent) {};

public slots:
    void setMinimum(int value) {
        this->QSpinBox::setMinimum(value);
    };
    void setMaximum(int value) {
        this->QSpinBox::setMaximum(value);
    };
    void setMaximumValue() {
        setValue(maximum());
    };
};

class MTDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    MTDoubleSpinBox(QWidget * parent = 0):
    QDoubleSpinBox(parent) {};

public slots:
    void setMinimum(double value) {
        this->QDoubleSpinBox::setMinimum(value);
    };
    void setMaximum(double value) {
        this->QDoubleSpinBox::setMaximum(value);
    };
    void setMaximumValue() {
        setValue(maximum());
    };
};

#endif // MTSPINBOX_H
