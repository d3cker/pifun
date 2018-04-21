#!/bin/bash
humi=`cat /tmp/sensor.txt | sed s/";"// | awk {'print $1'}`
/usr/bin/rrdtool update /rrd/datahumi.rrd N:${humi}
