#!/bin/bash

cnt=0
while true
do
	./memory-alloc
	let cnt=${cnt}+1
	echo "Executed ${cnt} times"
	sleep 10
done
