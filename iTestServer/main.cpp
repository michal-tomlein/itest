/******************************************************************************
 *                                    iTest                                   *
 * -------------------------------------------------------------------------- *
 * Version:      1.2.0                                                        *
 * Qt version:   4.3.1                                                        *
 * -------------------------------------------------------------------------- *
 * iTest is a Qt application consisting of a Database Editor and a Test       *
 * Writer designed for easy computerised examination.                         *
 * -------------------------------------------------------------------------- *
 * This programme is distributed under the terms of the GPL v2.               *
 * -------------------------------------------------------------------------- *
 * The programme is provided AS IS with ABSOLUTELY NO WARRANTY OF ANY KIND,   *
 * INCLUDING THE WARRANTY OF DESIGN, MERCHANTIBILITY AND FITNESS FOR          *
 * A PARTICULAR PURPOSE.                                                      *
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
		translator->load(QString(":/i18n/%1.qm").arg(lang.replace(" ", "_")));
		app.installTranslator(translator);
	}

	MainWindow * itest_window = new MainWindow;
	itest_window->show();
	return app.exec();
}

// ---------------------------- version changelog: -----------------------------
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
