CONFIG       += qt
TEMPLATE      = app
FORMS        += main_window_v2.ui \
                about_widget_v2.ui
HEADERS      += main_window.h \
                about_widget.h \
                question_item.h \
                client.h \
                student.h \
                session.h \
                archived_session.h
RESOURCES    += resources.qrc \
                i18n.qrc
SOURCES      += main.cpp \
                main_window.cpp \
                database.cpp \
                questions.cpp \
                text_formatting.cpp \
                flags.cpp \
                about_widget.cpp \
                question_item.cpp \
                server.cpp \
                client.cpp \
                env_vars.cpp \
                print_engine.cpp \
                session_viewer.cpp \
                student.cpp \
                session.cpp \
                archived_session.cpp
QT           += network
win32 {
RC_FILE       = itest.rc
OBJECTS_DIR   = .objects.win32/
MOC_DIR       = .moc.win32/
RCC_DIR       = .rcc.win32/
}
unix {
OBJECTS_DIR   = .objects.unix/
MOC_DIR       = .moc.unix/
RCC_DIR       = .rcc.unix/
exists(/usr/bin/apgcc) {
QMAKE_CC      = apgcc
}
exists(/usr/bin/apg++) {
QMAKE_CXX     = apg++
}
}
macx {
OBJECTS_DIR   = .objects.macx/
MOC_DIR       = .moc.macx/
RCC_DIR       = .rcc.macx/
}
DESTDIR       = ../
TRANSLATIONS += i18n/Slovak.ts \
                i18n/Russian.ts
