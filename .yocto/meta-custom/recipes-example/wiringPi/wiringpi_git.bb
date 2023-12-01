DESCRIPTION = "A library to control Raspberry Pi GPIO channels"
HOMEPAGE = "https://projects.drogon.net/raspberry-pi/wiringpi/"
SECTION = "devel/libs"
LICENSE = "LGPLv3+"
LIC_FILES_CHKSUM = "file://COPYING.LESSER;md5=e6a600fd5e1d9cbde2d983680233ad02"

DEPENDS = "libxcrypt"

S = "${WORKDIR}/git"

SRCREV = "1ad03dd3265402baaf0f783708e28c6255032820"
SRC_URI = " \
    git://github.com/WiringPi/WiringPi.git;branch=master;protocol=https \
    file://0001-Add-initial-cross-compile-support.patch \
    "
SRC_URI[sha256sum] = "5756ce251bdc8fd77a8fed467a5ac64dd04ec34f3ae14ba19fe25edf8f81114a"

COMPATIBLE_MACHINE = "raspberrypi"

CFLAGS:prepend = "-I${S}/wiringPi -I${S}/devLib"

EXTRA_OEMAKE += "'INCLUDE_DIR=${D}${includedir}' 'LIB_DIR=${D}${libdir}' 'LD_EXTRA=${LDFLAGS}'"
EXTRA_OEMAKE += "'DESTDIR=${D}/usr' 'PREFIX=""'"

do_compile() {
    oe_runmake -C devLib 'LDFLAGS=${LDFLAGS}'
    oe_runmake -C wiringPi 'LDFLAGS=${LDFLAGS}'
    oe_runmake -C gpio 'LDFLAGS=${LDFLAGS} -L${S}/wiringPi -L${S}/devLib'
}

do_install() {
    oe_runmake -C devLib install
    oe_runmake -C wiringPi install
    oe_runmake -C gpio install
}
