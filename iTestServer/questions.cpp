#include "main_window.h"

void MainWindow::addQuestion()
{
    bool ok; bool allright = true; addQuestion_start:
    QString q_name;
    if (allright) {q_name = QInputDialog::getText(this, tr("Add question"), tr("Question name:"), QLineEdit::Normal, tr("### New question"), &ok);
    } else {q_name = QInputDialog::getText(this, tr("Add question"), tr("A question with this name already exists.\nPlease choose a different name:"), QLineEdit::Normal, tr("### Another question"), &ok);}
    if (!ok || q_name.isEmpty()) { return; }
    /*for (int i = 0; i < LQListWidget->count(); ++i) {
        if (LQListWidget->item(i)->text() == q_name)
            { allright = false; goto addQuestion_start; }
    }*/
    QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
    while (q.hasNext()) { q.next();
    	if (q.value()->name() == q_name) { allright = false; goto addQuestion_start; break; }
    }
    QuestionItem * item = new QuestionItem (q_name);
    QListWidgetItem * q_item = new QListWidgetItem (q_name);
    current_db_questions.insert(q_item, item);
    q_item->setIcon(QIcon(QString::fromUtf8(":/images/images/easy.png")));
    LQListWidget->addItem(q_item);
    setDatabaseModified();
    int numflags = 0;
    for (int i = 0; i < 20; ++i) { if (current_db_fe[i]) { numflags++; } }
    if (numflags > 0) {
        item->setFlag(SQFlagComboBox->itemData(0).toInt());
        setQuestionItemColour(q_item, item->flag());
    }
}

void MainWindow::deleteQuestion()
{
    if (LQListWidget->currentIndex().isValid()) {
        switch (QMessageBox::information(this, tr("Delete question"), tr("Are you sure you want to delete question \"%1\"?").arg(LQListWidget->currentItem()->text()), tr("&Delete"), tr("Cancel"), 0, 1)) {
            case 0: // Delete
                break;
            case 1: // Cancel
                return; break;
        }
        QString q_name = current_db_question;
        delete current_db_questions.value(LQListWidget->currentItem());
        current_db_questions.remove(LQListWidget->currentItem());
        delete LQListWidget->currentItem();
        setCurrentQuestion();
        setDatabaseModified();
    } else {QMessageBox::information(this, tr("Delete question"), tr("Select a question to be deleted first."));}
}

void MainWindow::duplicateQuestion()
{
    if (LQListWidget->currentIndex().isValid()) {
        QString q_name = current_db_question;
        QuestionItem * item = current_db_questions.value(LQListWidget->currentItem());
        QStringList bufferlist = q_name.split(" ");
        int copynum = 0; QString buffer = bufferlist.at(bufferlist.count()-1);
        if ((buffer.at(0) == '[') && (buffer.at(buffer.length()-1) == ']')) {
            bool isInt;
            buffer.remove(0, 1);
            buffer.remove(buffer.length()-1, 1);
            copynum = buffer.toInt(&isInt);
            if (isInt) {
                copynum++;
                bufferlist.removeAt(bufferlist.count()-1);
                q_name = bufferlist.join(" ");
            }
        }
        bool ok; bool allright = true; addQuestion_start:
        QString new_q_name;
        if (allright) {new_q_name = QInputDialog::getText(this, tr("Duplicate question"), tr("Question name:"), QLineEdit::Normal, QString("%1 [%2]").arg(q_name).arg(copynum), &ok);
        } else {new_q_name = QInputDialog::getText(this, tr("Duplicate question"), tr("A question with this name already exists.\nPlease choose a different name:"), QLineEdit::Normal, QString("%1 [%2]").arg(q_name).arg(copynum), &ok);}
        if (ok && !new_q_name.isEmpty())
        {   
             /*for (int i = 0; i < LQListWidget->count(); ++i) {
                 if (LQListWidget->item(i)->text() == new_q_name)
                     { allright = false; copynum++; goto addQuestion_start; }
             }*/
             QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
             while (q.hasNext()) { q.next();
             	if (q.value()->name() == new_q_name) { allright = false; copynum++; goto addQuestion_start; break; }
             }
             QuestionItem * new_item = new QuestionItem (new_q_name, 
                                                    item->flag(), 
                                                    item->group(),
                                                    item->difficulty(), 
                                                    item->text(), 
                                                    item->answers(),
                                                    item->incorrectAnsCount(),
                                                    item->correctAnsCount(),
                                                    item->isHidden());
             QListWidgetItem * new_q_item = new QListWidgetItem (new_item->group().isEmpty() ? new_q_name : QString("[%1] %2").arg(new_item->group()).arg(new_q_name));
             current_db_questions.insert(new_q_item, new_item);
             setQuestionItemIcon(new_q_item, new_item->difficulty());
             setQuestionItemColour(new_q_item, new_item->flag());
             hideQuestion(new_q_item, new_item);
             LQListWidget->insertItem(LQListWidget->currentRow()+1, new_q_item);
             LQListWidget->setCurrentRow(LQListWidget->currentRow()+1);
             setDatabaseModified();
        }
    } else {QMessageBox::information(this, tr("Duplicate question"), tr("Select a question to be duplicated first."));}
}

