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

void Client::init(MainWindow * parent)
{
    c_score = 0;
    c_maxscore = 0;
    c_ready = false;
    c_results = new QMap<QString, QuestionAnswer>;
    c_blocksize = 1;
    c_parent = parent;
    c_test_sent = false;
    c_passed = true;

    QObject::connect(c_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

Client::Client(MainWindow * parent, QString name, int number)
{
    c_name = name;
    c_identified = true;
    c_socket = new QTcpSocket;
    c_number = number;

    init(parent);
}

Client::Client(MainWindow * parent, QTcpSocket * socket, int number)
{
    c_name = tr("Unknown");
    c_identified = false;
    c_socket = socket;
    c_number = number;

    init(parent);
}

Client::~Client()
{
    delete c_socket;
    delete c_results;
}

void Client::setName(const QString & name) { c_name = name; }

QString Client::name() { return c_name; }

void Client::setNumber(int number) { c_number = number; }

int Client::number() { return c_number; }

void Client::setSocket(QTcpSocket * socket)
{
    delete c_socket;
    c_socket = socket;
}

QTcpSocket * Client::socket()
{
    if (c_socket == NULL) {c_socket = new QTcpSocket;}
    return c_socket;
}

double Client::score() { return c_score; }

double Client::maximumScore() { return c_maxscore; }

bool Client::isReady() { return c_ready; }

QMap<QString, QuestionAnswer> * Client::results() { return c_results; }

void Client::setPassed(bool passed) { c_passed = passed; }

bool Client::passed() { return c_passed; }

void Client::loadResults(QString input)
{
    QTextStream in(&input); QString buffer; QuestionItem * item;
    Question::Answer ans; c_score = 0; c_maxscore = 0;

    do {
        item = NULL;
        if (in.readLine() != "[Q_NAME]") { return; }
        buffer = in.readLine();
        QMapIterator<QListWidgetItem *, QuestionItem *> i(c_parent->current_db_questions);
        while (i.hasNext()) { i.next();
            if (i.value()->name() == buffer) { item = i.value(); break; }
        }
        if (item == NULL) { in.readLine(); in.readLine(); continue; }
        if (in.readLine() != "[Q_ANSWERED]") { return; }
        ans = (Question::Answer)in.readLine().toInt();
        QuestionAnswer qans(item->correctAnswer(), ans, item->flag(), item->difficulty(), item->selectionType(), item->explanation());
        c_results->insert(item->name(), qans);
        c_score += qans.score(c_parent->current_db_scoringsystem);
        c_maxscore += qans.maximumScore(c_parent->current_db_scoringsystem);
        if (c_score > 0.0) { item->addCorrectAns(); }
        else { item->addIncorrectAns(); }
    } while (!in.atEnd());

    c_passed = c_parent->current_db_passmark.check(c_results, &c_parent->current_db_questions, c_parent->current_db_scoringsystem);
    c_ready = true;
    emit resultsLoaded(this);
}

void Client::readClientFeedback()
{
    QDataStream in(c_socket);
    in.setVersion(QDataStream::Qt_4_2);

    if (c_blocksize == 0) {
        if (c_socket->bytesAvailable() < (int)sizeof(quint64))
            return;

        in >> c_blocksize;
    } else if (!c_test_sent) {
        if (c_socket->bytesAvailable() < (int)sizeof(quint64))
            return;

        quint64 current_entry;
        in >> current_entry;
        if (current_entry < (quint64)c_parent->current_db_test.count()) {
            c_socket->write(c_parent->current_db_test.at(current_entry));
        }
        if (current_entry >= (quint64)(c_parent->current_db_test.count() - 1)) {
            c_test_sent = true; c_blocksize = 0;
        }
        return;
    }

    if ((quint64)c_socket->bytesAvailable() < c_blocksize)
        return;

    if (c_test_sent) { c_blocksize = 0; }

    QString received_string; QString buffer;
    do {
        in >> buffer; received_string.append(buffer);
    } while (!in.atEnd());

    if (c_identified) {
        emit finished(this);
        loadResults(received_string);
    } else {
        c_name = received_string;
        c_identified = true;
        emit identified(this);
    }
}

bool Client::isIdentified() { return c_identified; }

void Client::socketDisconnected() { emit disconnected(this); }

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(c_parent, tr("iTestServer"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(c_parent, tr("iTestServer"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the iTest server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(c_parent, tr("iTestServer"),
                                     tr("The following error occurred: %1.")
                                     .arg(c_socket->errorString()));
    }
}
