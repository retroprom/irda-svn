######################################################################
##                
## Filename:      Makefile
## Version:       
## Description:   Makefile for irsockets test programs
## Status:        Experimental.
## Author:        Dag Brattli <dagb@cs.uit.no>
## Created at:    Thu Feb 19 00:10:23 1998
## Modified at:   Sat Dec 25 16:13:54 1999
## Modified by:   Dag Brattli <dagb@cs.uit.no>
## 
## $Id$
##
##     Copyright (c) 1998-1999 Dag Brattli, All Rights Reserved.
##      
##     This program is free software; you can redistribute it and/or 
##     modify it under the terms of the GNU General Public License as 
##     published by the Free Software Foundation; either version 2 of 
##     the License, or (at your option) any later version.
##  
##     Neither Dag Brattli nor University of Tromsø admit liability nor
##     provide warranty for any of this software. This material is 
##     provided "AS-IS" and at no charge.
##     
######################################################################

CC = gcc
LD = ld
RM = rm -f

RM_CMD = $(RM) *.BAK *.bak *.o ,* *~ *.a client

INCLUDES  = $(SYS_INCLUDES) -I../include
LIBRARIES = $(SYS_LIBRARIES)
LIBPATH   = $(SYS_LIBPATH)

#
# System stuff
#
SYS_INCLUDES = -I/usr/include -I/usr/src/linux/include
SYS_LIBRARIES = 
SYS_LIBPATH = -L/usr/lib -L/usr/local/lib 

CFLAGS= $(SYS_INCLUDES) $(RPM_OPT_FLAGS) -Wall -Wstrict-prototypes

# 
# Files to make 
# 

all:: echo irdaspray ias_query

echo: echo.o
	$(CC) $(CFLAGS) echo.o -o $@
	ln -s echo discard

irdaspray: irdaspray.o
	$(CC) $(CFLAGS) irdaspray.o -o $@

ias_query: ias_query.o
	$(CC) $(CFLAGS) ias_query.o -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

install: all
	install irdaspray $(ROOT)/usr/bin

clean::
	$(RM_CMD)

depend::
	makedepend -s "# DO NOT DELETE" -- $(INCLUDES) -- $(SRCS)

# DO NOT DELETE
