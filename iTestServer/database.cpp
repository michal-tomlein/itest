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

#include "main_window.h"

bool MainWindow::saveChangesBeforeProceeding(const QString & title, bool close)
{
	if (current_db_open && this->isWindowModified()) {
        QMessageBox message(this);
        message.setWindowTitle(title);
        message.setWindowModality(Qt::WindowModal);
        message.setWindowFlags(message.windowFlags() | Qt::Sheet);
        message.setIcon(QMessageBox::Information);
        message.setText(tr("The database has been modified."));
        message.setInformativeText(tr("Do you want to save your changes?"));
        message.addButton(tr("&Save"), QMessageBox::AcceptRole);
        message.addButton(tr("&Discard"), QMessageBox::DestructiveRole);
        message.addButton(tr("Cancel"), QMessageBox::RejectRole);
        switch (message.exec()) {
			case 0: // Save
				current_db_open = false;
				save(); if (close) { closeDB(); } return false;
				break;
			case 1: // Discard
				current_db_open = false;
				if (close) { closeDB(); } return false;
				break;
			case 2: // Cancel
				return true;
				break;
		}
	} else if (current_db_open && (!this->isWindowModified())) {
		if (close) { closeDB(); } return false;
	}
	return false;
}

void MainWindow::newDB()
{
    this->setEnabled(false);
    // Save changes before proceeding?
    bool cancelled = saveChangesBeforeProceeding(tr("New database"), true);
    if (cancelled) { this->setEnabled(true); return; }
    // Creating database -------------------------------------------------------
    bool ok;
    QString db_name = QInputDialog::getText(this, tr("New database"), tr("Database name:"), QLineEdit::Normal, tr("Untitled database"), &ok);
    QString saveDBName;
    if (!ok) { this->setEnabled(true); return; }
    if (!db_name.isEmpty()) {
		saveDBName = QFileDialog::getSaveFileName(this, tr("Create database file"), tr("%1.itdb").arg(db_name), tr("iTest databases (*.itdb)"));
    } else {
		saveDBName = QFileDialog::getSaveFileName(this, tr("Create database file"), tr("untitled.itdb"), tr("iTest databases (*.itdb)"));
    }
	if (saveDBName.isNull() || saveDBName.isEmpty()) { this->setEnabled(true); return; }
    QFile file(saveDBName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) 
    {
		QMessageBox::critical(this, tr("Create database file"), tr("Cannot write file %1:\n%2.").arg(saveDBName).arg(file.errorString()));
		this->setWindowTitle(tr("iTestServer"));
		this->setEnabled(true); return;
    }
    clearAll();
	setProgress(10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    QTextStream sfile(&file);
	sfile.setCodec("UTF-8");
	sfile << "[ITEST_VERSION]\n" << f_ver << endl;
	sfile << "[ITEST_DB_VERSION]\n" << f_itdb_ver << endl;
	sfile << "[DB_NAME]\n" << db_name << endl;
	sfile << "[DB_DATE]\n" << endl;
	sfile << "[DB_DATE_ULSD]\ntrue" << endl;
	sfile << "[DB_COMMENTS]\n" << endl;
	sfile << "[DB_QNUM]\n0" << endl;
	sfile << "[DB_SNUM]\n0" << endl;
	sfile << "[DB_CNUM]\n0" << endl;
	sfile << "[DB_FLAGS]" << endl;
	sfile << "--------------------\n";
	for (int i = 0; i < current_db_f.size(); ++i) { sfile << "[DB_F" << i << "]\n" << endl; }
	sfile << "[DB_FLAGS_END]";
	setProgress(50); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // APPLY
    setAllEnabled(true);
    DBIDatabaseNameLineEdit->setText( db_name );
    current_db_file = saveDBName;
    current_db_name = db_name;
    current_db_open = true;
    addRecent(saveDBName);
#ifdef Q_WS_MAC
	this->setWindowTitle(QString("%1[*]").arg(current_db_name));
#else
    this->setWindowTitle(QString("%1[*] - iTestServer").arg(current_db_name));
#endif
    this->setWindowModified(false);
    statusBar()->showMessage(tr("Ready"), 10000);
    setProgress(100); setProgress(-1); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    actionEdit_questions->setChecked(true);
    actionUse_last_save_date->setChecked(true);
    DBIUseLastSaveDateCheckBox->setChecked(true);
    DBIDateEdit->setDateTime(QDateTime::currentDateTime());
    DBIDateEdit->setEnabled(false);
	setPage(actionEdit_questions);
    this->setEnabled(true);
    // -------------------------------------------------------------------------
}

void MainWindow::save() { saveDB(current_db_file); }

void MainWindow::saveAs()
{
	QString db_name = DBIDatabaseNameLineEdit->text();
	QString saveDBName = QFileDialog::getSaveFileName(this, tr("Save database"), QString("%1.itdb").arg(db_name), tr("iTest databases (*.itdb)"));
	if (!saveDBName.isNull() || !saveDBName.isEmpty()) { addRecent(saveDBName); saveDB(saveDBName); }
}

void MainWindow::saveCopy()
{
	if (this->isWindowModified()) {
		switch (QMessageBox::information(this, tr("Save a copy"), tr("It is necessary to save any changes you have made to the database before proceeding."), tr("&Save"), tr("Cancel"), 0, 1)) {
			case 0: // Save
				save(); break;
			case 1: // Cancel
				return; break;
		}
	}
	QString db_name = DBIDatabaseNameLineEdit->text();
	QString saveDBName = QFileDialog::getSaveFileName(this, tr("Save a copy"), QString("%1.itdb").arg(db_name), tr("iTest databases (*.itdb)"));
	if (!saveDBName.isNull() || !saveDBName.isEmpty())
		{ addRecent(saveDBName); saveDB(saveDBName, true, false); }
}

void MainWindow::saveBackup()
{
	if (this->isWindowModified()) {
		switch (QMessageBox::information(this, tr("Save a backup"), tr("It is necessary to save any changes you have made to the database before proceeding."), tr("&Save"), tr("Cancel"), 0, 1)) {
			case 0: // Save
				save(); break;
			case 1: // Cancel
				return; break;
		}
	}
	QString db_name = DBIDatabaseNameLineEdit->text();
	QString saveDBName = QFileDialog::getSaveFileName(this, tr("Save a backup"), QString("%1.itdb").arg(db_name), tr("iTest databases (*.itdb)"));
	if (!saveDBName.isNull() || !saveDBName.isEmpty())
		{ addRecent(saveDBName); saveDB(saveDBName, true, true); }
}

void MainWindow::saveDB(const QString & db_file_name, bool savecopy, bool allsessions)
{
    this->setEnabled(false); qApp->processEvents();
	// Prepare
	if (LQListWidget->currentIndex().isValid()) {
		applyQuestionChanges();
	}
    // Gather info
    QString db_name = DBIDatabaseNameLineEdit->text();
	if (db_name != current_db_name) {
		QSettings archive(QSettings::IniFormat, QSettings::UserScope, "Michal Tomlein", "iTest");
		QStringList dbs, sns; QString buffer;
		switch (QMessageBox::information(this, tr("iTestServer"), tr("Are you sure you want to change the database name?\nIf you do so, any archived sessions associated to this database\non other computers will not load, unless you change it back.\nThis computer's archive will be updated."), tr("&Change"), tr("Do &not change"), 0, 1)) {
			case 0: // Change
				dbs = archive.value("databases").toStringList();
				if (!dbs.contains(current_db_name)) { break; }
				dbs.removeAll(current_db_name);
				dbs << db_name;
				archive.setValue("databases", dbs);
				sns = archive.value(QString("%1/sessions").arg(current_db_name)).toStringList();
				archive.setValue(QString("%1/sessions").arg(db_name), sns);
				for (int i = 0; i < sns.count(); ++i) {
					buffer = archive.value(QString("%1/%2").arg(current_db_name).arg(sns.at(i))).toString();
					archive.setValue(QString("%1/%2").arg(db_name).arg(sns.at(i)), buffer);
				}
				archive.remove(current_db_name);
				break;
			case 1: // Do not change
				db_name = current_db_name;
				DBIDatabaseNameLineEdit->setText(current_db_name);
				break;
		}
	}
    QString use_last_save_date; QString db_date;
    if (actionUse_last_save_date->isChecked()) {
		db_date = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm");
		use_last_save_date = "true";
    } else {
		db_date = DBIDateEdit->dateTime().toString("yyyy.MM.dd-hh:mm");
		use_last_save_date = "false";
    }
    QString db_comments = removeLineBreaks(ECTextEdit->toHtml());
	QMap<QDateTime, Session *> sessions = current_db_sessions;
	if (allsessions) {
		QMapIterator<QDateTime, ArchivedSession *> a(current_db_archivedsessions);
		while (a.hasNext()) { a.next();
			if (!sessions.contains(a.key())) {
				sessions.insert(a.key(), a.value());
			}
		}
	}
	uint db_snum = (uint)sessions.size();
    // Save database -----------------------------------------------------------
    QFile file(db_file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::critical(this, tr("Save database"), tr("Cannot write file %1:\n%2.").arg(db_file_name).arg(file.errorString()));
		this->setEnabled(true); return;
    }
    setProgress(5); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    QTextStream sfile(&file);
	sfile.setCodec("UTF-8");
    sfile << "[ITEST_VERSION]\n" << f_ver << endl;
    sfile << "[ITEST_DB_VERSION]\n" << f_itdb_ver << endl;
    sfile << "[DB_NAME]\n" << db_name << endl;
    sfile << "[DB_DATE]\n" << db_date << endl;
    sfile << "[DB_DATE_ULSD]\n" << use_last_save_date << endl;
    sfile << "[DB_COMMENTS]\n" << db_comments << endl;
    sfile << "[DB_QNUM]\n" << current_db_questions.size() << endl;
    sfile << "[DB_SNUM]\n" << db_snum << endl;
    sfile << "[DB_CNUM]\n" << current_db_classes.size() << endl;
    sfile << "[DB_FLAGS]" << endl;
    int num_flags = current_db_f.size();
    if (num_flags > 20 && !current_db_fe[num_flags - 1]) num_flags--;
    for (int i = 0; i < num_flags; ++i) { sfile << (current_db_fe[i] ? "+" : "-"); }
    sfile << endl;
    for (int i = 0; i < num_flags; ++i) {
        sfile << "[DB_F" << i << "]\n" << current_db_f[i] << endl;
    }
    sfile << "[DB_FLAGS_END]" << endl;
    setProgress(10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    uint count = (uint)current_db_questions.size() + (uint)sessions.size();
    uint n = current_db_questions.size();
    for (int i = 0; i < LQListWidget->count(); ++i) {
		sfile << current_db_questions.value(LQListWidget->item(i))->allProperties() << endl;
		setProgress((90/(i+1)*count)+10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>
    }
    QMapIterator<QDateTime, Session *> i(sessions);
    while (i.hasNext()) { i.next(); n++;
		sfile << i.value()->sessionData() << endl;
		for (int s = 0; s < i.value()->numStudents(); ++s) {
            sfile << i.value()->student(s)->studentData() << endl;
        }
		setProgress((90/n*count)+10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    }
    QMapIterator<QListWidgetItem *, Class *> c(current_db_classes);
    while (c.hasNext()) { c.next();
		sfile << c.value()->classData() << endl;
    }
    // -------------------------------------------------------------------------
    while (!current_db_queued_sessions.isEmpty()) {
        ArchivedSession * session = current_db_queued_sessions.dequeue();
        if (session->status() == ArchivedSession::Remove) {
            session->removeFromArchive();
            session->destruct();
            delete session;
        } else if (session->status() == ArchivedSession::Archive) {
            session->archive();
            session->setStatus(ArchivedSession::Default);
        }
    }
    // -------------------------------------------------------------------------
	if (!savecopy) {
		current_db_name = db_name;
		current_db_date = db_date;
		current_db_comments = db_comments;
		current_db_file = db_file_name;
		if (actionUse_last_save_date->isChecked()) {
			DBIDateEdit->setDateTime(QDateTime::fromString(current_db_date, "yyyy.MM.dd-hh:mm"));
		}
	}
#ifdef Q_WS_MAC
	this->setWindowTitle(QString("%1[*]").arg(current_db_name));
#else
    this->setWindowTitle(QString("%1[*] - iTestServer").arg(current_db_name));
#endif
	this->setWindowModified(false);
	statusBar()->showMessage(tr("Database saved"), 10000);
	setProgress(100); setProgress(-1); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// -------------------------------------------------------------------------
	this->setEnabled(true);
}

void MainWindow::open()
{
    // Save changes before proceeding?
    bool cancelled = saveChangesBeforeProceeding(tr("Open database"), true);
    if (!cancelled) {
    // Opening database --------------------------------------------------------
		QString openDBName = QFileDialog::getOpenFileName(this, tr("Open database file"), "", tr("iTest databases (*.itdb);;All files (*.*)"));
		if (!openDBName.isNull()) { addRecent(openDBName); openDB(openDBName); }
    }
}

void MainWindow::openRecent()
{
    if (recentDBsListWidget->currentIndex().isValid()) {
		addRecent(recentDBsListWidget->currentItem()->text());
		openDB(recentDBsListWidget->currentItem()->text());
    }
}

void MainWindow::openRecent(QListWidgetItem * item)
{
    QString buffer = item->text();
    addRecent(buffer);
    openDB(buffer);
}

void MainWindow::openDB(const QString & openDBName, bool useCP1250)
{
    if (openDBName.isNull()) { return; }
    this->setEnabled(false); qApp->processEvents();
    try {
		QFile file(openDBName);
	    if (!file.open(QFile::ReadOnly | QFile::Text))
	    {
			QMessageBox::critical(this, tr("Open database"), tr("Cannot read file %1:\n%2.").arg(openDBName).arg(file.errorString()));
			this->setEnabled(true); return;
	    }
	    setProgress(3); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	    QTextStream rfile(&file);
		if (useCP1250) { rfile.setCodec("CP 1250"); }
		else { rfile.setCodec("UTF-8"); }

	    QString db_buffer; QStringList bufferlist;
	    // ---------------------------------------------------------------------
		if (rfile.readLine() != "[ITEST_VERSION]") { throw xInvalidDBFile(0); }
		double version = rfile.readLine().toDouble();
    	if (rfile.readLine() != "[ITEST_DB_VERSION]") { throw xInvalidDBFile(1); }
    	double db_version = rfile.readLine().toDouble();
    	if ((version > f_ver) && (db_version == f_itdb_ver))
    	{ QMessageBox::information(this, tr("iTest version notice"), tr("There is a newer version of iTest available.\nNonetheless, this version is able to open the database file you selected,\nbut you are most probably missing a whole bunch of cool new features.")); }
    	if ((version > f_ver) && (db_version > f_itdb_ver))
    	{ QMessageBox::critical(this, tr("iTest version notice"), tr("You need a newer version of iTest to open this database file.")); this->setEnabled(true); return; }
		if (!useCP1250) {
			if (db_version == 1.0) { openDB(openDBName, true); return; }
		}
	
	    if (rfile.readLine() != "[DB_NAME]") { throw xInvalidDBFile(10); }
	    // Database name
	    QString db_name = rfile.readLine();
	    if (rfile.readLine() != "[DB_DATE]") { throw xInvalidDBFile(12); }
	    // Database date
	    QString db_date = rfile.readLine();
   		if (rfile.readLine() != "[DB_DATE_ULSD]") { throw xInvalidDBFile(14); }
		// Use last save date
		bool db_ulsd = (rfile.readLine() == "true");
    	if (rfile.readLine() != "[DB_COMMENTS]") { throw xInvalidDBFile(16); }
    	// Database comments
    	QString db_comments = rfile.readLine();
    	if (rfile.readLine() != "[DB_QNUM]") { throw xInvalidDBFile(18); }
    	// Question number
    	int db_qnum = rfile.readLine().toInt();
    	if (rfile.readLine() != "[DB_SNUM]") { throw xInvalidDBFile(20); }
    	// Number of saved sessions
    	int db_snum = rfile.readLine().toInt();
        int db_cnum = 0;
    	if (db_version >= 1.35) {
    	    if (rfile.readLine() != "[DB_CNUM]") { throw xInvalidDBFile(22); }
    	    // Number of classes
    	    db_cnum = rfile.readLine().toInt();
    	}
    	if (rfile.readLine() != "[DB_FLAGS]") { throw xInvalidDBFile(50); }
    	setProgress(6); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    	// Flags enabled
    	db_buffer = rfile.readLine();
        QVector<bool> db_fe(db_buffer.length());
    	for (int i = 0; i < db_fe.size(); ++i) {
			if (db_buffer.at(i) == '+') { db_fe[i] = true; } else if (db_buffer.at(i) == '-')
			{ db_fe[i] = false; } else { throw xInvalidDBFile(52); }
    	}
    	// Flags
        QVector<QString> db_f(db_buffer.length());
    	for (int i = 0; i < db_f.size(); ++i) {
			if (rfile.readLine() != QString("[DB_F%1]").arg(i)) { throw xInvalidDBFile(54); }
			db_f[i] = rfile.readLine();
    	}
    	// End of flags
    	if (rfile.readLine() != "[DB_FLAGS_END]") { throw xInvalidDBFile(59); }
    	setProgress(10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		int count = db_qnum + db_snum;
    	// Questions
        QuestionItem * item; QStringList answers;
    	for (int i = 0; i < db_qnum; ++i) {
			answers.clear();
			// Question name
			if (rfile.readLine() != "[Q_NAME]") { throw xInvalidDBFile(100); }
			item = new QuestionItem (rfile.readLine());
			// Flag
			if (rfile.readLine() != "[Q_FLAG]") { throw xInvalidDBFile(102); }
			item->setFlag(rfile.readLine().toInt());
			if (db_version >= 1.2) {
				// Question group
				if (rfile.readLine() != "[Q_GRP]") { throw xInvalidDBFile(104); }
				item->setGroup(rfile.readLine());
			}
			// Difficulty
			if (db_version >= 1.2) { if (rfile.readLine() != "[Q_DIF]") { throw xInvalidDBFile(106); } }
			else { if (rfile.readLine() != "[Q_DIFFICULTY]") { throw xInvalidDBFile(108); } }
			item->setDifficulty(rfile.readLine().toInt());
			// Question text
			if (rfile.readLine() != "[Q_TEXT]") { throw xInvalidDBFile(110); }
			item->setText(rfile.readLine());
            if (db_version >= 1.35) {
                // Answers
                if (rfile.readLine() != "[Q_ANS]") { throw xInvalidDBFile(112); }
                item->setSelectionType((Question::SelectionType)rfile.readLine().toInt());
                item->setCorrectAnswers((Question::Answer)rfile.readLine().toInt());
                int numanswers = rfile.readLine().toInt();
				for (int a = 0; a < numanswers; ++a) { answers << rfile.readLine(); }
                // Explanation
                if (rfile.readLine() != "[Q_EXPL]") { throw xInvalidDBFile(114); }
                item->setExplanation(rfile.readLine());
            } else {
                // Answer A
                if (rfile.readLine() != "[Q_ANSA]") { throw xInvalidDBFile(116); }
                answers << rfile.readLine();
                if (db_version < 1.2) { if (rfile.readLine() != "[Q_ANSA_C]") { throw xInvalidDBFile(118); } }
                item->setAnswerCorrect(Question::A, rfile.readLine() == "true");
                // Answer B
                if (rfile.readLine() != "[Q_ANSB]") { throw xInvalidDBFile(120); }
                answers << rfile.readLine();
                if (db_version < 1.2) { if (rfile.readLine() != "[Q_ANSB_C]") { throw xInvalidDBFile(122); } }
                item->setAnswerCorrect(Question::B, rfile.readLine() == "true");
                // Answer C
                if (rfile.readLine() != "[Q_ANSC]") { throw xInvalidDBFile(124); }
                answers << rfile.readLine();
                if (db_version < 1.2) { if (rfile.readLine() != "[Q_ANSC_C]") { throw xInvalidDBFile(126); } }
                item->setAnswerCorrect(Question::C, rfile.readLine() == "true");
                // Answer D
                if (rfile.readLine() != "[Q_ANSD]") { throw xInvalidDBFile(128); }
                answers << rfile.readLine();
                if (db_version < 1.2) { if (rfile.readLine() != "[Q_ANSD_C]") { throw xInvalidDBFile(130); } }
                item->setAnswerCorrect(Question::D, rfile.readLine() == "true");
            }
			// Statistics
			if (db_version < 1.2) { if (rfile.readLine() != "[Q_ICNT]") { throw xInvalidDBFile(132); } }
			else { if (rfile.readLine() != "[Q_ICCNT]") { throw xInvalidDBFile(134); } }
			item->setIncorrectAnsCount(rfile.readLine().toUInt());
			if (db_version < 1.2) { if (rfile.readLine() != "[Q_CCNT]") { throw xInvalidDBFile(136); } }
			item->setCorrectAnsCount(rfile.readLine().toUInt());
			// Hidden
			if (db_version >= 1.2) {
				if (rfile.readLine() != "[Q_HID]") { throw xInvalidDBFile(138); }
				item->setHidden(rfile.readLine() == "true");
			}
			if (db_version > 1.25) {
				// SVG
				if (rfile.readLine() != "[Q_SVG]") { throw xInvalidDBFile(140); }
				int numsvgitems = rfile.readLine().toInt();
				for (int g = 0; g < numsvgitems; ++g) {
				    db_buffer = rfile.readLine();
				    item->addSvgItem(new SvgItem(db_buffer, rfile.readLine()));
				}
			}
			// End
			if (db_version < 1.25) {
			    if (rfile.readLine() != "[Q_END]") { throw xInvalidDBFile(199); }
			}
			// Add map entry
			item->setAnswers(answers);
			QListWidgetItem * q_item = new QListWidgetItem (item->group().isEmpty() ? item->name() : QString("[%1] %2").arg(item->group()).arg(item->name()));
			LQListWidget->addItem(q_item);
			current_db_questions.insert(q_item, item);
			setQuestionItemIcon(q_item, item->difficulty());
			setQuestionItemColour(q_item, item->flag());
			hideQuestion(q_item, item);
			setProgress((85/(i+1)*count)+10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>
	    }
		// Saved sessions
        int ans_flag = -1; int ans_dif = 0;
        Question::Answer c_ans; Question::Answer ans;
        Question::SelectionType ans_selectiontype = Question::SingleSelection;
	    for (int i = 0; i < db_snum; ++i) {
			if (rfile.atEnd()) { break; }
			if (rfile.readLine() != "[SESSION]") { continue; }
			Session * session = new Session;
	        session->setName(rfile.readLine());
			session->setDateTimeFromString(rfile.readLine());
			if (db_version >= 1.2) {
				if (rfile.readLine() != "[PASSMARK]") { throw xInvalidDBFile(202); }
				PassMark pm(rfile.readLine().toInt());
				int pm_count = rfile.readLine().toInt(); int pm_c, pm_v;
				for (int i = 0; i < pm_count; ++i) {
					pm_c = rfile.readLine().toInt();
                    pm_v = rfile.readLine().toInt();
					pm.addCondition(pm_c, pm_v, pm_v);
				}
				session->setPassMark(pm);
			} else {
				session->setPassMark(PassMark(rfile.readLine().toInt()));
			}
            ScoringSystem sys;
            if (db_version >= 1.35) {
                db_buffer = rfile.readLine(); db_buffer.append("\n");
                db_buffer.append(rfile.readLine()); db_buffer.append("\n");
                db_buffer.append(rfile.readLine());
                sys.loadData(db_buffer);
            }
			int s_snum = rfile.readLine().toInt();
			int s_lenum = rfile.readLine().toInt();
			for (int le = 0; le < s_lenum; ++le) {
				bufferlist.clear();
				bufferlist = rfile.readLine().split(';');
				if (bufferlist.count() == 6) {
					session->addLogEntry(bufferlist.at(0).toInt(),
										bufferlist.at(1).toInt(),
										bufferlist.at(2).toInt(),
										bufferlist.at(3).toInt(),
										bufferlist.at(4).toInt(),
										bufferlist.at(5).toInt(),
										rfile.readLine());
				} else {
					session->addLogEntry(255, 255, 255, 0, 0, 0, rfile.readLine());
				}
			}
			for (int s = 0; s < s_snum; ++s) {
				if (rfile.atEnd()) { break; }
				if (rfile.readLine() != "[STUDENT]") { continue; }
				Student * student = new Student(rfile.readLine());
				student->setReady(rfile.readLine() == "true");
				if (db_version >= 1.2) {
					student->setPassed(rfile.readLine() == "true");
				}
				student->setNumber(rfile.readLine().toInt());
                if (db_version < 1.35) { rfile.readLine(); } // SCORE
				int numresults = rfile.readLine().toInt();
				QMap<QString, QuestionAnswer> * results = new QMap<QString, QuestionAnswer>;
				for (int a = 0; a < numresults; ++a) {
					db_buffer = rfile.readLine();
					if (db_version >= 1.2) {
						ans_flag = rfile.readLine().toInt();
                    }
                    if (db_version >= 1.35) {
                        ans_dif = rfile.readLine().toInt();
                        ans_selectiontype = (Question::SelectionType)rfile.readLine().toInt();
                    } else {
						QuestionItem * item = NULL;
						QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
						while (q.hasNext()) { q.next();
							if (q.value()->name() == db_buffer) { item = q.value(); break; }
						}
						if (item == NULL) {
							if (db_version < 1.2) { ans_flag = -1; }
                            if (db_version < 1.35) { ans_dif = 0; ans_selectiontype = Question::SingleSelection; }
						} else {
							if (db_version < 1.2) { ans_flag = item->flag(); }
                            if (db_version < 1.35) { ans_dif = item->difficulty(); ans_selectiontype = item->selectionType(); }
						}
					}
					if (db_version < 1.27) {
					    ans = Question::convertOldAnsNumber(rfile.readLine().toInt());
					    c_ans = Question::convertOldAnsNumber(rfile.readLine().toInt());
					} else {
					    ans = (Question::Answer)rfile.readLine().toInt();
					    c_ans = (Question::Answer)rfile.readLine().toInt();
					}
					QuestionAnswer qans(c_ans, ans, ans_flag, ans_dif, ans_selectiontype);
					results->insert(db_buffer, qans);
				}
				student->setResults(results);
                student->updateScore(sys);
				if (db_version < 1.2) {
					student->setPassed(session->passMark().check(student->results(), &current_db_questions, sys));
				}
				session->addStudent(student);
			}
            session->setScoringSystem(sys);
			current_db_sessions.insert(session->dateTime(), session);
			QListWidgetItem * item = new QListWidgetItem (QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name()));
			SVLSListWidget->insertItem(0, item);
        	item->setData(Qt::UserRole, session->dateTime());
			setProgress((85/(db_qnum+i+1)*count)+10); // PROGRESS >>>>>>>>>>>>>>
    	}
    	// Classes
    	for (int i = 0; i < db_cnum; ++i) {
            if (rfile.atEnd()) { break; }
            if (rfile.readLine() != "[CLASS]") { continue; }
            Class * cl = new Class(rfile.readLine());
            bufferlist = rfile.readLine().split("-", QString::SkipEmptyParts);
            cl->setFirstYear(bufferlist.at(0).toInt());
            cl->setLastYear(bufferlist.at(1).toInt());
            QStringList cl_sessions = rfile.readLine().split(";", QString::SkipEmptyParts);
            for (int s = 0; s < cl_sessions.count(); ++s) {
                cl->addSession(cl_sessions.at(s));
            }
            int mnum = rfile.readLine().toInt();
            for (int m = 0; m < mnum; ++m) {
                if (rfile.readLine() != "[MEM]") { continue; }
                ClassMember * mem = new ClassMember(rfile.readLine());
                int msnum = rfile.readLine().toInt();
                for (int ms = 0; ms < msnum; ++ms) {
                    db_buffer = rfile.readLine();
                    mem->addSession(db_buffer, rfile.readLine().toInt());
                }
                cl->addMember(mem);
            }
            QListWidgetItem * cl_item = new QListWidgetItem(QString("%1-%2: %3").arg(cl->firstYear()).arg(cl->lastYear()).arg(cl->name()), CLLCListWidget);
            current_db_classes.insert(cl_item, cl);
    	}
    	// Set texts - DBI section
    	DBIDatabaseNameLineEdit->setText( db_name );
    	DBIDateEdit->setDateTime( QDateTime::fromString(db_date, "yyyy.MM.dd-hh:mm") );
        DBIUseLastSaveDateCheckBox->setChecked(db_ulsd);
        actionUse_last_save_date->setChecked(db_ulsd);
        DBIDateEdit->setEnabled(!db_ulsd);
    	ECTextEdit->setHtml( db_comments );
	    // Set flags
        current_db_fe = db_fe;
        current_db_f = db_f;
	    setProgress(97); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	    // Apply flags
	   	setFlags(); loadFlags();
	   	setProgress(98); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	    // Enable All
	    setAllEnabled(true);
	    setProgress(99); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	    // Save values
	    current_db_file = openDBName;
	    current_db_name = db_name;
	    current_db_date = db_date;
	    current_db_comments = db_comments;
	    current_db_open = true;
#ifdef Q_WS_MAC
        this->setWindowTitle(QString("%1[*]").arg(current_db_name));
#else
        this->setWindowTitle(QString("%1[*] - iTestServer").arg(current_db_name));
#endif
	    this->setWindowModified(false);
	    statusBar()->showMessage(tr("Database open"), 10000);
	    setProgress(100); setProgress(-1); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>
	    actionEdit_questions->setChecked(true);
		mainStackedWidget->setCurrentIndex(1);
	    // ---------------------------------------------------------------------
	    this->setEnabled(true);
		// Load archived sessions
		openArchive();
	}
	catch (xInvalidDBFile e) {
		errorInvalidDBFile(tr("Open database"), openDBName, e.error());
	}
	catch (...) {
		QMessageBox::critical(this, tr("iTestServer"), tr("Error opening database."));
		clearAll(); setAllEnabled(false); current_db_open = false;
		this->setWindowTitle(tr("iTestServer"));
		this->setWindowModified(false); setProgress(-1);
		mainStackedWidget->setCurrentIndex(0);
		this->setEnabled(true);
	}
}

void MainWindow::closeDB()
{
	// Save changes before proceeding?
	if (saveChangesBeforeProceeding(tr("Close database"), false)) { return; }
	// Closing database --------------------------------------------------------
	clearAll(); setAllEnabled(false); current_db_open = false;
	this->setWindowTitle(tr("iTestServer"));
	this->setWindowModified(false);
	setProgress(100); setProgress(-1); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	mainStackedWidget->setCurrentIndex(0);
	statusBar()->showMessage(tr("Database closed"), 10000);
	// -------------------------------------------------------------------------
}
