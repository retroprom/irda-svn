Name: irda-utils
Summary: IrDA Utilities
Version: 0.9.13
Release: 1
Source: ftp://irda.sourceforge.net/pub/irda/irda-utils/
Group: Applications/Networking
URL: http://irda.sourceforge.net/
BuildRoot: /var/tmp/%{name}-buildroot
Copyright: GPL
Prefix: /usr

%description
Tools required to use IrDA (infrared ports)

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q

%build
make all RPM_BUILD_ROOT="$RPM_BUILD_ROOT" RPM_OPT_FLAGS="$RPM_OPT_FLAGS" ROOT="$RPM_BUILD_ROOT"

%install
make install RPM_OPT_FLAGS="$RPM_OPT_FLAGS" ROOT="$RPM_BUILD_ROOT"

if [ -d $RPM_BUILD_ROOT/usr/man ]; then
  find $RPM_BUILD_ROOT/usr/man -type f -exec bzip2 -9f {} \;
fi
if [ -d $RPM_BUILD_ROOT/usr/info ]; then
  find $RPM_BUILD_ROOT/usr/info -type f -exec bzip2 -9f {} \;
fi
if [ -d $RPM_BUILD_ROOT/usr/X11R6/man ]; then
  find $RPM_BUILD_ROOT/usr/X11R6/man -type f -exec bzip2 -9f {} \;
fi
if [ -d $RPM_BUILD_ROOT/usr/lib/perl5/man ]; then
  find $RPM_BUILD_ROOT/usr/lib/perl5/man -type f -exec bzip2 -9f {} \;
fi

for dir in irattach irkbd tekram; do
    cp $dir/README $dir/README.$dir
done

for dir in irdadump irdaping; do
    cp $dir/README.txt $dir/README.$dir
done

%clean
rm -rf $RPM_BUILD_ROOT $RPM_BUILD_DIR/file.list.%{name}

%files
%defattr(-,root,root,0755)
%doc README
%doc irattach/README.irattach
%doc irdadump/README.irdadump
%doc irdaping/README.irdaping
%doc irkbd/README.irkbd
%doc tekram/README.tekram
/usr/sbin/irattach
/usr/sbin/irdaping
/usr/sbin/dongle_attach
/usr/sbin/findchip
/usr/bin/irdadump
/usr/bin/irpsion5
/usr/bin/irkbd
%config /etc/rc.d/init.d/irda
%config /etc/sysconfig/irda
%config /etc/sysconfig/network-scripts/ifcfg-irlan0

%changelog
* Sun Nov 19 2000 Dag Brattli <dag@brattli.net>
- 0.9.13
- Changed config scripts (now that irmanager is gone)
- Removed irmanager (not needed anymore)

* Wed Jan 19 2000 Ian Soboroff <ian@cs.umbc.edu>
- 0.9.8
- Added findchip to package
- Added READMEs for the various utilities to the doc directory

* Wed Nov 10 1999 Dag Brattli <dagb@cs.uit.no>
- 0.9.5
- Some fixes to irattach, so it works with the latest kernels and patches
- Removed OBEX which will now become its own distribution
- Removed irdadump-X11 which will be replaced with a GNOME version

* Wed Sep 8 1999 Bernhard Rosenkraenzer <bero@linux-mandrake.com>
- 0.9.4
- include new stuff (palm3, psion, obex_tcp, ...)
- various fixes

* Tue Sep 7 1999 Bernhard Rosenkraenzer <bero@linux-mandrake.com>
- Fix .spec bug

* Tue Sep 7 1999 Bernhard Rosenkraenzer <bero@linux-mandrake.com>
- add README to %doc
- compile gnobex, now in irda-utils-X11

* Tue Sep 7 1999 Bernhard Rosenkraenzer <bero@linux-mandrake.com>
- initial RPM:
  - handle RPM_OPT_FLAGS and RPM_BUILD_ROOT
  - fix build
  - split in normal and X11 packages
