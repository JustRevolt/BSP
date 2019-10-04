#! /bin/bash

if [ -n "$1" ]
then
awk -v group=$1 -F:  '{if($1 == group) print $NF}' /etc/group | sed 's/,/\n/'
else
echo "No parameters found. "
fi

