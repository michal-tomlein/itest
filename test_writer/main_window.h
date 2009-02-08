#include "ui_main_window.h"

#include <QFile>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QButtonGroup>
#include <QTcpSocket>
#include <QProgressDialog>
#include <QTextStream>
#include <QHeaderView>
#include <QTimer>
#include <QTime>
#include <QSettings>
#include <QTranslator>

#include "question_item.h"

class QuestionAnswer
{
public:
    QuestionAnswer();
    QuestionAnswer(QuestionItem::Answer, QuestionItem::Answer);
    void setAnswered(QuestionItem::Answer); QuestionItem::Answer answered();
    void setCorrectAnswer(QuestionItem::Answer); QuestionItem::Answer correctAnswer();
    bool isAnsweredCorrectly();

private:
    QuestionItem::Answer qa_answered;
    QuestionItem::Answer qa_correct_answer;
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    // UI-RELATED
    void varinit();
    void about();
    void updateGeometry();
    void errorInvalidData();
    void getReady();
    void start();
    void browse_i(); void browse_o();
    // ENABLE, DISABLE, TOGGLE
    void enableConnectButton();
    void enableLoadButton();
    void toggleInputType(QAbstractButton *);
    void enableInputTypeSelection();
    void disableInputTypeSelection();
    // SOCKET-RELATED
    void connectSocket();
    void readIncomingData();
    void displayError(QAbstractSocket::SocketError);
    // TEST-RELATED
    void loadTest(QString); void loadFile(); void loadFile(QString);
    void randomlySelectQuestions();
    void updateTime();
    void finish(); void sendResults(); void saveResults();
    void loadResults(QMap<QString, QuestionAnswer> *, QTableWidget *);
    void readResults(QString);
    // QUESTION-RELATED
    void setCurrentQuestion();
    void nextQuestion(); void lastQuestion();
    void setQuestionAnswered(QAbstractButton *);

private:
    // CURRENT DB
    QString current_db_name;
    QString current_db_date;
    QString current_db_comments;
    QList<QuestionItem *> current_db_questions;
    // CURRENT TEST
    int current_test_qnum;
    QMap<QListWidgetItem *, QuestionItem *> current_test_questions;
    QMap<QString, QuestionAnswer> * current_test_results;
    int current_test_time_remaining;
    int current_test_score;
    bool current_test_results_sent;
    QString current_test_date;
    QString current_test_time_finished;
    // FLAGS
    bool current_db_fe[20]; QString current_db_f[20];
    // UI-RELATED
    void closeEvent(QCloseEvent*);
    QButtonGroup * rbtngrpInputType;
    QProgressDialog * progress_dialog;
    QButtonGroup * rbtngrpAnswer;
    // SOCKET-RELATED
    QTcpSocket * tcpSocket;
    quint64 blocksize;
    quint32 client_number;
    bool test_loaded;
    quint64 num_entries;
    quint64 current_entry;
    QString received_data;
    // INFOTABLEWIDGET CELL POINTERS
    QTableWidgetItem * ITW_test_name;
    QTableWidgetItem * ITW_test_date;
    QTableWidgetItem * ITW_test_timestamp;
    QTableWidgetItem * ITW_test_time;
    QTableWidgetItem * ITW_test_qnum;
    QTableWidgetItem * ITW_test_fnum;
    QTableWidgetItem * ITW_test_flags;
    QTextBrowser * ITW_test_comments;
    // ITEST & DB VERSION
    QString ver; float f_ver; QString db_ver; float f_db_ver;
};
