CONFIG       += qt
TEMPLATE      = app
FORMS        += main_window_v2.ui \
                ../shared/about_widget_v2.ui \
                session_wizard.ui \
                mttextedit.ui
HEADERS      += main_window.h \
                ../shared/main.h \
                ../shared/about_widget.h \
                ../shared/question.h \
                question_item.h \
                client.h \
                student.h \
                session.h \
                archived_session.h \
                ../shared/pass_mark.h \
                svg_item.h \
                extendedlineedit.h \
                mtadvancedgroupbox.h \
                mtspinbox.h \
                class.h \
                mtwidgetpalettes.h \
                mtlistwidget.h \
                mttablewidget.h \
                session_wizard.h \
                mttoolbutton.h \
                mtprogressbar.h \
                answersedit.h \
                print_engine.h \
                mttextedit.h
RESOURCES    += resources.qrc \
                i18n.qrc
SOURCES      += main.cpp \
                main_window.cpp \
                database.cpp \
                questions.cpp \
                text_formatting.cpp \
                flags.cpp \
                ../shared/about_widget.cpp \
                ../shared/question.cpp \
                question_item.cpp \
                server.cpp \
                client.cpp \
                env_vars.cpp \
                print_engine.cpp \
                session_viewer.cpp \
                student.cpp \
                session.cpp \
                archived_session.cpp \
                ../shared/pass_mark.cpp \
                svg_item.cpp \
                class.cpp \
                class_viewer.cpp \
                session_wizard.cpp \
                answersedit.cpp
QT           += network svg
DEFINES      += ITESTSERVER
win32 {
DESTDIR       = ../
RC_FILE       = itest.rc
OBJECTS_DIR   = .build.win32
MOC_DIR       = .build.win32
RCC_DIR       = .build.win32
exists($(QTDIR)/bin/splitui.exe) {
QMAKE_UIC     = splitui.exe
SOURCES      += ui_main_window_v2.cpp \
                ui_about_widget_v2.cpp \
                ui_session_wizard.cpp \
                ui_mttextedit.cpp
}
}
macx {
ICON          = images/itest.icns
icons.path    = Contents/Resources
icons.files   = ../itdb.icns ../itcl.icns
QMAKE_BUNDLE_DATA += icons
CONFIG       += x86 ppc
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
}
unix {
OBJECTS_DIR   = .build.unix
MOC_DIR       = .build.unix
RCC_DIR       = .build.unix
exists(/usr/bin/splitui) {
QMAKE_UIC     = splitui
SOURCES      += ui_main_window_v2.cpp \
                ui_about_widget_v2.cpp \
                ui_session_wizard.cpp \
                ui_mttextedit.cpp
}
!macx {
DESTDIR       = ../bin/
exists(/usr/bin/apgcc) {
QMAKE_CC      = apgcc
}
exists(/usr/bin/apg++) {
QMAKE_CXX     = apg++
}
}
}
TRANSLATIONS += i18n/iTestServer-Slovak.ts \
                i18n/iTestServer-Russian.ts \
                i18n/iTestServer-Turkish.ts \
                i18n/iTestServer-Portuguese.ts \
                i18n/iTestServer-Spanish.ts \
                i18n/iTestServer-Italian.ts \
                i18n/iTestServer-Latvian.ts
QMAKE_RESOURCE_FLAGS += -compress 9
