QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Add include paths for external libraries
INCLUDEPATH += $$PWD/cpp-httplib
INCLUDEPATH += $$PWD/json

SOURCES += \
    apiclient.cpp \
    apiLogin.cpp \
    filecardwidget.cpp \
    filehierarchyview.cpp \
    loginwindow.cpp \
    main.cpp \
    MainWindow.cpp \
    searchbar.cpp \
    sidebar.cpp \
    toolbar.cpp

HEADERS += \
    MainWindow.h \
    apiclient.h \
    apiLogin.h \
    filecardwidget.h \
    filehierarchyview.h \
    loginwindow.h \
    searchbar.h \
    sidebar.h \
    toolbar.h

FORMS += \
    loginwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
