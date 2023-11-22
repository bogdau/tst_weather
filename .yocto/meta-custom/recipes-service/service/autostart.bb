LICENSE = "CLOSED"

SRC_URI:append = " file://autostart.sh file://script.sh"

do_install:append() {
    install -d ${D}${sysconfdir}/init.d/
    install -m 0755 ${WORKDIR}/autostart.sh ${D}${sysconfdir}/init.d/
    install -d ${D}${sysconfdir}/rc5.d/
    ln -sf ../init.d/autostart.sh ${D}${sysconfdir}/rc5.d/S99autostart
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/script.sh ${D}${bindir}/script.sh
}

FILES_${PN} += "${sysconfdir}/init.d/autostart.sh \
                ${sysconfdir}/rc5.d/S99autostart \
                ${bindir}/script.sh"