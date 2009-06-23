#!/usr/bin/perl

#use strict; # commented out becouse do settings

do('spt2c.conf.pl');

my $help = do('readme');

for ($bitclear, $bitset, $begin_sep, $end_sep) { s/(.)/\\$1/g }

my ($infile,$outfile) = @ARGV;

if ((not defined $infile) || (not defined $outfile))
{ die ("$help\nError: Parameter missing.\n");} 

my (@lines);

if    (open IN, ($infile))
      { @lines = (<IN>); }
else { die ("$help\nError: Can not open $infile\n"); }

close IN;

my $cnt = 0;
my $ind = 2;

if (open OUT, ">$outfile") 
      { print OUT <<HEADER; }
/* C code data file generated by spt2c.pl */

HEADER
else { die ("$help\nError: Can not write $outfile\n"); }

my $name = 'sprite0';

for (@lines)
{
    chomp;
#    s/\r//;  # only if .spt in win and .c in unix format
    my $line = $_;
    
    
    $line =~ s/$bitclear/0/g;
    $line =~ s/$bitset/1/g;
    
    if ($line =~ /$begin_sep([01]{2,})$end_sep/)
    {
      if ($ind == 0)
      {
        print OUT ("const unsigned char $name\[] = \n{\n");
        $ind = 1;
        $cnt++;
      }
        
      my $data = $1;      
      my $code = '  ';
      
      while ($data =~ s/([01]{8}|[01]{1,7})//)
      {
        $code .= '0x'.sprintf("%02x",eval("0b$1".'0'x(8-length("$1")))).', ';
      }
      $code .= ' 'x($commentcol-length($code));
      $code .= "/* $_ */\n";
      print OUT $code; 
    }
    else
    {
       if ($ind > 0)
       {
         if ($ind == 1) { print OUT ("}\n"); }
         $name = "sprite$cnt";
         $ind = 0;
       } 
       if (/name:\s*(\w+)/) { $name = $1; }
       
       print OUT ((' 'x($commentcol))."/* $_ */\n");
    }
}

close OUT;
