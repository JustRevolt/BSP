#!/usr/bin/perl

$type = 1;
$title = 0;
$zero = 0;
$num = 10;
@files = ();

$state = 0;
@number = ();

for ($i=0; $i<=$#ARGV; $i++){
	$n=0;

	print ("Argument: ",$ARGV[$i], "\n");

	@arg = split(//, $ARGV[$i]);
	foreach $symb (@arg) {
		print("Symbol: ", $symb, "\n");
		if ($state == 0){
			print("state: 0\n");
			if($symb == '-'){
				print("symb: -\n");
				$state = 1;
			}else{
				("file in: ", $i,"\n");
				push(@files, $i);
				goto file_saving;
			}
		}
		elsif ($state == 1){
			print("state: 1\n");

			if ($symb == 'c'){
				print("symb: c\n");
				$type = 0;
				$state = 2;
			}
			elsif ($symb == 'n'){
				print("symb: n\n");
				$type = 1;
				$state = 2;
			}
			elsif ($symb == 'q'){
				print("symb: q\n");
				$title = 0;
			}
			elsif ($symb == 'v'){
				print("symb: v\n");
				$title = 1;
			}
			elsif ($symb == 'z'){
				print("symb: z\n");
				$zero = 1;
			}
			else{
				printf ("head: invalid option -- '%c'\n", $symb);
			}

		}
		elsif ($state == 2){
			push (@number, $symb);
		}
		print("next iter\n");
	}
	file_saving:
	if ($state == 1) {$state = 0;}
	if (state == 2 && length(@number) != 0){
		$num = 0;
		$start = 0;
		if ($number[0] == '-') {
			$start = 1;
			shift(@number);
		}
		foreach $numeric (@number){
			if($numeric =~ /\b/){
				$num = $num * 10 + int($numeric);
				shift (@number);
			}else{
				printf("head: invalid number of lines: '%s'\n",@number);
				goto exit;
			}
		}
		if ($start == 1) {$num *= -1;}
		$state = 0;
	}

	print("Type: ",$type, "\n");
	print("Title: ",$title, "\n");
	print("Zero: ", $zero , "\n");
	print("Num: ",$num , "\n");
	print("FIELS: \n");
	foreach $l (@files){
		print($l, "\n");
	}


foreach $file (@files){
	$counter = 0;
	open(FH,'<'.$ARGV[$file].'>') || die;
	if ($title == 1) {
		printf("\n==> %s <==\n", $ARGV[$file]);
	}
}

}



exit:
