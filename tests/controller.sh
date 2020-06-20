#!/bin/sh

>/dev/tty stty -echo

X_VALUE=0
Y_VALUE=0

while :; do
    tput cup 0 0 > /dev/tty

    key="\0"

    while [ "$key" != "" ]; do
        tput cup $Y_VALUE $((X_VALUE * 2)) > /dev/tty

        read -rsn1 key

        if [ "$key" == "A" ]; then
            Y_VALUE=$((Y_VALUE - 1))
        elif [ "$key" == "B" ]; then
            Y_VALUE=$((Y_VALUE + 1))
        elif [ "$key" == "C" ]; then
            X_VALUE=$((X_VALUE + 1))
        elif [ "$key" == "D" ]; then
            X_VALUE=$((X_VALUE - 1))
        fi

        if [ $X_VALUE -lt 0 ]; then
            X_VALUE=0
        fi
        if [ $Y_VALUE -lt 0 ]; then
            Y_VALUE=0
        fi
    done

    tput clear > /dev/tty
    tput cup 0 0 > /dev/tty

    echo $X_VALUE
    echo $((Y_VALUE - 3))
done
