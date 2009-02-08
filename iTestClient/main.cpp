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

#include "about_widget.h"
#include "main_window.h"

MainWindow::MainWindow()
{
    varinit();
    setupUi(this);
    this->showFullScreen();
    QTimer::singleShot(200, this, SLOT(updateGeometry()));

    remainingTimeLcdNumber->setVisible(false);
    remainingTimeProgressBar->setVisible(false);
    label_minutes->setVisible(false);

    tcpSocket = new QTcpSocket(this);
    current_test_results = new QMap<QString, QuestionAnswer>;
    progress_dialog = NULL;
    current_test_use_groups = false;
    current_connection_local = false;

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    QObject::connect(tbtnQuit, SIGNAL(released()), this, SLOT(close()));
    QObject::connect(tbtnAbout, SIGNAL(released()), this, SLOT(about()));
    QObject::connect(tbtnGetReady, SIGNAL(released()), this, SLOT(getReady()));
    QObject::connect(tbtnStart, SIGNAL(released()), this, SLOT(start()));
    QObject::connect(tbtnBrowse_DBPath, SIGNAL(released()), this, SLOT(browse_i()));
    QObject::connect(tbtnBrowse_savePath, SIGNAL(released()), this, SLOT(browse_o()));
    QObject::connect(tbtnLoad, SIGNAL(released()), this, SLOT(loadFile()));
    QObject::connect(useDefaultOutputCheckBox, SIGNAL(toggled(bool)), savePathLineEdit, SLOT(setDisabled(bool)));
    QObject::connect(useDefaultOutputCheckBox, SIGNAL(toggled(bool)), tbtnBrowse_savePath, SLOT(setDisabled(bool)));
    QObject::connect(serverNameLineEdit, SIGNAL(textChanged(const QString &)),
                     this, SLOT(enableConnectButton()));
    QObject::connect(serverPortLineEdit, SIGNAL(textChanged(const QString &)),
                     this, SLOT(enableConnectButton()));
    QObject::connect(DBPathLineEdit, SIGNAL(textChanged(const QString &)),
                     this, SLOT(enableLoadButton()));
    QObject::connect(tbtnConnect, SIGNAL(released()), this, SLOT(connectSocket()));
    QObject::connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readIncomingData()));
    QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(displayError(QAbstractSocket::SocketError)));
    QObject::connect(LQListWidget, SIGNAL(currentTextChanged(QString)),
                     this, SLOT(setCurrentQuestion()));
    QObject::connect(btnNext, SIGNAL(released()), this, SLOT(nextQuestion()));
    QObject::connect(btnLast, SIGNAL(released()), this, SLOT(lastQuestion()));
    QObject::connect(btnFinish, SIGNAL(released()), this, SLOT(finish()));
    QObject::connect(btnNewTest, SIGNAL(released()), this, SLOT(newTest()));
    QObject::connect(btnQuit, SIGNAL(released()), this, SLOT(close()));

    rbtngrpInputType = new QButtonGroup (this);
    rbtngrpInputType->addButton(rbtnNetwork);
    rbtngrpInputType->addButton(rbtnFromFile);
    QObject::connect(rbtngrpInputType, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(toggleInputType(QAbstractButton *)));

    rbtngrpAnswer = new QButtonGroup (this);
    rbtngrpAnswer->addButton(rbtnAnswerA);
    rbtngrpAnswer->addButton(rbtnAnswerB);
    rbtngrpAnswer->addButton(rbtnAnswerC);
    rbtngrpAnswer->addButton(rbtnAnswerD);
    QObject::connect(rbtngrpAnswer, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(setQuestionAnswered(QAbstractButton *)));

    for (int i = 0; i < 8; ++i) {infoTableWidget->setItem(i, 0, new QTableWidgetItem);}
    ITW_test_name = infoTableWidget->item(0, 0);
    ITW_test_date = infoTableWidget->item(1, 0);
    ITW_test_timestamp = infoTableWidget->item(2, 0);
    ITW_test_time = infoTableWidget->item(3, 0);
    ITW_test_qnum = infoTableWidget->item(4, 0);
    ITW_test_fnum = infoTableWidget->item(5, 0);
    ITW_test_flags = infoTableWidget->item(6, 0);
    ITW_test_passmark = infoTableWidget->item(7, 0);
    ITW_test_comments = new QTextBrowser (infoTableWidget);
    infoTableWidget->setCellWidget(8, 0, ITW_test_comments);
    
    // Check app args ----------------------------------------------------------
    if (qApp->arguments().count() > 2) {
    	if (qApp->arguments().at(1) == "-port") {
    		serverNameLineEdit->setText("Localhost");
    		serverPortLineEdit->setText(qApp->arguments().at(2));
    		connectSocket();
        }
    } else if (qApp->arguments().count() > 1) {
        QFileInfo file_info (qApp->arguments().at(1));
        if (file_info.exists()) {
            rbtnFromFile->setChecked(true);
            toggleInputType(rbtnFromFile);
            DBPathLineEdit->setText(file_info.absoluteFilePath());
            loadFile(file_info.absoluteFilePath());
        }
    }
}

