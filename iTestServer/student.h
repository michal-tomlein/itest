#ifndef STUDENT_H
#define STUDENT_H

#include "client.h"

class Student : public QObject
{
    Q_OBJECT

public:
    Student();
    Student(int);
    Student(QString);
    Student(int, QString);
    Student(Client *);
    Student(Student *);
    virtual ~Student();

public slots:
    void setName(QString); QString name();
    void setNumber(int); int number();
    void setScore(int); int score();
    void setReady(bool); bool isReady();
    void setResults(QMap<QString, QuestionAnswer> *);
    QMap<QString, QuestionAnswer> * results();
    void setPassed(bool); bool passed();
    /*bool loadStudentData(QString);*/
    QString studentData(); QString studentArchiveData();
    bool wasAsked(QString); uint replaceOccurrences(QString, QString);

private:
    QString s_name;
    int s_score;
    bool s_ready;
    QMap<QString, QuestionAnswer> * s_results;
    int s_number;
    bool s_passed;
};

#endif // STUDENT_H
