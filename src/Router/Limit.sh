#!/bin/bash

/sbin/tc qdisc change dev eth0 root tbf rate 6000kbit latency 50ms burst 10000
sleep 60

#2
/sbin/tc qdisc change dev eth0 root tbf rate 4000kbit latency 50ms burst 10000
sleep 80

#3
/sbin/tc qdisc change dev eth0 root tbf rate 3000kbit latency 50ms burst 10000
sleep 100

#4
/sbin/tc qdisc change dev eth0 root tbf rate 2000kbit latency 50ms burst 10000
sleep 80

/sbin/tc qdisc change dev eth0 root tbf rate 1500kbit latency 50ms burst 10000
sleep 60

#set 1Mb
#1
/sbin/tc qdisc change dev eth0 root tbf rate 1000kbit latency 50ms burst 10000
sleep 40

#2
/sbin/tc qdisc change dev eth0 root tbf rate 500kbit latency 50ms burst 10000
sleep 60

#3
/sbin/tc qdisc change dev eth0 root tbf rate 300kbit latency 50ms burst 10000
sleep 100

#4
/sbin/tc qdisc change dev eth0 root tbf rate 700kbit latency 50ms burst 10000
sleep 80

#5
/sbin/tc qdisc change dev eth0 root tbf rate 500kbit latency 50ms burst 10000
sleep 60

#6
/sbin/tc qdisc change dev eth0 root tbf rate 800kbit latency 50ms burst 10000
sleep 40

#7
/sbin/tc qdisc change dev eth0 root tbf rate 1200kbit latency 50ms burst 10000
sleep 70

#8
/sbin/tc qdisc change dev eth0 root tbf rate 1500kbit latency 50ms burst 10000
sleep 60

#9
/sbin/tc qdisc change dev eth0 root tbf rate 3000kbit latency 50ms burst 10000
sleep 80

#10
/sbin/tc qdisc change dev eth0 root tbf rate 5000kbit latency 50ms burst 10000
sleep 100

#11
/sbin/tc qdisc change dev eth0 root tbf rate 2000kbit latency 50ms burst 10000
sleep 80

#12
/sbin/tc qdisc change dev eth0 root tbf rate 500kbit latency 50ms burst 10000
sleep 50

/sbin/tc qdisc change dev eth0 root tbf rate 6000kbit latency 50ms burst 10000

ps aux|grep nodejs|grep -v grep|awk '{print $2}' |xargs  kill -9
echo Finish




