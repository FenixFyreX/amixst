#-------------------------------------------------
#
# Project created by QtCreator 2017-01-28T19:50:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = amixst
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -lasound

SOURCES += \
    app.cpp \
    main.cpp \
    mixer.cpp \
    widget/sysicon.cpp \
    widget/mixerctrl.cpp \
    ui/traydialog.cpp \
    runguard.cpp

HEADERS += \
    app.h \
    mixer.h \
    amixst.h \
    widget/sysicon.h \
    widget/mixerctrl.h \
    ui/traydialog.h \
    runguard.h

RESOURCES += \
    amixst.qrc

FORMS += \
    widget/mixerctrl.ui \
    ui/traydialog.ui
