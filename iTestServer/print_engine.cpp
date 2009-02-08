#include "main_window.h"

bool MainWindow::loadPrinterSettings()
{
	QSettings settings("Michal Tomlein", "iTest");
	printer_configured = settings.value("printer/configured", false).toBool();
	printer_colorMode = settings.value("printer/colorMode").toInt();
	printer_doubleSidedPrinting = settings.value("printer/doubleSidedPrinting").toBool();
	printer_fontEmbeddingEnabled = settings.value("printer/fontEmbeddingEnabled").toBool();
	printer_numCopies = settings.value("printer/numCopies").toInt();
	printer_orientation = settings.value("printer/orientation").toInt();
	printer_outputFileName = settings.value("printer/outputFileName").toString();
	printer_outputFormat = settings.value("printer/outputFormat").toInt();
	printer_pageOrder = settings.value("printer/pageOrder").toInt();
	printer_pageSize = settings.value("printer/pageSize").toInt();
	printer_paperSource = settings.value("printer/paperSource").toInt();
	printer_printRange = settings.value("printer/printRange").toInt();
	printer_printerName = settings.value("printer/printerName").toString();
	printer_resolution = settings.value("printer/resolution").toInt();
	return printer_configured;
}

void MainWindow::savePrinterSettings()
{
	QSettings settings("Michal Tomlein", "iTest");
	settings.setValue("printer/configured", printer_configured);
	if (!printer_configured) return;
	settings.setValue("printer/colorMode", printer_colorMode);
	settings.setValue("printer/doubleSidedPrinting", printer_doubleSidedPrinting);
	settings.setValue("printer/fontEmbeddingEnabled", printer_fontEmbeddingEnabled);
	settings.setValue("printer/numCopies", printer_numCopies);
	settings.setValue("printer/orientation", printer_orientation);
	settings.setValue("printer/outputFileName", printer_outputFileName);
	settings.setValue("printer/outputFormat", printer_outputFormat);
	settings.setValue("printer/pageOrder", printer_pageOrder);
	settings.setValue("printer/pageSize", printer_pageSize);
	settings.setValue("printer/paperSource", printer_paperSource);
	settings.setValue("printer/printRange", printer_printRange);
	settings.setValue("printer/printerName", printer_printerName);
	settings.setValue("printer/resolution", printer_resolution);
}

