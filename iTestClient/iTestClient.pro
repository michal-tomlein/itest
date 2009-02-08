CONFIG       += qt
TEMPLATE      = app
FORMS        += main_window.ui \
                about_widget_v2.ui
HEADERS      += main_window.h \
                about_widget.h \
                question_item.h \
                pass_mark.h
RESOURCES    += resources.qrc \
                i18n.qrc
SOURCES      += main.cpp \
                database.cpp \
                about_widget.cpp \
                client.cpp \
                question_item.cpp \
                env_vars.cpp \
                question_answer.cpp \
                pass_mark.cpp
QT           += network
win32 {
DESTDIR       = ../
RC_FILE       = itestwri.rc
OBJECTS_DIR   = .objects.win32/
MOC_DIR       = .moc.win32/
RCC_DIR       = .rcc.win32/
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
CONFIG       += x86 ppc
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
}
unix {
exists(/usr/bin/splitui) {
QMAKE_UIC     = splitui
SOURCES      += ui_main_window.cpp \
                ui_about_widget_v2.cpp
}
!macx {
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
}
TRANSLATIONS += i18n/Slovak.ts \
                i18n/Russian.ts \
                i18n/Turkish.ts \
                i18n/Portuguese.ts
QMAKE_RESOURCE_FLAGS += -compress 9
