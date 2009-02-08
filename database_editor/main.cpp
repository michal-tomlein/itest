/******************************************************************************
 *                                    iTest                                   *
 * -------------------------------------------------------------------------- *
 * Version:      1.1.0                                                        *
 * Qt version:   4.2.2                                                        *
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
	QString lang = settings.value("lang", "English").toString();
	if (lang != "English") {
		QTranslator * translator = new QTranslator;
		translator->load(QString(":/i18n/%1.qm").arg(lang));
		app.installTranslator(translator);
	}

	MainWindow * itest_window = new MainWindow;
	itest_window->show();
	return app.exec();
}

// ---------------------------- version changelog: -----------------------------
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
