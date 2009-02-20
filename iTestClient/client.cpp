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

void MainWindow::connectSocket()
{
    disableInputTypeSelection();
    client_number = 0;
    blocksize = 0;
    test_loaded = false;
    num_entries = 0;
    current_entry = 0;
    current_connection_local = (serverNameLineEdit->text() == "Localhost" || serverNameLineEdit->text() == "localhost");
    tcpSocket->abort();
    tcpSocket->connectToHost(serverNameLineEdit->text(), serverPortLineEdit->text().toInt());
    if (progress_dialog != NULL) delete progress_dialog;
    progress_dialog = new QProgressDialog (this);
    progress_dialog->setLabelText(tr("Retrieving test data..."));
    progress_dialog->setMinimum(0);
    progress_dialog->setMinimumDuration(0);
    progress_dialog->setWindowModality(Qt::WindowModal);
}

void MainWindow::readIncomingData()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_2);

    if (client_number == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint32))
            return;

        in >> client_number;
    }

    if (num_entries == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint64))
            return;

        in >> num_entries; progress_dialog->setMaximum(num_entries);
    }

    if (blocksize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint64))
            return;

        in >> blocksize;
    }

    if ((quint64)tcpSocket->bytesAvailable() < blocksize)
        return;

    if (!test_loaded) {
        current_entry++; progress_dialog->setValue(current_entry);
    }

    QString received_string; QString buffer;
    do {
        in >> buffer; received_string.append(buffer);
    } while (!in.atEnd());

    blocksize = 0;

    if (!test_loaded) {
        received_data.append(received_string);
        if (current_entry >= num_entries) {
            loadTest(received_data); test_loaded = true;
        } else {
            QByteArray ba;
            QDataStream out(&ba, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_2);
            out << current_entry;
            tcpSocket->write(ba);
        }
    } else {
        readResults(received_string);
    }
}

void MainWindow::sendResults()
{
    if (rbtnNetwork->isChecked()) {
        QByteArray results;
        QDataStream out(&results, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_2);
        out << (quint64)0;
        // ---------------------------------------------------------------------
        for (int i = 0; i < LQListWidget->count(); ++i) {
            out << QString("[Q_NAME]\n");
            out << current_test_questions.value(LQListWidget->item(i))->name();
            out << QString("\n[Q_ANSWERED]\n");
            if (current_db_multiple_ans_support) {
                out << QString("%1\n").arg(current_test_questions.value(LQListWidget->item(i))->answered());
            } else {
                out << QString("%1\n").arg(Question::convertToOldAnsNumber(current_test_questions.value(LQListWidget->item(i))->answered()));
            }
        }
        // ---------------------------------------------------------------------
        out.device()->seek(0);
        out << (quint64)(results.size() - sizeof(quint64));
        tcpSocket->write(results);
    }
    
    QString save_file_name; // yyyy.MM.dd-hh:mm
    QString datetime = current_test_date; datetime.replace(13, 1, '.');
    if (useDefaultOutputCheckBox->isChecked()) {
        save_file_name = QString("%1/%2-%3-%4.itcl").arg(QDir::homePath()).arg(current_db_name).arg(datetime).arg(nameLineEdit->text());
    } else {
        save_file_name = savePathLineEdit->text();
    }
    if (save_file_name.isNull() || save_file_name.isEmpty()) { return; }
	QFile file(save_file_name);
	if (!file.open(QFile::WriteOnly | QFile::Text)) 
	{
		QMessageBox::critical(this, tr("Save answer log"), tr("Cannot write file %1:\n%2.").arg(save_file_name).arg(file.errorString()));
		return;
	}
	QTextStream sfile(&file);
	sfile.setCodec("UTF-8");
	sfile << "[TEST_NAME]\n" << current_db_name << endl;
	sfile << "[TEST_TIMESTAMP]\n" << current_test_date << endl;
	sfile << "[TIME_FINISHED]\n" << current_test_time_finished << endl;
	sfile << "[CLIENT_NAME]\n" << nameLineEdit->text() << endl;
	for (int i = 0; i < LQListWidget->count(); ++i) {
        sfile << "[Q_NAME]\n";
        sfile << current_test_questions.value(LQListWidget->item(i))->name();
        sfile << "\n[Q_ANSWERED]\n";
        if (current_db_multiple_ans_support) {
            sfile << current_test_questions.value(LQListWidget->item(i))->answered();
        } else {
            sfile << Question::convertToOldAnsNumber(current_test_questions.value(LQListWidget->item(i))->answered());
        }
        sfile << endl;
    }
}

void MainWindow::readResults(QString input)
{
    QTextStream in(&input); QString buffer; QuestionItem * item; double maxscore = 0.0;
    do {
        if (in.readLine() != "[Q_NAME]") { return; }
        buffer = in.readLine(); item = NULL;
        for (int i = 0; i < LQListWidget->count(); ++i) {
            if (current_test_questions.value(LQListWidget->item(i))->name() == buffer) {
                item = current_test_questions.value(LQListWidget->item(i)); break;
            }
        }
        if (item == NULL) { in.readLine(); in.readLine(); continue; }
        if (in.readLine() != "[Q_C_ANS]") { return; }
        if (current_db_multiple_ans_support) {
            item->setCorrectAnswers((Question::Answer)in.readLine().toInt());
        } else {
            item->setCorrectAnswers(Question::convertOldAnsNumber(in.readLine().toInt()));
        }
        current_test_score += item->score();
        maxscore += item->maximumScore();
        if (current_db_itdb1_4_support) {
            if (in.readLine() != "[Q_EXPL]") { return; }
            item->setExplanation(in.readLine());
        }
    } while (!in.atEnd());
    scoreLabel->setText(tr("%1 out of %2 (%3)").arg(current_test_score).arg(maxscore).arg(current_test_passmark.check(current_test_questions.values()) ? tr("PASSED") : tr("FAILED")));
    loadResults(resultsTableWidget);
}

void MainWindow::loadResults(QTableWidget * tw)
{
    QList<QuestionItem *> questions = current_test_questions.values();
    tw->setRowCount(questions.count());
    bool highlight_correct_answers = !rbtnFromFile->isChecked() && !hideCorrectAnswersCheckBox->isChecked();
    for (int i = 0; i < questions.count(); ++i) {
        tw->setCellWidget(i, 0, new QuestionWidget(questions.at(i), highlight_correct_answers));
        tw->setRowHeight(i, tw->cellWidget(i, 0)->sizeHint().height());
    }
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("iTestClient"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("iTestClient"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the iTest server is running, "
                                         "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("iTestClient"),
                                     tr("The following error occurred: %1.")
                                     .arg(tcpSocket->errorString()));
    }

    enableInputTypeSelection(); progress_dialog->cancel();
}
