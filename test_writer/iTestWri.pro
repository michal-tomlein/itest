CONFIG       += qt
TEMPLATE      = app
FORMS        += main_window.ui \
                about_widget_v2.ui
HEADERS      += main_window.h \
                about_widget.h \
                question_item.h
RESOURCES    += resources.qrc \
                i18n.qrc
SOURCES      += main.cpp \
                database.cpp \
                about_widget.cpp \
                client.cpp \
                question_item.cpp \
                env_vars.cpp \
                question_answer.cpp
QT           += network
win32 {
RC_FILE       = itestwri.rc
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
