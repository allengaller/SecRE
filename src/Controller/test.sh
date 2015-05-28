#!/bin/bash

if [ ! $# == 3 ]; then

echo "Usage : ./test.sh {Conf} {Num} {Time}"
exit

fi

Conf=$1
Num=$2
Time=$3


get_char()
{
        SAVEDSTTY=`stty -g`
        stty -echo
        stty raw
        dd if=/dev/tty bs=1 count=1 2> /dev/null
        stty -raw
        stty echo
        stty $SAVEDSTTY
}


CURTIME=`date +%s`
CURTIME=$(($CURTIME+10))

rm -f res

for i in $(seq 1 $Num)
do

echo ./TestSys $i $Time $Conf $CURTIME
./TestSys $i $Time $Conf $CURTIME >> res &

done

echo "Press any key to continue..."
char=`get_char`


awk 'BEGIN{total=0}{total+=$1}END{print total}' res

