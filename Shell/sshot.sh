#!/bin/bash
/usr/bin/v4l2-ctl -c exposure_auto=1
/usr/bin/v4l2-ctl -c exposure_absolute=5
/usr/bin/v4l2-ctl -c white_balance_temperature_auto=1

/usr/bin/fswebcam -q -r 640x480 -s brightness=-64 -s sharpness=10  /var/www/html/image2.jpg
