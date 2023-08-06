QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BSFatStructure/BSCluster.cpp \
    BSFatStructure/BSFatFile.cpp \
    DirectProperties.cpp \
    DirectoryStructure/AbstractElement.cpp \
    DirectoryStructure/AbstractFile.cpp \
    DirectoryStructure/Directory.cpp \
    createdirectory.cpp \
    createfile.cpp \
    deletedirectory.cpp \
    deletefile.cpp \
    dialog.cpp \
    fileproperties.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DirectProperties.h \
    createdirectory.h \
    createfile.h \
    deletedirectory.h \
    deletefile.h \
    dialog.h \
    fileproperties.h \
    mainwindow.h

FORMS += \
    createdirectory.ui \
    createfile.ui \
    deletedirectory.ui \
    deletefile.ui \
    dialog.ui \
    directProperties.ui \
    fileproperties.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
