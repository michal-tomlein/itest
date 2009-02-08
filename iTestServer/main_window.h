#include "ui_main_window_v2.h"
#include "archived_session.h"

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
    void updateGeometry(); void updateVSSGeometry();
    void updateTSTWGeometry(); void updatePMDTWGeometry();
    void onInfoDisplayChange(bool); void setPage(QAction *);
    bool saveChangesBeforeProceeding(QString, bool);
    void setProgress(int); void setNullProgress();
    uint searchListWidgetItems(QString, QListWidget *, QLineEdit *);
    uint searchTableWidgetItems(QString, QTableWidget *, QLineEdit *);
    void checkForUpdates(); void httpRequestFinished(bool);
    void openDocumentation();
    void statsWidgetClosed();
    void changeLanguage(); void langChanged();
    void hideArchive();
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
    void overallStatistics(); void statsAdjustAll();
    void statsAdjust(QAbstractButton *);
    // FLAGS-RELATED
    void setupFlagsPage();
    void setFlags(); void loadFlags(); void applyFlags(); void discardFlags();
    void setFlagLineEditPalettes(); void updateFlags(QAbstractButton *);
    void updateFlagQnums(); void checkForUnflaggedQuestions();
    QColor backgroundColourForFlag(int);
    QColor foregroundColourForFlag(int, bool = false);
    // CLEAR & REMOVE & SUBSTITUTE
    void clearAll(); void clearSQ(); void clearSQNoFlags(); void clearDBI();
    void clearCurrentValues(); QString removeLineBreaks(QString);
    void clearSMSC(); QString substituteHtmlTags(QString);
    void clearSM(); void clearVSS(); void clearLQ();
    // QUESTION-RELATED
    void setCurrentQuestion();
    void addQuestion(); void deleteQuestion(); void duplicateQuestion();
    void applyQuestionChanges(); void discardQuestionChanges();
    void setQuestionItemColour(QListWidgetItem *, int);
    void setQuestionItemIcon(QListWidgetItem *, int);
    QIcon iconForDifficulty(int);
    void sortQuestionsAscending(); void sortQuestionsDescending();
    void sortQuestions(Qt::SortOrder);
    void hideQuestion(); void hideQuestion(QListWidgetItem *, QuestionItem *);
    void moveUp(); void moveDown(); void moveToTop(); void moveToBottom();
    void moveQuestion(int); void adjustQuestionDifficulty();
    void filterLQ(QAbstractButton *); void filterLQFlagChanged();
    void filterLQAction(QAction *); void filterLQSearch();
    void updateTestQnum(); void searchByGroup();
    uint numOccurrences(QString); uint replaceAllOccurrences(QString, QString);
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
	void resizeEvent(QResizeEvent *);
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
	// LANG
	QComboBox * langComboBox;
	// FLAG-WIDGETS
	QLineEdit * EFFlagLineEdit[20];
	QCheckBox * EFFlagCheckBox[20];
	QLabel * EFFlagQnumLabel[20];
	// PALETTES
	QPalette search_active_palette; QPalette search_noresults_palette;
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
