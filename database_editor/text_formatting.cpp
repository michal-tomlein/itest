#include "main_window.h"

const int EQ_index = 1; const int EC_index = 2;

void MainWindow::setupTextEdits()
{
    QPixmap pix(14, 14);
    pix.fill(Qt::black);
    tbtnColour->setIcon(pix);
    
    tbtngrpAlign = new QButtonGroup (this);
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
    QObject::connect(SQQuestionTextEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    QObject::connect(SQQuestionTextEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));
            
    ECtbtnColour->setIcon(pix);
            
    tbtngrpECAlign = new QButtonGroup (this);
    tbtngrpECAlign->addButton(ECtbtnAlignLeft);
    tbtngrpECAlign->addButton(ECtbtnAlignCentre);
    tbtngrpECAlign->addButton(ECtbtnAlignRight);
    tbtngrpECAlign->addButton(ECtbtnAlignJustify);
            
    QObject::connect(ECtbtnItalic, SIGNAL(released()), this, SLOT(textItalic()));
    QObject::connect(ECtbtnBold, SIGNAL(released()), this, SLOT(textBold()));
    QObject::connect(ECtbtnUnderlined, SIGNAL(released()), this, SLOT(textUnderline()));
    QObject::connect(ECtbtnColour, SIGNAL(released()), this, SLOT(textColor()));
    QObject::connect(ECsizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(textSize(const QString &)));
    QObject::connect(ECfontComboBox, SIGNAL(activated(const QString &)), this, SLOT(textFamily(const QString &)));
    QObject::connect(tbtngrpECAlign, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(textAlign(QAbstractButton *)));
    QObject::connect(ECTextEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    QObject::connect(ECTextEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));
}

void MainWindow::textBold()
{
    QTextCharFormat fmt;
    if (mainStackedWidget->currentIndex() == EQ_index) {
        fmt.setFontWeight(tbtnBold->isChecked() ? QFont::Bold : QFont::Normal);
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        fmt.setFontWeight(ECtbtnBold->isChecked() ? QFont::Bold : QFont::Normal);
    }
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    if (mainStackedWidget->currentIndex() == EQ_index) {
        fmt.setFontUnderline(tbtnUnderlined->isChecked());
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        fmt.setFontUnderline(ECtbtnUnderlined->isChecked());
    }
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textItalic()
{
    QTextCharFormat fmt;
    if (mainStackedWidget->currentIndex() == EQ_index) {
        fmt.setFontItalic(tbtnItalic->isChecked());
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        fmt.setFontItalic(ECtbtnItalic->isChecked());
    }
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textSize(const QString &p)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(p.toFloat());
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textColor()
{
    QColor col;
    if (mainStackedWidget->currentIndex() == EQ_index) {
        col = QColorDialog::getColor(SQQuestionTextEdit->textColor(), this);
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        col = QColorDialog::getColor(ECTextEdit->textColor(), this);
    }
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void MainWindow::textAlign(QAbstractButton *b)
{
    if (b == tbtnAlignLeft)
        SQQuestionTextEdit->setAlignment(Qt::AlignLeft);
    else if (b == tbtnAlignCentre)
        SQQuestionTextEdit->setAlignment(Qt::AlignHCenter);
    else if (b == tbtnAlignRight)
        SQQuestionTextEdit->setAlignment(Qt::AlignRight);
    else if (b == tbtnAlignJustify)
        SQQuestionTextEdit->setAlignment(Qt::AlignJustify);
    else if (b == ECtbtnAlignLeft)
        ECTextEdit->setAlignment(Qt::AlignLeft);
    else if (b == ECtbtnAlignCentre)
        ECTextEdit->setAlignment(Qt::AlignHCenter);
    else if (b == ECtbtnAlignRight)
        ECTextEdit->setAlignment(Qt::AlignRight);
    else if (b == ECtbtnAlignJustify)
        ECTextEdit->setAlignment(Qt::AlignJustify);
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    if (mainStackedWidget->currentIndex() == EQ_index) {
        QTextCursor cursor = SQQuestionTextEdit->textCursor();
        if (!cursor.hasSelection())
            cursor.select(QTextCursor::WordUnderCursor);
        cursor.mergeCharFormat(format);
        SQQuestionTextEdit->mergeCurrentCharFormat(format);
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        QTextCursor cursor = ECTextEdit->textCursor();
        if (!cursor.hasSelection())
            cursor.select(QTextCursor::WordUnderCursor);
        cursor.mergeCharFormat(format);
        ECTextEdit->mergeCurrentCharFormat(format);
    }
}

void MainWindow::fontChanged(const QFont &f)
{
    if (mainStackedWidget->currentIndex() == EQ_index) {
        fontComboBox->setCurrentIndex(fontComboBox->findText(QFontInfo(f).family()));
        sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(f.pointSize())));
        tbtnBold->setChecked(f.bold());
        tbtnItalic->setChecked(f.italic());
        tbtnUnderlined->setChecked(f.underline());
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        ECfontComboBox->setCurrentIndex(ECfontComboBox->findText(QFontInfo(f).family()));
        ECsizeComboBox->setCurrentIndex(ECsizeComboBox->findText(QString::number(f.pointSize())));
        ECtbtnBold->setChecked(f.bold());
        ECtbtnItalic->setChecked(f.italic());
        ECtbtnUnderlined->setChecked(f.underline());
    }
}

void MainWindow::colorChanged(const QColor &c)
{
    QPixmap pix(14, 14);
    pix.fill(c);
    if (mainStackedWidget->currentIndex() == EQ_index) {
        tbtnColour->setIcon(pix);
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        ECtbtnColour->setIcon(pix);
    }
}

void MainWindow::alignmentChanged(Qt::Alignment a)
{
    if (mainStackedWidget->currentIndex() == EQ_index) {
        if (a & Qt::AlignLeft) {
            tbtnAlignLeft->setChecked(true);
        } else if (a & Qt::AlignHCenter) {
            tbtnAlignCentre->setChecked(true);
        } else if (a & Qt::AlignRight) {
            tbtnAlignRight->setChecked(true);
        } else if (a & Qt::AlignJustify) {
            tbtnAlignJustify->setChecked(true);
        }
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        if (a & Qt::AlignLeft) {
            ECtbtnAlignLeft->setChecked(true);
        } else if (a & Qt::AlignHCenter) {
            ECtbtnAlignCentre->setChecked(true);
        } else if (a & Qt::AlignRight) {
            ECtbtnAlignRight->setChecked(true);
        } else if (a & Qt::AlignJustify) {
            ECtbtnAlignJustify->setChecked(true);
        }
    }
}

void MainWindow::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void MainWindow::cursorPositionChanged()
{
    if (mainStackedWidget->currentIndex() == EQ_index) {
        alignmentChanged(SQQuestionTextEdit->alignment());
    } else if (mainStackedWidget->currentIndex() == EC_index) {
        alignmentChanged(ECTextEdit->alignment());
    }
}
