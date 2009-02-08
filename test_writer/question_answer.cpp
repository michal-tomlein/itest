#include "main_window.h"

QuestionAnswer::QuestionAnswer()
{
    qa_answered = QuestionItem::None;
    qa_correct_answer = QuestionItem::None;
}

QuestionAnswer::QuestionAnswer(QuestionItem::Answer correct, QuestionItem::Answer ans)
{
    qa_answered = ans;
    qa_correct_answer = correct;
}

void QuestionAnswer::setAnswered(QuestionItem::Answer ans) { qa_answered = ans; }

QuestionItem::Answer QuestionAnswer::answered() { return qa_answered; }

void QuestionAnswer::setCorrectAnswer(QuestionItem::Answer ans) { qa_correct_answer = ans; }

QuestionItem::Answer QuestionAnswer::correctAnswer() { return qa_correct_answer; }

bool QuestionAnswer::isAnsweredCorrectly()
{
    if (qa_answered == qa_correct_answer) { return true; }
    return false;
}
