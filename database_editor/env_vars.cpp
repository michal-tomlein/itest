#include "main_window.h"

void MainWindow::varinit()
{
    // GLOBAL
        // iTest version
        ver = "1.0.1"; f_ver = 1.01;
        // Supported iTest DB file version
        db_ver = "1.0"; f_db_ver = 1.0;
        // URLs
        itest_url = "itest.sourceforge.net";
        docs_url = tr("http://itest.sourceforge.net/documentation/1.0/en/");
        // i18n
        itest_i18n.insert(tr("English"), "English");
        itest_i18n.insert(tr("Slovak"), "Slovak");
    // CURRENT_DB
        current_db_open = false;
}
