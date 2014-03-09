CONFIG       += qt
TEMPLATE      = app
FORMS        += main_window.ui \
                ../shared/about_widget_v2.ui
HEADERS      += main_window.h \
                ../shared/main.h \
                ../shared/about_widget.h \
                ../shared/question.h \
                question_item.h \
                ../shared/pass_mark.h \
                mtsplitter.h \
                mtmultisvgwidget.h \
                flowlayout.h \
                question_widget.h \
                answersview.h \
                ../shared/defs.h
RESOURCES    += resources.qrc \
                i18n.qrc
SOURCES      += main.cpp \
                main_window.cpp \
                database.cpp \
                ../shared/about_widget.cpp \
                client.cpp \
                ../shared/question.cpp \
                question_item.cpp \
                ../shared/pass_mark.cpp \
                flowlayout.cpp \
                question_widget.cpp \
                answersview.cpp

lessThan(QT_MAJOR_VERSION, 5) {
    QT           += network svg
    CONFIG       += depend_includepath
} else {
    QT           += widgets network svg
}

DEFINES      += ITESTCLIENT
INCLUDEPATH  += ../shared

win32 {
    DESTDIR       = ../
    RC_FILE       = itestwri.rc
    OBJECTS_DIR   = build/win32
    MOC_DIR       = build/win32
    RCC_DIR       = build/win32

    exists($(QTDIR)/bin/splitui.exe) {
        QMAKE_UIC     = splitui.exe
        SOURCES      += ui_main_window.cpp \
                        ui_about_widget_v2.cpp
    }
}

macx {
    ICON          = images/itestwri.icns
    icons.path    = Contents/Resources
    icons.files   = ../itos.icns
    QMAKE_BUNDLE_DATA += icons
    CONFIG       += x86_64
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
}

macx-xcode {
    QMAKE_CC      = clang
    QMAKE_CXX     = clang++
}

unix {
    OBJECTS_DIR   = build/unix
    MOC_DIR       = build/unix
    RCC_DIR       = build/unix

    exists(/usr/bin/splitui) {
        QMAKE_UIC     = splitui
        SOURCES      += ui_main_window.cpp \
                        ui_about_widget_v2.cpp
    }

    !macx {
        DESTDIR       = ../bin/
    }
}

TRANSLATIONS += i18n/iTestClient-sk.ts \
                i18n/iTestClient-ru.ts \
                i18n/iTestClient-tr.ts \
                i18n/iTestClient-pt.ts \
                i18n/iTestClient-es.ts \
                i18n/iTestClient-it.ts \
                i18n/iTestClient-lv.ts \
                i18n/iTestClient-uk.ts \
                i18n/iTestClient-cs.ts \
                i18n/iTestClient-hu.ts \
                i18n/iTestClient-de.ts

QMAKE_RESOURCE_FLAGS += -compress 9
