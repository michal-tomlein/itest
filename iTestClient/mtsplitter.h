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

#ifndef MTSPLITTER_H
#define MTSPLITTER_H

#include <QSplitter>

class MTSplitter : public QSplitter
{
    Q_OBJECT

public:
    MTSplitter(QWidget * parent = 0):
    QSplitter(parent) {};
    MTSplitter(Qt::Orientation orientation, QWidget * parent = 0):
    QSplitter(orientation, parent) {};

public slots:
    void moveSplitter(int pos) { QSplitter::moveSplitter(pos, 1); };
    void moveSplitter(int pos, int index) { QSplitter::moveSplitter(pos, index); };
    void moveLeft() { moveSplitter(0); };
    void moveRight() {
        switch (orientation()) {
            case Qt::Horizontal: moveSplitter(width()); break;
            case Qt::Vertical: moveSplitter(height()); break;
        }
    };
    void moveToCentre() {
        switch (orientation()) {
            case Qt::Horizontal: moveSplitter(width()/2); break;
            case Qt::Vertical: moveSplitter(height()/2); break;
        }
    };
};

#endif // MTSPLITTER_H
