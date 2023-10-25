LICENSE = "CLOSED"
inherit systemd

SYSTEMD_AUTO_ENABLE:${PN} = "enable"
SYSTEMD_SERVICE:${PN} = "start-tst-weather-app.service"

SRC_URI:append = " file://start-tst-weather-app.service file://script.sh"
FILES:${PN} += "${systemd_unitdir}/system/start-tst-weather-app.service \
		/opt/tst_weather_ui/bin/ "

do_install:append() {
  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/start-tst-weather-app.service ${D}/${systemd_unitdir}/system
  install -d ${D}${bindir}
  install -m 0755 ${WORKDIR}/script.sh  ${D}${bindir}/script.sh
}
