#!/bin/sh 

sleep 20

cd /opt/tst_weather_ui/bin/

ntpdate pool.ntp.org
modprobe i2c_dev
/opt/tst_weather_ui/bin/tst_weather_ui
