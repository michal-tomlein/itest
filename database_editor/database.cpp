#include "main_window.h"

bool MainWindow::saveChangesBeforeProceeding(QString parentFunction, bool close)
{
	if (current_db_open && this->isWindowModified()) {
		switch (QMessageBox::information(this, parentFunction, tr("Save changes before proceeding?"), tr("&Save"), tr("&Discard"), tr("Cancel"), 0, 2)) {
			case 0: // Save
				current_db_open = false;
				save(); if (close) {closeDB();}; return false;
				break;
			case 1: // Discard
				current_db_open = false;
				if (close) {closeDB();}; return false;
				break;
			case 2: // Cancel
				return true;
				break;
		}
	} else if (current_db_open && (!this->isWindowModified())) {
		if (close) {closeDB();}; return false;
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
		this->setWindowTitle(tr("iTest - Database Editor"));
		this->setEnabled(true); return;
    }
	setProgress(10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    QTextStream sfile(&file);
	sfile.setCodec("CP 1250");
	sfile << "[ITEST_VERSION]\n" << f_ver << endl;
	sfile << "[ITEST_DB_VERSION]\n" << f_db_ver << endl;
	sfile << "[DB_NAME]\n" << db_name << endl;
	sfile << "[DB_DATE]\n" << endl;
	sfile << "[DB_DATE_ULSD]\ntrue" << endl;
	sfile << "[DB_COMMENTS]\n" << endl;
	sfile << "[DB_QNUM]\n0" << endl;
	sfile << "[DB_SNUM]\n0" << endl;
	sfile << "[DB_FLAGS]" << endl;
	sfile << "--------------------\n";
	for (int i = 0; i < 20; ++i) {sfile << "[DB_F" << i << "]\n" << endl;}
	sfile << "[DB_FLAGS_END]";
	setProgress(50); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // APPLY
    enableAll(); clearAll();
    DBIDatabaseNameLineEdit->setText( db_name );
    current_db_file = saveDBName;
    current_db_name = db_name;
    current_db_open = true;
    addRecent(saveDBName);
    this->setWindowTitle(tr("%1[*] - iTest - Database Editor").arg(current_db_name));
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

void MainWindow::saveDB(QString db_file_name, bool savecopy, bool allsessions)
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
		switch (QMessageBox::information(this, tr("iTest - Database Editor"), tr("Are you sure you want to change the database name?\nIf you do so, any archived sessions associated to this database\non other computers will not load, unless you change it back.\nThis computer's archive will be updated."), tr("&Change"), tr("Do &not change"), 0, 1)) {
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
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
		QMessageBox::critical(this, tr("Save database"), tr("Cannot write file %1:\n%2.").arg(db_file_name).arg(file.errorString()));
		this->setEnabled(true); return;
    }
    setProgress(5); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    QTextStream sfile(&file); QString q_file_name;
	sfile.setCodec("CP 1250");
    sfile << "[ITEST_VERSION]\n" << f_ver << endl;
    sfile << "[ITEST_DB_VERSION]\n" << f_db_ver << endl;
    sfile << "[DB_NAME]\n" << db_name << endl;
    sfile << "[DB_DATE]\n" << db_date << endl;
    sfile << "[DB_DATE_ULSD]\n" << use_last_save_date << endl;
    sfile << "[DB_COMMENTS]\n" << db_comments << endl;
    sfile << "[DB_QNUM]\n" << current_db_questions.size() << endl;
    sfile << "[DB_SNUM]\n" << db_snum << endl;
    sfile << "[DB_FLAGS]" << endl;
    for (int i = 0; i < 20; ++i)
    {if (current_db_fe[i]) {sfile << "+";} else {sfile << "-";}} sfile << endl;
    for (int i = 0; i < 20; ++i)
    {sfile << "[DB_F" << i << "]" << endl; sfile << current_db_f[i] << endl;}
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
	this->setWindowTitle(tr("%1[*] - iTest - Database Editor").arg(current_db_name));
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

void MainWindow::openDB(QString openDBName)
{
    if (openDBName.isNull()) { return; }
    this->setEnabled(false); qApp->processEvents();
    QFile file(openDBName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
		QMessageBox::critical(this, tr("Open database"), tr("Cannot read file %1:\n%2.").arg(openDBName).arg(file.errorString()));
		this->setEnabled(true); return;
    }
    setProgress(3); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    QTextStream rfile(&file);
	rfile.setCodec("CP 1250");

    QString db_buffer; float version; float db_version; QString db_name;
    QString db_date; QString db_comments; int db_qnum; bool db_fxxe[20];
    QString db_f[20]; QString q_file_name; QString db_ulsd; int s_lenum;
    QuestionItem * item; QStringList answers; int db_snum; int s_snum;
	QStringList bufferlist; QuestionItem::Answer ans;
	QuestionItem::Answer c_ans;
    // -------------------------------------------------------------------------
    db_buffer = rfile.readLine();
    if (db_buffer != "[ITEST_VERSION]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    db_buffer = rfile.readLine();
    version = db_buffer.toFloat();
    db_buffer = rfile.readLine();
    if (db_buffer != "[ITEST_DB_VERSION]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    db_buffer = rfile.readLine();
    db_version = db_buffer.toFloat();
    if ((version > f_ver) && (db_version == f_db_ver))
    { QMessageBox::information(this, tr("iTest version notice"), tr("There is a newer version of iTest available.\nNonetheless, this version is able to open the database file you selected,\nbut you are most probably missing a whole bunch of cool new features.")); }
    if ((version > f_ver) && (db_version > f_db_ver))
    { QMessageBox::critical(this, tr("iTest version notice"), tr("You need a newer version of iTest to open this database file.")); this->setEnabled(true); return; }

    if (rfile.readLine() != "[DB_NAME]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    // Database name
    db_name = rfile.readLine();
    if (rfile.readLine() != "[DB_DATE]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    // Database date
    db_date = rfile.readLine();
    if (rfile.readLine() != "[DB_DATE_ULSD]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    // Use last save date
    db_ulsd = rfile.readLine();
    if (rfile.readLine() != "[DB_COMMENTS]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    // Database comments
    db_comments = rfile.readLine();
    if (rfile.readLine() != "[DB_QNUM]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    // Question number
    db_qnum = rfile.readLine().toInt();
    if (rfile.readLine() != "[DB_SNUM]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    // Number of saved sessions
    db_snum = rfile.readLine().toInt();
    if (rfile.readLine() != "[DB_FLAGS]")
    { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    setProgress(6); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // fxxe
    db_buffer = rfile.readLine();
    for (int i = 0; i < 20; ++i) {
		if (db_buffer.at(i) == '+') {db_fxxe[i] = true;} else if (db_buffer.at(i) == '-')
		{db_fxxe[i] = false;} else { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    }
    // flags
    for (int i = 0; i < 20; ++i) {
		if (rfile.readLine() != QString("[DB_F%1]").arg(i)) { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		db_f[i] = rfile.readLine();
    }
    // End of flags
    if (rfile.readLine() != "[DB_FLAGS_END]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
    setProgress(10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	int count = db_qnum + db_snum;
    // Questions
    for (int i = 0; i < db_qnum; ++i) {
		answers.clear();
		// Question name
		if (rfile.readLine() != "[Q_NAME]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		item = new QuestionItem (rfile.readLine());
		// Flag
		if (rfile.readLine() != "[Q_FLAG]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		item->setFlag(rfile.readLine().toInt());
		// Difficulty
		if (rfile.readLine() != "[Q_DIFFICULTY]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		item->setDifficulty(rfile.readLine().toInt());
		// Question text
		if (rfile.readLine() != "[Q_TEXT]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		item->setText(rfile.readLine());
		// Answer A
		if (rfile.readLine() != "[Q_ANSA]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		if (rfile.readLine() != "[Q_ANSA_C]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		// Answer B
		if (rfile.readLine() != "[Q_ANSB]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		if (rfile.readLine() != "[Q_ANSB_C]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		// Answer C
		if (rfile.readLine() != "[Q_ANSC]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		if (rfile.readLine() != "[Q_ANSC_C]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		// Answer D
		if (rfile.readLine() != "[Q_ANSD]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		if (rfile.readLine() != "[Q_ANSD_C]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		answers << rfile.readLine();
		// Statistics
		if (rfile.readLine() != "[Q_ICNT]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		item->setIncorrectAnsCount(rfile.readLine().toUInt());
		if (rfile.readLine() != "[Q_CCNT]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		item->setCorrectAnsCount(rfile.readLine().toUInt());
		// End
		if (rfile.readLine() != "[Q_END]") { errorInvalidDBFile(tr("Open database"), openDBName); return; }
		// Add map entry
		item->setAnswers(answers);
		QListWidgetItem * q_item = new QListWidgetItem (item->name());
		LQListWidget->addItem(q_item);
		current_db_questions.insert(q_item, item);
		switch (item->difficulty()) {
			case 0: q_item->setIcon(QIcon(QString::fromUtf8(":/images/images/easy.png"))); break;
			case 1: q_item->setIcon(QIcon(QString::fromUtf8(":/images/images/medium.png"))); break;
			case 2: q_item->setIcon(QIcon(QString::fromUtf8(":/images/images/difficult.png"))); break;
			default: q_item->setIcon(QIcon(QString::fromUtf8(":/images/images/easy.png"))); break;
		}
		setQuestionItemColour(q_item, item->flag());
		setProgress((85/(i+1)*count)+10); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>
    }
	// Saved sessions
    for (int i = 0; i < db_snum; ++i) {
		if (rfile.atEnd()) { break; }
		if (rfile.readLine() != "[SESSION]") { continue; }
		Session * session = new Session;
        session->setName(rfile.readLine());
		session->setDateTimeFromString(rfile.readLine());
		session->setPassMark(rfile.readLine().toInt());
		s_snum = rfile.readLine().toInt();
		s_lenum = rfile.readLine().toInt();
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
			Student * student = new Student (rfile.readLine());
			if (rfile.readLine() == "true") { student->setReady(true); }
			else { student->setReady(false); }
			student->setNumber(rfile.readLine().toInt());
			student->setScore(rfile.readLine().toInt());
			int numresults = rfile.readLine().toInt();
			QMap<QString, QuestionAnswer> * results = new QMap<QString, QuestionAnswer>;
			for (int a = 0; a < numresults; ++a) {
				db_buffer = rfile.readLine();
				ans = (QuestionItem::Answer)rfile.readLine().toInt();
				c_ans = (QuestionItem::Answer)rfile.readLine().toInt();
				QuestionAnswer qans (c_ans, ans);
				results->insert(db_buffer, qans);
			}
			student->setResults(results);
			session->addStudent(student);
		}
		current_db_sessions.insert(session->dateTime(), session);
		QListWidgetItem * item = new QListWidgetItem (QString("%1 - %2").arg(session->dateTimeToString()).arg(session->name()));
		VSSLSListWidget->insertItem(0, item);
        item->setData(Qt::UserRole, session->dateTime());
		setProgress((85/(db_qnum+i+1)*count)+10); // PROGRESS >>>>>>>>>>>>>>>>
    }
    // Set texts - DBI section
    DBIDatabaseNameLineEdit->setText( db_name );
    DBIDateEdit->setDateTime( QDateTime::fromString(db_date, "yyyy.MM.dd-hh:mm") );
    if (db_ulsd == "true") {
		DBIUseLastSaveDateCheckBox->setChecked(true);
		actionUse_last_save_date->setChecked(true);
		DBIDateEdit->setEnabled(false);
    } else if (db_ulsd == "false") {
		DBIUseLastSaveDateCheckBox->setChecked(false);
		actionUse_last_save_date->setChecked(false);
		DBIDateEdit->setEnabled(true);
    }
    ECTextEdit->setHtml( db_comments );
    // Set flags
    for (int i = 0; i < 20; ++i) {current_db_fe[i] = db_fxxe[i];}
    for (int i = 0; i < 20; ++i) {current_db_f[i] = db_f[i];}
    setProgress(97); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Apply flags
    setFlags(); loadFlags();
    setProgress(98); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Enable All
    enableAll();
    setProgress(99); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Save values
    current_db_file = openDBName;
    current_db_name = db_name;
    current_db_date = db_date;
    current_db_comments = db_comments;
    current_db_open = true;
    this->setWindowTitle(tr("%1[*] - iTest - Database Editor").arg(current_db_name));
    this->setWindowModified(false);
    statusBar()->showMessage(tr("Database open"), 10000);
    setProgress(100); setProgress(-1); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    actionEdit_questions->setChecked(true);
	mainStackedWidget->setCurrentIndex(1); updateGeometry();
    // -------------------------------------------------------------------------
    this->setEnabled(true);
	// Load archived sessions
	openArchive();
}

void MainWindow::closeDB()
{
	// Save changes before proceeding?
	if (saveChangesBeforeProceeding(tr("Close database"), false)) { return; }
	// Closing database --------------------------------------------------------
	clearAll(); disableAll(); current_db_open = false;
	this->setWindowTitle(tr("iTest - Database Editor"));
	this->setWindowModified(false);
	setProgress(100); setProgress(-1); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	mainStackedWidget->setCurrentIndex(0); updateGeometry();
	statusBar()->showMessage(tr("Database closed"), 10000);
	// -------------------------------------------------------------------------
}
