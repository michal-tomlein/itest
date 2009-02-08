#ifndef QUESTION_ITEM_H
#define QUESTION_ITEM_H

#include <QString>
#include <QStringList>

class QuestionItem
{
public:
    QuestionItem();
    QuestionItem(QString);
    QuestionItem(QString, int, int, QString, QStringList);
    enum Answer { None, A, B, C, D };

public slots:
    QString name();
    int flag();
    int difficulty();
    QString text();
    QString ansA(); QString ansB(); QString ansC(); QString ansD();
    bool isAnsACorrect(); bool isAnsBCorrect();
    bool isAnsCCorrect(); bool isAnsDCorrect();
    QStringList answers();
    void setName(QString);
    void setFlag(int);
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
    int q_difficulty;
    QString q_text;
    QStringList q_answers;
    Answer q_answer;
};

#endif // QUESTION_ITEM_H
