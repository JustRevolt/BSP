#!/usr/bin/perl
$num = 0;
@number = ('2', '4');
foreach $numeric (@number){
	if(int($numeric) =~ /\d/){
		
		$num = $num * 10 + int($numeric);
		print($num, "\n");
	}
}
