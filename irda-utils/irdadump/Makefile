include ../output.mak

CC = gcc
LD = ld
AR = ar
RANLIB = ranlib
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

CFLAGS= -O2 -W -Wall -I. `pkg-config --cflags glib-2.0`

LIBIRDADUMP_OBJS = irdadump.o irlap.o irlmp.o obex.o ircomm.o netbuf.o capture.o
IRDADUMP_OBJS = main.o

LIBIRDADUMP_TARGET = lib_irdadump.a
IRDADUMP_TARGET = irdadump

all: lib_irdadump.a irdadump


lib_irdadump.a: $(LIBIRDADUMP_OBJS)
	$(prn_ar)
	$(ECMD)$(AR) cru  $(LIBIRDADUMP_TARGET) $(LIBIRDADUMP_OBJS)
	$(prn_ranlib) 
	$(ECMD)$(RANLIB) $(LIBIRDADUMP_TARGET)



irdadump: $(IRDADUMP_OBJS) $(LIBIRDADUMP_TARGET)
	$(prn_cc_o)
	$(ECMD)$(CC) $(CFLAGS) `pkg-config --libs glib-2.0` -o  $(IRDADUMP_TARGET) $< $(LIBIRDADUMP_TARGET)


.c.o:
	$(prn_cc)
	$(ECMD)$(CC) $(CFLAGS) $(INCLUDES) -c $<

install: $(IRDADUMP_TARGET)
	$(prn_install)
	$(ECMD)install $(IRDADUMP_TARGET) $(ROOT)/usr/sbin

clean:
	$(RM_CMD)

distclean:
	$(RM_CMD)
	$(ECMD)$(RM) $(IRDADUMP_TARGET)
