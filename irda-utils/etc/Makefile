CC = gcc $(RPM_OPT_FLAGS)
LD = ld
RM = rm -f

RM_CMD = $(RM) *.BAK *.bak *.o ,* *~ *.a

all:

clean:
	$(RM_CMD)

install:
	mkdir -p $(ROOT)/etc/irda
	install network $(ROOT)/etc/irda/
	install network.orig $(ROOT)/etc/irda/
	install network.redhat $(ROOT)/etc/irda/
	install network.opts $(ROOT)/etc/irda/
	install drivers $(ROOT)/etc/irda/
	install irda $(ROOT)/etc/rc.d/init.d/
