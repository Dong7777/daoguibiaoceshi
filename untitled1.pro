QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialport
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    messageprocessor.cpp \
    widget.cpp

HEADERS += \
    dll/dlt698.h \
    messageprocessor.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -ldlt698
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -ldlt698
else:unix: LIBS += -L$$PWD/./ -ldlt698

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''
