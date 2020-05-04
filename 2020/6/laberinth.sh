#!/bin/bash

session=$1

[ -z "$session" ] && echo "A session file must be provided." && exit 1
[ ! -f "$session" ] && echo "Session file '$session' doesn't exists." && exit 1

while read mov; do
    echo "$mov"
    echo "$mov" | nc 127.0.0.1 4444
    sleep 1
done < $session

echo "Next movement: "
while read mov; do
    echo "$mov" | nc 127.0.0.1 4444
    echo "$mov" >> session.txt
    echo "Next movement: "
done
