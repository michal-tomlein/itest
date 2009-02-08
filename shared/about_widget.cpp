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

#include "about_widget.h"

AboutWidget::AboutWidget(QString ver, QString year)
{
	setupUi(this);
	QObject::connect(btnClose, SIGNAL(released()), this, SLOT(close()));
	QString about = "<p style=\"font-family: sans-serif; font-style:italic;\"><span style=\"font-size:12pt;\">iTest</span><br>";
	about.append("<span style=\"font-size:8pt;\">");
    about.append(tr("Version"));
    about.append(QString(" %1</span></p><p></p>").arg(ver));
	about.append("<p style=\"font-family: sans-serif; font-size:8pt;\">");
    about.append(tr("iTest is a Qt application consisting of a Server and a Client designed for easy computerised examination."));
    about.append("</p><p></p>");
	about.append("<p style=\"font-family: sans-serif; font-size:8pt;\">");
    about.append(tr("This programme is distributed under the terms of the GPL v2."));
	about.append("</p><p></p>");
	about.append(QString("<p style=\"font-family: sans-serif; font-size:8pt;\">2005-%1 <span style=\"font-style:italic;\">Michal Tomlein</span> (michal.tomlein@gmail.com)</p>").arg(year));
	about.append("<p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	about.append(tr("The programme is provided AS IS with ABSOLUTELY NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE."));
	about.append("</p>");
#ifdef Q_WS_MAC
	about.remove("font-family: sans-serif;");
	about.replace("font-size:12pt;", "font-size:14pt;");
	about.replace("font-size:8pt;", "font-size:10pt;");
#endif
    aboutTextBrowser->setHtml(about);
	QString aboutQt = "<p style=\"font-family: sans-serif; font-style:italic;\"><span style=\"font-size:12pt;\">";
    aboutQt.append(tr("About Qt"));
	aboutQt.append("</span></p><p></p><p style=\"font-family: sans-serif; font-size:8pt; font-style:italic;\">");
	aboutQt.append(tr("This programme uses Qt Open Source Edition version %1.").arg(qVersion()));
	aboutQt.append("</p><p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	aboutQt.append(tr("Qt is a C++ toolkit for cross-platform application development."));
	aboutQt.append("</p><p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	aboutQt.append(tr("Qt provides single-source portability across MS Windows, Mac OS X, Linux and all major commercial Unix variants. Qt is also available for embedded devices as Qtopia Core."));
	aboutQt.append("</p><p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	aboutQt.append(tr("Qt is a Trolltech product. See <span style=\"font-style:italic;\">http://www.trolltech.com/qt/</span> for more information."));
	aboutQt.append("</p>");
#ifdef Q_WS_MAC
	aboutQt.remove("font-family: sans-serif;");
	aboutQt.replace("font-size:12pt;", "font-size:14pt;");
	aboutQt.replace("font-size:8pt;", "font-size:10pt;");
#endif
    aboutQtTextBrowser->setHtml(aboutQt);
}
