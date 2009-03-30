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

#include "mttextedit.h"

MTTextEdit::MTTextEdit(QWidget * parent):
QWidget(parent)
{
    setupUi(this);

    tbtnUndo->setText(QString::fromUtf8("\342\227\204"));
    tbtnRedo->setText(QString::fromUtf8("\342\226\272"));

    QPixmap pix(14, 14);
    pix.fill(Qt::black);
    tbtnColour->setIcon(pix);

    tbtngrpAlign = new QButtonGroup(this);
    tbtngrpAlign->addButton(tbtnAlignLeft);
    tbtngrpAlign->addButton(tbtnAlignCentre);
    tbtngrpAlign->addButton(tbtnAlignRight);
    tbtngrpAlign->addButton(tbtnAlignJustify);

    QObject::connect(tbtnItalic, SIGNAL(released()), this, SLOT(textItalic()));
    QObject::connect(tbtnBold, SIGNAL(released()), this, SLOT(textBold()));
    QObject::connect(tbtnUnderlined, SIGNAL(released()), this, SLOT(textUnderline()));
    QObject::connect(tbtnColour, SIGNAL(released()), this, SLOT(textColor()));
    QObject::connect(sizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(textSize(const QString &)));
    QObject::connect(fontComboBox, SIGNAL(activated(const QString &)), this, SLOT(textFamily(const QString &)));
    QObject::connect(tbtngrpAlign, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(textAlign(QAbstractButton *)));
    QObject::connect(theTextEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    QObject::connect(theTextEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));
    QObject::connect(theTextEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
}

void MTTextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(tbtnBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void MTTextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(tbtnUnderlined->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MTTextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(tbtnItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MTTextEdit::textFamily(const QString & f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void MTTextEdit::textSize(const QString & p)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(p.toDouble());
    mergeFormatOnWordOrSelection(fmt);
}

void MTTextEdit::textColor()
{
    QColor col = QColorDialog::getColor(theTextEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void MTTextEdit::textAlign(QAbstractButton * b)
{
    if (b == tbtnAlignLeft)
        theTextEdit->setAlignment(Qt::AlignLeft);
    else if (b == tbtnAlignCentre)
        theTextEdit->setAlignment(Qt::AlignHCenter);
    else if (b == tbtnAlignRight)
        theTextEdit->setAlignment(Qt::AlignRight);
    else if (b == tbtnAlignJustify)
        theTextEdit->setAlignment(Qt::AlignJustify);
}

void MTTextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat & format)
{
    QTextCursor cursor = theTextEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    theTextEdit->mergeCurrentCharFormat(format);
}

void MTTextEdit::fontChanged(const QFont & f)
{
    fontComboBox->setCurrentIndex(fontComboBox->findText(QFontInfo(f).family()));
    sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(f.pointSize())));
    tbtnBold->setChecked(f.bold());
    tbtnItalic->setChecked(f.italic());
    tbtnUnderlined->setChecked(f.underline());
}

void MTTextEdit::colorChanged(const QColor & c)
{
    QPixmap pix(14, 14);
    pix.fill(c);
    tbtnColour->setIcon(pix);
}

void MTTextEdit::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
        tbtnAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        tbtnAlignCentre->setChecked(true);
    } else if (a & Qt::AlignRight) {
        tbtnAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        tbtnAlignJustify->setChecked(true);
    }
}

void MTTextEdit::currentCharFormatChanged(const QTextCharFormat & format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void MTTextEdit::cursorPositionChanged()
{
    alignmentChanged(theTextEdit->alignment());
}
