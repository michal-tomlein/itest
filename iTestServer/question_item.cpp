#include "question_item.h"

QuestionItem::QuestionItem()
{
     q_name = "";
     q_flag = -1;
     q_group = "";
     q_difficulty = 0;
     q_text = "";
     q_answers << "" << "false" << "" << "false" << "" << "false" << "" << "false";
     q_incorrectanscount = 0;
     q_correctanscount = 0;
     q_hidden = false;
}

QuestionItem::QuestionItem(QString name)
{
     q_name = name;
     q_flag = -1;
     q_group = "";
     q_difficulty = 0;
     q_text = "";
     q_answers << "" << "false" << "" << "false" << "" << "false" << "" << "false";
     q_incorrectanscount = 0;
     q_correctanscount = 0;
     q_hidden = false;
}

QuestionItem::QuestionItem(QString name, int flag, QString group, int difficulty, QString text, QStringList answers, unsigned int inccount, unsigned int ccount, bool hidden)
{
     q_name = name;
     q_flag = flag;
     q_group = group;
     q_difficulty = difficulty;
     setText(text);
     q_answers = answers;
     q_incorrectanscount = inccount;
     q_correctanscount = ccount;
     q_hidden = hidden;
}

QString QuestionItem::name() { return q_name; }

int QuestionItem::flag() { return q_flag; }

QString QuestionItem::group() { return q_group; }

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

bool QuestionItem::isHidden() { return q_hidden; }

QStringList QuestionItem::answers() { return q_answers; }

QString QuestionItem::allProperties()
{
    QString out;
    // Q_NAME
    out.append("[Q_NAME]\n");
    out.append(q_name);
    // Q_FLAG
    out.append("\n[Q_FLAG]\n");
    out.append(QString("%1").arg(q_flag));
    // Q_GROUP
    out.append("\n[Q_GRP]\n");
    out.append(q_group);
    // Q_DIFFICULTY
    out.append("\n[Q_DIF]\n");
    out.append(QString("%1").arg(q_difficulty));
    // Q_TEXT
    out.append("\n[Q_TEXT]\n");
    out.append(q_text);
    // Q_ANSA
    out.append("\n[Q_ANSA]\n");
    out.append(q_answers.at(0));
    out.append("\n");
    out.append(q_answers.at(1));
    // Q_ANSB
    out.append("\n[Q_ANSB]\n");
    out.append(q_answers.at(2));
    out.append("\n");
    out.append(q_answers.at(3));
    // Q_ANSC
    out.append("\n[Q_ANSC]\n");
    out.append(q_answers.at(4));
    out.append("\n");
    out.append(q_answers.at(5));
    // Q_ANSD
    out.append("\n[Q_ANSD]\n");
    out.append(q_answers.at(6));
    out.append("\n");
    out.append(q_answers.at(7));
    // STATISTICS
    out.append("\n[Q_ICCNT]\n");
    out.append(QString("%1\n%2").arg(q_incorrectanscount).arg(q_correctanscount));
    // Q_HIDDEN
    out.append("\n[Q_HID]\n");
    out.append(q_hidden ? "true" : "false");
    // Q_END
    out.append("\n[Q_END]");
    return out;
}

QString QuestionItem::allPublicProperties()
{
    QString out;
    // Q_NAME
    out.append("[Q_NAME]\n");
    out.append(q_name);
    // Q_FLAG
    out.append("\n[Q_FLAG]\n");
    out.append(QString("%1").arg(q_flag));
    // Q_GROUP
    out.append("\n[Q_GRP]\n");
    out.append(q_group);
    // Q_DIFFICULTY
    out.append("\n[Q_DIF]\n");
    out.append(QString("%1").arg(q_difficulty));
    // Q_TEXT
    out.append("\n[Q_TEXT]\n");
    out.append(q_text);
    // Q_ANSA
    out.append("\n[Q_ANSA]\n");
    out.append(q_answers.at(0));
    // Q_ANSB
    out.append("\n[Q_ANSB]\n");
    out.append(q_answers.at(2));
    // Q_ANSC
    out.append("\n[Q_ANSC]\n");
    out.append(q_answers.at(4));
    // Q_ANSD
    out.append("\n[Q_ANSD]\n");
    out.append(q_answers.at(6));
    // Q_END
    out.append("\n[Q_END]");
    return out;
}

