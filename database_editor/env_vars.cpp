#include "main_window.h"

void MainWindow::varinit()
{
    // GLOBAL
        // iTest version
        ver = "1.1.0"; f_ver = 1.1;
        // Supported iTest DB file version
        db_ver = "1.0"; f_db_ver = 1.0;
        // URLs
        itest_url = "itest.sourceforge.net";
        docs_url = tr("http://itest.sourceforge.net/documentation/1.1/en/");
        // i18n
        itest_i18n.insert(tr("English"), "English");
        itest_i18n.insert(tr("Slovak"), "Slovak");
        itest_i18n.insert(tr("Russian"), "Russian");
    // CURRENT_DB
        current_db_open = false;
}
