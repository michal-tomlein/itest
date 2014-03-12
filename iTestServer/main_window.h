/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2014 Michal Tomlein

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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "defs.h"

#include "ui_main_window_v2.h"
#include "pass_mark.h"

#include <QTcpSocket>

class Class;
class ClassMember;
class Client;
class PrintQuestionsDialogue;
class Session;
class Student;

class QNetworkAccessManager;
class QNetworkReply;
class QPrinter;
class QTcpServer;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void openFile(const QString &);

    static QString databaseNameForFilePath(const QString &filePath);
    QString currentDatabaseName();

private slots:
    // UI-RELATED
    void quit();
    void about();
    void addRecent(const QString &);
    void loadSettings();
    void saveSettings();
    void setPage(QAction *);
    void currentPageChanged(int);
    bool saveChangesBeforeProceeding(const QString &, bool);
    void setProgress(int);
    void setNullProgress();
    void checkForUpdates(bool silent = false);
    void httpRequestFinished(QNetworkReply *reply);
    void httpRequestFailed(bool silent);
    void openDocumentation();
    void changeLanguage();
    void langChanged();
    void previewSvg(QListWidgetItem *);
    // ENABLE/DISABLE
    void setAllEnabled(bool);
    void setSQEnabled(bool);
    void setLQToolsEnabled(bool);
    void setEQToolsEnabled(bool);
    void setSMSCEnabled(bool);
    void enableSMTools();
    void enableSVTools();
    void enableTools();
    void setCLSCEnabled(bool);
    void setCLSSEnabled(bool);
    // DATABASE-RELATED
    void newDB();
    void closeDB();
    void openDB(const QString &, bool = false);
    void open();
    void openRecent();
    void openRecent(QListWidgetItem *);
    void saveDB(const QString &, bool = false);
    void save();
    void saveAs();
    void saveCopy();
    void exportCSV();
    void setDatabaseModified();
    // STATS
    void overallStatistics();
    void statsAdjustAll();
    void statsAdjust(QAbstractButton *);
    void statsWidgetClosed();
    // PRINT QUESTIONS
    void showPrintQuestionsDialogue();
    // CATEGORIES-RELATED
    void addCategoryItem(int);
    void removeCategoryItem(int);
    void setupCategoriesPage();
    void setCategoryEnabled(QTreeWidgetItem *);
    void setCategories();
    void loadCategories();
    void applyCategories();
    void discardCategories();
    void updateCategories(QAbstractButton *);
    void updateCategoryQnums();
    void checkForUncategorisedQuestions();
    int qnumForCategory(int, bool = false);
    static QColor backgroundColourForCategory(int);
    static QColor foregroundColourForCategory(int, bool = false);
    // CLEAR
    void clearAll();
    void clearCurrentValues();
    void clearLQ();
    void clearSQ();
    void clearSQNoCategories();
    void clearSM();
    void clearSMSC();
    void clearSV();
    void clearCL();
    void clearCLSC();
    void clearCLSS();
    // QUESTION-RELATED
    void setCurrentQuestion();
    void addQuestion();
    void deleteQuestion();
    void duplicateQuestion();
    void applyQuestionChanges();
    void discardQuestionChanges();
    static void setQuestionItemColour(QListWidgetItem *, int);
    static void setQuestionItemIcon(QListWidgetItem *, int);
    static QIcon iconForDifficulty(int);
    void sortQuestionsAscending();
    void sortQuestionsDescending();
    void sortQuestionsByCategory();
    void sortQuestions(Qt::SortOrder, bool = false);
    void hideQuestion();
    void hideQuestion(QListWidgetItem *, QuestionItem *);
    void moveUp();
    void moveDown();
    void moveToTop();
    void moveToBottom();
    void moveQuestion(int);
    void adjustQuestionDifficulty();
    void filterLQ(QAbstractButton *);
    void filterLQCategoryChanged();
    void filterLQAction(QAction *);
    void filterLQSearch();
    void searchByGroup();
    uint numOccurrences(const QString &);
    uint replaceAllOccurrences(const QString &, const QString &);
    void addSvg();
    void removeSvg();
    void exportSvg();
    void editSvg();
    void browseForSvg();
    void applySvgChanges();
    void currentSvgChanged();
    // SERVER-RELATED
    void setupServer();
    void reloadAvailableItems();
    bool isDifficultyIncluded(int difficulty);
    void updateTestQnum();
    void updateTestQnum(bool, bool, bool, QSpinBox *, QTableWidget *);
    void updateTestTime();
    int numIdentifiedClients();
    void toggleStartServerEnabled();
    void toggleSaveSessionEnabled();
    void resetScoringSystemValues(bool);
    static QTableWidgetItem * toTableItem(QListWidgetItem *, bool = false);
    static QListWidgetItem * toListItem(QTableWidgetItem *);
    void addToList();
    void removeFromList();
    void startServer();
    void stopServer();
    void addClient();
    void setCurrentClient();
    void setClientStatus(QAbstractSocket::SocketState);
    void clientIdentified(Client *);
    void clientFinished(Client *);
    void clientResultsLoaded(Client *);
    void clientDisconnected(Client *);
    void loadClientResults(QMap<QString, QuestionAnswer> *);
    void loadClientResults(QMap<QString, QuestionAnswer> *, QTableWidget *, ScoringSystem);
    void addOfflineClient();
    bool addOfflineClient(const QString &);
    void addOfflineClients();
    void updateLC(Client *);
    void sendCorrectAnswers(Client *);
    void exportTest();
    void exportLog();
    void runTestWriter();
    // PRINTER-RELATED
    bool loadPrinterSettings();
    void savePrinterSettings();
    bool printerConfiguration(QString &);
    bool configurePrinter(bool);
    bool loadPrinterConfiguration();
    bool printClientResults(Client *, QPrinter *);
    bool printStudentResults(Student *, QPrinter *, const QString &, ScoringSystem);
    void print();
    void quickPrint();
    void togglePrintEnabled();
    void printAll();
    void printSessionSummary();
    bool printSessionSummary(Session *, QPrinter *);
    void printQuestions(PrintQuestionsDialogue *);
    void printClassSummary();
    bool printClassSummary(Class *, QPrinter *);
    QString htmlForQuestion(QuestionItem *, int, QTextDocument &, bool = false, bool = true, bool = true, bool = false);
    QString htmlForClassMember(ClassMember *);
    // SESSIONVIEWER-RELATED
    void setupSessionViewer();
    void setCurrentSession(QListWidgetItem *);
    void setCurrentStudent();
    void selectSessionItem(QListWidgetItem *);
    void loadStudentResults(QMap<QString, QuestionAnswer> *);
    void deleteLog();
    void searchSVLS(const QString &);
    void clearSVSC();
    void showSelectedStudentTab();
    // CLASSVIEWER-RELATED
    void setupClassViewer();
    void setCurrentClass(QListWidgetItem *);
    void setCurrentClassMember(QListWidgetItem *);
    void updateClassProperties();
    void updateClassMemberName();
    void addClass();
    void deleteClass();
    void addStudent();
    void deleteStudent();
    void addSession();
    void deleteSession();
    void viewSession(QListWidgetItem *);
    void viewSessionAndStudent(QTableWidgetItem *);
    void toggleAddSessionToMemberEnabled();
    void addSessionToMember();
    void removeSessionFromMember();
    QDialog * createAddSessionDialogue(const QString &, MTListWidget * = NULL);
    // ERROR MESSAGES
    void errorInvalidDBFile(const QString &, const QString &, int);

