#include "main_window.h"

void MainWindow::connectSocket()
{
    disableInputTypeSelection();
    client_number = 0;
    blocksize = 0;
    test_loaded = false;
    num_entries = 0;
    current_entry = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(serverNameLineEdit->text(),
                             serverPortLineEdit->text().toInt());
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

    if (tcpSocket->bytesAvailable() < blocksize)
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
            out << QString("%1\n").arg(current_test_questions.value(LQListWidget->item(i))->answered());
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
	sfile.setCodec("CP 1250");
	sfile << "[TEST_NAME]\n" << current_db_name << endl;
	sfile << "[TEST_TIMESTAMP]\n" << current_test_date << endl;
	sfile << "[TIME_FINISHED]\n" << current_test_time_finished << endl;
	sfile << "[CLIENT_NAME]\n" << nameLineEdit->text() << endl;
	for (int i = 0; i < LQListWidget->count(); ++i) {
        sfile << "[Q_NAME]\n";
        sfile << current_test_questions.value(LQListWidget->item(i))->name();
        sfile << "\n[Q_ANSWERED]\n";
        sfile << current_test_questions.value(LQListWidget->item(i))->answered();
        sfile << endl;
    }
}

void MainWindow::readResults(QString input)
{
    QTextStream in(&input); QString buffer; QuestionItem * item;
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
        switch (in.readLine().toInt()) {
            case 0: break;
            case 1: item->setAnsACorrect(true); break;
            case 2: item->setAnsBCorrect(true); break;
            case 3: item->setAnsCCorrect(true); break;
            case 4: item->setAnsDCorrect(true); break;
            default: break;
        }
    } while (!in.atEnd());
    saveResults();
}

void MainWindow::saveResults()
{
    current_test_results->clear(); QuestionItem * item;
    for (int i = 0; i < LQListWidget->count(); ++i) {
        item = current_test_questions.value(LQListWidget->item(i));
        switch (item->answered()) {
            case QuestionItem::None:
            case QuestionItem::A:
            case QuestionItem::B:
            case QuestionItem::C:
            case QuestionItem::D:
                if (!item->hasCorrectAnswer()) {
                    QuestionAnswer qans (QuestionItem::None, item->answered());
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsACorrect()) {
                    QuestionAnswer qans (QuestionItem::A, item->answered());
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsBCorrect()) {
                    QuestionAnswer qans (QuestionItem::B, item->answered());
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsCCorrect()) {
                    QuestionAnswer qans (QuestionItem::C, item->answered());
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsDCorrect()) {
                    QuestionAnswer qans (QuestionItem::D, item->answered());
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                }
                break;
            default:
                if (!item->hasCorrectAnswer()) {
                    QuestionAnswer qans (QuestionItem::None, QuestionItem::None);
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsACorrect()) {
                    QuestionAnswer qans (QuestionItem::A, QuestionItem::None);
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsBCorrect()) {
                    QuestionAnswer qans (QuestionItem::B, QuestionItem::None);
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsCCorrect()) {
                    QuestionAnswer qans (QuestionItem::C, QuestionItem::None);
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                } else if (item->isAnsDCorrect()) {
                    QuestionAnswer qans (QuestionItem::D, QuestionItem::None);
                    current_test_results->insert(LQListWidget->item(i)->text(), qans);
                    if (qans.isAnsweredCorrectly()) { current_test_score++; }
                }
                break;
        }
    }
    loadResults(current_test_results, resultsTableWidget);
    scoreLabel->setText(tr("%1 out of %2").arg(current_test_score).arg(LQListWidget->count()));
}

void MainWindow::loadResults(QMap<QString, QuestionAnswer> * results, QTableWidget * tw)
{
    QMapIterator<QString, QuestionAnswer> i(*results); int row = 0;
    QTableWidgetItem * item; QuestionAnswer qans;
    tw->setRowCount(results->count());
    while (i.hasNext()) {
        i.next();
        item = new QTableWidgetItem (i.key()); qans = i.value();
        tw->setItem(row, 0, item);
        item = new QTableWidgetItem (qans.isAnsweredCorrectly() ? tr("yes") : tr("no"));
        if (qans.isAnsweredCorrectly()) {
            item->setBackground(QBrush::QBrush(QColor::QColor(197, 255, 120)));
            item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        } else {
            item->setBackground(QBrush::QBrush(QColor::QColor(204, 109, 0)));
            item->setForeground(QBrush::QBrush(QColor::QColor(0, 0, 0)));
        }
        tw->setItem(row, 1, item);
        item = new QTableWidgetItem;
        switch (qans.answered()) {
            case QuestionItem::None: item->setText(tr("None")); break;
            case QuestionItem::A: item->setText("A"); break;
            case QuestionItem::B: item->setText("B"); break;
            case QuestionItem::C: item->setText("C"); break;
            case QuestionItem::D: item->setText("D"); break;
            default: item->setText(tr("None")); break;
        }
        tw->setItem(row, 2, item);
        item = new QTableWidgetItem;
        switch (qans.correctAnswer()) {
            case QuestionItem::None: item->setText(tr("None")); break;
            case QuestionItem::A: item->setText("A"); break;
            case QuestionItem::B: item->setText("B"); break;
            case QuestionItem::C: item->setText("C"); break;
            case QuestionItem::D: item->setText("D"); break;
            default: item->setText(tr("None")); break;
        }
        tw->setItem(row, 3, item);
        row++;
    }
    tw->resizeRowsToContents();
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("iTest - Test Writer"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("iTest - Test Writer"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the iTest server is running, "
                                         "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("iTest - Test Writer"),
                                     tr("The following error occurred: %1.")
                                     .arg(tcpSocket->errorString()));
    }

    enableInputTypeSelection(); progress_dialog->cancel();
}
