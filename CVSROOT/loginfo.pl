#!/usr/bin/perl -w

# free of any copyright. Originally written by taj@kde.org with small changes
# by coolo@kde.org and daniel.naber@t-online.de

# Many improvements and fixes by dirk@kde.org
# Hacks for Linux-IrDA by claudiuc@kde.org


sub read_line($) {
  my $filename = shift @_;
  my $line;

  if ( open(FILE, "<$filename") ) {
    $line = <FILE>;
    close(FILE);
    chop($line);
  }
  return $line;
}

$ccrecipients = "";
$lastdir = "";
$basedir = "";
$cvsroot = $ENV{"CVSROOT"};
delete $ENV{"HOME"}; # make sure that CVS doesn't use "/root" as $HOME
$id = getpgrp();
$lastdir_file = "/tmp/#cvs.lastdir.$id";
$lastout_file = "/tmp/#cvs.out.$id";
$lastdiff_file = "/tmp/#cvs.diff.$id";
$max_diff_length=3072;
$modified_diff="";
$srcfile = '\.(cpp|cc|cxx|C|c\+\+|c|l|y||h|H|hh|hxx|hpp|h\+\+)$';

# key is a word matched against the commit-path,
# value is the recipient's address if the word matches
%directorysubscriptions = (
);

sub read_state()
{
  # here we have to "reload" our list of committed files
  # to be able to generate a final commit log message when
  # current dir == lastdir. 

  # load the last dir
  if (open(LD, "<$lastdir_file")) {
    $lastdir = <LD>;
    chop $lastdir;
    $basedir = <LD>;
    chop $basedir;
    close (LD);
  }

  # see if we have something from previous runs already
  if (open(TXT, "<$lastout_file")) {
    $outText = join('', <TXT>);
    close(TXT);
  }

  # see if we have some diff from previous run already
  if (open(TXT, "<$lastdiff_file")) {
    $modified_diff=join('', <TXT>);
    close(TXT);
  }
}

sub save_state()
{
  # save the new out text for the future loginfo call
  if (open(TXT, ">$lastout_file")) {
    print TXT $outText;
    close (TXT);
  }

  if(open(TXT, ">$lastdiff_file")) {
    print TXT $modified_diff;
    close(TXT);
  }
}

sub change_summary($) {
  my $file = shift @_;
  return "" if ($file eq "");

  open(RCS, "-|") || exec 'cvs', '-Qn', 'status', $file;

  my $rev = "";
  my $delta = "";
  my $rcsfile = "";

  while (<RCS>) {
    if (/^[ \t]*Repository revision/) {
      chop;
      my @revline = split(' ', $_);
      $rev = $revline[2];
      $rcsfile = $revline[3];
      $rcsfile =~ s,^$cvsroot/,,;
      $rcsfile =~ s/,v$//;
    }
  }
  close(RCS);

  if ($rev ne '' && $rcsfile ne '') {
    open(RCS, "-|") || exec 'cvs', '-Qfn', 'log', "-r$rev", $file;
    while (<RCS>) {
      $delta = $1 if (/^date:.*;\s+lines:\s+(\+\d+ \-\d+)/);
    }
    close(RCS);
  }

  return $delta;
}

