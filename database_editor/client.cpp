#include "main_window.h"

void Client::commonClientSetup(MainWindow * parent)
{
    c_score = 0;
    c_ready = false;
    c_results = new QMap<QString, QuestionAnswer>;
    c_blocksize = 1;
    c_parent = parent;
    c_test_sent = false;
    
    QObject::connect(c_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

Client::Client(MainWindow * parent)
{
    c_name = QObject::tr("Unknown");
    c_identified = false;
    c_socket = new QTcpSocket;
    c_number = 0;
    
    commonClientSetup(parent);
}

Client::Client(MainWindow * parent, int number)
{
    c_name = QObject::tr("Unknown");
    c_identified = false;
    c_socket = new QTcpSocket;
    c_number = number;
    
    commonClientSetup(parent);
}

Client::Client(MainWindow * parent, QString name)
{
    c_name = name;
    c_identified = true;
    c_socket = new QTcpSocket;
    c_number = 0;
    
    commonClientSetup(parent);
}

Client::Client(MainWindow * parent, QTcpSocket * socket)
{
    c_name = QObject::tr("Unknown");
    c_identified = false;
    c_socket = socket;
    c_number = 0;
    
    commonClientSetup(parent);
}

Client::Client(MainWindow * parent, int number, QTcpSocket * socket)
{
    c_name = QObject::tr("Unknown");
    c_identified = false;
    c_socket = socket;
    c_number = number;
    
    commonClientSetup(parent);
}

Client::Client(MainWindow * parent, QString name, QTcpSocket * socket)
{
    c_name = name;
    c_identified = true;
    c_socket = socket;
    c_number = 0;
    
    commonClientSetup(parent);
}

Client::Client(MainWindow * parent, int number, QString name, QTcpSocket * socket)
{
    c_name = name;
    c_identified = true;
    c_socket = socket;
    c_number = number;
    
    commonClientSetup(parent);
}

Client::~Client()
{
    delete c_socket;
    delete c_results;
}

void Client::setName(QString name) { c_name = name; }

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

void Client::setScore(int score) { c_score = score; }

int Client::score() { return c_score; }

void Client::setReady(bool ready) { c_ready = ready; }

bool Client::isReady() { return c_ready; }

void Client::setResults(QMap<QString, QuestionAnswer> * results)
{
    delete c_results;
    c_results = results;
}

QMap<QString, QuestionAnswer> * Client::results() { return c_results; }

void Client::loadResults(QString input)
{
    QTextStream in(&input); QString buffer; QuestionItem * item;
    QuestionItem::Answer ans;
    
    do {
        item = NULL;
        if (in.readLine() != "[Q_NAME]") { return; }
        buffer = in.readLine();
        QMapIterator<QListWidgetItem *, QuestionItem *> i(c_parent->current_db_questions);
        while (i.hasNext()) {
            i.next();
            if (i.value()->name() == buffer) {
                item = i.value(); break;
            }
        }
        if (item == NULL) { in.readLine(); in.readLine(); continue; }
        if (in.readLine() != "[Q_ANSWERED]") { return; }
        switch (in.readLine().toInt()) {
            case 0: ans = QuestionItem::None; break;
            case 1: ans = QuestionItem::A; break;
            case 2: ans = QuestionItem::B; break;
            case 3: ans = QuestionItem::C; break;
            case 4: ans = QuestionItem::D; break;
            default: ans = QuestionItem::None; break;
        }
        switch (ans) {
            case QuestionItem::None:
            case QuestionItem::A:
            case QuestionItem::B:
            case QuestionItem::C:
            case QuestionItem::D:
                if (!item->hasCorrectAnswer()) {
                    QuestionAnswer qans (QuestionItem::None, ans);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsACorrect()) {
                    QuestionAnswer qans (QuestionItem::A, ans);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsBCorrect()) {
                    QuestionAnswer qans (QuestionItem::B, ans);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsCCorrect()) {
                    QuestionAnswer qans (QuestionItem::C, ans);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsDCorrect()) {
                    QuestionAnswer qans (QuestionItem::D, ans);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                }
                break;
            default:
                if (!item->hasCorrectAnswer()) {
                    QuestionAnswer qans (QuestionItem::None, QuestionItem::None);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsACorrect()) {
                    QuestionAnswer qans (QuestionItem::A, QuestionItem::None);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsBCorrect()) {
                    QuestionAnswer qans (QuestionItem::B, QuestionItem::None);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsCCorrect()) {
                    QuestionAnswer qans (QuestionItem::C, QuestionItem::None);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                } else if (item->isAnsDCorrect()) {
                    QuestionAnswer qans (QuestionItem::D, QuestionItem::None);
                    c_results->insert(item->name(), qans);
                    if (qans.isAnsweredCorrectly()) { c_score++; item->addCorrectAns(); }
                    else { item->addIncorrectAns(); }
                }
                break;
        }
    } while (!in.atEnd());
    
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
        if (current_entry < c_parent->current_db_test.count()) {
            c_socket->write(c_parent->current_db_test.at(current_entry));
        }
        if (current_entry >= c_parent->current_db_test.count() - 1) {
            c_test_sent = true; c_blocksize = 0;
        }
        return;
    }

    if (c_socket->bytesAvailable() < c_blocksize)
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
            QMessageBox::information(c_parent, tr("iTest - Database Editor"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(c_parent, tr("iTest - Database Editor"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the iTest server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(c_parent, tr("iTest - Database Editor"),
                                     tr("The following error occurred: %1.")
                                     .arg(c_socket->errorString()));
    }
}

QuestionAnswer::QuestionAnswer()
{
    qa_answered = QuestionItem::None;
    qa_correct_answer = QuestionItem::None;
}

QuestionAnswer::QuestionAnswer(QuestionItem::Answer correct, QuestionItem::Answer ans)
{
    qa_answered = ans;
    qa_correct_answer = correct;
}

void QuestionAnswer::setAnswered(QuestionItem::Answer ans) { qa_answered = ans; }

QuestionItem::Answer QuestionAnswer::answered() { return qa_answered; }

void QuestionAnswer::setCorrectAnswer(QuestionItem::Answer ans) { qa_correct_answer = ans; }

QuestionItem::Answer QuestionAnswer::correctAnswer() { return qa_correct_answer; }

bool QuestionAnswer::isAnsweredCorrectly()
{
    if (qa_answered == qa_correct_answer) { return true; }
    return false;
}
