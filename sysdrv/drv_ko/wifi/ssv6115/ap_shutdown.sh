#! /bin/bash

echo "@@Unload hostapd..."
dir=$(pwd)
cd $dir
script/unload_ap.sh

PID=$!
wait $PID
sleep 2

echo "@@Unload driver..."
cd $dir
./unload.sh

PID=$!
wait $PID
sleep 2




