CC = gcc $(RPM_OPT_FLAGS)
LD = ld
RM = rm -f

RM_CMD = $(RM) *.BAK *.bak *.o ,* *~ *.a

all:

clean:
	$(RM_CMD)

install:
	install irda.sysconfig $(ROOT)/etc/sysconfig/
	install irda.rc $(ROOT)/etc/rc.d/init.d/irda
	install ifcfg-irlan0 $(ROOT)/etc/sysconfig/network-scrips/
