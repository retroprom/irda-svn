# Makefile for Linux/IrDA man pages.
# v0.1 26/03/2001 wh

DESTDIR=
VERSION=0.5
PREFIX=
MANDIR=$(PREFIX)/usr/man
MAN1DIR=$(MANDIR)/man1/
MAN8DIR=$(MANDIR)/man8/

MANPAGES8=irattach.8.gz irdaping.8.gz irdadump.8.gz irpsion5.8.gz findchip.8.gz

all: $(MANPAGES8)

# Translate docbook to man with proper dependancies
%.8: man-%.sgml man-%-inc.sgml
	docbook-to-man $< > $@

# GZIP the man page
%.gz: %
	gzip -f $^

install::
	install -m 755 -d $(MAN8DIR)
	install -m 644 $(MANPAGES8) $(MAN8DIR)/

clean::
	rm -f *.[1-9] *~

distclean:: clean

# Don't wipe out the gziped man pages by default, users don't have docbook
realclean::
	rm -f *.[1-9]*

dist		: all
	(cd ..; tar cvfz man-irda-$(VERSION).tgz man-irda-$(VERSION))