void MainWindow::setCurrentQuestion()
{
    if (LQListWidget->currentIndex().isValid()) {
        enableSQ(); clearSQNoFlags(); enableLQTools();
        QuestionItem * item = current_db_questions.value(LQListWidget->currentItem());
        SQQuestionNameLineEdit->setText(item->name());
        SQGroupLineEdit->setText(item->group());
        SQFlagComboBox->setCurrentIndex(current_db_flagentries.value(item->flag()));
        LQFlagComboBox->setCurrentIndex(current_db_flagentries.value(item->flag()));
        SQDifficultyComboBox->setCurrentIndex(item->difficulty());
        SQQuestionTextEdit->setHtml(item->text());
        SQAnswerALineEdit->setText(item->ansA());
        if (item->isAnsACorrect()) {SQCorrectACheckBox->setChecked( true );} else {SQCorrectACheckBox->setChecked( false );}
        SQAnswerBLineEdit->setText(item->ansB());
        if (item->isAnsBCorrect()) {SQCorrectBCheckBox->setChecked( true );} else {SQCorrectBCheckBox->setChecked( false );}
        SQAnswerCLineEdit->setText(item->ansC());
        if (item->isAnsCCorrect()) {SQCorrectCCheckBox->setChecked( true );} else {SQCorrectCCheckBox->setChecked( false );}
        SQAnswerDLineEdit->setText(item->ansD());
        if (item->isAnsDCorrect()) {SQCorrectDCheckBox->setChecked( true );} else {SQCorrectDCheckBox->setChecked( false );}
        if (item->incorrectAnsCount() == 0 && item->correctAnsCount() == 0) {
            SQStatisticsLabel->setVisible(false);
        } else {
            QString dif;
            switch (item->recommendedDifficulty()) {
                case -1: dif = tr("unavailable"); break;
                case 0: dif = tr("easy"); break;
                case 1: dif = tr("medium"); break;
                case 2: dif = tr("difficult"); break;
                default: dif = tr("unavailable"); break;
            }
            SQStatisticsLabel->setText(tr("Statistics: number of <b>correct</b> answers: <b>%1</b>; number of <b>incorrect</b> answers: <b>%2</b>; difficulty: <b>%3</b>; <a href=\"adjust.difficulty\">adjust difficulty</a>").arg(item->correctAnsCount()).arg(item->incorrectAnsCount()).arg(dif));
            SQStatisticsLabel->setVisible(true);
        }
        actionHide->setChecked(item->isHidden());
        current_db_question = item->name();
    } else {
    	disableSQ(); clearSQNoFlags(); disableLQTools();
    }
}

