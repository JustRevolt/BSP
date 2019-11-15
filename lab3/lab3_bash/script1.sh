#! /bin/bash


if [ $# -gt 0 ]

then
	uid=$( getfacl -n $1 | awk '{if($2 == "owner:") print $NF}' )
	gid=$( getfacl -n $1 | awk '{if($2 == "group:") print $NF}' )
	
	ux=$( getfacl -n $1 | awk '$1~/^u[[:print:]]*x$/ {print 1}' )
	gx=$( getfacl -n $1 | awk '$1~/^g[[:print:]]*x$/ {print 1}' )
	ox=$( getfacl -n $1 | awk '$1~/^o[[:print:]]*x$/ {print 1}' )
	
	if [[ $ux == 1 ]]
	then
		awk -v user=$uid -F:  '{if($3 == user) print $1}' /etc/passwd | sort
	fi		
	if [[ $gx == 1 ]]
	then
		awk -v group=$gid -F:  '{if($3 == group) print $NF}' /etc/group | sed 's/,/\n/' | sort
	fi
	if [[ $ox ]]
	then
		awk -F:  '{if($4 != $gid && $3 != uid) print $1}' /etc/passwd | sort
	fi

	
else
        echo "No parameters found. "
fi



