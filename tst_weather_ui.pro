QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    driver/bmp280.cpp \
    src/RawData.cpp \
    src/bmp280data.cpp \
    src/data_manager.cpp \
    src/data_output.cpp \
    src/data_reading.cpp \
    src/form.cpp \
    src/main.cpp \
    src/mqtt.cpp \
    src/raspberry_bmp280.cpp \
    src/settings.cpp\
    settings_pop_up.cpp\


HEADERS += \
    driver/bmp280.h \
    driver/bmp280_defs.h \
    include/RawData.h \
    include/bmp280data.h \
    include/data_manager.h \
    include/data_output.h \
    include/data_reading.h \
    include/mqtt.h \
    include/raspberry_bmp280.h \
    include/settings.h \
    include/bmp280data.h\
    src/form.h\
    settings_pop_up.h\

FORMS += \
    src/form.ui
    settings_pop_up.ui

LIBS +=  -lmosquittopp
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