void MainWindow::applyQuestionChanges()
{
	if (!LQListWidget->currentIndex().isValid()) { return; }
	QListWidgetItem * q_item = LQListWidget->currentItem();
	QuestionItem * item = current_db_questions.value(q_item);
	// CHECK GROUP
	QString q_group = removeLineBreaks(SQGroupLineEdit->text());
	int q_flag;
	if (SQFlagComboBox->count() != 0) {
		q_flag = SQFlagComboBox->itemData(SQFlagComboBox->currentIndex()).toInt();
	} else { q_flag = -1; }
	if (!q_group.isEmpty()) {
		QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
		while (q.hasNext()) { q.next();
			if (q.value()->group() == q_group) {
				if (q.value()->flag() != q_flag) {
	   	 			QMessageBox::information(this, tr("Apply changes"), tr("This group is used by one or more questions with a different flag.\nPlease choose a different group."));
	   	 			return;
	   	 		}
	   	 	}
		}
	}
	// CHECK FLAG
	/*if (item->flag() != q_flag && item->flag() != -1) {
		switch (QMessageBox::information(this, tr("iTestServer"), tr("It is strongly advised against changing the flag of a question.\nConsider duplicating the question and hiding the original instead.\nProceed only if you know what you are doing."), tr("&Change"), tr("Do &not change"), 0, 1)) {
			case 0: // Change
				break;
			case 1: // Do not change
				q_flag = item->flag();
				SQFlagComboBox->setCurrentIndex(current_db_flagentries.value(item->flag()));
				break;
		}
	}*/
	// CHECK NAME
    QString q_name = removeLineBreaks(SQQuestionNameLineEdit->text());
    if (current_db_question != q_name) {
        int n = 0;
        /*for (int i = 0; i < LQListWidget->count(); ++i) {
          if (LQListWidget->item(i)->text() == q_name) { n++; }
        }*/
        QMapIterator<QListWidgetItem *, QuestionItem *> q(current_db_questions);
        while (q.hasNext()) { q.next();
        	if (q.value()->name() == q_name) { n++; }
        }
        if (n > 0) { QMessageBox::critical(this, tr("Apply changes"), tr("A question with this name already exists.\nPlease choose a different name.")); return; }
		uint num_old = numOccurrences(current_db_question);
		uint num_new = numOccurrences(q_name);
		if (num_new != 0) {
			switch (QMessageBox::information(this, tr("iTestServer"), tr("This new name has been used before.\n%1 occurrences of a question with this name found in the saved sessions.\nChanging the name to this one will cause that this question will be used\ninstead of the no longer existent old one.\n%2 occurrences of the old name will also be updated.\nAre you sure you want to change the name?").arg(num_new).arg(num_old), tr("&Change"), tr("Do &not change"), 0, 1)) {
				case 0: // Change
					replaceAllOccurrences(current_db_question, q_name);
					break;
				case 1: // Do not change
					q_name = current_db_question;
					SQQuestionNameLineEdit->setText(current_db_question);
					break;
			}
		} else if (num_old != 0) {
			switch (QMessageBox::information(this, tr("iTestServer"), tr("Are you sure you want to change the name of the question?\n%1 occurrences of this question found in the saved sessions.\nAll occurrences will be updated.").arg(num_old), tr("&Change"), tr("Do &not change"), 0, 1)) {
				case 0: // Change
					replaceAllOccurrences(current_db_question, q_name);
					break;
				case 1: // Do not change
					q_name = current_db_question;
					SQQuestionNameLineEdit->setText(current_db_question);
					break;
			}
		}
    }
    // SAVE VALUES
    item->setName(q_name);
    item->setGroup(q_group);
    item->setFlag(q_flag);
    item->setDifficulty(SQDifficultyComboBox->currentIndex());
    item->setText(removeLineBreaks(SQQuestionTextEdit->toHtml()));
    // ansa
    item->setAnsA(removeLineBreaks(SQAnswerALineEdit->text()));
    item->setAnsACorrect(SQCorrectACheckBox->isChecked());
    // ansb
    item->setAnsB(removeLineBreaks(SQAnswerBLineEdit->text()));
    item->setAnsBCorrect(SQCorrectBCheckBox->isChecked());
    // ansc
    item->setAnsC(removeLineBreaks(SQAnswerCLineEdit->text()));
    item->setAnsCCorrect(SQCorrectCCheckBox->isChecked());
    // ansd
    item->setAnsD(removeLineBreaks(SQAnswerDLineEdit->text()));
    item->setAnsDCorrect(SQCorrectDCheckBox->isChecked());
    // hidden
    item->setHidden(actionHide->isChecked());
    // APPLY
    current_db_question = q_name;
    q_item->setText(item->group().isEmpty() ? q_name : QString("[%1] %2").arg(item->group()).arg(q_name));
    setQuestionItemIcon(q_item, item->difficulty());
    setQuestionItemColour(q_item, item->flag());
    hideQuestion(q_item, item);
    statusBar()->showMessage(tr("Data saved"), 10000);
    setDatabaseModified();
}

