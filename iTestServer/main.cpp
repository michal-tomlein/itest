/******************************************************************************
 *                                    iTest                                   *
 * -------------------------------------------------------------------------- *
 * Version:      1.4.1                                                        *
 * Qt version:   4.3.0 or higher required                                     *
 * -------------------------------------------------------------------------- *
 * iTest is a Qt application consisting of a Server and a Client designed for *
 * easy computerised examination.                                             *
 * -------------------------------------------------------------------------- *
 * iTest is distributed under the terms of the GPL v2, see details below.     *
 ******************************************************************************/

#include "../shared/main.h"

int main(int argc, char *argv[])
{
	MTApplication app(argc, argv);

	QSettings settings("Michal Tomlein", "iTest");
	QString lang = settings.value("lang").toString();
	if (lang.isEmpty()) {
		lang = QLocale::languageToString(QLocale::system().language());
		settings.setValue("lang", lang);
	}
	if (lang == "C") { lang = "English"; settings.setValue("lang", lang); }
	if (lang != "English") {
		QTranslator * translator = new QTranslator;
		translator->load(QString(":/i18n/iTestServer-%1.qm").arg(lang.replace(" ", "_")));
		app.installTranslator(translator);
	}

	MainWindow * itest_window = new MainWindow;
    app.setAppMainWindow(itest_window);
	itest_window->show();
	return app.exec();
}

// ---------------------------- version changelog: -----------------------------
/* version 1.4.1 - a minor update with some new features
                 - added Latvian translation
                 - number of flags now unlimited
                   - iTest 1.4.0 won't be able to open your database if you
                     create more than 20 flags (otherwise, the database format
                     is unchanged and compatible)
                 - added the "Shuffle answers" and "Do not print test results"
                   options to server settings
                 - "Other settings" moved from iTestClient to iTestServer
                 - Mac OS X: double click or drag and drop database files onto
                   the iTest icon to open them
                 - upgraded from Qt 4.4.0 to Qt 4.5.0 (Windows and Mac OS X
                   binary packages; tarballs still build with Qt 4.4)
                 - numerous bug-fixes and more...
*/
/* version 1.4.0 - a major update
                 - added Italian translation
                 - added the ability to create classes with students
                 - added an advanced scoring system
                 - added the ability to set the (minimum) number of questions in
                   a test for each flag separately
                 - added multiple choice questions support
                 - added support for up to 9 answers and the ability to set
                   this number for each question separately
                 - advanced test printing options, random order of questions in
                   each printout, the ability to print keys to tests
                 - improved appearance under Mac OS X
                 - new database format - older versions of iTest cannot open the
                   new itdb 1.4 files, iTest 1.4 can still open older databases
                 - code clean-up, code sharing with iTestClient
                 - upgraded from Qt 4.3.2 to Qt 4.4.0
                 - numerous bug-fixes and more...
*/
/* version 1.3.0 - a major update
                 - added Spanish translation
                 - added SVG (scalable vector graphics) support, which means
                   questions can now contain graphics
                 - added the ability to print questions and tests (with or
                   without graphics)
                 - added multiple correct answers support
                 - added search to the "overall statistics" dialogue
                 - made the ui more flexible
                 - many improvements in translations
                 - new database format - older versions of iTest cannot open the
                   new itdb 1.3 files, iTest 1.3 can still open older databases
                 - upgraded from Qt 4.3.1 to Qt 4.3.2
                 - numerous bug-fixes and more...
*/
/* version 1.2.0 - a major update
                 - renamed to iTestServer
                 - added Portuguese translation
                 - added the ability to set a pass mark for each flag separately
                   - this required some changes in the saved sessions view and
                     in printing
                 - added the ability to group similar questions in order to be
                   able to tell the client to choose one question of the group
                   at most (useful when knowing the answer for one question
                   would suffice to guess the answers for other questions)
                 - server setup (advanced): double clicking adds/removes items
                 - improved the "overall statistics" dialogue; question names
                   now coloured according to their flag
                 - improved the "change language" dialogue
                 - improved the comments editor (undo/redo)
                 - changed flag 1 colour to light green
                 - row height now calculated correctly in the "overall
                   statistics" dialogue
                 - fixed the "adjust difficulty" button - now the icon in the
                   list of questions changes when difficulty changed
                 - cleaned up the menus, improved their behaviour
                 - code clean-up
                 - new database format - older versions of iTest cannot open the
                   new itdb 1.2 files, iTest 1.2 can still open older databases
                 - upgraded from Qt 4.3.0 to Qt 4.3.1
                 - and more...
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
                 - printing: support for the & sign
*/
/* version 1.1.0 - a major update
                 - added Russian translation
                 - removed the ability to delete a session
                 - added the ability to archive, restore and copy sessions
                 - added the ability to save a copy and a backup copy
                 - changes made to questions auto-applied when saving
                 - the selected session now green in the list of saved sessions
                 - decreased font size of printed exam results
                 - items in the list of students now green if passed and orange
                   if failed
                 - warning when changing the name of the database or a question,
                   occurrences in sessions and archived sessions auto-updated
*/
/* version 1.0.3 - a bug-fix release
                 - fixed opening of a database (had no visible effect unless you
                   had more saved sessions than questions)
*/
/* version 1.0.2 - a bug-fix release
                 - fixed version number, now saved as a float
                 - fixed incorrect flags behaviour
                   (auto-assigning of flags when none set)
*/
/* version 1.0.1 - a bug-fix release
                 - fixed off-line client import if a custom test name is used
                 - test name used when exporting (off-line session; log)
                 - fixed segmentation fault when starting a server session
                 - fixed incorrect Slovak translations (window title and others)
                 - bool Session::mostPassed()
                 - printing: results progress bar colour auto-adjustment
                 - fixed save session checkbox behaviour
                 - fixed the "yyyy.MM.dd-hh:mm" vs. "yyyy.MM.dd-hh.mm" issue
                 - made log export possible even from saved sessions
*/
// version 1.0.0 - the first release

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
