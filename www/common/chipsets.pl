<:

sub render_one() {
  my $file = shift;
  my $line, my $inside, my $url, my $title,
	my $author, my $content, my $counter;
	
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
					$content = "\n<br>\n$content";
				}
				if ($url ne "") {
					$prettyurl="<a href=\"$url\">$title</a>";
				} else {
					$prettyurl=$title;
				}
				print <<"EOF";
  <tr bgcolor="#ffffe7">
    <td>$prettyurl$content</td>
    <td><img src="$(ROOT)img/$status.gif" width="16" height="16"></td>
  </tr>
EOF

        $inside=0;
        $counter=0;
				$content="";
        next;
      }
      if ($inside) {
				if ($counter==0) {
					$status=$line;
				}
				if ($counter==1) {
					$title=$line;
				}
				if ($counter==2) {
					$url=$line;
				}
				if ($counter>2) {
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

sub render_chipsets() {
  
  my $dir="../data/chipsets";
  my $file;
  my @datafiles;
	
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
			&render_one($item);
		}
		
  } else {
    print "<font color=\"#ff0000\">Internal Error:</font> Cannot open $dir directory!<br>\n";
  }
  
}

:>
