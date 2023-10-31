#!/bin/sh 

export DISPLAY=":0.0"
export QT_DEBUG_PLUGINS = 1
modprobe i2c_dev
/opt/tst_weather_ui/bin/tst_weather_ui
