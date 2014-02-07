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

#ifndef MTADVANCEDGROUPBOX_H
#define MTADVANCEDGROUPBOX_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>

class MTAdvancedGroupBox : public QWidget
{
    Q_OBJECT

protected:
    void init() {
        agb_vlayout = new QVBoxLayout(this);
        agb_vlayout->setMargin(0); agb_vlayout->setSpacing(0);
            agb_groupbox = new QGroupBox(tr("Advanced"), this);
            agb_groupbox->setCheckable(true);
            agb_groupbox->setChecked(false);
            agb_groupbox->setVisible(false);
            QObject::connect(agb_groupbox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
                agb_glayout = new QGridLayout(agb_groupbox);
#ifdef Q_WS_MAC
                agb_glayout->setContentsMargins(9, 9, 9, 9);
#else
                agb_glayout->setContentsMargins(9, 6, 9, 9);
#endif
                agb_glayout->setSpacing(6);
        agb_vlayout->addWidget(agb_groupbox);
            agb_checkbox = new QCheckBox(tr("Advanced"), this);
            agb_checkbox->setChecked(false);
            QObject::connect(agb_checkbox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
        agb_vlayout->addWidget(agb_checkbox);
    };

public:
    MTAdvancedGroupBox(QWidget * parent = 0):
    QWidget(parent) { init(); };
    MTAdvancedGroupBox(const QString & title, QWidget * parent = 0):
    QWidget(parent) { init(); setTitle(title); };
    
    bool isChecked() { return agb_checkbox->isChecked(); };
    void addLayout(QLayout * layout, int row, int column, Qt::Alignment alignment = 0)
    { agb_glayout->addLayout(layout, row, column, alignment); };
    void addLayout(QLayout * layout, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment = 0)
    { agb_glayout->addLayout(layout, row, column, rowSpan, columnSpan, alignment); };
    void addWidget(QWidget * widget, int row, int column, Qt::Alignment alignment = 0)
    { agb_glayout->addWidget(widget, row, column, alignment); };
    void addWidget(QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0)
    { agb_glayout->addWidget(widget, fromRow, fromColumn, rowSpan, columnSpan, alignment); };
    QGridLayout * gridLayout() { return agb_glayout; };

public slots:
    void setChecked(bool checked) {
        agb_checkbox->setChecked(checked);
        agb_groupbox->setChecked(checked);
        agb_checkbox->setVisible(!checked);
        agb_groupbox->setVisible(checked);
    };
    void setTitle(const QString & title) {
        agb_checkbox->setText(title);
        agb_groupbox->setTitle(title);
    };
    void setStatusTip(const QString & statustip) {
        agb_checkbox->setStatusTip(statustip);
        agb_groupbox->setStatusTip(statustip);
    };

private:
    QGridLayout * agb_glayout;
    QVBoxLayout * agb_vlayout;
    QGroupBox * agb_groupbox;
    QCheckBox * agb_checkbox;
};

#endif // MTADVANCEDGROUPBOX_H
