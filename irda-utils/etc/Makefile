CC = gcc $(RPM_OPT_FLAGS)
LD = ld
RM = rm -f

RM_CMD = $(RM) *.BAK *.bak *.o ,* *~ *.a

all:

clean:
	$(RM_CMD)

install:
	install -D irda.sysconfig $(ROOT)/etc/sysconfig/irda
	install -D irda.rc $(ROOT)/etc/rc.d/init.d/irda
	install -D ifcfg-irlan0 $(ROOT)/etc/sysconfig/network-scripts/ifcfg-irlan0
