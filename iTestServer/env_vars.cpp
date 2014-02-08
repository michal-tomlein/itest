/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2009 Michal Tomlein (michal.tomlein@gmail.com)

 iTest is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 iTest is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with iTest; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************/

#include "main_window.h"

void MainWindow::varinit()
{
    // GLOBAL
        // iTestServer version
        ver = "1.4.2"; f_ver = 1.42;
        // iTest ITDB file version
        itdb_ver = "1.4"; f_itdb_ver = 1.4;
        // iTest ITOS file version
        itos_ver = "1.4.2"; f_itos_ver = 1.42;
        // URLs
        itest_url = "itest.sourceforge.net";
        docs_url = tr("http://itest.sourceforge.net/documentation/%1/en/").arg("1.4");
        // i18n
        QTranslator translator; translator.load(":/i18n/iTest-i18n.qm");
        itest_i18n.insert("English", "en");
        itest_i18n.insert(translator.translate("LanguageNames", "Slovak"), "sk");
        itest_i18n.insert(translator.translate("LanguageNames", "Russian"), "ru");
        itest_i18n.insert(translator.translate("LanguageNames", "Turkish"), "tr");
        itest_i18n.insert(translator.translate("LanguageNames", "Portuguese"), "pt");
        itest_i18n.insert(translator.translate("LanguageNames", "Spanish"), "es");
        itest_i18n.insert(translator.translate("LanguageNames", "Italian"), "it");
        itest_i18n.insert(translator.translate("LanguageNames", "Latvian"), "lv");
        itest_i18n.insert(translator.translate("LanguageNames", "Ukrainian"), "uk");
        itest_i18n.insert(translator.translate("LanguageNames", "Czech"), "cs");
        itest_i18n.insert(translator.translate("LanguageNames", "Hungarian"), "hu");
        itest_i18n.insert(translator.translate("LanguageNames", "German"), "de");
    // CURRENT_DB
        current_db_open = false;
}
