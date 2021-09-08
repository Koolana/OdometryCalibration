QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#LIBS += -L~/Qt_utils/libserialport/.libs/serialport.so
#INCLUDEPATH += /home/nikolay/Qt_utils/libserialport

SOURCES += \
    src/itemdata.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/modelcalibrationalg.cpp \
    src/modelrobotcontroller.cpp \
    src/widgetcalculation.cpp \
    src/widgetdraw.cpp \
    src/widgetrobotsettings.cpp \
    src/widgettestsettings.cpp

HEADERS += \
    include/itemdata.h \
    include/mainwindow.h \
    include/modelcalibrationalg.h \
    include/modelrobotcontroller.h \
    include/odomdatatype.h \
    include/widgetcalculation.h \
    include/widgetdraw.h \
    include/widgetrobotsettings.h \
    include/widgettestsettings.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