sub check_for_unsafety($)
{
  my @stuff = @_;
  my $found = ""; 
  foreach my $current(@stuff) {
    next if ($current =~ /^\s*\/\//);
    $current =~ s/\"[^\"]*\"//g;
    $current =~ s/\/\*.*\*\///g;
    $current =~ s,//.*,,g;
#    $found .= "$1," if ($current =~ /\b(KRun::runCommand|KShellProcess|setUseShell)\b\s*[\(\r\n]/ and $found !~ /$1/);
#    $found .= "$1," if ($current =~ /\b(qDebug|system|popen|mktemp|mkstemp|tmpnam|gets|syslog|strptime)\b\s*[\(\r\n]/ and $found !~ /$1/);
#    $found .= "$1," if ($current =~ /(printf|scanf)\b\s*[\(\r\n]/ and $found !~ /$1/);
  }
  chop($found);
  return $found;
}

sub commit_diff($$$$)
{
  my ($module, $file, $r1, $r2) = @_;
  return "" if ($file eq "");

  my $copts = "";
  $copts = "dpbB" if ($file =~ /$srcfile/);

  open(RCS, "-|") || exec 'cvs', '-Qfn', 'di', '-kk', "-stu2$copts", "-r$r1", "-r$r2", $file;
  my $cnt = 0;
  my $diff = "";
  while(<RCS>) {
    next if ($cnt++ < 6);
    next if ($cnt == 7 and !/^---/);
    s/^--- \S+.*(\S+)$/--- $module\/$file  #$r1:$r2/ if($cnt == 7);
    next if ($cnt == 8);
    $diff .= $_;
  }
  close(RCS);
  $diff .= "\n" if (length($diff));
  return $diff;
}

sub check_commit_modified($$$$)
{
  my ($module, $file, $r1, $r2) = @_;
  my $current_diff = &commit_diff($module, $file, $r1, $r2);
  my $secproblems = "";
  if(length($current_diff)) {
    my @newstuff = grep (/^\+/, split(/^/, $current_diff));
    $secproblems = &check_for_unsafety(@newstuff) if($file =~ /$srcfile/o);
    $modified_diff .= $current_diff if(length($modified_diff) < $max_diff_length);
  }
  return $secproblems;
}

sub checklicense($)
{
   my ($text) = @_;

   $text =~ y/\t/ /;
   $text =~ y/ A-Za-z.,@1-9\(\)//cd;
   $text =~ s/\s+/ /g;

   my ($gl, $qte, $license, $massave);

   $gl = "";
   $qte = "";
   $license = "";
   $massave = "";

   $gl = " (v2+)" if ($text =~ /either version 2 of the License, or .at your option. any later version/);
   $gl = " (v2.1)" if ($text =~ /version 2\.1 as published by the Free Software Foundation/);
   $qte = " (+Qt exception)" if ($text =~ /([Pp]ermission is given|[pP]ermission is also granted|[pP]ermission) to link this program with (the Qt library|any edition of Qt)/);
   $massave = " (wrong address)" if ($text =~ /675 Mass Ave/i);

   $license="GENERATED FILE" if ($text =~ /(All changes made in this file will be lost|DO NOT EDIT|DO NOT delete this file|[Gg]enerated by)/);
   $license="LGPL$gl$massave $license" if ($text =~ /This (program|library) is free software you can redistribute it and.?or modify it under the terms of the GNU (Library|Lesser) General Public License/);
   $license="GPL$gl$qte$massave $license" if ($text =~ /is free software you can redistribute it and.?or modify it under the terms of the GNU General Public License/);
   $license="QPL (part of Qt) $license" if ($text =~ /This file is part of the .*Qt GUI Toolkit. This file may be distributed under the terms of the Q Public License as defined/);
   $license="QPL $license" if ($text =~ /may be distributed under the terms of the Q Public License as defined by Trolltech AS/);
   $license="X11 (BSD like) $license" if($text =~ /Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files .the Software., to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and.?or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED AS IS/);
   $license="no copyright $license" if ($text !~ /copyright/i);
   $license="UNKNOWN" if(!length($license));

   $license =~ s/ $//;
   return "$license";
}

sub check_commit_added($$$)
{
  my ($module, $file, $r2) = @_;
  my $secproblems = "";
  my $license = "";
  if($file =~ /$srcfile/o) {
      open(F, "-|") || exec 'cvs', '-Qfn', 'co', '-kk', "-p", "-r$r2", "$module/$file";
      my @c = <F>;
      my $htxt = join '', @c[0.. ($#c > 29 ? 29 : $#c)];
      $license .= &checklicense($htxt);
      $secproblems .= &check_for_unsafety(@c);
      close(F);
  }
  return ($secproblems,$license);
}

sub add_to_cc($)
{
  my ($email) = @_;
  $ccrecipients .= length($ccrecipients) ? ", $email" : "\nCc: $email" if($ccrecipients !~ /$email/);
}

# initialize with the default values
$outText = "";
$logmessage = "";
$author = "";
$module = "";
$recipient = 'irda-cvs@lists.sourceforge.net';
$from = "";
$tagname = "HEAD";

%rev_old = ();
%rev_new = ();
%rev_file = ();

#now load the state
&read_state();

$juststarted = 1;
$beforelogmsg = 1;
$silentcommit = 0;

while(<>) {
  if( $juststarted ) {
    next if /^\s*$/;
    $juststarted = 0;
    chop;
    if(/^(\S+) (.*)$/) {
       $module = $1;
       $_ = $2;
    }
    # "directory - New Directory" ?
    # "directory - Import sources" ?
    if(/^- /) {
        $silentcommit = 1 if(/^- New/);
        next;
    }
    while(/^([^,]+,(?:NONE|[\d\.]+),(?:NONE|[\d\.]+)) ?(.*)/) {
        push @files, $1;
        $_ = $2;
    }
    foreach my $f (@files) {
      next if (!length($f));
      ($file,$r1,$r2) = $f =~ /(.+),(.+),(.+)/;
      $rev_old{$file} = $r1;
      $rev_new{$file} = $r2;
      $rev_file{$file} = 1;
      my $status = "M";
      $status = "R" if ($r2 eq "NONE");
      $status = "A" if ($r1 eq "NONE");
      my $rev = $r2;
      $rev = $r1 if ($r2 eq "NONE");
      my $relfile = "$module/$file";
      $relfile = substr ($relfile, length($basedir)+1) if(length($basedir));
      my $delta="";
      my $secproblems="";
      my $license="";
      if ($status eq "M") {
        $delta = &change_summary($file);
        if(defined($delta) && $delta eq "+0 -0") {
          $delta = "";
          $status = "A";
        }
      }
      $secproblems = &check_commit_modified($module, $file, $r1, $r2) if ($status eq "M");
      ($secproblems,$license) = &check_commit_added($module, $file, $r2) if ($status eq "A");

      $outText .= " $status\t$relfile\t$rev\t$r1\t$r2\t$delta\t$secproblems\t$license\n";
    }
    next;
  }

  if ($beforelogmsg) {
    next if (/^Update of /);
    next if (/^In directory /);

    if (/^Author: \s*(\S+)/) { $author = $1;      }
    elsif (/^\s*Tag: (\S+)/) { $tagname = $1;     }
    elsif (/^Log Message:/)  { $beforelogmsg = 0; }
    next;
  }
  else {
    &add_to_cc($1) if( /^CCMAIL:\s*(.*)/ );
  }

  $logmessage .= $_;
  $silentcommit = 1 if (/CVS.?SILENT/);
}

while (chomp $logmessage) {};

&save_state();
exit 0 if (length($lastdir) and $lastdir ne $cvsroot."/".$module);

unlink $lastdir_file, $lastout_file, $lastdiff_file;

$module = "/" if (length($lastdir));
$module = $basedir if (length($basedir));
$subject = "$module";
$subject = "$tagname: $module" if ($tagname ne 'HEAD');

$scriptysilent = 0;

if( $silentcommit ) {
   if ($author eq 'www' || $module =~ '^scripts' || $module =~ '^CVSROOT') {
     $recipient = "dc2rpt\@gmx.de";
     $from = "CVS silently by ";
     $scriptysilent = 1;
   } else {
     $subject = "$subject (silent)";
   }
}

# sort the list of affected files to make it look cooler
@outlist = sort (split /^/, $outText);
$fmtText = "";
my $hassecproblems = 0;
foreach my $o (@outlist) {
  chop $o;
  my ($state, $file, $rev, $r1, $r2, $cvsinfo, $secproblems, $license) = split/\t/, $o;
  $fmtText .= sprintf(" %s %-10s %s   %s%s%s\n", $state, $cvsinfo, $file, $rev,
                      length($secproblems) ? " [POSSIBLY UNSAFE: $secproblems]" : "",
                      length($license) ? " [$license]" : "");
  $hassecproblems += length($secproblems);
}

$subject .= " [POSSIBLY UNSAFE]" if ($hassecproblems);

# generate a list of affected directories
my %dirs = ();
foreach my $d (@outlist) {
  $d = $1 if ($d =~ /^\s*\S\s*(\S+)/);
  my $current = $basedir . "/$d";
  $current = $1 if ($current =~ /^(.*)\/[^\/]+$/);
  $current = $1 if ($current =~ /^([^\/]+\/[^\/]+)\//);
  $dirs{$current} = 1;

  # check if we want to cc somebody
  if(!$scriptysilent) {
    foreach $key ( keys %directorysubscriptions ) {
      &add_to_cc($directorysubscriptions{$key}) if ($current =~ /$key/);
    }
  }
}

my $commit_list = join(' ', (sort { $dirs{$b} <=> $dirs{$a} } keys %dirs));
my $commit_dirs = "";
$commit_dirs = "\nX-Commit-Directories: " . substr($commit_list, 0, 512) if (length($commit_list));

my $blame = "$author <irda-cvs\@lists.sourceforge.net>";
if ($author eq 'www') {
  $blame = "Thomas - DC2RPT <dc2rpt\@gmx.de>";
} else {
  open(INFO, "/usr/bin/cvs -fn co -p common/accounts 2>/dev/null |");
  my @info = grep /^$author\s*(.*)\s+\S+\s*$/, <INFO>;
  close(INFO);

  if (defined($info[0]) && $info[0] =~ /^\S*\s+(\S.+\S)\s\s*(\S+)\s*$/) {
    my $name = $1;
    my $addr = $2;
    my $nname = '';
    if($name =~ /[_<>\@\(\),;:\"\/\[\]\?\.\x80-\xff]/) {
      foreach my $c (split('', $name)) {
        if ($c =~ /[ _<>\@\(\),;:\"\/\[\]\?\.\=\x80-\xff]/) {
         $nname .= sprintf("=%X", ord($c));
        } else {
         $nname .= $c;
        }
      }
      $blame = " =?utf-8?q?$nname?= <$addr>";
    }
    else {
      $blame = " $name <$addr>";
    }
  }
}


$modified_diff = "" if (length($modified_diff) >= $max_diff_length);

open (MAIL, "|/usr/lib/sendmail -t") or die "Could not open sendmail: $!";
print MAIL<<EOF;
From: $from$blame
To: $recipient$ccrecipients
Subject: $subject$commit_dirs
X-Warn: This a modified version for Linux-IrDA CVS
Content-Type: Text/Plain;
  charset="UTF-8"

CVS commit by $author: 

$logmessage


$fmtText

$modified_diff
.
EOF

close(MAIL);

