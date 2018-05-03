#!/bin/bash


mkdir /rrd

rrdtool create /rrd/datahumi.rrd \
--step '10' \
'DS:humi:GAUGE:60:0:100' \
'RRA:MIN:0.5:1:2000' \
'RRA:AVERAGE:0.5:1:2000' \
'RRA:MAX:0.5:1:2000'

rrdtool create /rrd/datatemp.rrd \
--step '10' \
'DS:temp:GAUGE:60:-100:100' \
'RRA:MIN:0.5:1:2000' \
'RRA:AVERAGE:0.5:1:2000' \
'RRA:MAX:0.5:1:2000'
