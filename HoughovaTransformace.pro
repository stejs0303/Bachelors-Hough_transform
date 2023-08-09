QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += C:\OpenCV\release\install\include

LIBS += C:\OpenCV\release\bin\libopencv_core450.dll
LIBS += C:\OpenCV\release\bin\libopencv_imgcodecs450.dll
LIBS += C:\OpenCV\release\bin\libopencv_imgproc450.dll

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    houghtransform.cpp \
    mygeneralizedhoughballard.cpp \
    myhough.cpp \
    myhoughcircles.cpp \
    myhoughlines.cpp

HEADERS += \
    houghtransform.h \
    mygeneralizedhoughballard.h \
    myhough.h \
    myhoughcircles.h \
    myhoughlines.h

FORMS += \
    houghtransform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Obrazky.qrc
