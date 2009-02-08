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

#ifndef MTTOOLBUTTON_H
#define MTTOOLBUTTON_H

#include <QToolButton>
#include <QAction>

class MTToolButton : public QToolButton
{
    Q_OBJECT

public:
    MTToolButton(QWidget * parent = 0, QString id = ""):
    QToolButton(parent) {
        tb_id = id;
        QObject::connect(this, SIGNAL(released()), this, SLOT(emitReleased()));
    };

    void setAction(QAction * action, bool copy_properties = false) {
        if (action == NULL) { return; }
        tb_action = action;
        if (copy_properties) {
            setIcon(action->icon());
            setText(action->text());
            setStatusTip(action->statusTip());
            setToolTip(action->toolTip());
            setWhatsThis(action->whatsThis());
            setCheckable(action->isCheckable());
        }
        QObject::connect(this, SIGNAL(clicked(bool)), action, SLOT(setChecked(bool)));
        QObject::connect(this, SIGNAL(toggled(bool)), action, SLOT(setChecked(bool)));
        QObject::connect(action, SIGNAL(triggered(bool)), this, SLOT(setChecked(bool)));
        QObject::connect(action, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
        //QObject::connect(action, SIGNAL(triggered()), this, SLOT(click()));
        //QObject::connect(action, SIGNAL(toggled(bool)), this, SLOT(click()));
        //QObject::connect(this, SIGNAL(clicked()), action, SLOT(trigger()));
        //QObject::connect(this, SIGNAL(toggled(bool)), action, SLOT(trigger()));
        //QObject::connect(this, SIGNAL(pressed()), action, SLOT(trigger()));
        QObject::connect(this, SIGNAL(released()), action, SLOT(trigger()));
        QObject::connect(action, SIGNAL(changed()), this, SLOT(updateProperties()));
    };

signals:
    void released(QString);

protected slots:
    void updateProperties() {
        setEnabled(tb_action->isEnabled());
    };
    void emitReleased() { emit released(tb_id); };

private:
    QAction * tb_action;
    QString tb_id;
};

#endif // MTTOOLBUTTON_H
