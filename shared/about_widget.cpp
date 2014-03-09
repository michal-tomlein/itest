/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2014 Michal Tomlein

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

#include "about_widget.h"

#include <QDesktopServices>

AboutWidget::AboutWidget(const QString & ver)
{
    setupUi(this);

    QObject::connect(aboutQtButton, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
#ifdef Q_OS_MAC
    QObject::connect(aboutQtButton, SIGNAL(clicked()), this, SLOT(close()));
#endif
    QObject::connect(licenceButton, SIGNAL(clicked()), this, SLOT(showLicence()));

    versionLabel->setText(tr("Version %1").arg(ver));

    QString about;
    about.append("<p style=\"font-family: sans-serif; font-size:8pt;\">");
    about.append(tr("iTest is an application suite consisting of a Server and a Client designed for easy computerised examination."));
    about.append("</p><p></p>");
    about.append("<p style=\"font-family: sans-serif; font-size:8pt;\">");
    about.append(tr("This program is distributed under the terms of the GPL v2."));
    about.append("</p><p></p>");
    about.append(QString("<p style=\"font-family: sans-serif; font-size:8pt;\">2005&ndash;2014 <span style=\"font-style:italic;\">Michal Tomlein</span> (michal@tomlein.org)</p>"));
    about.append("<p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
    about.append(tr("The program is provided AS IS with ABSOLUTELY NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE."));
    about.append("</p>");
#ifdef Q_WS_MAC
    about.remove("font-family: sans-serif;");
    about.replace("font-size:8pt;", "font-size:11pt;");
#endif

    aboutTextBrowser->setHtml(QString("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; margin-left:0px; margin-right:0px; color: #9F9F9F; }\n</style></head><body style=\"margin-top:0px; margin-bottom:0px; margin-left:9px; margin-right:9px;\">\n%1</body></html>").arg(about));
}

void AboutWidget::showLicence()
{
    QDesktopServices::openUrl(QUrl(tr("http://www.gnu.org/licenses/gpl-2.0.html")));
}
