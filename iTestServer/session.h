#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include <QColor>

#include "pass_mark.h"

class LogEntry
{
public:
    LogEntry();
    LogEntry(int, int, int, int, int, int, QString);
    QColor entryBackgroundColour(); QColor entryForegroundColour();
    QString entryText(); QString entryData();
    int bgRed(); int bgGreen(); int bgBlue();
    int fgRed(); int fgGreen(); int fgBlue();

private:
    int le_bgr; int le_bgg; int le_bgb;
    int le_fgr; int le_fgg; int le_fgb;
    QString le_text;
};

class ArchivedSession;

class Session : public QObject
{
    Q_OBJECT

public:
    Session();
    Session(ArchivedSession *);
    virtual ~Session();

public slots:
    void setName(QString); QString name();
    void setDateTimeFromString(QString); void setDateTime(QDateTime);
    QString dateTimeToString(); QDateTime dateTime();
    void addLogEntry(int, int, int, int, int, int, QString);
    int numLogEntries(); LogEntry logEntry(int); void deleteLog();
    void addStudent(Student *); int numStudents(); Student * student(int);
    uint numQuestions(); uint numCorrect();
    void setPassMark(PassMark); PassMark passMark(); void loadPassMark(QString);
    QString sessionData();
    bool mostPassed(); bool isArchived();
    void destruct();

private:
    QString s_name;
    QDateTime s_datetime;
    uint s_qnum;
    uint s_cqnum;
    QList<LogEntry> s_log;
    QList<Student *> s_students;
    bool s_archived;
    PassMark s_passmark;

    friend class ArchivedSession;
};

#endif // SESSION_H
