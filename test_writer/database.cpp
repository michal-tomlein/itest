#include "main_window.h"

void MainWindow::browse_i()
{
     DBPathLineEdit->setText(QFileDialog::getOpenFileName(this, tr("Open database file"), "", tr("iTest off-line test sessions (*.itos);;All files (*.*)")));
}

void MainWindow::browse_o()
{
     savePathLineEdit->setText(QFileDialog::getSaveFileName(this, tr("Output file path"), tr("answers.itcl"), tr("iTest off-line client log (*.itcl)")));
}

void MainWindow::loadTest(QString input)
{
     QProgressDialog progress (this);
     progress.setLabelText(tr("Loading test data..."));
     progress.setMinimum(0);
     progress.setMinimumDuration(0);
     progress.setWindowModality(Qt::WindowModal);
     
     QTextStream in(&input);
     
     QString db_buffer; float version; float db_version; QString db_name;
     QString db_date; QString db_comments; int db_qnum; bool db_fxxe[20];
     QString db_f[20]; QString q_file_name; QString db_ulsd; QString test_date;
     QuestionItem * item; QStringList answers; int test_qnum; QString test_time;
     // ------------------------------------------------------------------------
     db_buffer = in.readLine();
     if (db_buffer != "[ITEST_VERSION]")
     { errorInvalidData(); return; }
     version = in.readLine().toFloat();
     db_buffer = in.readLine();
     if (db_buffer != "[ITEST_DB_VERSION]")
     { errorInvalidData(); return; }
     db_version = in.readLine().toFloat();
     if ((version > f_ver) && (db_version == f_db_ver))
     { QMessageBox::information(this, tr("iTest version notice"), tr("There is a newer version of iTest available.\nNonetheless, this version is able to open the database file you selected,\nbut you are most probably missing a whole bunch of cool new features.")); }
     if ((version > f_ver) && (db_version > f_db_ver))
     { QMessageBox::critical(this, tr("iTest version notice"), tr("You need a newer version of iTest to open this database file.")); return; }
     
     db_buffer = in.readLine(); if (db_buffer != "[DB_NAME]") { errorInvalidData(); return; }
     // Database name
     db_name = in.readLine();
     db_buffer = in.readLine(); if (db_buffer != "[DB_DATE]") { errorInvalidData(); return; }
     // Database date
     db_date = in.readLine();
     db_buffer = in.readLine(); if (db_buffer != "[DB_DATE_ULSD]") { errorInvalidData(); return; }
     // Use last save date
     db_ulsd = in.readLine();
     db_buffer = in.readLine(); if (db_buffer != "[TEST_DATE]") { errorInvalidData(); return; }
     // Test date
     test_date = in.readLine();
     db_buffer = in.readLine(); if (db_buffer != "[TEST_TIME]") { errorInvalidData(); return; }
     // Test time
     test_time = in.readLine();
     db_buffer = in.readLine(); if (db_buffer != "[TEST_QNUM]") { errorInvalidData(); return; }
     // Test qnum
     test_qnum = in.readLine().toInt();
     db_buffer = in.readLine(); if (db_buffer != "[DB_COMMENTS]") { errorInvalidData(); return; }
     // Database comments
     db_comments = in.readLine();
     db_buffer = in.readLine(); if (db_buffer != "[DB_QNUM]") { errorInvalidData(); return; }
     // Question number
     db_qnum = in.readLine().toInt();
     progress.setMaximum(db_qnum);
     db_buffer = in.readLine(); if (db_buffer != "[DB_FLAGS]") { errorInvalidData(); return; }
     // fxxe
     db_buffer = in.readLine();
     for (int i = 0; i < 20; ++i) {
         if (db_buffer.at(i) == '+') {db_fxxe[i] = true;} else if (db_buffer.at(i) == '-')
         {db_fxxe[i] = false;} else { errorInvalidData(); return; }
     }
     // flags
     for (int i = 0; i < 20; ++i) {
         db_buffer = in.readLine(); if (db_buffer != QString("[DB_F%1]").arg(i)) { errorInvalidData(); return; }
         db_f[i] = in.readLine();
     }
     // End of flags
     db_buffer = in.readLine(); if (db_buffer != "[DB_FLAGS_END]") { errorInvalidData(); return; }
     // Questions
     for (int i = 0; i < db_qnum; ++i) {
         answers.clear();
         // Question name
         db_buffer = in.readLine(); if (db_buffer != "[Q_NAME]") { errorInvalidData(); return; }
         item = new QuestionItem (in.readLine());
         // Flag
         db_buffer = in.readLine(); if (db_buffer != "[Q_FLAG]") { errorInvalidData(); return; }
         item->setFlag(in.readLine().toInt());
         // Difficulty
         db_buffer = in.readLine(); if (db_buffer != "[Q_DIFFICULTY]") { errorInvalidData(); return; }
         item->setDifficulty(in.readLine().toInt());
         // Question text
         db_buffer = in.readLine(); if (db_buffer != "[Q_TEXT]") { errorInvalidData(); return; }
         item->setText(in.readLine());
         // Answer A
         db_buffer = in.readLine(); if (db_buffer != "[Q_ANSA]") { errorInvalidData(); return; }
         answers << in.readLine();
         //db_buffer = in.readLine(); if (db_buffer != "[Q_ANSA_C]") { errorInvalidData(); return; }
         //answers << in.readLine();
         answers << "false";
         // Answer B
         db_buffer = in.readLine(); if (db_buffer != "[Q_ANSB]") { errorInvalidData(); return; }
         answers << in.readLine();
         //db_buffer = in.readLine(); if (db_buffer != "[Q_ANSB_C]") { errorInvalidData(); return; }
         //answers << in.readLine();
         answers << "false";
         // Answer C
         db_buffer = in.readLine(); if (db_buffer != "[Q_ANSC]") { errorInvalidData(); return; }
         answers << in.readLine();
         //db_buffer = in.readLine(); if (db_buffer != "[Q_ANSC_C]") { errorInvalidData(); return; }
         //answers << in.readLine();
         answers << "false";
         // Answer D
         db_buffer = in.readLine(); if (db_buffer != "[Q_ANSD]") { errorInvalidData(); return; }
         answers << in.readLine();
         //db_buffer = in.readLine(); if (db_buffer != "[Q_ANSD_C]") { errorInvalidData(); return; }
         //answers << in.readLine();
         answers << "false";
         // End
         db_buffer = in.readLine(); if (db_buffer != "[Q_END]") { errorInvalidData(); return; }
         // Add map entry
         item->setAnswers(answers);
         current_db_questions << item;
         
         progress.setValue(i); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
         qApp->processEvents();
     }
     // Set flags
     for (int i = 0; i < 20; ++i) {current_db_fe[i] = db_fxxe[i];}
     for (int i = 0; i < 20; ++i) {current_db_f[i] = db_f[i];}
     // Save values
     current_db_name = db_name;
     current_db_date = db_date;
     current_test_date = test_date;
     current_db_comments = db_comments;
     current_test_qnum = test_qnum;
     current_test_time_remaining = 0;
     current_test_time_remaining += (QTime::fromString(test_time, "hh:mm").hour() * 60);
     current_test_time_remaining += QTime::fromString(test_time, "hh:mm").minute();
     // Display info
     ITW_test_name->setText(current_db_name);
     ITW_test_date->setText(current_db_date);
     ITW_test_timestamp->setText(current_test_date);
     ITW_test_time->setText(test_time);
     ITW_test_qnum->setText(QString("%1 of total %2").arg(current_test_qnum).arg(db_qnum));
     ITW_test_comments->setHtml(current_db_comments);
     infoTableWidget->setColumnWidth(0, infoTableWidget->geometry().width() - infoTableWidget->verticalHeader()->length());
     infoTableWidget->setRowHeight(7, 200);
     infoTableWidget->setEnabled(true);
	 
     progress.setValue(db_qnum);
     qApp->processEvents();
     
     tbtnGetReady->setEnabled(true);
     // ------------------------------------------------------------------------
     randomlySelectQuestions();
}

