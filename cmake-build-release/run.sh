#!/bin/bash

b=$2
origin=$1
rf=$3
j=0.1
file=$4
rm $file | echo

echo [ >> $file

for ((sp=1;sp<10;sp++))
do
    k=0.1
    for ((ip=1;ip+sp<10;ip++))
    do
        echo runing balances=$b search percent=$j insert percent=$k
        echo [ >> $file
        i=0
        while ((i<50))
        do
            ./rwlock 10 $origin $b $j $k $rf >> $file
            if ((i < 49))
            then
                echo , >> $file
            else
                echo >> $file
            fi
            ((i++))
        done

        if ((sp==8 && ip+sp==9))
        then
            echo ] >> $file
        else
            echo ], >> $file
        fi

        k=$(echo "scale=1; $k+0.1" | bc | awk '{printf "%.1f", $0}')
    done
    j=$(echo "scale=1; $j+0.1" | bc | awk '{printf "%.1f", $0}')
done

echo ] >> $file