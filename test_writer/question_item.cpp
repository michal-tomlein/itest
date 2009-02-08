#include "question_item.h"

QuestionItem::QuestionItem()
{
     q_name = "";
     q_flag = -1;
     q_difficulty = 0;
     q_text = "";
     q_answers << "" << "false" << "" << "false" << "" << "false" << "" << "false";
     q_answer = None;
}

QuestionItem::QuestionItem(QString name)
{
     q_name = name;
     q_flag = -1;
     q_difficulty = 0;
     q_text = "";
     q_answers << "" << "false" << "" << "false" << "" << "false" << "" << "false";
     q_answer = None;
}

QuestionItem::QuestionItem(QString name, int flag, int difficulty, QString text, QStringList answers)
{
     q_name = name;
     q_flag = flag;
     q_difficulty = difficulty;
     q_text = text;
     q_answers = answers;
     q_answer = None;
}

QString QuestionItem::name() { return q_name; }

int QuestionItem::flag() { return q_flag; }

int QuestionItem::difficulty() { return q_difficulty; }

QString QuestionItem::text() { return q_text; }

QString QuestionItem::ansA() { return q_answers.at(0); }

QString QuestionItem::ansB() { return q_answers.at(2); }

QString QuestionItem::ansC() { return q_answers.at(4); }

QString QuestionItem::ansD() { return q_answers.at(6); }

bool QuestionItem::isAnsACorrect()
{
     if (q_answers.at(1) == "true") {return true;} else {return false;}
}

bool QuestionItem::isAnsBCorrect()
{
     if (q_answers.at(3) == "true") {return true;} else {return false;}
}

bool QuestionItem::isAnsCCorrect()
{
     if (q_answers.at(5) == "true") {return true;} else {return false;}
}

bool QuestionItem::isAnsDCorrect()
{
     if (q_answers.at(7) == "true") {return true;} else {return false;}
}

QStringList QuestionItem::answers() { return q_answers; }

void QuestionItem::setName(QString name) { q_name = name; }

void QuestionItem::setFlag(int flag) { q_flag = flag; }

void QuestionItem::setDifficulty(int difficulty) { q_difficulty = difficulty; }

void QuestionItem::setText(QString text) { q_text = text; }

void QuestionItem::setAnsA(QString ans) { q_answers.replace(0, ans); }

void QuestionItem::setAnsB(QString ans) { q_answers.replace(2, ans); }

void QuestionItem::setAnsC(QString ans) { q_answers.replace(4, ans); }

void QuestionItem::setAnsD(QString ans) { q_answers.replace(6, ans); }

void QuestionItem::setAnsACorrect(bool correct)
{
     if (correct) {q_answers.replace(1, "true");} else {q_answers.replace(1, "false");}
}

void QuestionItem::setAnsBCorrect(bool correct)
{
     if (correct) {q_answers.replace(3, "true");} else {q_answers.replace(3, "false");}
}

void QuestionItem::setAnsCCorrect(bool correct)
{
     if (correct) {q_answers.replace(5, "true");} else {q_answers.replace(5, "false");}
}

void QuestionItem::setAnsDCorrect(bool correct)
{
     if (correct) {q_answers.replace(7, "true");} else {q_answers.replace(7, "false");}
}

void QuestionItem::setAnswers(QStringList answers) { q_answers = answers; }

bool QuestionItem::hasCorrectAnswer()
{
    if ((q_answers.at(1) == "true") || (q_answers.at(3) == "true") ||
        (q_answers.at(5) == "true") || (q_answers.at(7) == "true"))
    { return true; }
    return false;
}

QuestionItem::Answer QuestionItem::answered() { return q_answer; }

void QuestionItem::setAnswered(Answer ans) { q_answer = ans; }

bool QuestionItem::isAnsweredCorrectly()
{
    if (hasCorrectAnswer()) {
        switch (q_answer) {
            case None: return false; break;
            case A: return (isAnsACorrect()); break;
            case B: return (isAnsBCorrect()); break;
            case C: return (isAnsCCorrect()); break;
            case D: return (isAnsDCorrect()); break;
        }
    } else {
        switch (q_answer) {
            case None: return true; break;
            case A: return false; break;
            case B: return false; break;
            case C: return false; break;
            case D: return false; break;
        }
    }
    return false;
}
