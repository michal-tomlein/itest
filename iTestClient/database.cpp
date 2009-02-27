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
    QProgressDialog progress(this);
    progress.setLabelText(tr("Loading test data..."));
    progress.setMinimum(0);
    progress.setMinimumDuration(0);
    progress.setWindowModality(Qt::WindowModal);

    QTextStream in(&input);

    QString db_buffer; QStringList db_bufferlist;
    // ------------------------------------------------------------------------
    if (in.readLine() != "[ITEST_VERSION]") { errorInvalidData(); return; }
    double version = in.readLine().toDouble();
    if (in.readLine() != "[ITEST_DB_VERSION]") { errorInvalidData(); return; }
    double db_version = in.readLine().toDouble();
    if ((version > f_ver) && (db_version == f_itos_ver))
    { QMessageBox::information(this, tr("iTest version notice"), tr("There is a newer version of iTest available.\nNonetheless, this version is able to open the database file you selected,\nbut you are most probably missing a whole bunch of cool new features.")); }
    if ((version > f_ver) && (db_version > f_itos_ver))
    { QMessageBox::critical(this, tr("iTest version notice"), tr("You need a newer version of iTest to open this database file.")); return; }
    if (db_version == 1.0) { errorInvalidData(); return; }
    current_db_multiple_ans_support = db_version >= 1.27;
    current_db_itdb1_4_support = db_version >= 1.35;

    if (in.readLine() != "[DB_NAME]") { errorInvalidData(); return; }
    // Database name
    QString db_name = in.readLine();
    if (in.readLine() != "[DB_DATE]") { errorInvalidData(); return; }
    // Database date
    QString db_date = in.readLine();
    if (in.readLine() != "[DB_DATE_ULSD]") { errorInvalidData(); return; }
    // Use last save date
    in.readLine(); //bool db_ulsd = (in.readLine() == "true");
    bool db_use_groups = false;
    if (db_version >= 1.2) {
        if (in.readLine() != "[TEST_GRPS]") { errorInvalidData(); return; }
        // Use groups
        db_use_groups = (in.readLine() == "true");
    }
    bool shuffle_answers = false;
    if (db_version >= 1.41) {
        if (in.readLine() != "[TEST_SHUFFLE_ANS]") { errorInvalidData(); return; }
        // Shuffle answers
        shuffle_answers = (in.readLine() == "true");
        if (in.readLine() != "[TEST_HIDE_QNAMES]") { errorInvalidData(); return; }
        // Hide question names
        hideQuestionNamesCheckBox->setChecked(in.readLine() == "true");
        hideQuestionNamesCheckBox->setEnabled(false);
        if (in.readLine() != "[TEST_HIDE_C_ANS]") { errorInvalidData(); return; }
        // Hide correct answers
        hideCorrectAnswersCheckBox->setChecked(in.readLine() == "true");
        hideCorrectAnswersCheckBox->setEnabled(false);
    }
    if (in.readLine() != "[TEST_DATE]") { errorInvalidData(); return; }
    // Test date
    QString test_date = in.readLine();
    if (in.readLine() != "[TEST_TIME]") { errorInvalidData(); return; }
    // Test time
    QString test_time = in.readLine();
    if (in.readLine() != "[TEST_QNUM]") { errorInvalidData(); return; }
    // Test qnum
    int test_qnum = in.readLine().toInt();
    if (db_version >= 1.35) {
        // Test scoring system
        db_buffer = in.readLine(); db_buffer.append("\n");
        db_buffer.append(in.readLine()); db_buffer.append("\n");
        db_buffer.append(in.readLine());
        ScoringSystem sys(db_buffer);
        QuestionItem::setScoringSystem(sys);
    }
    if (in.readLine() != "[DB_COMMENTS]") { errorInvalidData(); return; }
    // Database comments
    QString db_comments = in.readLine();
    if (in.readLine() != "[DB_QNUM]") { errorInvalidData(); return; }
    // Question number
    int db_qnum = in.readLine().toInt();
    progress.setMaximum(db_qnum);
    if (in.readLine() != "[DB_FLAGS]") { errorInvalidData(); return; }
    // Flags enabled
    db_buffer = in.readLine();
    QVector<bool> db_fe(db_buffer.length());
    for (int i = 0; i < db_fe.size(); ++i) {
        if (db_buffer.at(i) == '+') { db_fe[i] = true; } else if (db_buffer.at(i) == '-')
        { db_fe[i] = false; } else { errorInvalidData(); return; }
    }
    // Flags
    QVector<QString> db_f(db_buffer.length());
    for (int i = 0; i < db_f.size(); ++i) {
        if (in.readLine() != QString("[DB_F%1]").arg(i)) { errorInvalidData(); return; }
        db_f[i] = in.readLine();
    }
    // End of flags
    if (in.readLine() != "[DB_FLAGS_END]") { errorInvalidData(); return; }
    // Pass mark
    if (db_version >= 1.2) {
        if (in.readLine() != "[PASSMARK]") { errorInvalidData(); return; }
        current_test_passmark.setPassMark(in.readLine().toInt());
        int pm_count = in.readLine().toInt(); int pm_c, pm_v;
        for (int i = 0; i < pm_count; ++i) {
            pm_c = in.readLine().toInt();
            pm_v = in.readLine().toInt();
            current_test_passmark.addCondition(pm_c, pm_v, db_version >= 1.35 ? in.readLine().toInt() : pm_v);
        }
    }
    // Questions
    QuestionItem * item; QStringList answers;
    for (int i = 0; i < db_qnum; ++i) {
        answers.clear();
        // Question name
        if (in.readLine() != "[Q_NAME]") { errorInvalidData(); return; }
        item = new QuestionItem (in.readLine());
        // Flag
        if (in.readLine() != "[Q_FLAG]") { errorInvalidData(); return; }
        item->setFlag(in.readLine().toInt());
        if (db_version >= 1.2) {
        	// Group
        	if (in.readLine() != "[Q_GRP]") { errorInvalidData(); return; }
        	item->setGroup(in.readLine());
        }
        // Difficulty
        if (db_version >= 1.2) { if (in.readLine() != "[Q_DIF]") { errorInvalidData(); return; } }
        else { if (in.readLine() != "[Q_DIFFICULTY]") { errorInvalidData(); return; } }
        item->setDifficulty(in.readLine().toInt());
        // Question text
        if (in.readLine() != "[Q_TEXT]") { errorInvalidData(); return; }
        item->setText(in.readLine());
        if (db_version >= 1.35) {
            // Answers
            if (in.readLine() != "[Q_ANS]") { errorInvalidData(); return; }
            item->setSelectionType((Question::SelectionType)in.readLine().toInt());
            int numanswers = in.readLine().toInt();
            for (int a = 0; a < numanswers; ++a) { answers << in.readLine(); }
        } else {
            // Answer A
            if (in.readLine() != "[Q_ANSA]") { errorInvalidData(); return; }
            answers << in.readLine();
            // Answer B
            if (in.readLine() != "[Q_ANSB]") { errorInvalidData(); return; }
            answers << in.readLine();
            // Answer C
            if (in.readLine() != "[Q_ANSC]") { errorInvalidData(); return; }
            answers << in.readLine();
            // Answer D
            if (in.readLine() != "[Q_ANSD]") { errorInvalidData(); return; }
            answers << in.readLine();
        }
        if (db_version > 1.25) {
            // SVG
            if (in.readLine() != "[Q_SVG]") { errorInvalidData(); return; }
            int numsvgitems = in.readLine().toInt();
            for (int g = 0; g < numsvgitems; ++g) {
                db_buffer = in.readLine();
                item->addSvgItem(db_buffer, in.readLine());
            }
        }
        // End
        if (db_version < 1.25) {
            if (in.readLine() != "[Q_END]") { errorInvalidData(); return; }
        }
        // Add map entry
        item->setAnswers(answers);
        current_db_questions << item;

        progress.setValue(i); // PROGRESS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        qApp->processEvents();
    }
    // Set flags
    current_db_fe = db_fe;
    current_db_f = db_f;
    // Save values
    current_db_name = db_name;
    current_db_date = db_date;
    current_test_date = test_date;
    current_db_comments = db_comments;
    current_test_qnum = test_qnum;
    current_test_time_remaining = 0;
    current_test_time_remaining += (QTime::fromString(test_time, "hh:mm").hour() * 60);
    current_test_time_remaining += QTime::fromString(test_time, "hh:mm").minute();
    current_test_use_groups = db_use_groups;
    current_test_shuffle_answers = shuffle_answers;
    // Display info
    ITW_test_name->setText(current_db_name);
    ITW_test_date->setText(current_db_date);
    ITW_test_timestamp->setText(current_test_date);
    ITW_test_time->setText(test_time);
    ITW_test_qnum->setText(tr("%1 of total %2").arg(current_test_qnum).arg(db_qnum));
    QString pm_str = tr("Total");
    pm_str.append(QString(": %1;").arg(current_test_passmark.passMark()));
    for (int i = 0; i < current_test_passmark.count(); ++i) {
        if (current_test_passmark.condition(i) < 0 && current_test_passmark.condition(i) >= current_db_f.size()) { continue; }
        pm_str.append(QString(" %1: %2;").arg(current_db_f[current_test_passmark.condition(i)]).arg(current_test_passmark.value(i)));
    }
    ITW_test_passmark->setText(pm_str);
    ITW_test_comments->setHtml(current_db_comments);
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