void MainWindow::discardQuestionChanges()
{
    setCurrentQuestion();
    statusBar()->showMessage(tr("Data discarded"), 10000);
}

void MainWindow::hideQuestion()
{
	if (!LQListWidget->currentIndex().isValid()) { return; }
	QuestionItem * item = NULL; item = current_db_questions.value(LQListWidget->currentItem());
	if (item == NULL) { return; }
	item->setHidden(actionHide->isChecked());
	hideQuestion(LQListWidget->currentItem(), item);
	setDatabaseModified();
}

void MainWindow::hideQuestion(QListWidgetItem * q_item, QuestionItem * item)
{
	q_item->setHidden(item->isHidden() && !actionShow_hidden->isChecked());
	q_item->setForeground(QBrush::QBrush(foregroundColourForFlag(item->flag(), item->isHidden())));
	if (item->isHidden()) {
		QFont font; font.setBold(true);
		q_item->setFont(font);
	} else {
		QFont font;
		q_item->setFont(font);
	}
}

void MainWindow::setQuestionItemIcon(QListWidgetItem * q_item, int q_difficulty_i)
{
	q_item->setIcon(iconForDifficulty(q_difficulty_i));
}

QIcon MainWindow::iconForDifficulty(int q_difficulty_i)
{
	switch (q_difficulty_i) {
		case 0: return QIcon(QString::fromUtf8(":/images/images/easy.png")); break;
		case 1: return QIcon(QString::fromUtf8(":/images/images/medium.png")); break;
		case 2: return QIcon(QString::fromUtf8(":/images/images/difficult.png")); break;
		default: return QIcon(QString::fromUtf8(":/images/images/easy.png")); break;
	}
	return QIcon(QString::fromUtf8(":/images/images/easy.png"));
}

void MainWindow::setQuestionItemColour(QListWidgetItem * q_item, int q_flag_i)
{
	q_item->setBackground(QBrush::QBrush(backgroundColourForFlag(q_flag_i)));
	q_item->setForeground(QBrush::QBrush(foregroundColourForFlag(q_flag_i)));
}

void MainWindow::searchByGroup() {
	if (SQGroupLineEdit->text().isEmpty()) { return; }
	LQSearchLineEdit->setText(QString("[%1]").arg(SQGroupLineEdit->text()));
	filterLQSearch();
}

void MainWindow::filterLQSearch() { filterLQ(rbtngrpFilterLQ->checkedButton()); }

void MainWindow::filterLQFlagChanged() 
{
    if (LQFlagRadioButton->isChecked()) {filterLQ(LQFlagRadioButton);}
}

void MainWindow::filterLQAction(QAction * act)
{
    if (act == actionShow_all) {
        filterLQ(LQAllRadioButton); LQAllRadioButton->setChecked(true);
    } else if (act == actionShow_easy) {
        filterLQ(LQEasyRadioButton); LQEasyRadioButton->setChecked(true);
    } else if (act == actionShow_medium) {
        filterLQ(LQMediumRadioButton); LQMediumRadioButton->setChecked(true);
    } else if (act == actionShow_difficult) {
        filterLQ(LQDifficultRadioButton); LQDifficultRadioButton->setChecked(true);
    } else if (act == actionShow_flag) {
        filterLQ(LQFlagRadioButton); LQFlagRadioButton->setChecked(true);
    }
}

