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

#ifndef MTPROGRESSBAR_H
#define MTPROGRESSBAR_H

#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>

class MTProgressBar : public QWidget
{
    Q_OBJECT

public:
    MTProgressBar(QWidget * parent = 0):
    QWidget(parent) {
        QHBoxLayout * hlayout = new QHBoxLayout(this);
        hlayout->setContentsMargins(0, 0, 0, 0);
        hlayout->setSpacing(6);
        progress_bar = new QProgressBar;
        progress_bar->setTextVisible(false);
        hlayout->addWidget(progress_bar);
        value_label = new QLabel("100% ");
        value_label->setMinimumSize(value_label->sizeHint().width(), 0);
        value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        value_label->setVisible(false);
        hlayout->addWidget(value_label);
        //QObject::connect(progress_bar, SIGNAL(valueChanged(int)), this, SLOT(setLabelValue(int)));
        QObject::connect(progress_bar, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
    };

    Qt::Alignment alignment() const { return progress_bar->alignment(); };
    void setAlignment(Qt::Alignment alignment) { progress_bar->setAlignment(alignment); };
    QString format() const { return progress_bar->format(); };
    void setFormat(const QString & format) { progress_bar->setFormat(format); };
    bool invertedAppearance() { return progress_bar->invertedAppearance(); };
    void setInvertedAppearance(bool invertedAppearance) { progress_bar->setInvertedAppearance(invertedAppearance); };
    int maximum() const { return progress_bar->maximum(); };
    int minimum() const { return progress_bar->minimum(); };
    Qt::Orientation orientation() const { return progress_bar->orientation(); };
    QString text() const { return progress_bar->text(); };
    QProgressBar::Direction textDirection() { return progress_bar->textDirection(); };
    void setTextDirection(QProgressBar::Direction textDirection) { progress_bar->setTextDirection(textDirection); };
    bool isTextVisible() const { return progress_bar->isTextVisible(); };
    void setTextVisible(bool textVisible) { value_label->setVisible(textVisible); };
    int value() const { return progress_bar->value(); };

public slots:
    void reset() { progress_bar->reset(); };
    void setMaximum(int maximum) { progress_bar->setMaximum(maximum); };
    void setMinimum(int minimum) { progress_bar->setMinimum(minimum); };
    void setOrientation(Qt::Orientation orientation) { progress_bar->setOrientation(orientation); };
    void setRange(int minimum, int maximum) { progress_bar->setRange(minimum, maximum); };
    void setValue(int value) {
        progress_bar->setValue(value > minimum() ? value : minimum());
        value_label->setText(QString("%1% ").arg((maximum() - minimum()) != 0 ? (100 * (value - minimum()) / (maximum() - minimum())) : 100));
    };

signals:
    void valueChanged(int value);

private:
    QProgressBar * progress_bar;
    QLabel * value_label;

/*protected slots:
    void setLabelValue(int) { value_label->setText(progress_bar->text() + " "); };*/
};

#endif // MTPROGRESSBAR_H
