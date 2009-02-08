#ifndef EXTENDEDLINEEDIT_H
#define EXTENDEDLINEEDIT_H

#include <QLineEdit>
#include <QResizeEvent>
#include <QToolButton>
#include <QStyle>

class ExtendedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    ExtendedLineEdit(QWidget * parent = 0):
    QLineEdit(parent) {
        xle_btn_clear = new QToolButton(this);
        xle_btn_clear->setIcon(QIcon(QString::fromUtf8(":/images/images/clear.png")));
        xle_btn_clear->setIconSize(QSize(13, 13));
        xle_btn_clear->setCursor(Qt::ArrowCursor);
        xle_btn_clear->setStyleSheet("QToolButton { border: none; padding: 0px; }");
        xle_btn_clear->hide();
        QObject::connect(xle_btn_clear, SIGNAL(clicked()), this, SLOT(clear()));
        QObject::connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(setClearButtonVisible(const QString &)));
        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(xle_btn_clear->sizeHint().width() + frameWidth + 1));
        QSize minsize = minimumSizeHint();
        setMinimumSize(qMax(minsize.width(), xle_btn_clear->sizeHint().height() + frameWidth * 2 + 2),
                        qMax(minsize.height(), xle_btn_clear->sizeHint().height() + frameWidth * 2 + 2));
    };

protected:
    void resizeEvent(QResizeEvent *) {
        QSize s = xle_btn_clear->sizeHint();
        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        xle_btn_clear->move(rect().right() - frameWidth - s.width(), (rect().bottom() + 1 - s.height())/2);
    };

private slots:
    void setClearButtonVisible(const QString & text) {
        xle_btn_clear->setVisible(!text.isEmpty());
    };

private:
    QToolButton * xle_btn_clear;
};

#endif // EXTENDEDLINEEDIT_H
