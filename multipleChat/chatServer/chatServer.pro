QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    chatserver.cpp \
    serverworker.cpp

HEADERS += \
    mainwindow.h \
    chatserver.h \
    serverworker.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD/../bin
TARGET = chatServer

win32 {
    DEPLOYQT = $$shell_path($$[QT_INSTALL_BINS]/windeployqt.exe)
    TARGET_PATH = $$shell_path($$DESTDIR/$$TARGET.exe)
    QMAKE_POST_LINK += $$quote($$DEPLOYQT) $$quote($$TARGET_PATH)
}
