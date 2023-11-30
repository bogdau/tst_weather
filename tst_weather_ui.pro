QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCE_FOLDER = $$PWD/fontx
DESTINATION_FOLDER = $$OUT_PWD/fontx

copy_folder.commands = $$quote(cp -r $$quote($$SOURCE_FOLDER) $$quote($$DESTINATION_FOLDER))
QMAKE_EXTRA_TARGETS += copy_folder
PRE_TARGETDEPS += copy_folder

SOURCES += \
    driver/lis3mdl_reg.c \
    src/data_base.cpp \
    src/aws.cpp \
    src/RawData.cpp \
    src/bmp280data.cpp \
    src/data_manager.cpp \
    src/data_output.cpp \
    src/data_reading.cpp \
    src/form.cpp \
    src/main.cpp \
    src/mqtt.cpp \
#    src/raspberry_bmp280.cpp \
    src/settings.cpp\
    src/settings_pop_up.cpp\
    src/mki109v1.cpp \
    i2c_impl/I2CInputOutput.cpp \
    i2c_impl/RaspberryPi.cpp \
    i2c_impl/I2CDevice.cpp \
    BMP280/BMP280.cpp \
    dispay_sh1106/oled.cpp \
    dispay_sh1106/fontx.cpp \


HEADERS += \
    driver/lis3mdl_reg.h \
    include/aws.h \
    include/data_base.h \
    include/RawData.h \
    include/bmp280data.h \
    include/data_manager.h \
    include/data_output.h \
    include/data_reading.h \
    include/mqtt.h \
#    include/raspberry_bmp280.h \
    include/settings.h \
    include/bmp280data.h\
    include/form.h\
    include/settings_pop_up.h\
    include/mki109v1.h \
    i2c_impl/I2CInputOutput.h \
    i2c_impl/RaspberryPi.h \
    i2c_impl/I2CDevice.h \
    BMP280/BMP280.h \
    dispay_sh1106/oled.h \
    dispay_sh1106/fontx.h \


FORMS += \
    ui/form.ui \
    ui/settings_pop_up.ui \

UI_DIR = $$PWD/ui
UI_HEADERS_DIR = $$PWD

LIBS += \
    -lmosquittopp \
    -lsqlite3 \
    -laws-crt-cpp \
    -laws-c-common \
    -lcrypto \
    -lwiringPi \
    -lpthread \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

