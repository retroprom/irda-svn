######################################################################
##                
## Filename:      Makefile
## Version:       
## Description:   Makefile for irdaping
## Status:        Experimental.
## Author:        Dag Brattli <dagb@cs.uit.no>
## Created at:    Thu Feb 19 00:10:23 1998
## Modified at:   Mon Jun 21 10:20:50 1999
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
##     Neither Dag Brattli nor University of Troms? admit liability nor
##     provide warranty for any of this software. This material is 
##     provided "AS-IS" and at no charge.
##     
######################################################################
include ../output.mak

CC = gcc
LD = ld
RM = rm -f

RM_CMD = $(ECMD)$(RM) *.BAK *.bak *.o ,* *~ *.a

INCLUDES  = $(SYS_INCLUDES) -I../include/
LIBRARIES = $(SYS_LIBRARIES)
LIBPATH   = $(SYS_LIBPATH)

#
# System stuff
#
SYS_INCLUDES = -I/usr/include
SYS_LIBRARIES = 
SYS_LIBPATH = -L/usr/lib -L/usr/local/lib 

CFLAGS= $(RPM_OPT_FLAGS) $(SYS_INCLUDES) -Wall -Wstrict-prototypes

# 
# Files to make 
# 
SRCS = irdaping.c
OBJS = $(SRCS:.c=.o)
TARGETS = irdaping

all: $(TARGETS)


irdaping: $(OBJS)
	$(prn_cc_o)
	$(ECMD)$(CC) $(CFLAGS) $(OBJS) -o $@


.c.o:
	$(prn_cc)
	$(ECMD)$(CC) $(CFLAGS) $(INCLUDES) -c $<

install: $(TARGETS)
	$(prn_install)
	$(ECMD)install irdaping $(ROOT)/usr/sbin

clean:
	$(RM_CMD)

distclean:
	$(RM_CMD)
	$(ECMD)$(RM) irdaping

depend:
	$(prn_depend)
	$(ECMD)makedepend -s "# DO NOT DELETE" -- $(INCLUDES) -- $(SRCS)

# DO NOT DELETE
