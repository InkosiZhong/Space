QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += _VERSION_=\\\"1.4.1\\\"
VERSION = 1.4.1

SOURCES += \
    main.cpp \
    constants.cpp \
    module/ocrhandler.cpp \
    moduledock.cpp \
    module/backgroundremover.cpp \
    module/capturescreen.cpp \
    module/latexformulaextractor.cpp \
    ui/mainwindow.cpp \
    ui/ocrarea.cpp \
    ui/promptform.cpp \
    ui/settingsform.cpp \
    ui/exhibitform.cpp \
    ui/singlekeysequenceedit.cpp \
    utils/codecutil.cpp \
    utils/curlutil.cpp \
    utils/cvtformatutil.cpp \
    utils/jsoncpp.cpp \
    utils/jsonutil.cpp \
    utils/networkutil.cpp \
    utils/util.cpp \
    utils/xmlutil.cpp

HEADERS += \
    constants.h \
    module/ocrhandler.h \
    moduledock.h \
    module/backgroundremover.h \
    module/capturescreen.h \
    module/latexformulaextractor.h \
    ui/mainwindow.h \
    ui/ocrarea.h \
    ui/promptform.h \
    ui/settingsform.h \
    ui/exhibitform.h \
    ui/singlekeysequenceedit.h \
    utils/codecutil.h \
    utils/curlutil.h \
    utils/cvtformatutil.h \
    utils/jsonutil.h \
    utils/networkutil.h \
    utils/util.h \
    utils/xmlutil.h

FORMS += \
    ui/mainwindow.ui \
    ui/promptform.ui \
    ui/settingsform.ui \
    ui/exhibitform.ui

# opencv 4.5.2
macx:{
INCLUDEPATH += $$PWD/opencv/include/opencv4 \
               $$PWD/opencv/include/opencv4/opencv2

LIBS += $$PWD/opencv/lib/libopencv_*
}

win32:{
INCLUDEPATH += ../opencv/include \
               ../opencv/include/opencv2

LIBS += ../opencv/x64/vc15/lib/opencv_world452d.lib
LIBS += User32.LIB
}

# curl
INCLUDEPATH += ./utils
win32: LIBS += ../curl-7.78.0-win64-mingw/lib/libcurl.dll.a
macx: LIBS += $$PWD/../curl-7.78.0/build/lib/libcurl.dylib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(utils/qxtglobalshortcut5/qxt.pri)

RESOURCES += \
    res.qrc

RC_ICONS = "res/icon.ico"
macx: ICON = res/icon.icns

