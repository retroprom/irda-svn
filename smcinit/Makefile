#
# $Id$
#
#
# Copyright (C) 2003, Claudiu Costin <claudiuc@kde.org>
#
# 'Makefile' for compiling Toshiba laptop IrDA utilities
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 Temple
# Place, Suite 330, Boston, MA  02111-1307  USA
#

ifeq ($(PREFIX),)
  PREFIX = /usr/local
endif

CC       = gcc
CFLAGS   = -pipe -Wall -W -O2
INCPATH  = 
LIBS     = -lpci
LIBS1    = 
LIBS2    = 
LINK     = gcc
LFLAGS   = 
INSTALL         = /usr/bin/install -c -p
INSTALL_PROGRAM = ${INSTALL} -D -m 755 -s 
INSTALL_DATA    = ${INSTALL} -D -m 644
INSTALL_SCRIPT  = ${INSTALL}
INSTALL_HEADER  = $(INSTALL_DATA)



SOURCES1 = tosh1800-smcinit.c 
SOURCES2 = smcinit.c 
SOURCES  = $(SOURCES1) $(SOURCES2) 
OBJECTS1 = $(SOURCES1:%.c=%.o)
OBJECTS2 = $(SOURCES2:%.c=%.o)
OBJECTS  = $(OBJECTS1) $(OBJECTS2)
TARGETS1 = tosh1800-smcinit
TARGETS2 = smcinit
TARGETS = $(TARGETS1) $(TARGETS2)



all: $(TARGETS)

install: install-man install-bin

uninstall: uninstall-man uninstall-bin


install-man:
	$(INSTALL_DATA) smcinit.7 $(INSTALL_ROOT)$(PREFIX)/share/man/man7/smcinit.7
	$(INSTALL_DATA) smcinit.8 $(INSTALL_ROOT)$(PREFIX)/share/man/man8/smcinit.8
	$(INSTALL_DATA) tosh1800-smcinit.8 $(INSTALL_ROOT)$(PREFIX)/share/man/man8/tosh1800-smcinit.8
    
install-bin: all
	$(INSTALL_PROGRAM) $(TARGETS1) $(INSTALL_ROOT)$(PREFIX)/sbin/$(TARGETS1)
	$(INSTALL_PROGRAM) $(TARGETS2) $(INSTALL_ROOT)$(PREFIX)/sbin/$(TARGETS2)


uninstall-man:
	rm -f $(INSTALL_ROOT)$(PREFIX)/share/man/man7/smcinit.7
	rm -f $(INSTALL_ROOT)$(PREFIX)/share/man/man8/smcinit.8
	rm -f $(INSTALL_ROOT)$(PREFIX)/share/man/man8/tosh1800-smcinit.8

uninstall-bin:
	rm -f $(INSTALL_ROOT)$(PREFIX)/sbin/$(TARGETS1)
	rm -f $(INSTALL_ROOT)$(PREFIX)/sbin/$(TARGETS2)

clean:
	rm -f $(OBJECTS) $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

config.h: config.h.in
	./configure

$(SOURCES1) $(SOURCES2): config.h

$(TARGETS1): $(OBJECTS1)
	$(LINK) $(LFLAGS) -o $(TARGETS1) $(OBJECTS1) $(LIBS1) $(LIBS)

$(TARGETS2): $(OBJECTS2)
	$(LINK) $(LFLAGS) -o $(TARGETS2) $(OBJECTS2) $(LIBS2) $(LIBS)