/*bool QuestionItem::allPropertiesToDataStream(QDataStream & out)
{
    // Q_NAME
    out << QString("[Q_NAME]") << (quint16)0;
    out << q_name << (quint16)0;
    // Q_FLAG
    out << QString("[Q_FLAG]") << (quint16)0;
    out << QString("%1").arg(q_flag) << (quint16)0;
    // Q_DIFFICULTY
    out << QString("[Q_DIFFICULTY]") << (quint16)0;
    out << QString("%1").arg(q_difficulty) << (quint16)0;
    // Q_TEXT
    out << QString("[Q_TEXT]") << (quint16)0;
    out << q_text << (quint16)0;
    // Q_ANSA
    out << QString("[Q_ANSA]") << (quint16)0;
    out << q_answers.at(0) << (quint16)0;
    out << QString("[Q_ANSA_C]") << (quint16)0;
    out << q_answers.at(1) << (quint16)0;
    // Q_ANSB
    out << QString("[Q_ANSB]") << (quint16)0;
    out << q_answers.at(2) << (quint16)0;
    out << QString("[Q_ANSB_C]") << (quint16)0;
    out << q_answers.at(3) << (quint16)0;
    // Q_ANSC
    out << QString("[Q_ANSC]") << (quint16)0;
    out << q_answers.at(4) << (quint16)0;
    out << QString("[Q_ANSC_C]") << (quint16)0;
    out << q_answers.at(5) << (quint16)0;
    // Q_ANSD
    out << QString("[Q_ANSD]") << (quint16)0;
    out << q_answers.at(6) << (quint16)0;
    out << QString("[Q_ANSD_C]") << (quint16)0;
    out << q_answers.at(7) << (quint16)0;
    // Q_END
    out << QString("[Q_END]") << (quint16)0;
    return true;
}*/

void QuestionItem::setName(QString name) { q_name = name; }

void QuestionItem::setFlag(int flag) { q_flag = flag; }

void QuestionItem::setGroup(QString group) { q_group = group; }

void QuestionItem::setDifficulty(int difficulty) { q_difficulty = difficulty; }

void QuestionItem::setText(QString text)
{
	QTextDocument doc; doc.setHtml(text); QString final = text; QString lastgood; QTextDocument testdoc;
	QStringList before; QString after = "font-size:10pt;";
	before << "font-size:8.25pt;" << "font-size:8pt;" << "font-size:9pt;";
	for (int b = 0; b < before.count(); ++b) {
		int skip = 0; int c = text.count(before.at(b), Qt::CaseInsensitive);
		for (int i = 0; i < c; ++i) {
			lastgood = final;
			if (final.contains(before.at(b), Qt::CaseInsensitive)) {
				final.replace(final.indexOf(before.at(b), skip), before.at(b).count(), after);
				testdoc.setHtml(final);
				if (doc.toPlainText() != testdoc.toPlainText()) {
					skip = final.indexOf(before.at(b), skip) + 10;
					final = lastgood;
				}
			}
		}
	}
	q_text = final;
}

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

void QuestionItem::setIncorrectAnsCount(unsigned int count) { q_incorrectanscount = count; }

void QuestionItem::setCorrectAnsCount(unsigned int count) { q_correctanscount = count; }

void QuestionItem::setHidden(bool hidden) { q_hidden = hidden; }

unsigned int QuestionItem::incorrectAnsCount() { return q_incorrectanscount; }

unsigned int QuestionItem::correctAnsCount() { return q_correctanscount; }

void QuestionItem::addIncorrectAns() { q_incorrectanscount++; }

void QuestionItem::addCorrectAns() { q_correctanscount++; }

int QuestionItem::recommendedDifficulty()
{
    if (q_correctanscount == 0 && q_incorrectanscount == 0) { return -1; }
    else if (q_correctanscount == 0 && q_incorrectanscount > 0) { return 2; }
    long double ansratio = (long double)q_incorrectanscount / (long double)q_correctanscount;
    if (ansratio <= 0.5) { return 0; }
    else if (ansratio > 0.5 && ansratio < 2.0) { return 1; }
    else if (ansratio >= 2.0) { return 2; }
    return -1;
}
