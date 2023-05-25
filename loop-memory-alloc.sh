#!/bin/bash

cnt=0
for (( ; ; )); do
	./memory-alloc
	let cnt=${cnt}+1
	echo "Executed ${cnt} times"
	sleep 10
done

