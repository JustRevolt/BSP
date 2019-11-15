if ($args.length -gt 0) {
	

    (Get-ACL $args[0]).Access | foreach {
        $Rights = $_.FileSystemRights

        $str = $Rights.ToString()
        
        if ($str.Contains("FullControl") -or $str.Contains("ReadAndExecute") -or $str.Contains("ExecuteFile") -or $str.Contains("ReadPermissions") -or $str.Contains("ReadAttributes") -or $str.Contains("WriteExtendedAttributes") -or $str.Contains("ReadData")) {
            $USERS = $_.IdentityReference
            Write-Host $USERS
            $strSID = $USERS.Translate([System.Security.Principal.SecurityIdentifier])
            $SID = $strSID.Value
            Write-Host $SID
            Write-Host "-------------"
            #Get-ADUser -Identity $SID
            #wmic useraccount where sid=$SID get name
            
        }

    }

}
else {
	write-host "No parametrs found."
}

