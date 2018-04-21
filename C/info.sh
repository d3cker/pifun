#!/bin/bash
a=0

while [ $a -lt 1 ];do 

link=`iwconfig wlan0 | grep Bit | cut -f2 -d"=" | cut -f1 -d" "`
tx=`iwconfig wlan0 | grep Bit | cut -f3 -d"=" | cut -f1 -d" "`

lk=`iwconfig wlan0 | grep Link | cut -f2 -d"=" | cut -f1 -d" "`
cur=`echo $lk|cut -f1 -d"/"`
max=`echo $lk|cut -f2 -d"/"`

ile=`echo $[cur*10/max]`
ilb=$ile

rys=""

while [ $ile -gt 0 ];do 
    rys="${rys}#"
    ile=$[ile-1]
done
echo $rys
echo "${link}Mb/s | ${tx}dbm | ${lk} ${cur} ${max} ${ilb}"

sudo ./mytest "${link}Mb/s | ${tx}dbm" "${lk} ${rys}"

sleep 1

done