private:
    // CURRENT DB
    bool current_db_open;
    QString current_db_file;
    QString current_db_comments;
    QString current_db_question;
    QMap<QListWidgetItem *, QuestionItem *> current_db_questions;
    QMap<QDateTime, Session *> current_db_sessions;
    QMap<QListWidgetItem *, Class *> current_db_classes;
    QMap<QListWidgetItem *, Student *> current_db_students;
    QMap<int, int> current_db_categoryentries;
    Session * current_db_session;
    Class * current_db_class;
    PassMark current_db_passmark;
    // CATEGORIES
    QVector<bool> current_db_categories_enabled;
    QVector<QString> current_db_categories;
    // UI-RELATED
    void closeEvent(QCloseEvent *);
    QNetworkAccessManager *network_access_manager;
    // SERVER-RELATED
    QTcpServer * tcpServer;
    QMap<QListWidgetItem *, Client *> current_db_clients;
    QList<QByteArray> current_db_test;
    QString current_db_testdate;
    QString current_db_testname;
    ScoringSystem current_db_scoringsystem;
    // PRINTER-RELATED
    QPrinter * default_printer;
    bool printer_configured;
    int printer_colorMode;
    bool printer_doubleSidedPrinting;
    bool printer_fontEmbeddingEnabled;
    int printer_numCopies;
    int printer_orientation;
    QString printer_outputFileName;
    int printer_outputFormat;
    int printer_pageOrder;
    int printer_pageSize;
    int printer_paperSource;
    int printer_printRange;
    QString printer_printerName;
    int printer_resolution;
    // WIDGETS
    QProgressBar * progressBar;
    QButtonGroup * rbtngrpFilterLQ;
    QActionGroup * actgrpFilterLQ;
    QActionGroup * actgrpPage;
    QButtonGroup * rbtngrpAdvSelect;
    QButtonGroup * rbtngrpTestTime;
    QPushButton * btnApply;
    QPushButton * btnDiscard;
    // STATS
    MTTableWidget * stats_tw;
    QPushButton * stats_btn_adjustall;
    QMap<QAbstractButton *, QuestionItem *> stats_qmap;
    QMap<QAbstractButton *, int> stats_twmap;
    QMap<QAbstractButton *, int> stats_lwmap;
    QButtonGroup * btngrpStatsAdjust;
    // EDIT SVG
    QLineEdit * editsvg_lineedit_name;
    QString editsvg_svgpath;
    // LANG
    QComboBox * langComboBox;
    // FLAG-WIDGETS
    QVector<QLineEdit *> EFCategoryLineEdit;
    // PALETTES
    SearchLineEditPalettes searchLineEditPalettes;

    QString docs_url;
    QMap<QString, QString> itest_i18n;
    // EXCEPTIONS
    class xInvalidDBFile {
    public:
        xInvalidDBFile(int e): x_error(e) {}
        int error() { return x_error; }
    private:
        int x_error;
    };
    // FRIENDS
    friend class Client;
    friend class Session;
    friend class PrintQuestionsDialogue;
};

#endif // MAIN_WINDOW_H
