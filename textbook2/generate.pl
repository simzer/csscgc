
use strict;

#------------- load datas

open SRC, "input.xls";
my @lines = (<SRC>);
close SRC;

my ($datas, $cnt);

my $startpage = '';

for (@lines)
{
  my $line = $_;
  
  chomp($line);

  my ($id, $content, @links) = split(/\t/, $line);
  
  if ($startpage eq '') { $startpage = $id }
  
#  $content =~ s/\\t/\t/g;
#  $content =~ s/\\n/\n/g;
  $content =~ s/"/\\"/g;  
  
  $datas->{$id}->{content} = $content;
  
  $cnt = 0;
  for (@links) 
  { 
    if (/\s*(\w+)\s*/) 
	  {
	  $datas->{$id}->{links}->[$cnt] = $1; 
	  }
	  $cnt++
  }
}

#------------ convert datas

my $poi4conts = 0;
my $poi4links = 0;

my ($ids);

my $contents = '';
my (@cont_pointer);

my (@links);
my (@link_pointer);
  
my ($i) = 0;
my($id, $data);


my $def_contents;
my $def_cont_pointer;
my $def_links;

while (($id, $data) = each(%{$datas}))
{ 
  $ids->{$id} = $i;  
  $i++;
}

my $imax = $i+1;

while (($id, $data) = each(%{$datas}))
{
  $i = $ids->{$id};

  $cont_pointer[$i] = $poi4conts; 
  $contents .= $data->{content};
  
  my $content = $data->{content};
  $content =~ s/\\n/\\n"\n"/g;
  $def_contents .= <<OUT;
/* $i: $id*/
const char content_$id\[] = 
"$content";
OUT

  $def_cont_pointer .= <<OUT;
  cont_pointer[$i] = content_$id;
OUT

  ($content = $data->{content}) =~ s/\\//g;
  $poi4conts += length($content);
  
  $link_pointer[$i] = $poi4links;
  
  if(defined $data->{links})
  { 
    push @links, @{$data->{links}};
    $poi4links += @{$data->{links}};
     
	my @datalinks;
	for ( @{$data->{links}} )
	{
	  if (exists $ids->{$_})
	  {
	    push @datalinks, $ids->{$_};
	  }
	  else
	  {
	    warn("'$_' row id does not exists.\n");
	  }
	}	
	
    #my @datalinks = map ( "$ids->{$_}"), @{$data->{links}};  
    $def_links .= join(', ',@datalinks).", /* $i: $id*/\n";
  }
  else
  {
    $poi4links += 1;
    $def_links .= "$i, /* $i: $id*/\n";    
  }
}

if ($i>254) { die("max 255 page allowed.\n") }

#-------------- write out code

#@links = map "$ids->{$_}", @links;

#$def_contents = $contents;
#my $def_cont_pointer = join(', ',@cont_pointer, $poi4conts);
$def_cont_pointer =~ s/((?:\s*\d+\s*,\s*){10})/$1\n/g;

#$def_links = join(', ',@links);
my $def_link_pointer = join(', ',@link_pointer, $poi4links);
$def_link_pointer =~ s/((?:\s*\d+\s*,\s*){10})/$1\n/g;

open OUT, ">data.inc";
	
print OUT <<OUT;

$def_contents

const unsigned short cont_pointer[$imax];

const unsigned char links[$poi4links] = 
{
$def_links};

const unsigned short link_pointer[$imax] = 
{
$def_link_pointer
};

const unsigned char poi_def = $ids->{$startpage};

void init(void)
{
$def_cont_pointer 
}

OUT

close OUT;
