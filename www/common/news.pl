<:

sub render_one_news() {
  my $file = shift;
  my $newsmax = shift;
  my $line, my $inside, my $title,
	my $date, my $content, my $counter;

  $newscounter=0;	
  if (open(FH,$file)) {
    $inside=0;
    $counter=0;
		$content="";
    while ($line=<FH>) {
			chop $line;
      if ($line =~ /^\#/)     { next; }
      if ($line =~ /^:::begin/) {
        $inside=1;
        next;				
      }
      if ($line =~ /^:::end/) {
				if ($content ne "") {
					$content = "\n$content";
				}
      if ($style == 0) {
				print <<"EOF";
<p>
<table border="0" cellspacing="0" cellpadding="0" width="95%">
<tr bgcolor="#ffffe7">
  <td><nobr>&nbsp;$date</nobr></td>
  <td width="100%">&nbsp;&nbsp;<big><b>$title</b></big></td>
</tr>
<tr bgcolor="#777777">
  <td><px width="1" height="1"></td>
  <td><px width="1" height="1"></td>
</tr>
</table>
<px width="1" height="5"><br>
$content
EOF
      } else {
				print <<"EOF";
<p>
  &nbsp;$date&nbsp;&nbsp;<big><b>$title</b></big><br>
<px width="1" height="5"><br>
$content
EOF
      }      
        $inside=0;
        $counter=0;
				$content="";
        $newscounter++;
        if ($newscounter >= $newsmax) {
          last;
        }
        next;
      }
      if ($inside) {
				if ($counter==0) {
					$title=$line;
				}
				if ($counter==1) {
					$date=$line;
				}
				if ($counter>1) {
					$content.=$line . "\n";
				}
      	$counter++;
      }
    }
    close(FH);
  } else {
    print "<font color=\"#ff0000\">Internal Error:</font> Cannot open $file file!<br>\n";
  }  
}

sub render_news() {

  $newsmax=shift;  
  $style=shift;
  my $dir="../data/news";
  my $file;
  my @datafiles;
	
  if ($newsmax == 0) {
    $newsmax=100000;
  }
  if (opendir(DH,$dir)) {
    while ($item=readdir(DH)) {
      if ($item =~ /^\./)     { next; }
      if ($item !~ /\.data$/) { next; }
      if (! -f "$dir/$item")  { next; }
			push @datafiles, "$dir/$item";
    }
    close(DH);
		
		@datafiles = sort @datafiles;
		foreach $item (@datafiles) {
			&render_one_news($item, $newsmax, $style);
		}
		
  } else {
    print "<font color=\"#ff0000\">Internal Error:</font> Cannot open $dir directory!<br>\n";
  }
  
}





:>
