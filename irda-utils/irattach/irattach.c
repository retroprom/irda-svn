/*********************************************************************
 *                
 * Filename:      irattach.c
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sun Dec  7 23:21:05 1997
 * Modified at:   Tue Jan 25 11:33:48 2000
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * Sources:       
 *
 *     Copyright (c) 1997, 1999-2000 Dag Brattli <dagb@cs.uit.no>, 
 *     All Rights Reserved.
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 *
 *     Neither Dag Brattli nor University of Tromsø admit liability nor
 *     provide warranty for any of this software. This material is 
 *     provided "AS-IS" and at no charge.
 *
 ********************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/utsname.h>

#include <irda.h>

#ifndef N_IRDA
#define N_IRDA 11 /* This one should go in .../asm/termio.h */
#endif /* N_IRDA */

#ifndef AF_IRDA
#define AF_IRDA 23
#endif

/* Need to be compatible with the "old" 2.2 kernel */
#ifndef IRDA_TEKRAM_DONGLE
#define IRDA_TEKRAM_DONGLE       0
#endif
#ifndef IRDA_ESI_DONGLE
#define IRDA_ESI_DONGLE          1
#endif
#ifndef IRDA_ACTISYS_DONGLE
#define IRDA_ACTISYS_DONGLE      2
#endif
#ifndef IRDA_ACTISYS_PLUS_DONGLE
#define IRDA_ACTISYS_PLUS_DONGLE 3
#endif
#ifndef IRDA_GIRBIL_DONGLE
#define IRDA_GIRBIL_DONGLE       4
#endif
#ifndef IRDA_LITELINK_DONGLE
#define IRDA_LITELINK_DONGLE     5
#endif
#ifndef IRDA_AIRPORT_DONGLE
#define IRDA_AIRPORT_DONGLE      6
#endif
#ifndef IRDA_OLD_BELKIN_DONGLE
#define IRDA_OLD_BELKIN_DONGLE   7
#endif

extern void fork_now(void);
extern int set_sysctl_param(char *name, char *value);
extern int execute(char *msg, char *cmd);

#define VERSION "1.1 Tue Nov  9 15:30:55 1999 Dag Brattli"

static int initfdflags = -1;	/* Initial file descriptor flags */
static int initdisc = -1;
static int fd = -1;

/* Default path for pid file */
char *pidfile = "/var/run/irattach.pid";

/* Used by ioctl to the tty to obtain the network device name */
struct irtty_info {
	char name[6];
};

#define IRTTY_IOC_MAGIC 'e'
#define IRTTY_IOCTDONGLE  _IO(IRTTY_IOC_MAGIC, 1)
#define IRTTY_IOCGNAME   _IOR(IRTTY_IOC_MAGIC, 2, struct irtty_info)
#define IRTTY_IOC_MAXNR  2  

struct irtty_info info;
char device[20];

/*
 * Function load_module (name)
 *
 *    Tries to load (modprobe) the module with the given name
 *
 */
int load_module(char *name)
{
	char msg[128], cmd[512];
	int ret;
	
	sprintf(msg, "Trying to load module %s", name);
	sprintf(cmd, "/sbin/modprobe %s", name);

	ret = execute(msg, cmd);

	return ret;
}

/*
 * Function establish_irda (fd)
 * 
 *    Turn the serial port into a irda interface.
 */
void establish_irda (int fd) 
{
	int irdadisc = N_IRDA;
	
	if (ioctl(fd, TIOCEXCL, 0) < 0) {
		syslog (LOG_WARNING, "ioctl(TIOCEXCL): %m");
	}
	
	if (ioctl(fd, TIOCGETD, &initdisc) < 0) {
		syslog(LOG_ERR, "ioctl(TIOCGETD): %m");
		exit (1);
	}
	
	if (ioctl(fd, TIOCSETD, &irdadisc) < 0){
		fprintf(stderr,  
			 "Maybe you don't have IrDA support in your kernel?\n");
		syslog(LOG_ERR, "irattach: tty: set_disc(%d): %s\n", 
			irdadisc, strerror(errno));
		exit (1);
	}
}

/*
 * Function tty_configure (tios)
 *
 *    Put a IrDA line discipline in a transparent mode. 
 *
 */
static int tty_configure(struct termios *tios) 
{
	tios->c_cflag     = CS8|CREAD|B9600|CLOCAL;
	
	/* Ignore break condition and parity errors */
 	tios->c_iflag     = IGNBRK | IGNPAR;
	tios->c_oflag     = 0;
	tios->c_lflag     = 0; /* set input mode (non-canonical, no echo,..) */
	tios->c_cc[VMIN]  = 1; /* num of chars to wait for, before delivery */
	tios->c_cc[VTIME] = 0; /* timeout before delivery */
	
	return(0);
}

/*
 * Function init_irda (fd)
 *
 *    Initialize IrDA line discipline
 *
 */
void init_irda_ldisc(int fd) 
{
	struct termios tios;
	
	tty_configure(&tios);
	
	/* tcflush(fd, TCIFLUSH); */
	if (tcsetattr(fd, TCSAFLUSH, &tios) < 0) {
		syslog(LOG_ERR, "tcsetattr: %m");
		exit(1);
	}
}

/*
 * Function modify_flags (set, clear)
 *
 *    Modify the flags
 *
 */
