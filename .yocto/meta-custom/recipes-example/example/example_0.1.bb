SUMMARY = "Weather app recipee"
DESCRIPTION = "Pressure and temperature applications"
LICENSE = "MIT"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

#FILESEXTRAPATHS:append= "${THISDIR}/files:"

SRC_URI = "git://github.com/bohdan-kharytonov/tst_weather.git;branch=dev;protocol=https "
#SRC_URI += "file://aust.service "
#SRC_URI = "git@github.com:bohdan-kharytonov/tst_weather.git"
SRCREV = "4ee5093a009c20429e6dd1ccc9d62bf210a5735d"
SRC_URI[sha256sum] = "16fed98a21dbb03a8080bce8cf4ac85df11842b214e64beebab2144e6e53b4af"

S = "${WORKDIR}/git"
#B = "${WORKDIR}

DEPENDS = "mosquitto boost  qtbase qtx11extras"
#RDEPENDS:${PN} += "qtwayland"

CFLAGS += "-I${STAGING_INCDIR}/mosquitto -I${STAGING_INCDIR}/boost"
LDFLAGS += "-L${STAGING_LIBDIR}/boost"

#inherit systemd

#SYSTEMD_AUTO_ENABLE = "enable"
#SYSTEMD_SERVICE:${PN} = "aust.service"
FILES:${PN} += "/lib/systemd/system/aust.service \ 
		 ${systemd_system_unitdir} \
		/opt/tst_weather_ui/bin/tst_weather_ui"

do_configure() {
    # ...
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 tst_weather ${D}${bindir}/tst_weather
	install -d ${D}/${systemd_system_unitdir}/system
  	install -m 0644 ${WORKDIR}/aust.service ${D}/${systemd_system_unitdir}/system
}

inherit qmake5