void MainWindow::updateGeometry()
{
    mainStackedWidget->setGeometry(this->geometry());
    mainGridLayout->setGeometry(this->geometry());
    startGridLayout->setGeometry(this->geometry());
    testGridLayout->setGeometry(this->geometry());
    welcomeVerticalLayout->setGeometry(welcomeFrame->geometry());
    resultsGridLayout->setGeometry(this->geometry());
}

void MainWindow::setQuestionAnswered(QAbstractButton * rbtn)
{
    if (LQListWidget->currentIndex().isValid()) {
        QuestionItem * item = current_test_questions.value(LQListWidget->currentItem());
        if (rbtn == rbtnAnswerA) {
            item->setAnswered(QuestionItem::A);
        } else if (rbtn == rbtnAnswerB) {
            item->setAnswered(QuestionItem::B);
        } else if (rbtn == rbtnAnswerC) {
            item->setAnswered(QuestionItem::C);
        } else if (rbtn == rbtnAnswerD) {
            item->setAnswered(QuestionItem::D);
        } else {
            item->setAnswered(QuestionItem::None);
            LQListWidget->currentItem()->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
            LQListWidget->currentItem()->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        }
        if ((rbtn == rbtnAnswerA) || (rbtn == rbtnAnswerB) || (rbtn == rbtnAnswerC) || (rbtn == rbtnAnswerD)) {
            LQListWidget->currentItem()->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
            LQListWidget->currentItem()->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
            int progress = 0;
            for (int i = 0; i < LQListWidget->count(); ++i) {
                if (current_test_questions.value(LQListWidget->item(i))->answered() != QuestionItem::None) {
                    progress++;
                }
            }
            progressBar->setValue(progress);
        }
     }
}

void MainWindow::setCurrentQuestion()
{
     if (LQListWidget->currentIndex().isValid()) {
        questionTextBrowser->clear();
        answerA_textBrowser->clear();
        answerB_textBrowser->clear();
        answerC_textBrowser->clear();
        answerD_textBrowser->clear();
        rbtnAnswerA->setEnabled(true);
        rbtnAnswerB->setEnabled(true);
        rbtnAnswerC->setEnabled(true);
        rbtnAnswerD->setEnabled(true);
        QuestionItem * item = current_test_questions.value(LQListWidget->currentItem());
        questionTextBrowser->setHtml(item->text());
        answerA_textBrowser->setText(item->ansA());
        answerB_textBrowser->setText(item->ansB());
        answerC_textBrowser->setText(item->ansC());
        answerD_textBrowser->setText(item->ansD());
        switch (item->answered()) {
            case QuestionItem::None:
                rbtngrpAnswer->setExclusive(false);
                rbtnAnswerA->setChecked(false);
                rbtnAnswerB->setChecked(false);
                rbtnAnswerC->setChecked(false);
                rbtnAnswerD->setChecked(false);
                rbtngrpAnswer->setExclusive(true);
                break;
            case QuestionItem::A:
                rbtnAnswerA->setChecked(true);
                break;
            case QuestionItem::B:
                rbtnAnswerB->setChecked(true);
                break;
            case QuestionItem::C:
                rbtnAnswerC->setChecked(true);
                break;
            case QuestionItem::D:
                rbtnAnswerD->setChecked(true);
                break;
        }
        if (LQListWidget->currentRow() >= (LQListWidget->count()-1)) {
            btnNext->setEnabled(false);
        } else {
            btnNext->setEnabled(true);
        }
        if (LQListWidget->currentRow() <= 0) {
            btnLast->setEnabled(false);
        } else {
            btnLast->setEnabled(true);
        }
     } else {
        questionTextBrowser->clear();
        answerA_textBrowser->clear();
        answerB_textBrowser->clear();
        answerC_textBrowser->clear();
        answerD_textBrowser->clear();
        btnNext->setEnabled(false);
        btnLast->setEnabled(false);
        rbtnAnswerA->setEnabled(false);
        rbtnAnswerB->setEnabled(false);
        rbtnAnswerC->setEnabled(false);
        rbtnAnswerD->setEnabled(false);
     }
}

void MainWindow::nextQuestion()
{
    if (LQListWidget->currentRow() < (LQListWidget->count()-1)) {
        LQListWidget->setCurrentRow(LQListWidget->currentRow()+1);
    }
}

void MainWindow::lastQuestion()
{
    if (LQListWidget->currentRow() > 0) {
        LQListWidget->setCurrentRow(LQListWidget->currentRow()-1);
    }
}

void MainWindow::finish()
{
    switch (QMessageBox::information(this, tr("Finish the exam"), tr("Are you sure you want to finish? Once you click Finish there is no going back."), tr("&Finish"), tr("&Cancel"), 0, 1)) {
         case 0: // Finish
              break;
         case 1: // Cancel
              return; break;
    }
    mainStackedWidget->setCurrentIndex(3);
    if (current_test_results_sent) { return; }
    current_test_results_sent = true;
    current_test_time_finished = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm");
    sendResults();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if ((mainStackedWidget->currentIndex() == 0) || (mainStackedWidget->currentIndex() == 3)) {
	   event->accept();
    } else if (current_connection_local) { event->accept(); }
    else { event->ignore(); }
}

void MainWindow::enableConnectButton()
{
    tbtnConnect->setEnabled(!serverNameLineEdit->text().isEmpty()
                            && !serverPortLineEdit->text().isEmpty()
                            && rbtnNetwork->isChecked());
}

void MainWindow::enableLoadButton()
{
    tbtnLoad->setEnabled(!DBPathLineEdit->text().isEmpty() && rbtnFromFile->isChecked());
}

void MainWindow::toggleInputType(QAbstractButton * rbtn)
{
    if (rbtn == rbtnNetwork) {
        serverNameLineEdit->setEnabled(true);
        serverPortLineEdit->setEnabled(true);
        enableConnectButton();
        DBPathLineEdit->setEnabled(false);
        tbtnBrowse_DBPath->setEnabled(false);
        tbtnLoad->setEnabled(false);
    } else if (rbtn == rbtnFromFile) {
        serverNameLineEdit->setEnabled(false);
        serverPortLineEdit->setEnabled(false);
        tbtnConnect->setEnabled(false);
        DBPathLineEdit->setEnabled(true);
        tbtnBrowse_DBPath->setEnabled(true);
        enableLoadButton();
    }
}

void MainWindow::enableInputTypeSelection()
{
    rbtnNetwork->setEnabled(true);
    rbtnFromFile->setEnabled(true);
    toggleInputType(rbtnNetwork->isChecked() ? rbtnNetwork : rbtnFromFile);
}

