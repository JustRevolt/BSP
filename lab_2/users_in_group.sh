#! /bin/bash
if [ $# -gt 0 ] 
then
	awk -v group=$1 -F:  '{if($1 == group) print $NF}' /etc/group | sed 's/,/\n/' | sort
else 
	echo "No parameters found. "
fi
