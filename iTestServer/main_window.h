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

#include "ui_main_window_v2.h"
#include "archived_session.h"
#include "mtadvancedgroupbox.h"

#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QDir>
#include <QSet>
#include <QList>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QStatusBar>
#include <QProgressBar>
#include <QTimer>
#include <QCloseEvent>
#include <QMap>
#include <QColorDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QPrinter>
#include <QPrintDialog>
#include <QHttp>
#include <QBuffer>
#include <QDesktopServices>
#include <QHeaderView>
#include <QTranslator>
#include <QQueue>
#include <QProcess>
#include <QSvgWidget>
#include <QUrl>

#include <QtAlgorithms>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
    
private slots:
    // UI-RELATED
    void varinit();
    void quit(); void about(); void addRecent(QString);
    void loadSettings(); void saveSettings();
    void onInfoDisplayChange(bool); void setPage(QAction *);
    bool saveChangesBeforeProceeding(QString, bool);
    void setProgress(int); void setNullProgress();
    uint searchListWidgetItems(QString, QListWidget *, QLineEdit *);
    uint searchTableWidgetItems(QString, QTableWidget *, QLineEdit *);
    void checkForUpdates(); void httpRequestFinished(bool);
    void openDocumentation();
    void changeLanguage(); void langChanged();
    void hideArchive();
    void previewSvg(QListWidgetItem *);
    // ENABLE
    void enableAll(); void enableSQ();
    void enableLQTools(); void enableEQTools();
    void enableSMSC(); void enableSMTools();
    void enableVSSTools();
    // DISABLE
    void disableAll(); void disableSQ();
    void disableLQTools(); void disableEQTools();
    void disableSMSC();
    // DATABASE-RELATED
    void newDB(); void closeDB();
    void openDB(QString, bool = false); void open();
    void openRecent(); void openRecent(QListWidgetItem *);
    void saveDB(QString, bool = false, bool = false);
    void save(); void saveAs(); void saveCopy(); void saveBackup();
    void setDatabaseModified();
    // STATS
    void overallStatistics(); void statsAdjustAll();
    void statsAdjust(QAbstractButton *); void statsWidgetClosed();
    void searchStatistics(const QString &);
    // PRINT QUESTIONS
    void showPrintQuestionsDialogue();
    void toggleAddRemoveQuestionToPrintEnabled();
    void searchQuestionsNotToPrint(const QString &);
    void searchQuestionsToPrint(const QString &);
    void addQuestionToPrint(); void removeQuestionToPrint();
    void addAllQuestionsToPrint();
    void togglePrintSelection(QAbstractButton *);
    // FLAGS-RELATED
    void setupFlagsPage();
    void setFlags(); void loadFlags(); void applyFlags(); void discardFlags();
    void setFlagLineEditPalettes(); void updateFlags(QAbstractButton *);
    void updateFlagQnums(); void checkForUnflaggedQuestions();
    static QColor backgroundColourForFlag(int);
    static QColor foregroundColourForFlag(int, bool = false);
    // CLEAR & REMOVE & SUBSTITUTE
    void clearAll(); void clearSQ(); void clearSQNoFlags(); void clearDBI();
    void clearCurrentValues(); static QString removeLineBreaks(QString);
    void clearSMSC(); static QString substituteHtmlTags(QString);
    void clearSM(); void clearVSS(); void clearLQ();
    // QUESTION-RELATED
    void setCurrentQuestion();
    void addQuestion(); void deleteQuestion(); void duplicateQuestion();
    void applyQuestionChanges(); void discardQuestionChanges();
    static void setQuestionItemColour(QListWidgetItem *, int);
    static void setQuestionItemIcon(QListWidgetItem *, int);
    static QIcon iconForDifficulty(int);
    void sortQuestionsAscending(); void sortQuestionsDescending();
    void sortQuestions(Qt::SortOrder);
    void hideQuestion(); void hideQuestion(QListWidgetItem *, QuestionItem *);
    void moveUp(); void moveDown(); void moveToTop(); void moveToBottom();
    void moveQuestion(int); void adjustQuestionDifficulty();
    void filterLQ(QAbstractButton *); void filterLQFlagChanged();
    void filterLQAction(QAction *); void filterLQSearch();
    void updateTestQnum(); void searchByGroup();
    uint numOccurrences(QString); uint replaceAllOccurrences(QString, QString);
    void addSvg(); void removeSvg(); void exportSvg();
    void editSvg(); void browseForSvg(); void applySvgChanges();
    void currentSvgChanged();
    // SERVER-RELATED
    void setupServer(); void reloadAvailableItems(); void setMaximumQuestions();
    void searchAvailableItems(const QString &); void updateTestTime();
    void searchUsedItems(const QString &); int numIdentifiedClients();
    void toggleSaveSessionEnabled(); void setMaximumPassMark(int);
    //void switchTestTimeSlotConnections(QAbstractButton *);
    void addToList(); void removeFromList(); void toggleAddRemoveEnabled();
    void startServer(); void stopServer(); void addClient();
    void setCurrentClient(); void setClientStatus(QAbstractSocket::SocketState);
    void clientIdentified(Client *); void clientFinished(Client *);
    void clientResultsLoaded(Client *); void clientDisconnected(Client *);
    void loadClientResults(QMap<QString, QuestionAnswer> *, QTableWidget *);
    void addOfflineClient(); bool addOfflineClient(QString);
    void addOfflineClients();
    void updateLC(Client *); void sendCorrectAnswers(Client *);
    void exportTest(); void exportLog();
    void runTestWriter();
    // PRINTER-RELATED
    bool loadPrinterSettings(); void savePrinterSettings();
    bool printerConfiguration(QString &); bool configurePrinter(bool);
    bool loadPrinterConfiguration();
    bool printClientResults(Client *, QPrinter *);
    bool printStudentResults(Student *, QPrinter *, QString);
    void print(); void quickPrint(); void togglePrintEnabled();
    void printAll(); void printSessionSummary();
    bool printSessionSummary(Session *, QPrinter *);
    void printQuestions();
    QString htmlForQuestion(QuestionItem *, QTextDocument &, bool = false, bool = true, bool = true, bool = false);
    // SESSIONVIEWER-RELATED
    void setupSessionViewer(); void setCurrentSession(QListWidgetItem *);
    void setCurrentStudent(); void selectSessionItem(QListWidgetItem *);
    void loadStudentResults(QMap<QString, QuestionAnswer> *, QTableWidget *);
    void deleteLog(); void archiveSession(); void restoreSession();
    void copyToArchive(); void copyFromArchive();
    void searchVSSLS(const QString &); void clearVSSSC();
    void searchPassMarkDetails(const QString &);
    void openArchive(); void showPassMarkDetails();
    // ERROR MESSAGES
    void errorInvalidDBFile(QString, QString);
    //void errorInvalidTempFileFormat(QString, QString);
    //void errorRWTempFile(QString, QString, QString, QFile&);
    //void errorInvalidTempFileFormat_SpecialChars(QString, QString);
    // TEXTEDIT-RELATED
    void setupTextEdits();
    void textBold(); void textUnderline(); void textItalic();
    void textFamily(const QString &f); void textSize(const QString &p);
    void textColor(); void textAlign(QAbstractButton *);
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

