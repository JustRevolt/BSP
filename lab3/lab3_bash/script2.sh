#! /bin/bash

if [ $# -gt 0 ]

then
	user_uid=$( id -u $1 )
	user_gid=$( id -g $1 )

	for var in $(ls -l | awk '$1~/^d/ {print $NF}')
	do
		dir_uid=$( getfacl -n $var | awk '{if($2 == "owner:") print $NF}' )
        	dir_gid=$( getfacl -n $var | awk '{if($2 == "group:") print $NF}' )

		uwx=$( getfacl -n $var | awk '$1~/^u[[:print:]]*wx$/ {print 1}' )
        	gwx=$( getfacl -n $var | awk '$1~/^g[[:print:]]*wx$/ {print 1}' )
        	owx=$( getfacl -n $var | awk '$1~/^o[[:print:]]*wx$/ {print 1}' )
		
		if [[ 	( $user_uid == $dir_uid && $uwx == 1 ) || 
			( $user_gid == $dir_gid && $gwx == 1 ) || 
			( $owx == 1 && $user_uid != $dir_uid && $user_gid != $dir_gid ) ]]
		then
			echo $var
		fi
	done
else
	echo "No parameters found. "
fi
