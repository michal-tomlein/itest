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
    void commonClientSetup(MainWindow *);
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
