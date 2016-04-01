#!/bin/bash

# iterate all .in files in /tests directory
for f in tests/*.in
do
	echo "Test nr $f: "
	fullname=$(basename S$f)
	filename=${fullname%.*} #file name without extension
	outFile=$filename".out"
	errFile=$filename".err"
	./szpital -v < $f > tmp.out 2> tmp.err
	result=$(diff tmp.out tests/$outFile)
	if [ $? -eq 0 ] 
	then
		echo "OK .out"
	else
		echo "ERROR .out"
	fi
	result=$(diff tmp.err tests/$errFile)
	if [ $? -eq 0 ] 
	then
		echo "OK .err"
	else
		echo "ERROR .err"
	fi
	rm tmp.out tmp.err
done