void MainWindow::disableInputTypeSelection()
{
    rbtnNetwork->setEnabled(false);
    rbtnFromFile->setEnabled(false);
    serverNameLineEdit->setEnabled(false);
    serverPortLineEdit->setEnabled(false);
    tbtnConnect->setEnabled(false);
    DBPathLineEdit->setEnabled(false);
    tbtnBrowse_DBPath->setEnabled(false);
    tbtnLoad->setEnabled(false);
}

void MainWindow::about()
{
    AboutWidget * itest_about = new AboutWidget(ver, QString("4.3.1"), QString("2007"));
    itest_about->setWindowFlags(Qt::Dialog /*| Qt::WindowMaximizeButtonHint*/ | Qt::WindowStaysOnTopHint);
	itest_about->show();
}

void MainWindow::getReady()
{
	if (hideQuestionNamesCheckBox->isChecked()) {
		for (int i = 0; i < LQListWidget->count(); ++i) {
			LQListWidget->item(i)->setText(QString("%1").arg(i + 1));
		}
	} else {
		for (int i = 0; i < LQListWidget->count(); ++i) {
			LQListWidget->item(i)->setText(current_test_questions.value(LQListWidget->item(i))->name());
		}
	}
	mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::start()
{
    if (nameLineEdit->text().isEmpty()) {
        QMessageBox::information(this, tr("Your name, please."), tr("You cannot proceed unless you give us your name."));
    } else {
        mainStackedWidget->setCurrentIndex(2);
        LQListWidget->setCurrentRow(0);
        btnLast->setEnabled(false);
        current_test_score = 0;
        current_test_results_sent = false;
        progressBar->setMaximum(current_test_qnum);
        remainingTimeLcdNumber->display(current_test_time_remaining);
        remainingTimeProgressBar->setMaximum(current_test_time_remaining);
        remainingTimeProgressBar->setValue(current_test_time_remaining);
        timer.start(60000);
        QByteArray client_name;
        QDataStream out(&client_name, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_2);
        out << (quint64)0;
        out << nameLineEdit->text();
        out.device()->seek(0);
        out << (quint64)(client_name.size() - sizeof(quint64));
        tcpSocket->write(client_name);
    }
}

void MainWindow::newTest()
{
	current_db_questions.clear();
	current_test_qnum = 0;
	current_test_questions.clear();
	current_test_results->clear();
	current_test_time_remaining = 0;
	current_test_score = 0;
	current_test_results_sent = false;
	current_test_passmark.clear();
	current_test_use_groups = false;
	test_loaded = false;
	timer.stop();
	LQListWidget->clear();
	nameLineEdit->clear();
	scoreLabel->clear();
	resultsTableWidget->setRowCount(0);
	mainStackedWidget->setCurrentIndex(0);
	for (int i = 0; i < 20; ++i) { current_db_fe[i] = false; current_db_f[i].clear(); }
	if (rbtnNetwork->isChecked()) {
		tcpSocket->disconnectFromHost();
		current_connection_local = false;
		blocksize = 0;
		client_number = 0;
		num_entries = 0;
		current_entry = 0;
		connectSocket();
	} else {
		current_connection_local = true;
		loadFile();
	}
}

void MainWindow::updateTime()
{
    if (current_test_time_remaining > 1) {
        timer.start(60000);
        current_test_time_remaining--;
        remainingTimeLcdNumber->display(current_test_time_remaining);
        remainingTimeProgressBar->setValue(current_test_time_remaining);
    } else {
        if (mainStackedWidget->currentIndex() != 2) { return; }
        if (current_test_results_sent) { return; }
        current_test_results_sent = true;
        current_test_time_finished = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm");
        mainStackedWidget->setCurrentIndex(3); sendResults();
        QMessageBox::information(this, tr("Exam finished"), tr("You have run out of time. Your answers are being sent."));
    }
}

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

void MainWindow::errorInvalidData()
{
     QMessageBox::critical(this, tr("iTestClient - Load test data"), tr("Invalid data received. There might be something wrong with the server."));
     enableInputTypeSelection();
}

// ---------------------------- version changelog: -----------------------------
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
