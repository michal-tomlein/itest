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

#ifndef SESSION_WIZARD_H
#define SESSION_WIZARD_H

#include "ui_session_wizard.h"
#include "class.h"

#include <QHeaderView>

class SessionWizard : public QDialog, private Ui::SessionWizard
{
    Q_OBJECT

public:
    SessionWizard(Session *, Class *, QWidget * = 0, Qt::WindowFlags = 0);

    int numMatchedPairs();
    int studentNumberInSession(int);
    int studentNumberInClass(int);

private slots:
    void match();
    void split();

    void toggleMatchEnabled();
    void toggleSplitEnabled();
};

#endif // SESSION_WIZARD_H