void MainWindow::loadFile()
{
    QString file_name = DBPathLineEdit->text();
    if (file_name.isEmpty()) { return; }
    loadFile(file_name);
}

void MainWindow::loadFile(QString file_name)
{
    QProgressDialog progress(this);
    progress.setLabelText(tr("Reading database file..."));
    progress.setMinimum(0);
    progress.setMaximum(2);
    progress.setMinimumDuration(0);
    progress.setWindowModality(Qt::WindowModal);
    
    if (file_name.isEmpty()) { return; }
    
    progress.setValue(0); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    qApp->processEvents();
    
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this, tr("Open database"), tr("Cannot read file %1:\n%2.").arg(file_name).arg(file.errorString()));
        return;
    }
    QTextStream rfile(&file);
    rfile.setCodec("CP 1250");
    
    progress.setValue(1); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    qApp->processEvents();
    
    client_number = QTime::currentTime().second();
    loadTest(rfile.readAll());
    
    progress.setValue(2); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    qApp->processEvents();
    
    disableInputTypeSelection();
    // -------------------------------------------------------------------------
}

void MainWindow::randomlySelectQuestions()
{
    QProgressDialog progress(this);
    progress.setLabelText(tr("Generating test..."));
    progress.setMinimum(0);
    progress.setMaximum(current_test_qnum);
    progress.setMinimumDuration(0);
    progress.setWindowModality(Qt::WindowModal);
    
    int rand; QList<int> randlist; QStringList flags;
    for (int i = 0; i < current_test_qnum; ++i) {
        do {
            rand = (qrand() + client_number) % current_db_questions.size();
        } while (randlist.contains(rand));
        randlist << rand;
        QListWidgetItem * q_item = new QListWidgetItem;
        if (hideQuestionNamesCheckBox->isChecked()) {
            q_item->setText(QString("%1").arg(LQListWidget->count() + 1));
        } else {
            q_item->setText(current_db_questions.at(rand)->name());
        }
        q_item->setData(Qt::UserRole, current_db_questions.at(rand)->name());
        LQListWidget->addItem(q_item);
        current_test_questions.insert(q_item, current_db_questions.at(rand));
        if ((current_db_questions.at(rand)->flag() >= 0) && (current_db_questions.at(rand)->flag() < 20)) {
        if (!flags.contains(current_db_f[current_db_questions.at(rand)->flag()]))
        { flags << current_db_f[current_db_questions.at(rand)->flag()]; } }
        progress.setValue(i); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        qApp->processEvents();
    }
    ITW_test_fnum->setText(QString("%1").arg(flags.count()));
    ITW_test_flags->setText(flags.join(", "));
}
