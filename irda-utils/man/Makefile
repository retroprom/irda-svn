# Makefile for Linux/IrDA man pages.
# v0.1 26/03/2001 wh
# v0.2 Jean II
# v0.3 29/04/2003 wh, added -9 to gzip, changed docbook-to-man to docbook2man
include ../output.mak

DOCBOOKCMD=docbook2man

DESTDIR=
VERSION=0.10
PREFIX=
MANDIR=$(PREFIX)/usr/man
MAN1DIR=$(MANDIR)/man1/
MAN4DIR=$(MANDIR)/man4/
MAN7DIR=$(MANDIR)/man7/
MAN8DIR=$(MANDIR)/man8/

MANPAGES4=irnet.4.gz
MANPAGES7=irda.7.gz
MANPAGES8=irattach.8.gz irdaping.8.gz irdadump.8.gz irpsion5.8.gz \
	findchip.8.gz irnetd.8.gz smcinit.8.gz tosh1800-smcinit.8.gz

# Man pages generated by DOCBOOK
SGMLMAN=irattach.8 irdaping.8 irdadump.8 irpsion5.8 findchip.8

all: $(MANPAGES4) $(MANPAGES7) $(MANPAGES8)

man-pages:all

# Translate docbook to man with proper dependancies
%.8: man-%.sgml man-%-inc.sgml
	$(prn_man)
	$(ECMD)$(DOCBOOKCMD) $< > $@


# GZIP the man page
%.gz: %
	$(prn_gzip)
	$(ECMD)gzip -9 -f $^


install: man-pages
	$(prn_install)
	$(ECMD)install -m 755 -d $(MAN4DIR)
	$(ECMD)install -m 644 $(MANPAGES4) $(MAN4DIR)/
	$(ECMD)install -m 755 -d $(MAN7DIR)
	$(ECMD)install -m 644 $(MANPAGES7) $(MAN7DIR)/
	$(ECMD)install -m 755 -d $(MAN8DIR)
	$(ECMD)install -m 644 $(MANPAGES8) $(MAN8DIR)/


clean:
	$(ECMD)rm -f $(SGMLMAN) *~ manpage.*


distclean: clean


# Don't wipe out the gziped man pages by default, users don't have docbook
# Don't wipe out manpage generated by hand
realclean:
	$(ECMD)rm -f $(SGMLMAN) manpage.*


dist: all
	(cd ..; tar cvfz man-irda-$(VERSION).tgz man-irda-$(VERSION))
