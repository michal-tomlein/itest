/******************************************************************************
 *                                    iTest                                   *
 * -------------------------------------------------------------------------- *
 * Version:      1.3.0                                                        *
 * Qt version:   4.3.2                                                        *
 * -------------------------------------------------------------------------- *
 * iTest is a Qt application consisting of a Server and a Client designed for *
 * easy computerised examination.                                             *
 * -------------------------------------------------------------------------- *
 * iTest is distributed under the terms of the GPL v2, see details below.     *
 ******************************************************************************/

#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QSettings settings("Michal Tomlein", "iTest");
	QString lang = settings.value("lang").toString();
	if (lang.isEmpty()) {
		lang = QLocale::languageToString(QLocale::system().language());
		settings.setValue("lang", lang);
	}
	if (lang == "C") { lang = "English"; settings.setValue("lang", lang); }
	if (lang != "English") {
		QTranslator * translator = new QTranslator;
		translator->load(QString(":/i18n/iTestClient-%1.qm").arg(lang.replace(" ", "_")));
		app.installTranslator(translator);
	}

	MainWindow * itest_window = new MainWindow;
	itest_window->showFullScreen();
	return app.exec();
}

// ---------------------------- version changelog: -----------------------------
/* version 1.3.0 - a major update
                 - added Spanish translation
                 - added SVG (scalable vector graphics) support
                 - added multiple correct answers support
                 - added the ability to hide correct answers at the end of tests
                 - made the ui more flexible
                 - fixed a bug where the status of the test progress bar was not
                   refreshed after starting a new test
                 - upgraded from Qt 4.3.1 to Qt 4.3.2
*/
/* version 1.2.0 - a major update
                 - renamed to iTestClient
                 - added Portuguese translation
                 - added the ability to start a new test
                 - more advanced test generation, support for groups added
                 - upgraded from Qt 4.3.0 to Qt 4.3.1
*/
/* version 1.1.1 - a bug-fix release with some new features
                 - added Turkish translation
                 - if available, translation to the system language loaded by
                   default
                 - NEW ENCODING: UTF-8 - adds support for more languages and
                   special characters
                   - iTest 1.1.1 can still open old CP 1250 databases from older
                     versions of iTest
                   - older versions of iTest cannot open the new UTF-8 databases
                     from iTest 1.1.1
                 - upgraded from Qt 4.2.2 to Qt 4.3.0
*/
/* version 1.1.0 - a major update
                 - added Russian translation
                 - added the ability to hide question names
*/
// version 1.0.3 - a bug-fix release
// version 1.0.2 - a bug-fix release
/* version 1.0.1 - a bug-fix release
                 - fixed segmentation fault when connecting
*/
// version 1.0.0 - the first release

/*******************************************************************
 This file is part of iTest
 Copyright (C) 2007 Michal Tomlein (michal.tomlein@gmail.com)

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