void MainWindow::loadFile(const QString & file_name)
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
    rfile.setCodec("UTF-8");

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
    progress.setMinimumDuration(0);
    progress.setWindowModality(Qt::WindowModal);

    QList<Question *> questions;
    for (int i = 0; i < current_db_questions.count(); ++i) { questions << current_db_questions.at(i); }
    QList<int> randlist = Question::randomise(questions, current_test_passmark, current_test_use_groups, current_test_qnum, client_number, &progress, qApp);
    QStringList flags;
    for (int i = 0; i < randlist.count(); ++i) {
        if (current_test_shuffle_answers) { current_db_questions.at(randlist.at(i))->shuffleAnswers(); }
        QListWidgetItem * q_item = new QListWidgetItem;
        if (hideQuestionNamesCheckBox->isChecked()) {
            q_item->setText(QString("%1").arg(LQListWidget->count() + 1));
        } else {
            q_item->setText(current_db_questions.at(randlist.at(i))->name());
        }
        q_item->setData(Qt::UserRole, current_db_questions.at(randlist.at(i))->name());
    	LQListWidget->addItem(q_item);
    	current_test_questions.insert(q_item, current_db_questions.at(randlist.at(i)));
    	if ((current_db_questions.at(randlist.at(i))->flag() >= 0) && (current_db_questions.at(randlist.at(i))->flag() < current_db_f.size())) {
    		if (!flags.contains(current_db_f[current_db_questions.at(randlist.at(i))->flag()]))
                { flags << current_db_f[current_db_questions.at(randlist.at(i))->flag()]; }
        }
    }
    ITW_test_fnum->setText(QString("%1").arg(flags.count()));
    ITW_test_flags->setText(flags.join(", "));

    if (qApp->arguments().count() > 2) {
        if (qApp->arguments().at(1) == "-port") {
        		getReady();
        }
    }
}