int modify_flags(char *dev, int set, int clear)
{
	struct ifreq ifr;
	int fd;

	/* Create socket */
        fd = socket(AF_IRDA, SOCK_STREAM, 0);
        if (fd < 0) {
                perror("socket");
                exit(-1);
        }

        strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
		return -1;

        ifr.ifr_flags |= set;
	ifr.ifr_flags &= ~clear;
	strncpy(ifr.ifr_name, dev, IFNAMSIZ);

        if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0) {
                return -1;
        }
        return 0;	
}

/*
 * Function ifup (name)
 *
 *    Start device
 *
 */
void ifup(char *dev)
{
	syslog(LOG_INFO, "Starting device %s", dev);
	modify_flags(dev, IFF_UP|IFF_RUNNING, 0);
}

/*
 * Function ifdown (name)
 *
 *    Stop device
 *
 */
void ifdown(char *dev)
{
	modify_flags(dev, 0, IFF_UP);
}

void cleanup(int signo)
{
	switch (signo) {
	case SIGTERM:
	case SIGINT:
	        syslog(LOG_INFO, "got SIGTERM or SIGINT\n");
		break;
	case SIGHUP:
	        syslog(LOG_INFO, "got SIGHUP\n");
		break;
	default:
		break;
	}
	ifdown(device);

	unlink(pidfile);

	syslog(LOG_INFO, "exiting ...\n");
	exit(0);
}

void start_tty(char *dev) 
{
	/*
	 * Open the serial device and set it up to be the irda interface.
	 */
	if ((fd = open(dev, O_NONBLOCK | O_RDWR, 0)) < 0) {
		syslog(LOG_ERR, "Failed to open %s: %m", dev);
		exit(1);
	}
	if ((initfdflags = fcntl(fd, F_GETFL)) == -1) {
		syslog(LOG_ERR, "Couldn't get device fd flags: %m");
		exit(1);
	}
	
	initfdflags &= ~O_NONBLOCK;
	fcntl(fd, F_SETFL, initfdflags);
		
	/* Set up the serial device as a irda interface */	
	init_irda_ldisc(fd);
	establish_irda(fd);

	sleep(1);  /* give it time to set up its terminal */
	
	/*
	 *  Set device for non-blocking reads.
	 */
	if (fcntl(fd, F_SETFL, initfdflags | O_NONBLOCK) == -1) {
		syslog(LOG_ERR, 
		       "Couldn't set device to non-blocking mode: %m");
		exit(1);
	}
}

/*
 * Function main (argc, )
 *
 *    Main function
 *
 */
int main(int argc, char *argv[]) 
{
	struct utsname buf;
	int dongle = -1;
	int c, fir;

	printf("%s\n", VERSION);
	if (argc < 2) {
		fputs("Usage: irattach <dev> [-d dongle] [-s]\n\n", stderr);
	}
	
	fork_now();

	if (signal(SIGHUP, cleanup) == SIG_ERR)
		syslog(LOG_INFO, "signal(SIGHUP): %m");
	if (signal(SIGTERM, cleanup) == SIG_ERR)
		syslog(LOG_INFO, "signal(SIGTERM): %m");
	if (signal(SIGINT, cleanup) == SIG_ERR)
		syslog(LOG_INFO, "signal(SIGINT): %m");

	strncpy(device, argv[1], 20);
	if (strncmp("/dev", device, 4) == 0) {
		start_tty(device);
		fir = 0;
	} else {
		load_module(device);
		fir = 1;
	}

	while ((c = getopt(argc, argv, "sd:v")) != -1) {
		switch (c) {
		case 's':
		       	/* User wants to start discovery */
			set_sysctl_param("discovery", "1");
			break;
		case 'd':
			if (strcmp(optarg, "esi") == 0)
				dongle = IRDA_ESI_DONGLE;
			else if (strcmp(optarg, "tekram") == 0)
				dongle = IRDA_TEKRAM_DONGLE;
			else if (strcmp(optarg, "actisys") == 0)
				dongle = IRDA_ACTISYS_DONGLE;
			else if (strcmp(optarg, "actisys+") == 0)
				dongle = IRDA_ACTISYS_PLUS_DONGLE;
			else if (strcmp(optarg, "girbil") == 0)
				dongle = IRDA_GIRBIL_DONGLE;
			else if (strcmp(optarg, "litelink") == 0)
				dongle = IRDA_LITELINK_DONGLE;
			else if (strcmp(optarg, "airport") == 0)
				dongle = IRDA_AIRPORT_DONGLE;
			else if (strcmp(optarg, "old_belkin") == 0)
				dongle = IRDA_OLD_BELKIN_DONGLE;
			if (dongle == -1) {
				syslog(LOG_ERR, 
				       "Sorry, dongle not supported yet!");
				exit(-1);
			}	
			ioctl(fd, IRTTY_IOCTDONGLE, dongle);
			break;
		case 'v':
			printf("Version: %s\n", VERSION);
			exit(-1);
		}
	}

	/* Start the network interface */
	if (fir) {
		ifup(device);
	} else {
		if (ioctl(fd, IRTTY_IOCGNAME, &info) < 0)
			syslog(LOG_ERR, "Are you using an old kernel?");
		else {
			strncpy(device, info.name, 20);
			ifup(device);
		}
	}

	/* Use hostname as device name */
	if (uname(&buf) == 0)
		set_sysctl_param("devname", strtok(buf.nodename, "."));

	/*
	 *  Loop forever and wait for kill or ctrl-C since closing this 
	 *  process will also close all open files for this process
	 *  which will in turn close the tty used for IrDA which is not
	 *  really what we want :-)
	 */
	while (1)
		sleep(2);

	return 0;
}
