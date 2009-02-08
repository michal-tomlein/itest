#ifndef ARCHIVED_SESSION_H
#define ARCHIVED_SESSION_H

#include <QSettings>

#include "session.h"

class ArchivedSession : public Session
{
    Q_OBJECT

public:
    ArchivedSession(MainWindow *);
    ArchivedSession(MainWindow *, Session *);
    ArchivedSession(MainWindow *, QString);
    virtual ~ArchivedSession();
    enum Status { Default, Archive, Remove };

public slots:
    void archive();
    void restore(QString);
    void removeFromArchive();
    Status status(); void setStatus(Status);
    QString sessionArchiveData();

private:
    MainWindow * as_parent;
    Status as_status;

    friend class Session;
};

#endif // ARCHIVED_SESSION_H
