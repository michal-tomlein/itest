#include "main_window.h"

void MainWindow::varinit()
{
    // GLOBAL
        // iTestServer version
        ver = "1.2.0"; f_ver = 1.2;
        // iTest ITDB file version
        itdb_ver = "1.2"; f_itdb_ver = 1.2;
        // iTest ITOS file version
        itos_ver = "1.2"; f_itos_ver = 1.2;
        // URLs
        itest_url = "itest.sourceforge.net";
        docs_url = tr("http://itest.sourceforge.net/documentation/%1/en/").arg("1.2");
        // i18n
        itest_i18n.insert(tr("English"), "English");
        itest_i18n.insert(tr("Slovak"), "Slovak");
        itest_i18n.insert(tr("Russian"), "Russian");
        itest_i18n.insert(tr("Turkish"), "Turkish");
        itest_i18n.insert(tr("Portuguese"), "Portuguese");
    // CURRENT_DB
        current_db_open = false;
}
