#!/usr/bin/perl

$ver_major = 1;
$ver_minor = 13;

$appname = $ARGV[0]; # application name from argv
$scrfile = $ARGV[1];
$binfile = $ARGV[2];
$outfile = $ARGV[3];


my($appname, $scrfile, $binfile, $outfile) = ('imarath', 'loadscr.scr', '_mpimy.bin', 'imarhath.tzx');

#------------------

$file_header = pack "a7CCC", "ZXTape!", 0x1A, $ver_major, $ver_minor;

#------------------

open BIN, $binfile;
while (read BIN, $bin, 16384) {};
close BIN;

$pause = 1000;
$bin_length = length($bin);

$bin_header = pack "vv", $pause, $bin_length;

#------------------

open SCR, $scrfile || die ($!);
#seek SCR, 0, 0; 
#print tell(SCR), "\n";
$length = read(SCR, $scr, 6912);
print "$length, scr:".length($scr)."\n";
close SCR;

$scr_name = $appname;
(($scr_length = 256 * 192 / 8 + 32 * 24) == length($scr));# || die "scr file size mismatch!\n";
$border_color = 0;


print "data:$datalength\n";

$scr_header = pack "Ca9aVCC", 35, $scr_name, ' ', $scr_length, 0, $border_color;

#------------------

open OUT, ">$outfile";
#print OUT $file_header, $scr_header, $scr, $bin_header, $bin; 
print OUT $file_header, $scr_header, $scr; 
close OUT;
