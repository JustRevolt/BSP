#! /bin/bash

if [ $# -gt 0 ]
then
	ls -lt | awk -v file="$1" '/^l/{if ($NF == file) print $(NF-2)}'
else
	echo "No parameters found. "
fi

