#ifndef ABOUT_WIDGET_H
#define ABOUT_WIDGET_H

#include "ui_about_widget_v2.h"

#include <QResizeEvent>
#include <QTimer>

class AboutWidget : public QWidget, private Ui::AboutWidget
{
    Q_OBJECT

public:
    AboutWidget(QString, QString, QString);

private slots:
    void updateGeometry();
    
private:
    void resizeEvent(QResizeEvent*);
    
};

#endif // ABOUT_WIDGET_H
