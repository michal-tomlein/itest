/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2008 Michal Tomlein (michal.tomlein@gmail.com)

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

#include "../shared/about_widget.h"
#include "main_window.h"

MainWindow::MainWindow()
{
    varinit();
    setupUi(this);
    if (tr("LTR") == "RTL") { qApp->setLayoutDirection(Qt::RightToLeft); }

#ifndef Q_WS_WIN
    testPageSplitter->setPalette(this->palette());
#endif

    remainingTimeLcdNumber->setVisible(false);
    remainingTimeProgressBar->setVisible(false);
    label_minutes->setVisible(false);
    questionTextSvgSplitter->setCollapsible(0, false);
    questionTextSvgSplitter->setCollapsible(1, true);
    tcpSocket = new QTcpSocket(this);
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
    QObject::connect(svgDisplayWidget, SIGNAL(titleClicked(QString)), this, SLOT(previewSvg(QString)));
    QObject::connect(btnNext, SIGNAL(released()), this, SLOT(nextQuestion()));
    QObject::connect(btnLast, SIGNAL(released()), this, SLOT(lastQuestion()));
    QObject::connect(btnFinish, SIGNAL(released()), this, SLOT(finish()));
    QObject::connect(btnNewTest, SIGNAL(released()), this, SLOT(newTest()));
    QObject::connect(btnQuit, SIGNAL(released()), this, SLOT(close()));

    rbtngrpInputType = new QButtonGroup (this);
    rbtngrpInputType->addButton(rbtnNetwork);
    rbtngrpInputType->addButton(rbtnFromFile);
    QObject::connect(rbtngrpInputType, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(toggleInputType(QAbstractButton *)));

    QObject::connect(answersView, SIGNAL(buttonReleased(Question::Answers)), this, SLOT(setQuestionAnswered(Question::Answers)));

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
    infoTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    infoTableWidget->verticalHeader()->setResizeMode(8, QHeaderView::Stretch);
    resultsTableWidget->setColumnCount(1);
    resultsTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    resultsTableWidget->horizontalHeader()->hide();
    
    loadSettings();
    
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

void MainWindow::loadSettings()
{
    QSettings settings("Michal Tomlein", "iTest");
    serverNameLineEdit->setText(settings.value("client/serverName").toString());
    serverPortLineEdit->setText(settings.value("client/serverPort").toString());
    hideQuestionNamesCheckBox->setChecked(settings.value("client/hideQuestionNames", false).toBool());
    hideCorrectAnswersCheckBox->setChecked(settings.value("client/hideCorrectAnswers", false).toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings("Michal Tomlein", "iTest");
    settings.setValue("client/serverName", serverNameLineEdit->text());
    settings.setValue("client/serverPort", serverPortLineEdit->text());
    settings.setValue("client/hideQuestionNames", hideQuestionNamesCheckBox->isChecked());
    settings.setValue("client/hideCorrectAnswers", hideCorrectAnswersCheckBox->isChecked());
}

void MainWindow::setQuestionAnswered(Question::Answers selected_answers)
{
    if (!LQListWidget->currentIndex().isValid()) { return; }
    QuestionItem * item = current_test_questions.value(LQListWidget->currentItem());
    item->setAnswered(selected_answers);
    if (selected_answers == Question::None) {
        LQListWidget->currentItem()->setBackground(QBrush::QBrush(QColor::QColor(255, 255, 255)));
        LQListWidget->currentItem()->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
    } else {
        LQListWidget->currentItem()->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
        LQListWidget->currentItem()->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        int progress = 0;
        for (int i = 0; i < LQListWidget->count(); ++i) {
            if (current_test_questions.value(LQListWidget->item(i))->answered() != Question::None) {
                progress++;
            }
        }
        progressBar->setValue(progress);
    }
}

void MainWindow::setCurrentQuestion()
{
     if (LQListWidget->currentIndex().isValid()) {
        questionTextBrowser->clear();
        answersView->setEnabled(true);
        QuestionItem * item = current_test_questions.value(LQListWidget->currentItem());
        questionTextBrowser->setHtml(item->text());
        answersView->setAnswers(item->answers(), item->answered(), item->selectionType());
        svgDisplayWidget->clear();
        if (item->numSvgItems() > 0) {
            svgDisplayWidget->setVisible(true);
            //int h = 0;
            for (int i = 0; i < item->numSvgItems(); ++i) {
                QSvgWidget * svg_widget = new QSvgWidget;
                QSize minimum_size = svg_widget->sizeHint();
                minimum_size.scale(128, 128, Qt::KeepAspectRatioByExpanding);
                svg_widget->setMinimumSize(minimum_size);
                svg_widget->load(item->svg(i).toUtf8());
                /*if (svg_widget->renderer()->defaultSize().height() + 40 > h)
                    { h = svg_widget->renderer()->defaultSize().height() + 40; }*/
                svgDisplayWidget->addWidget(svg_widget, item->svgName(i), true);
            }
            //if (h < 168) { h = 168; }
            qApp->processEvents();
            questionTextSvgSplitter->moveSplitter(questionTextSvgSplitter->height()-svgDisplayWidget->layoutHeightForWidth(questionTextSvgSplitter->width()));
        } else {
            svgDisplayWidget->setVisible(false);
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
        answersView->clear();
        answersView->setEnabled(false);
        svgDisplayWidget->clear();
        btnNext->setEnabled(false);
        btnLast->setEnabled(false);
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

void MainWindow::previewSvg(QString link)
{
    if (!LQListWidget->currentIndex().isValid()) { return; }
	QuestionItem * item = current_test_questions.value(LQListWidget->currentItem());
	if (item == NULL) { return; }
	if (link.toInt() < 0 || link.toInt() >= item->numSvgItems()) { return; }
    QSvgWidget * svg_widget = new QSvgWidget;
	svg_widget->setAttribute(Qt::WA_DeleteOnClose);
	svg_widget->setParent(this);
#ifndef Q_WS_MAC
	svg_widget->setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowStaysOnTopHint);
#else
	svg_widget->setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowStaysOnTopHint | Qt::WindowSystemMenuHint);
#endif
	svg_widget->setWindowTitle(item->svgName(link.toInt()));
	QSize minimum_size = svg_widget->sizeHint();
	minimum_size.scale(128, 128, Qt::KeepAspectRatioByExpanding);
	svg_widget->setMinimumSize(minimum_size);
	svg_widget->load(item->svg(link.toInt()).toUtf8());
	svg_widget->show();
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
    if (rbtnFromFile->isChecked()) { loadResults(resultsTableWidget); }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if ((mainStackedWidget->currentIndex() == 0) || (mainStackedWidget->currentIndex() == 3)) {
	   saveSettings(); event->accept();
    } else if (current_connection_local) { saveSettings(); event->accept(); }
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
        testPageSplitter->moveLeft();
        qApp->processEvents();
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
	//current_test_results->clear();
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
	progressBar->setValue(0);
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
        if (rbtnFromFile->isChecked()) { loadResults(resultsTableWidget); }
        QMessageBox::information(this, tr("Exam finished"), tr("You have run out of time. Your answers are being sent."));
    }
}

void MainWindow::errorInvalidData()
{
     QMessageBox::critical(this, tr("iTestClient - Load test data"), tr("Invalid data received. There might be something wrong with the server."));
     enableInputTypeSelection();
}

void MainWindow::about()
{
    AboutWidget * itest_about = new AboutWidget(ver, QString("2008"));
	itest_about->setParent(this);
    itest_about->setWindowFlags(Qt::Dialog /*| Qt::WindowMaximizeButtonHint*/ | Qt::WindowStaysOnTopHint);
	itest_about->show();
}
