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

#ifndef MTMULTISVGWIDGET_H
#define MTMULTISVGWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextDocument>

#include "flowlayout.h"

class MTMultiSvgWidget : public QWidget
{
    Q_OBJECT

protected:
    void init() {
        msw_flayout = new FlowLayout(this);
        msw_flayout->setContentsMargins(0, 0, 0, 0);
        msw_flayout->setSpacing(6);
    };
public:
    MTMultiSvgWidget(QWidget * parent = 0):
    QWidget(parent) { init(); };
    
    void addWidget(QWidget * w) { msw_flayout->addWidget(w); };
    void addWidget(QWidget * w, QString title, bool link = false) {
        QWidget * widget = new QWidget;
        QVBoxLayout * vlayout = new QVBoxLayout(widget);
        QLabel * label = new QLabel(widget);
        if (!link) {
            label->setText(title);
        } else {
            label->setText(QString("<a href=\"%1\">%2</a>").arg(msw_flayout->count()).arg(Qt::escape(title)));
            QObject::connect(label, SIGNAL(linkActivated(QString)), this, SLOT(emitTitleClicked(QString)));
        }
        label->setAlignment(Qt::AlignCenter);
        vlayout->addWidget(label);
        vlayout->addWidget(w);
        msw_flayout->addWidget(widget);
	};
	QSize layoutSizeHint() { return msw_flayout->sizeHint(); };
	int layoutHeightForWidth(int w) { return msw_flayout->heightForWidth(w); };

public slots:
    void clear() {
        for (int i = 0; i < msw_flayout->count();) {
            delete msw_flayout->takeAt(i)->widget();
        }
    };

private slots:
	void emitTitleClicked(QString text) { emit titleClicked(text); };

signals:
	void titleClicked(QString);

private:
    FlowLayout * msw_flayout;
};

#endif // MTMULTISVGWIDGET_H
