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
    QuestionItem(QString, int, QString, int, QString, QStringList);
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
    QStringList answers();
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
    Answer answered(); void setAnswered(Answer);
    bool isAnsweredCorrectly();

private:
    QString q_name;
    int q_flag;
    QString q_group;
    int q_difficulty;
    QString q_text;
    QStringList q_answers;
    Answer q_answer;
};

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

#endif // QUESTION_ITEM_H