bool MainWindow::printerConfiguration(QString & printer_config)
{
    QTextStream conf(&printer_config);
    conf << "<br><br>\n<b>" << tr("Colour mode:") << "</b>" << " ";
    switch (printer_colorMode) {
        case QPrinter::Color: conf << tr("Colour") << endl; break;
        case QPrinter::GrayScale: conf << tr("GrayScale") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Double-sided printing:") << "</b>" << " ";
    if (printer_doubleSidedPrinting) { conf << tr("Yes") << endl; }
    else { conf << tr("No") << endl; }
    conf << "<br><b>" << tr("Font embedding:") << "</b>" << " ";
    if (printer_fontEmbeddingEnabled) { conf << tr("Yes") << endl; }
    else { conf << tr("No") << endl; }
    conf << "<br><b>" << tr("Number of copies:") << "</b>" << " ";
    conf << printer_numCopies << endl;
#ifndef Q_WS_X11
    conf << "<br>" << tr("Note: On Windows and Mac OS X, the number above is always 1<br>\nas these operating systems handle the number of copies internally.<br>\nTo print more copies, click 'Review configuration' and set the<br>\nnumber of copies each time you start the server.") << endl;
#endif
    conf << "<br><b>" << tr("Orientation:") << "</b>" << " ";
    switch (printer_orientation) {
        case QPrinter::Portrait: conf << tr("Portrait") << endl; break;
        case QPrinter::Landscape: conf << tr("Landscape") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    if (!printer_outputFileName.isEmpty()) {
        conf << "<br><b>" << tr("Output file name:") << "</b>" << " ";
        conf << printer_outputFileName << endl;
    }
    conf << "<br><b>" << tr("Output format:") << "</b>" << " ";
    switch (printer_outputFormat) {
        case QPrinter::NativeFormat: conf << tr("Native") << endl; break;
        case QPrinter::PdfFormat: conf << tr("PDF") << endl; break;
        case QPrinter::PostScriptFormat: conf << tr("PostScript") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Page order:") << "</b>" << " ";
    switch (printer_pageOrder) {
        case QPrinter::FirstPageFirst: conf << tr("First page first") << endl; break;
        case QPrinter::LastPageFirst: conf << tr("Last page first") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Paper format:") << "</b>" << " ";
    switch (printer_pageSize) {
        case QPrinter::A0: conf << "A0" << endl; break;
        case QPrinter::A1: conf << "A1" << endl; break;
        case QPrinter::A2: conf << "A2" << endl; break;
        case QPrinter::A3: conf << "A3" << endl; break;
        case QPrinter::A4: conf << "A4" << endl; break;
        case QPrinter::A5: conf << "A5" << endl; break;
        case QPrinter::A6: conf << "A6" << endl; break;
        case QPrinter::A7: conf << "A7" << endl; break;
        case QPrinter::A8: conf << "A8" << endl; break;
        case QPrinter::A9: conf << "A9" << endl; break;
        case QPrinter::B0: conf << "B0" << endl; break;
        case QPrinter::B1: conf << "B1" << endl; break;
        case QPrinter::B10: conf << "B10" << endl; break;
        case QPrinter::B2: conf << "B2" << endl; break;
        case QPrinter::B3: conf << "B3" << endl; break;
        case QPrinter::B4: conf << "B4" << endl; break;
        case QPrinter::B5: conf << "B5" << endl; break;
        case QPrinter::B6: conf << "B6" << endl; break;
        case QPrinter::B7: conf << "B7" << endl; break;
        case QPrinter::B8: conf << "B8" << endl; break;
        case QPrinter::B9: conf << "B9" << endl; break;
        case QPrinter::C5E: conf << "C5E" << endl; break;
        case QPrinter::Comm10E: conf << "U.S. Common 10 Envelope" << endl; break;
        case QPrinter::DLE: conf << "DLE" << endl; break;
        case QPrinter::Executive: conf << "Executive" << endl; break;
        case QPrinter::Folio: conf << "Folio" << endl; break;
        case QPrinter::Ledger: conf << "Ledger" << endl; break;
        case QPrinter::Legal: conf << "Legal" << endl; break;
        case QPrinter::Letter: conf << "Letter" << endl; break;
        case QPrinter::Tabloid: conf << "Tabloid" << endl; break;
        case QPrinter::Custom: conf << tr("Custom") << endl; break;
        default: conf << tr("Custom") << endl; break;
    }
    conf << "<br><b>" << tr("Paper source:") << "</b>" << " ";
    switch (printer_paperSource) {
        case QPrinter::OnlyOne: conf << tr("Only one") << endl; break;
        case QPrinter::Lower: conf << tr("Lower") << endl; break;
        case QPrinter::Middle: conf << tr("Middle") << endl; break;
        case QPrinter::Manual: conf << tr("Manual") << endl; break;
        case QPrinter::Envelope: conf << tr("Envelope") << endl; break;
        case QPrinter::EnvelopeManual: conf << tr("Envelope (manual)") << endl; break;
        case QPrinter::Auto: conf << tr("Auto") << endl; break;
        case QPrinter::Tractor: conf << tr("Tractor") << endl; break;
        case QPrinter::SmallFormat: conf << tr("Small format") << endl; break;
        case QPrinter::LargeFormat: conf << tr("Large format") << endl; break;
        case QPrinter::LargeCapacity: conf << tr("Large capacity") << endl; break;
        case QPrinter::Cassette: conf << tr("Cassette") << endl; break;
        case QPrinter::FormSource: conf << tr("Form source") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    conf << "<br><b>" << tr("Print range:") << "</b>" << " ";
    switch (printer_printRange) {
        case QPrinter::AllPages: conf << tr("All pages") << endl; break;
        case QPrinter::Selection: conf << tr("Selection") << endl; break;
        case QPrinter::PageRange: conf << tr("Page range") << endl; break;
        default: conf << tr("Unknown") << endl; break;
    }
    if (printer_outputFileName.isEmpty()) {
        conf << "<br><b>" << tr("Printer name:") << "</b>" << " ";
        conf << printer_printerName << endl;
    }
    conf << "<br><b>" << tr("Resolution:") << "</b>" << " ";
    conf << printer_resolution << endl;
    conf << "<br><br>" << endl;
    return true;
}

bool MainWindow::configurePrinter(bool review_config)
{
	if (!review_config) {
		if (default_printer != NULL) delete default_printer;
		default_printer = new QPrinter (QPrinter::HighResolution);
	} else {
		if (!loadPrinterConfiguration()) return false;
	}
	QPrintDialog * dialog = new QPrintDialog (default_printer, this);
	dialog->setWindowTitle(tr("Configure printer"));
	if (dialog->exec() != QDialog::Accepted) return false;

	printer_configured = true;
	printer_colorMode = default_printer->colorMode();
	printer_doubleSidedPrinting = default_printer->doubleSidedPrinting();
	printer_fontEmbeddingEnabled = default_printer->fontEmbeddingEnabled();
	printer_numCopies = default_printer->numCopies();
	printer_orientation = default_printer->orientation();
	printer_outputFileName = default_printer->outputFileName();
	printer_outputFormat = default_printer->outputFormat();
	printer_pageOrder = default_printer->pageOrder();
	printer_pageSize = default_printer->pageSize();
	printer_paperSource = default_printer->paperSource();
	printer_printRange = default_printer->printRange();
	printer_printerName = default_printer->printerName();
	printer_resolution = default_printer->resolution();
	savePrinterSettings();
    return true;
}

bool MainWindow::loadPrinterConfiguration()
{
	if (default_printer != NULL) delete default_printer;
	default_printer = new QPrinter (QPrinter::HighResolution);
	default_printer->setColorMode((QPrinter::ColorMode)printer_colorMode);
	default_printer->setDoubleSidedPrinting(printer_doubleSidedPrinting);
	default_printer->setFontEmbeddingEnabled(printer_fontEmbeddingEnabled);
	default_printer->setNumCopies(printer_numCopies);
	default_printer->setOrientation((QPrinter::Orientation)printer_orientation);
	default_printer->setOutputFileName(printer_outputFileName);
	default_printer->setOutputFormat((QPrinter::OutputFormat)printer_outputFormat);
	default_printer->setPageOrder((QPrinter::PageOrder)printer_pageOrder);
	default_printer->setPageSize((QPrinter::PageSize)printer_pageSize);
	default_printer->setPaperSource((QPrinter::PaperSource)printer_paperSource);
	default_printer->setPrintRange((QPrinter::PrintRange)printer_printRange);
	default_printer->setPrinterName(printer_printerName);
	default_printer->setResolution(printer_resolution);
	return true;
}

bool MainWindow::printClientResults(Client * client, QPrinter * printer)
{
	Student * student = new Student(client);
	bool ok = printStudentResults(student, printer, current_db_testname);
	delete student;
	return ok;
}

bool MainWindow::printStudentResults(Student * student, QPrinter * printer, QString session_name)
{
	if (!student->isReady()) return false;
	QTextDocument doc; QString html; QTextStream out(&html);
	QuestionItem * item; QTextDocument qdoc;
	QString header = tr("Exam results"); header.append(QString(" - %1 - %2 - %3").arg(substituteHtmlTags(session_name)).arg(substituteHtmlTags(student->name())).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm")));
	out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
	out << header << endl << "</title><style type=\"text/css\">" << endl;
	out << ".heading { font-family: sans-serif; font-size: medium; font-weight: bold; color: black; }" << endl;
	out << ".default_text { font-family: sans-serif; font-size: 7pt; color: black; }" << endl;
	out << ".bold_text { font-family: sans-serif; font-size: 7pt; color: black; font-weight: bold; }" << endl;
	out << ".correct_qname { font-family: sans-serif; font-size: 7pt; font-weight: bold; color: rgb(69, 110, 14); }" << endl;
	out << ".incorrect_qname { font-family: sans-serif; font-size: 7pt; font-weight: bold; color: rgb(204, 109, 0); }" << endl;
	out << ".question { font-family: sans-serif; font-size: 7pt; color: black; }" << endl;
	out << ".answer { font-family: sans-serif; font-size: 7pt; font-style: italic; color: black; }" << endl;
	out << ".answered { text-decoration: underline; }" << endl;
	out << ".correct { font-weight: bold; }" << endl;
	out << "</style></head><body>" << endl;
	out << "<p class=\"heading\" align=\"center\">" << endl << header << endl << "</p>" << endl;
	QMapIterator<QString, QuestionAnswer> i(*(student->results())); QuestionAnswer qans;
	while (i.hasNext()) {
		i.next(); qans = i.value();
		out << "<p>" << endl;
		if (qans.isAnsweredCorrectly()) {
			out << "<div class=\"correct_qname\">" << endl;
		} else {
			out << "<div class=\"incorrect_qname\">" << endl;
		}
		item = NULL;
		/*for (int n = 0; n < LQListWidget->count(); ++n) {
			if (LQListWidget->item(n)->text() == i.key()) {
				item = current_db_questions.value(LQListWidget->item(n)); break;
			}
		}*/
		QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
		while (q.hasNext()) { q.next();
			if (q.value()->name() == i.key()) { item = q.value(); break; }
		}
		if (item != NULL) {
			if (item->flag() >= 0 && item->flag() < 20) {
				out << "<div class=\"bold_text\">";
				out << substituteHtmlTags(current_db_f[item->flag()]) << ": </div>";
			}
			if (!item->group().isEmpty()) {
				out << "[" << substituteHtmlTags(item->group()) << "] ";
			}
			out << substituteHtmlTags(item->name()) << endl << "</div>" << endl;
			out << "<div class=\"question\">" << endl;
			qdoc.setHtml(item->text());
			out << substituteHtmlTags(qdoc.toPlainText()) << endl << "</div>" << endl;
			out << "<div class=\"answer";
			if (qans.answered() == QuestionItem::A) { out << " answered"; }
			if (item->isAnsACorrect()) { out << " correct"; }
			out << "\">\na) " << substituteHtmlTags(item->ansA()) << endl << "</div>" << endl;
			out << "<div class=\"answer";
			if (qans.answered() == QuestionItem::B) { out << " answered"; }
			if (item->isAnsBCorrect()) { out << " correct"; }
			out << "\">\nb) " << substituteHtmlTags(item->ansB()) << endl << "</div>" << endl;
			out << "<div class=\"answer";
			if (qans.answered() == QuestionItem::C) { out << " answered"; }
			if (item->isAnsCCorrect()) { out << " correct"; }
			out << "\">\nc) " << substituteHtmlTags(item->ansC()) << endl << "</div>" << endl;
			out << "<div class=\"answer";
			if (qans.answered() == QuestionItem::D) { out << " answered"; }
			if (item->isAnsDCorrect()) { out << " correct"; }
			out << "\">\nd) " << substituteHtmlTags(item->ansD()) << endl << "</div>" << endl;
		} else {
			out << substituteHtmlTags(i.key()) << endl << "</div>" << endl;
			out << "<div class=\"question\">" << endl;
			out << tr("Not available") << endl << "</div>" << endl;
			out << "<div class=\"answer\">" << endl;
			out << tr("Answered:");
			switch (qans.answered()) {
				case QuestionItem::None: out << " None; "; break;
				case QuestionItem::A: out << " A; "; break;
				case QuestionItem::B: out << " B; "; break;
				case QuestionItem::C: out << " C; "; break;
				case QuestionItem::D: out << " D; "; break;
				default: out << " None; "; break;
			}
			out << tr("Correct answer:");
			switch (qans.correctAnswer()) {
				case QuestionItem::None: out << " None; "; break;
				case QuestionItem::A: out << " A; "; break;
				case QuestionItem::B: out << " B; "; break;
				case QuestionItem::C: out << " C; "; break;
				case QuestionItem::D: out << " D; "; break;
				default: out << " None; "; break;
			}
			out << endl << "</div>" << endl;
		}
		out << "</p>" << endl;
	}
	out << "<p class=\"default_text\" align=\"right\">" << endl;
	out << "<b>" << tr("Total score:") << "</b> ";
	out << tr("%1 out of %2").arg(student->score()).arg(student->results()->count());
	out << QString(" (%3)").arg(student->passed() ? tr("PASSED") : tr("FAILED"));
	out << endl << "</p></body></html>" << endl;
	doc.setHtml(html); printer->setDocName(header); doc.print(printer);
	return true;
}

void MainWindow::quickPrint()
{
    if (!SMLCListWidget->currentIndex().isValid()) return;
    Client * client = current_db_clients.value(SMLCListWidget->currentItem());
    if (client == NULL) return;
    if (!printClientResults(client, default_printer)) {
        QListWidgetItem * log_entry = new QListWidgetItem (tr("%1 > Client #%2 (%3) > failed to print the client's results (Server QuickPrint call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(client->number()).arg(client->name()));
        SMSLListWidget->insertItem(0, log_entry);
        log_entry->setBackground(QBrush::QBrush(QColor::QColor(200, 0, 0)));
        log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
    } else {
        QListWidgetItem * log_entry = new QListWidgetItem (tr("%1 > Client #%2 (%3) > results printed successfully (Server QuickPrint call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(client->number()).arg(client->name()));
        SMSLListWidget->insertItem(0, log_entry);
        log_entry->setBackground(QBrush::QBrush(QColor::QColor(180, 255, 0)));
        log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
    }
}

void MainWindow::print()
{
	Student * student; QString printdialog_caption; QString session_name;
	if (mainStackedWidget->currentIndex() == 5) {
		if (!SMLCListWidget->currentIndex().isValid()) return;
		student = new Student(current_db_clients.value(SMLCListWidget->currentItem()));
		printdialog_caption = tr("Print client's results");
		session_name = current_db_testname;
	} else if (mainStackedWidget->currentIndex() == 6) {
		if (!VSSLCListWidget->currentIndex().isValid()) return;
		student = current_db_students.value(VSSLCListWidget->currentItem());
		printdialog_caption = tr("Print student's results");
		if (current_db_session != NULL) {
            session_name = current_db_session->name();
        } else { session_name = current_db_name; }
	} else { return; }
	if (student == NULL) return;

	QPrinter * printer = new QPrinter (QPrinter::HighResolution);
	QPrintDialog * dialog = new QPrintDialog (printer, this);
	dialog->setWindowTitle(printdialog_caption);
	if (dialog->exec() != QDialog::Accepted) return;

	if (!printStudentResults(student, printer, session_name)) {
		if (mainStackedWidget->currentIndex() == 5) {
			QListWidgetItem * log_entry = new QListWidgetItem (tr("%1 > Client #%2 (%3) > failed to print the client's results (Server Print call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(student->number()).arg(student->name()));
			SMSLListWidget->insertItem(0, log_entry);
			log_entry->setBackground(QBrush::QBrush(QColor::QColor(200, 0, 0)));
			log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
		} else {
            statusBar()->showMessage(tr("Failed to print the results"), 10000);
        }
	} else {
		if (mainStackedWidget->currentIndex() == 5) {
			QListWidgetItem * log_entry = new QListWidgetItem (tr("%1 > Client #%2 (%3) > results printed successfully (Server Print call)").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(student->number()).arg(student->name()));
			SMSLListWidget->insertItem(0, log_entry);
			log_entry->setBackground(QBrush::QBrush(QColor::QColor(180, 255, 0)));
			log_entry->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
		} else {
            statusBar()->showMessage(tr("Results printed successfully"), 10000);
        }
	}

	delete printer;
	if (mainStackedWidget->currentIndex() == 5) { delete student; }
}

void MainWindow::printAll()
{
	if (current_db_session == NULL) { return; }

	QPrinter * printer = new QPrinter (QPrinter::HighResolution);
	QPrintDialog * dialog = new QPrintDialog (printer, this);
	dialog->setWindowTitle(tr("Print the results of all students"));
	if (dialog->exec() != QDialog::Accepted) return;

	if (printSessionSummary(current_db_session, printer)) {
        statusBar()->showMessage(tr("Summary printed successfully"), 10000);
    } else {
        statusBar()->showMessage(tr("Failed to print the summary"), 10000);
    }
    int numfailed = 0, numsuccessful = 0;
	for (int i = 0; i < current_db_session->numStudents(); ++i) {
		if (printStudentResults(current_db_session->student(i), printer, current_db_session->name()))
            { numsuccessful++; } else { numfailed++; }
	}
	statusBar()->showMessage(tr("%1 results printed successfully; %2 failed").arg(numsuccessful).arg(numfailed), 10000);

	delete printer;
}

void MainWindow::printSessionSummary()
{
	if (current_db_session == NULL) { return; }

	QPrinter * printer = new QPrinter (QPrinter::HighResolution);
	QPrintDialog * dialog = new QPrintDialog (printer, this);
	dialog->setWindowTitle(tr("Print session summary"));
	if (dialog->exec() != QDialog::Accepted) return;

	if (printSessionSummary(current_db_session, printer)) {
        statusBar()->showMessage(tr("Summary printed successfully"), 10000);
    } else {
        statusBar()->showMessage(tr("Failed to print the summary"), 10000);
    }

	delete printer;
}

bool MainWindow::printSessionSummary(Session * session, QPrinter * printer)
{
	if (session == NULL) { return false; }
	if (printer == NULL) { return false; }
	QTextDocument doc; QString html; QTextStream out(&html);
	QString header = tr("Session statistics and summary");
	header.append(QString(" - %1 - %2").arg(substituteHtmlTags(session->name())).arg(session->dateTimeToString()));
	out << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
	out << header << endl << "</title><style type=\"text/css\">" << endl;
	out << ".heading { font-family: sans-serif; font-size: medium; font-weight: bold; color: black; }" << endl;
	out << ".default_text { font-family: sans-serif; font-size: small; color: black; }" << endl;
	out << ".bold_text { font-family: sans-serif; font-size: small; font-weight: bold; color: black; }" << endl;
	out << ".student_passed { font-family: sans-serif; font-size: small; font-weight: bold; color: rgb(69, 110, 14); }" << endl;
	out << ".student_failed { font-family: sans-serif; font-size: small; font-weight: bold; color: rgb(204, 109, 0); }" << endl;
	out << "</style></head><body>" << endl;
	out << "<p class=\"heading\" align=\"center\">" << endl << header << endl << "</p>" << endl;
	out << "<p>\n<div class=\"bold_text\">" << endl << tr("Pass mark:");
	out << "</div><br><table border=\"0\" width=\"100%\">" << endl;
	out << "<tr><td width=\"40%\"><div class=\"bold_text\">" << endl;
	out << tr("Total") << endl << "</div></td><td><div class=\"bold_text\">";
	out << session->passMark().passMark() << endl << "</div></td></tr>" << endl;
	for (int i = 0; i < session->passMark().count(); ++i) {
		out << "<tr><td width=\"40%\"><div class=\"default_text\">" << endl;
		out << current_db_f[session->passMark().condition(i)] << endl;
		out << "</div></td><td><div class=\"default_text\">" << endl;
		out << session->passMark().value(i) << endl << "</div></td></tr>" << endl;
	}
	out << "</table><br><table border=\"0\" width=\"100%\"><tr><td width=\"60%\"><div class=\"bold_text\">" << endl;
	out << tr("Average:") << endl;
	out << "</div></td><td><div align=\"right\">" << endl;
	out << "<table border=\"2\" width=\"100%\" height=\"20\"><tr><td bgcolor=\"";
	out << (session->mostPassed() ? "yellowgreen" : "#FF9419"); // "#456E0E" : "#CC6D00"
	out << "\" width=\"" << int(100 * session->numCorrect() / session->numQuestions()) << "%\"></td>" << endl;
	out << (int(100 * session->numCorrect() / session->numQuestions()) >= 100 ? "</tr>" : "<td></td></tr>") << "</table></div></td><td width=\"5%\" align=\"right\"><div class=\"default_text\">" << endl;
	out << int(100 * session->numCorrect() / session->numQuestions()) << "%" << endl;
	out << "</div></td></tr></table><br><div class=\"bold_text\">" << endl;
	out << tr("Students:") << endl << "</div><br>" << endl;
	for (int i = 0; i < session->numStudents(); ++i) {
		out << "<table border=\"0\" width=\"100%\"><tr><td width=\"40%\"><div class=\"" << (session->student(i)->passed() ? "student_passed" : "student_failed") << "\">" << endl;
		out << substituteHtmlTags(session->student(i)->name()) << endl;
		out << "</div></td><td width=\"20%\"><div class=\"default_text\">" << endl;
		if (session->student(i)->isReady()) {
            out << tr("%1 out of %2 (%3)").arg(session->student(i)->score()).arg(session->student(i)->results()->count()).arg(session->student(i)->passed() ? tr("PASSED") : tr("FAILED")) << endl;
        } else {
            out << tr("NOT AVAILABLE") << endl;
        }
		out << "</div></td><td><div align=\"right\">" << endl;
		if (session->student(i)->isReady()) {
            out << "<table border=\"2\" width=\"100%\" height=\"20\"><tr><td bgcolor=\"";
			out << (session->student(i)->passed() ? "yellowgreen" : "#FF9419");
			out << "\" width=\"" << int(100 * session->student(i)->score() / session->student(i)->results()->count()) << "%\"></td>" << endl;
            out << (int(100 * session->student(i)->score() / session->student(i)->results()->count()) >= 100 ? "</tr>" : "<td></td></tr>") << "</table></div></td><td width=\"5%\" align=\"right\"><div class=\"default_text\">" << endl;
            out << int(100 * session->student(i)->score() / session->student(i)->results()->count()) << "%" << endl;
        } else {
            out << "<table border=\"2\" width=\"100%\" height=\"20\"><tr><td></td></tr></table></div></td><td width=\"5%\" align=\"right\"><div class=\"default_text\">" << endl << tr("N/A") << endl;
        }
		out << "</div></td></tr></table>" << endl;
	}
	out << "</p></body></html>" << endl;
	doc.setHtml(html); printer->setDocName(header); doc.print(printer);
	return true;
}

QString MainWindow::substituteHtmlTags(QString str)
{
    str.replace(QString("<"), QString("&lt;"));
    str.replace(QString(">"), QString("&gt;"));
//  str.replace(QString("\n"), QString("<br>"));
	str.replace(QString("&"), QString("&amp;"));
    return str;
}