void MainWindow::filterLQ(QAbstractButton * rbtn)
{
    QuestionItem * item; QString keyword = LQSearchLineEdit->text();
    if (keyword.isEmpty()) {
        LQSearchLineEdit->setPalette(qApp->palette());
    } else {
        LQSearchLineEdit->setPalette(search_active_palette);
    }
    int n = 0;
    if (rbtn == LQAllRadioButton) {
        for (int i = 0; i < LQListWidget->count(); ++i) {
        	item = current_db_questions.value(LQListWidget->item(i));
            if (!keyword.isEmpty()) {
                if (LQListWidget->item(i)->text().contains(keyword, Qt::CaseInsensitive)) {
                    LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); n++;
                } else { LQListWidget->item(i)->setHidden(true); }
            } else { LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); }
        }
    } else if (rbtn == LQEasyRadioButton) {
        for (int i = 0; i < LQListWidget->count(); ++i) {
            item = current_db_questions.value(LQListWidget->item(i));
            if (item->difficulty() <= 0) {
                if (!keyword.isEmpty()) {
                    if (LQListWidget->item(i)->text().contains(keyword, Qt::CaseInsensitive)) {
                        LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); n++;
                    } else { LQListWidget->item(i)->setHidden(true); }
                } else { LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); }
            } else { LQListWidget->item(i)->setHidden(true); }
        }
    } else if (rbtn == LQMediumRadioButton) {
        for (int i = 0; i < LQListWidget->count(); ++i) {
            item = current_db_questions.value(LQListWidget->item(i));
            if (item->difficulty() == 1) {
                if (!keyword.isEmpty()) {
                    if (LQListWidget->item(i)->text().contains(keyword, Qt::CaseInsensitive)) {
                        LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); n++;
                    } else { LQListWidget->item(i)->setHidden(true); }
                } else { LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); }
            } else { LQListWidget->item(i)->setHidden(true); }
        }
    } else if (rbtn == LQDifficultRadioButton) {
        for (int i = 0; i < LQListWidget->count(); ++i) {
            item = current_db_questions.value(LQListWidget->item(i));
            if (item->difficulty() == 2) {
                if (!keyword.isEmpty()) {
                    if (LQListWidget->item(i)->text().contains(keyword, Qt::CaseInsensitive)) {
                        LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); n++;
                    } else { LQListWidget->item(i)->setHidden(true); }
                } else { LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); }
            } else { LQListWidget->item(i)->setHidden(true); }
        }
    } else if (rbtn == LQFlagRadioButton) {
        for (int i = 0; i < LQListWidget->count(); ++i) {
            item = current_db_questions.value(LQListWidget->item(i));
            if (current_db_flagentries.value(item->flag()) == LQFlagComboBox->currentIndex()) {
                if (!keyword.isEmpty()) {
                    if (LQListWidget->item(i)->text().contains(keyword, Qt::CaseInsensitive)) {
                        LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); n++;
                    } else { LQListWidget->item(i)->setHidden(true); }
                } else { LQListWidget->item(i)->setHidden(item->isHidden() && !actionShow_hidden->isChecked()); }
            } else { LQListWidget->item(i)->setHidden(true); }
        }
    }
    if ((!keyword.isEmpty()) && LQListWidget->count() != 0 && n == 0) {
        LQSearchLineEdit->setPalette(search_noresults_palette);
    }
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

bool caseInsensitiveMoreThan(const QString &s1, const QString &s2)
{
    return s1.toLower() > s2.toLower();
}

void MainWindow::sortQuestionsAscending() { sortQuestions(Qt::AscendingOrder); }

void MainWindow::sortQuestionsDescending() { sortQuestions(Qt::DescendingOrder); }

void MainWindow::sortQuestions(Qt::SortOrder order)
{
    QStringList list; QMap<QString, QListWidgetItem *> map;
    
    setDatabaseModified();
    
    for (int i = 0; i < LQListWidget->count();) {
        list << LQListWidget->item(i)->text();
        map.insert(LQListWidget->item(i)->text(), LQListWidget->takeItem(i));
    }
    
    if (order == Qt::AscendingOrder) {
        qSort(list.begin(), list.end(), caseInsensitiveLessThan);
    } else {
        qSort(list.begin(), list.end(), caseInsensitiveMoreThan);
    }
    
    for (int i = 0; i < list.size(); ++i)
    { LQListWidget->insertItem(i, map.value(list.at(i))); }
}

