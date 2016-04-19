#!/bin/bash
echo "bash version:  ${BASH_VERSION}"
for i in {1..50}  
do
#	echo $i 
	./test>>result.txt	
done
