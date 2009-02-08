/*******************************************************************
 This file is part of iTest
 Copyright (C) 2007 Michal Tomlein (michal.tomlein@gmail.com)

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

#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QMap>
#include <QMapIterator>
#include <QObject>
#include <QDataStream>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTextStream>

#include "question_item.h"

class MainWindow;

class QuestionAnswer
{
public:
    QuestionAnswer();
    QuestionAnswer(int, QuestionItem::Answer, QuestionItem::Answer);
    void setAnswered(QuestionItem::Answer); QuestionItem::Answer answered();
    void setCorrectAnswer(QuestionItem::Answer); QuestionItem::Answer correctAnswer();
    bool isAnsweredCorrectly();
    void setFlag(int); int flag();

private:
    QuestionItem::Answer qa_answered;
    QuestionItem::Answer qa_correct_answer;
    int qa_flag;
};

class Client : public QObject
{
    Q_OBJECT

private:
    void init(MainWindow *);

public:
    Client(MainWindow *);
    Client(MainWindow *, int);
    Client(MainWindow *, QString);
    Client(MainWindow *, QTcpSocket *);
    Client(MainWindow *, int, QTcpSocket *);
    Client(MainWindow *, QString, QTcpSocket *);
    Client(MainWindow *, int, QString, QTcpSocket *);
    virtual ~Client();

public slots:
    void setName(QString); QString name();
    void setNumber(int); int number();
    void setSocket(QTcpSocket *); QTcpSocket * socket();
    void setScore(int); int score();
    void setReady(bool); bool isReady();
    void setResults(QMap<QString, QuestionAnswer> *);
    QMap<QString, QuestionAnswer> * results();
    void setPassed(bool); bool passed();
    void loadResults(QString);
    void readClientFeedback();
    bool isIdentified();
    void displayError(QAbstractSocket::SocketError);
    void socketDisconnected();

signals:
    void identified(Client *);
    void finished(Client *);
    void resultsLoaded(Client *);
    void disconnected(Client *);

private:
    QString c_name;
    QTcpSocket * c_socket;
    int c_score;
    bool c_ready;
    QMap<QString, QuestionAnswer> * c_results;
    bool c_passed;
    quint64 c_blocksize;
    bool c_identified;
    MainWindow * c_parent;
    int c_number;
    bool c_test_sent;
};

#endif // CLIENT_H
