#ifndef QUESTION_ITEM_H
#define QUESTION_ITEM_H

#include <QString>
#include <QStringList>
#include <QTextDocument>

class QuestionItem
{
public:
    QuestionItem();
    QuestionItem(QString);
    QuestionItem(QString, int, QString, int, QString, QStringList, unsigned int, unsigned int, bool);
    enum Answer { None, A, B, C, D };

public slots:
    QString name();
    int flag();
    QString group();
    int difficulty();
    QString text();
    QString ansA(); QString ansB(); QString ansC(); QString ansD();
    bool isAnsACorrect(); bool isAnsBCorrect();
    bool isAnsCCorrect(); bool isAnsDCorrect();
    bool isHidden();
    QStringList answers();
    QString allProperties(); QString allPublicProperties();
    void setName(QString);
    void setFlag(int);
    void setGroup(QString);
    void setDifficulty(int);
    void setText(QString);
    void setAnsA(QString); void setAnsB(QString);
    void setAnsC(QString); void setAnsD(QString);
    void setAnsACorrect(bool); void setAnsBCorrect(bool);
    void setAnsCCorrect(bool); void setAnsDCorrect(bool);
    void setAnswers(QStringList);
    bool hasCorrectAnswer();
    void setIncorrectAnsCount(unsigned int);
    void setCorrectAnsCount(unsigned int);
    void setHidden(bool);
    unsigned int incorrectAnsCount(); unsigned int correctAnsCount();
    void addIncorrectAns(); void addCorrectAns();
    int recommendedDifficulty();

private:
    QString q_name;
    int q_flag;
    QString q_group;
    int q_difficulty;
    QString q_text;
    QStringList q_answers;
    unsigned int q_incorrectanscount;
    unsigned int q_correctanscount;
    bool q_hidden;
};

#endif // QUESTION_ITEM_H
