

IrDA Utilities

Dag Brattli

Abstract

The IrDA Utils package is a collection of programs, that enables the
use of the IrDA protocols. Some user-space configuration is required
in order to make IrDA work for your machine, and some IrDA features
like IrOBEX is actually implemented in user-space.

1 Introduction

IrDA is an exciting way of communicating with remote devices. IrDA
uses infrared wireless communication so no cables are required. Speeds
range from 9600bps to 4Mbps. The types of devices that support IrDA
are LAN adapters, PDA's, printers, laptops, mobile phones etc.

Linux-IrDA is a GNU implementation of the IrDA protocols specifications
written from scratch. Linux-IrDA supports most of the IrDA protocols
like IrLAP, IrLMP, IrIAP, IrTTP, IrLPT, IrLAN, IrCOMM and IrOBEX.

Most of the features are implemented in the kernel, so you must enable
IrDA support in the kernel before you can use any of the tools and
programs mentioned in this document.

2 Getting started

First of all you need to install the tools in this package. Make and
make install should do that for you. By default it will install the
files in /etc/irda, and in /usr/sbin. If you don't like that, you
should edit the makefile. Notice however that /etc/irda is required
by irmanager.

2.1 IRMANAGER

To enable IrDA support in your kernel you must start irmanager. Irmanager
is a program which is similar to cardmgr for the PCMCIA package. The
normal way to start irmanager is:

# /usr/sbin/irmanager -d 1

You should probably add this to your /etc/rc.d/rc.local file, so it's
started automatically when you boot your machine.

2.2 IRATTACH

The files in /etc/irda are used to configure the various services that
are implemented by remote devices. The drivers file are however used
to load and configure local IrDA ports on your machine. The drivers
file is not very intelligent, so you need to know which IrDA port
you have. There are currently two ways of configuring an IrDA port
for your machine.

1. irattach

2. modprobe driver

irattach is used if you have an IrDA port that functions like a normal
serial port. Most IrDA ports are UART compatible for SIR speeds (9600-115200bps),
so this should would for most machines. To start irattach you have
to specify the serial port where the IrDA port is attached. So if
your IrDA port is attached to the third serial port on your machine,
then you have to specify something like this:

# /usr/sbin/irattach /dev/ttyS2

If you have a IrDA dongle connected to your first serial port, then
you need to start irattach like this:

# /usr/sbin/irattach /dev/ttyS0 -d <dongle>

Where <dongle> is the type of dongle you have. Currently supported
dongles are:

esi, for Extended Systems JetEye PC dongles

tekam, for Tekram IrMate IR-210B dongles

actisys, for ACTiSYS IR-220L dongles

actisys+, for ACTiSYS IR-220L+ dongles

girbil, for Greenwich Instruments, GIrBIL dongles

litelink for Parallax LiteLink dongle

irattach uses the irtty module which connects to the Linux TTY and
serial driver. This works well for most machines, but limits the speed
to 115200bps (IrDA SIR mode). If you want higher speeds, you must
use a special device driver for the IrDA chipset used by your machine.
Currently there are not that many device drivers implemented, mostly
because it's very hard to get the documentation, and the drivers are
in fact quite hard to implement even if you are lucky and get the
documentation for the chipset.

If you are one of the lucky people which have a chipset that is supported,
then you don't need to use irattach anymore. Now you just have to
insmod the driver like this:

# /sbin/insmod pc87108

This will add support for the National Semiconductors PC87108 FIR controller
chipset. Visit the Linux-IrDA homepage or ask at the mailing-list
to know the status of the various FIR chipsets.

3 Other programs

irdadump Is a program that displays all the frames sent, and received
  on the infrared link. 

irdaping Makes it possible to try and ping a remote device using
  IrDA test frames. Not all devices implements support for test frames,
  so 

irkbd, Implements support for the mouse and keyboard protocol as
  used by the Tekram IR-660 infrared docking station. 

obex, Is a user-space implementation of the IrOBEX protocol. The
  directory also has some example programs which shows how you can
  use IrOBEX from your applications

gnobex, Is the start of a IrOBEX client using the GTK+ and GNOME
  user interface.

Have fun! 
