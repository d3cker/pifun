#!/bin/bash

wip=`ifconfig wlan0 | grep "inet addr" | cut -f2 -d":" | cut -f1 -d" "`
eip=`ifconfig eth0 | grep "inet addr" | cut -f2 -d":" | cut -f1 -d" "`

if [ "$wip" == "" ];then wip="0.0.0.0";fi
if [ "$eip" == "" ];then eip="0.0.0.0";fi

echo "W$wip" "E$eip"