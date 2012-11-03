#!/usr/bin/perl

while(<STDIN>)
{
  if(/(\w+):"([^"]+)"\s*L:(\d+)\s+(\w+):#(\d+):(.*)/)
  {
    ($compiler,$filename,$linenum,$errortype,$errornum,$errordesc) = ($1,$2,$3,$4,$5,$6);
    print STDERR "$filename:$linenum: \L$errortype: $compiler ($errornum): $errordesc\n";
  }
  else
  {
    print STDERR $_;
  }
}