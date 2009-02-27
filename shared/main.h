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

#ifndef ITEST_MAIN_H
#define ITEST_MAIN_H

#include <QApplication>

#ifdef ITESTSERVER
#include "../iTestServer/main_window.h"
#endif
#ifdef ITESTCLIENT
#include "../iTestClient/main_window.h"
#endif

class MTApplication : public QApplication
{
    Q_OBJECT

public:
    MTApplication(int & argc, char ** argv):
    QApplication(argc, argv) { init(); };
    MTApplication(int & argc, char ** argv, bool GUIenabled):
    QApplication(argc, argv, GUIenabled) { init(); };
    MTApplication(int & argc, char ** argv, Type type):
    QApplication(argc, argv, type) { init(); };
#ifdef Q_WS_X11
    MTApplication(Display * display, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0):
    QApplication(display, visual, colormap) { init(); };
    MTApplication(Display * display, int & argc, char ** argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0):
    QApplication(display, argc, argv, visual, colormap) { init(); };
#endif
    void setAppMainWindow(MainWindow * w) { app_main_window = w; };

protected:
    void init() { app_main_window = NULL; };
    bool event(QEvent * event) {
        switch (event->type()) {
            case QEvent::FileOpen:
                if (app_main_window) {
                    app_main_window->openFile(static_cast<QFileOpenEvent *>(event)->file());
                }
                return true;
            default:
                return this->QApplication::event(event);
        }
    };

private:
    MainWindow * app_main_window;
};

#endif // ITEST_MAIN_H
