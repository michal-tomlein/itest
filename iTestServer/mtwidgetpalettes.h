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

#ifndef MTWIDGETPALETTES_H
#define MTWIDGETPALETTES_H

#include <QPalette>

struct SearchLineEditPalettes
{
    QPalette search_active_palette;
    QPalette search_noresults_palette;
    SearchLineEditPalettes() {
        search_active_palette.setColor(QPalette::Active, QPalette::Base, QColor::QColor(255, 251, 0));
        search_active_palette.setColor(QPalette::Active, QPalette::Text, QColor::QColor(0, 0, 0));
        search_active_palette.setColor(QPalette::Inactive, QPalette::Base, QColor::QColor(255, 251, 0));
        search_active_palette.setColor(QPalette::Inactive, QPalette::Text, QColor::QColor(0, 0, 0));
        search_noresults_palette.setColor(QPalette::Active, QPalette::Base, QColor::QColor(255, 0, 0));
        search_noresults_palette.setColor(QPalette::Active, QPalette::Text, QColor::QColor(0, 0, 0));
        search_noresults_palette.setColor(QPalette::Inactive, QPalette::Base, QColor::QColor(255, 0, 0));
        search_noresults_palette.setColor(QPalette::Inactive, QPalette::Text, QColor::QColor(0, 0, 0));
    };
};

#endif // MTWIDGETPALETTES_H