private:
    // CURRENT DB
    bool current_db_open;
    QString current_db_file;
    QString current_db_name;
    QString current_db_date;
    QString current_db_comments;
    QString current_db_question;
    QMap<QListWidgetItem *, QuestionItem *> current_db_questions;
    QMap<QDateTime, Session *> current_db_sessions;
    QMap<QDateTime, ArchivedSession *> current_db_archivedsessions;
    QMap<QListWidgetItem *, Student *> current_db_students;
    QMap<int, int> current_db_flagentries;
    Session * current_db_session;
    QQueue<ArchivedSession *> current_db_queued_sessions;
    PassMark current_db_passmark;
    // FLAGS
    bool current_db_fe[20]; QString current_db_f[20];
    // UI-RELATED
    void closeEvent(QCloseEvent *);
	QHttp * http; QBuffer * http_buffer;
	// TEXTEDIT-RELATED
	void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);
    QButtonGroup * tbtngrpAlign;
    QButtonGroup * tbtngrpECAlign;
    // SERVER-RELATED
    QTcpServer * tcpServer;
    QMap<QListWidgetItem *, Client *> current_db_clients;
    QList<QByteArray> current_db_test;
    QString current_db_testdate;
	QString current_db_testname;
    // PRINTER-RELATED
    QPrinter * default_printer; bool printer_configured;
	int printer_colorMode; bool printer_doubleSidedPrinting;
	bool printer_fontEmbeddingEnabled; int printer_numCopies;
	int printer_orientation; QString printer_outputFileName;
	int printer_outputFormat; int printer_pageOrder;
	int printer_pageSize; int printer_paperSource;
	int printer_printRange; QString printer_printerName;
	int printer_resolution;
    // WIDGETS
	QProgressBar * progressBar;
	QButtonGroup * rbtngrpFilterLQ;
	QActionGroup * actgrpFilterLQ;
	QActionGroup * actgrpPage;
	QButtonGroup * rbtngrpAdvSelect;
	QButtonGroup * rbtngrpTestTime;
	QPushButton * btnApply; QPushButton * btnDiscard;
	// STATS
	QTableWidget * stats_tw;
	QPushButton * stats_btn_adjustall;
	QMap<QAbstractButton *, QuestionItem *> stats_qmap;
	QMap<QAbstractButton *, int> stats_twmap;
	QMap<QAbstractButton *, int> stats_lwmap;
	QButtonGroup * btngrpStatsAdjust;
	ExtendedLineEdit * stats_search;
	// PRINT QUESTIONS DIALOGUE
	QListWidget * printq_excludelist;
	QListWidget * printq_includelist;
	ExtendedLineEdit * printq_search_excluded;
	ExtendedLineEdit * printq_search_included;
	QPushButton * printq_btn_print;
	QPushButton * printq_add;
	QPushButton * printq_remove;
	QButtonGroup * rbtngrpPrintqSelect;
	QCheckBox * printq_advanced_statistics;
	QCheckBox * printq_advanced_formatting;
	QCheckBox * printq_advanced_test;
	QCheckBox * printq_advanced_graphics;
	// EDIT SVG
	QLineEdit * editsvg_lineedit_name;
	QString editsvg_svgpath;
	// LANG
	QComboBox * langComboBox;
	// FLAG-WIDGETS
	QLineEdit * EFFlagLineEdit[20];
	QCheckBox * EFFlagCheckBox[20];
	QLabel * EFFlagQnumLabel[20];
	// PALETTES
	QPalette search_active_palette;
	QPalette search_noresults_palette;
	// ITEST & DB VERSION & ENVIRONMENT VARIABLES
	QString ver; float f_ver;
	QString itdb_ver; float f_itdb_ver;
	QString itos_ver; float f_itos_ver;
	QString itest_url; QString docs_url;
	QMap<QString, QString> itest_i18n;
	// EXCEPTIONS
	class xInvalidDBFile {};
	// FRIENDS
	friend class Client;
	friend class Session;
	friend class ArchivedSession;
};
