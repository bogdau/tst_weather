SUMMARY = "Weather app recipee"
DESCRIPTION = "Pressure and temperature applications"
LICENSE = "MIT"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

#FILESEXTRAPATHS:append= "${THISDIR}/files:"

SRC_URI = "git://github.com/bohdan-kharytonov/tst_weather.git;branch=dev;protocol=https "

SRCREV = "fc05388f093db982dc01f704c1876d36e48ae681"
SRC_URI[sha256sum] = "16fed98a21dbb03a8080bce8cf4ac85df11842b214e64beebab2144e6e53b4af"

S = "${WORKDIR}/git"

DEPENDS = "mosquitto boost  qtbase qtx11extras aws-iot-device-sdk-cpp-v2 sqlite3 wiringpi "

CFLAGS += "-I${STAGING_INCDIR}/mosuitto -I${STAGING_INCDIR}/boost"
LDFLAGS += "-L${STAGING_LIBDIR}/boost"

do_configure() {
    # ...
}

do_install:append() {
	install -d ${D}${bindir}/fontx
  	install -m 0755 ${S}/fontx/ILGH16XB.FNT ${D}${bindir}/fontx/ILGH16XB.FNT
  	install -m 0755 ${S}/fontx/ILGZ16XB.FNT ${D}${bindir}/fontx/ILGZ16XB.FNT
}
FILES:${PN} += "\
				/opt/tst_weather_ui/bin/tst_weather_ui \
				/opt/tst_weather_ui/bin/fontx/ \
				"

inherit qmake5

