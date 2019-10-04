#! /bin/bash

[ $# -gt 0 ] && 
awk -v group=$1 -F:  '{if($1 == group) print $NF}' /etc/group | sed 's/,/\n/' || 
echo "No parameters found. "

