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

#ifndef MTTEXTEDIT_H
#define MTTEXTEDIT_H

#include <QColorDialog>

#include "ui_mttextedit.h"

class MTTextEdit : public QWidget, private Ui::MTTextEdit
{
    Q_OBJECT

public:
    MTTextEdit(QWidget * parent = 0);

    inline void setTitle(const QString & title) { nameLabel->setText(title); }
    inline QTextEdit * textEdit() { return theTextEdit; }

    inline QString toHtml() const { return theTextEdit->toHtml(); }
    inline QString toPlainText() const { return theTextEdit->toPlainText(); }
    inline void setHtml(const QString & text) { theTextEdit->setHtml(text); }
    inline void setPlainText(const QString & text) { theTextEdit->setPlainText(text); }
    inline void setText(const QString & text) { theTextEdit->setText(text); }
    inline void clear() { theTextEdit->clear(); }

signals:
    void textChanged();

private slots:
    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();
    void textAlign(QAbstractButton *);
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);

    QButtonGroup * tbtngrpAlign;
};

#endif // MTTEXTEDIT_H
