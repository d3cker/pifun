#!/bin/bash
temp=`cat /tmp/sensor.txt | sed s/";"// | awk {'print $2'}`
/usr/bin/rrdtool update /rrd/datatemp.rrd N:$temp