void MainWindow::moveToTop() { moveQuestion(-2); }

void MainWindow::moveUp()
{
    if (!LQAllRadioButton->isChecked()) 
        { filterLQ(LQAllRadioButton); LQAllRadioButton->setChecked(true); }
    moveQuestion(-1);
}

void MainWindow::moveDown()
{
    if (!LQAllRadioButton->isChecked()) 
        { filterLQ(LQAllRadioButton); LQAllRadioButton->setChecked(true); }
    moveQuestion(1);
}

void MainWindow::moveToBottom() { moveQuestion(2); }

void MainWindow::moveQuestion(int move)
{
    if (LQListWidget->currentIndex().isValid()) {
        QListWidgetItem * item; int row = LQListWidget->currentRow();
        if (move == -1) {
            if (row > 0) {
                item = LQListWidget->takeItem(LQListWidget->currentRow());
                LQListWidget->insertItem(row-1, item);
                LQListWidget->setCurrentRow(row-1);
            }
        } else if (move == 1) {
            if (row < (LQListWidget->count()-1)) {
                item = LQListWidget->takeItem(LQListWidget->currentRow());
                LQListWidget->insertItem(row+1, item);
                LQListWidget->setCurrentRow(row+1);
            }
        } else if (move == -2) {
            item = LQListWidget->takeItem(LQListWidget->currentRow());
            LQListWidget->insertItem(0, item);
            LQListWidget->setCurrentRow(0);
        } else if (move == 2) {
            item = LQListWidget->takeItem(LQListWidget->currentRow());
            LQListWidget->insertItem(LQListWidget->count(), item);
            LQListWidget->setCurrentRow(LQListWidget->count()-1);
        }
        setDatabaseModified();
    }
}

void MainWindow::adjustQuestionDifficulty()
{
	if (!LQListWidget->currentIndex().isValid()) { return; }
	QuestionItem * item = current_db_questions.value(LQListWidget->currentItem());
	int rdif = item->recommendedDifficulty();
	if (rdif >= 0 && rdif <= 2) {
		item->setDifficulty(rdif);
		SQDifficultyComboBox->setCurrentIndex(rdif);
		setQuestionItemIcon(LQListWidget->currentItem(), item->difficulty());
		setDatabaseModified();
	}
}

uint MainWindow::numOccurrences(QString qname)
{
	uint n = 0;
	QMapIterator<QDateTime, Session *> i(current_db_sessions);
	while (i.hasNext()) { i.next();
		for (int s = 0; s < i.value()->numStudents(); ++s) {
			if (i.value()->student(s)->wasAsked(qname)) { n++; }
		}
	}
	QMapIterator<QDateTime, ArchivedSession *> a(current_db_archivedsessions);
	while (a.hasNext()) { a.next();
		for (int s = 0; s < a.value()->numStudents(); ++s) {
			if (a.value()->student(s)->wasAsked(qname)) { n++; }
		}
	}
	return n;
}

uint MainWindow::replaceAllOccurrences(QString old_qname, QString new_qname)
{
	uint n = 0, as = 0, x = 0;
	QMapIterator<QDateTime, Session *> i(current_db_sessions);
	while (i.hasNext()) { i.next();
		for (int s = 0; s < i.value()->numStudents(); ++s) {
			n += i.value()->student(s)->replaceOccurrences(old_qname, new_qname);
		}
	}
	QMapIterator<QDateTime, ArchivedSession *> a(current_db_archivedsessions);
	while (a.hasNext()) { a.next();
		as = 0;
		for (int s = 0; s < a.value()->numStudents(); ++s) {
			x = a.value()->student(s)->replaceOccurrences(old_qname, new_qname);
			n += x; as += x;
		}
		if (as > 0) {
            a.value()->setStatus(ArchivedSession::Archive);
			current_db_queued_sessions.removeAll(a.value());
			current_db_queued_sessions.enqueue(a.value());
        }
	}
	return n;
}
