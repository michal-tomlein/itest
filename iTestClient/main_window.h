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

#include "ui_main_window.h"

#include <QFile>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QButtonGroup>
#include <QTcpSocket>
#include <QProgressDialog>
#include <QHeaderView>
#include <QTimer>
#include <QTime>
#include <QSettings>
#include <QTranslator>
#include <QSvgWidget>
#include <QSvgRenderer>

#include "question_widget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    // UI-RELATED
    void varinit();
    void about();
    void errorInvalidData();
    void getReady();
    void start();
    void browse_i(); void browse_o();
    void loadSettings(); void saveSettings();
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
    void finish(); void sendResults();
    void readResults(QString); void loadResults(QTableWidget *);
    void newTest();
    // QUESTION-RELATED
    void setCurrentQuestion();
    void nextQuestion(); void lastQuestion();
    void setQuestionAnswered(Question::Answers);
    void previewSvg(QString);

private:
    // CURRENT DB
    QString current_db_name;
    QString current_db_date;
    QString current_db_comments;
    QList<QuestionItem *> current_db_questions;
    bool current_db_multiple_ans_support;
    bool current_db_itdb1_4_support;
    // CURRENT TEST
    int current_test_qnum;
    QMap<QListWidgetItem *, QuestionItem *> current_test_questions;
    int current_test_time_remaining;
    float current_test_score;
    bool current_test_results_sent;
    QString current_test_date;
    QString current_test_time_finished;
    PassMark current_test_passmark;
    bool current_test_use_groups;
    bool current_connection_local;
    // FLAGS
    bool current_db_fe[20]; QString current_db_f[20];
    // UI-RELATED
    QTimer timer;
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
    QTableWidgetItem * ITW_test_passmark;
    QTextBrowser * ITW_test_comments;
    // ITEST & DB VERSION
    QString ver; float f_ver; QString itos_ver; float f_itos_ver;
};
