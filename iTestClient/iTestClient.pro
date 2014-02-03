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
                answersview.h
RESOURCES    += resources.qrc \
                i18n.qrc
SOURCES      += main.cpp \
                main_window.cpp \
                database.cpp \
                ../shared/about_widget.cpp \
                client.cpp \
                ../shared/question.cpp \
                question_item.cpp \
                env_vars.cpp \
                ../shared/pass_mark.cpp \
                flowlayout.cpp \
                question_widget.cpp \
                answersview.cpp

QT           += network svg
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

TRANSLATIONS += i18n/iTestClient-Slovak.ts \
                i18n/iTestClient-Russian.ts \
                i18n/iTestClient-Turkish.ts \
                i18n/iTestClient-Portuguese.ts \
                i18n/iTestClient-Spanish.ts \
                i18n/iTestClient-Italian.ts \
                i18n/iTestClient-Latvian.ts \
                i18n/iTestClient-Ukrainian.ts \
                i18n/iTestClient-Czech.ts \
                i18n/iTestClient-Hungarian.ts \
                i18n/iTestClient-German.ts

QMAKE_RESOURCE_FLAGS += -compress 9
