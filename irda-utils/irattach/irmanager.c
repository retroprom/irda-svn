/*********************************************************************
 *                
 * Filename:      irmanager.c
 * Version:       0.2
 * Description:   IrDA Manager Daemon
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Fri Oct 16 22:23:08 1998
 * Modified at:   Tue Nov  9 15:23:15 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 *
 *     Copyright (c) 1998-1999 Dag Brattli, All Rights Reserved.
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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <syslog.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/utsname.h>

#include "irmanager.h"

extern int  lookup_dev(char *name);
extern int  open_dev(dev_t dev, int mode);
extern void beep(unsigned int ms, unsigned int freq);
extern int  execute(char *msg, char *cmd);
extern int  execute_on_dev(char *action, char *class, char *dev, int minor);
extern void write_pid(void);
extern void fork_now(void);
extern int set_sysctl_param(char *name, char *value);

void catch_signal(int sig);

extern int caught_signal;
extern int be_quiet;

static char *configpath = "/etc/irda";

/* Default path for pid file */
char *pidfile = "/var/run/irmanager.pid";

#define BEEP_TIME 150
#define BEEP_OK   1000
#define BEEP_WARN 2000
#define BEEP_ERR  4000

/*
 * Function start_service (service, dev_name)
 *
 *    
 *
 */
void start_service(char *service, char *dev_name, int minor)
{
	int ret;

	ret = execute_on_dev("start", service, dev_name, minor);
	if (ret)
	     beep(BEEP_TIME, BEEP_ERR);
	else
	     beep(BEEP_TIME, BEEP_OK);
}

/*
 * Function stop_class (class, devname)
 *
 *    
 *
 */
void stop_service(char *class,char *devname)
{
	int ret;

	ret = execute_on_dev("stop", class, devname, 0);
	if (ret)
	     beep(BEEP_TIME, BEEP_ERR);
	else
	     beep(BEEP_TIME, BEEP_OK);
}

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
	if (ret)
		beep(BEEP_TIME, BEEP_ERR);
	else
		beep(BEEP_TIME, BEEP_OK);

	return ret;
}

static void cleanup(int signo)
{
	switch (signo) {
	case SIGTERM:
	case SIGINT:
	        syslog(LOG_INFO, "got SIGTERM or SIGINT\n");
		exit(0);
		break;
	case SIGHUP:
	        syslog(LOG_INFO, "got SIGHUP\n");
		exit(0);
		break;
	default:
		break;
	}
	unlink(pidfile);
}


/*
 * Function main (argc, )
 *
 *    Main program :-)
 *
 */
int main(int argc, char *argv[])
{
	struct irmanager_event event;
	struct utsname buf;
	int not_used; /* For now! */
	int fd;	
	int minor;
	int ret;
	int c;

	minor = lookup_dev("irda");
	if (minor < 0 ) {
		load_module("irda");
		minor = lookup_dev("irda");
	}
	
	while ((c = getopt(argc, argv, "c:d:s:")) != -1) {
		switch (c) {
		case 'c': /* Compression */
			if (strcmp(optarg, "1") == 0) {
				load_module("irda_deflate");
				
				set_sysctl_param("compression", "1");
			}
			break;
		case 'd': /* Discovery */
			if (strcmp(optarg, "1") == 0) {
				/* User wants to start discovery */
				set_sysctl_param("discovery", "1");
			}
			break;
		case 's': /* Sound events */
			if (strcmp(optarg, "1") == 0) {
				be_quiet = FALSE;
			}
			break;
		default:
			break;
		} 
	}
	
	/* Use hostname as device name */
	if (uname(&buf) == 0) {
		set_sysctl_param("devname", strtok(buf.nodename, "."));
	}
	
	fork_now();

	if (signal(SIGHUP, cleanup) == SIG_ERR)
		syslog(LOG_INFO, "signal(SIGHUP): %m");
	if (signal(SIGTERM, cleanup) == SIG_ERR)
		syslog(LOG_INFO, "signal(SIGTERM): %m");
	if (signal(SIGINT, cleanup) == SIG_ERR)
		syslog(LOG_INFO, "signal(SIGINT): %m");

	if (chdir(configpath) != 0)
		syslog(LOG_INFO, "chdir to %s failed: %m", configpath);

	fd = open_dev((10 << 8)+minor,S_IREAD|S_IFCHR);
	if (fd == -1) {
	        syslog(LOG_INFO, "Unable to open IrDA device!\n");
 		exit(-1); 
 	} 

	/* Kick off the low level IrDA device drivers */
	start_service("drivers", "", 0);

	while (TRUE) {
		ret = read(fd, &event, sizeof(event));
		if (ret <= 0) {
		     /* Device closed, so probably a shutdown */
		     exit(ret);
		}
		     
		switch (event.event) {
		case EVENT_NEED_PROCESS_CONTEXT:
		     ioctl(fd, IRMGR_IOCTNPC, not_used);
		     break;
		case EVENT_DEVICE_DISCOVERED:
			switch (event.service) {
			case S_LAN:
				load_module("irlan");
				break;
			case S_PRINTER:
				load_module("irlpt_client");
				break;
			case S_COMM:
				load_module("ircomm_tty");
				break;
			case S_OBEX:
				load_module("irobex");
				break;
			default:
			        syslog(LOG_INFO, "Unknown service discovered!\n");
				break;
			}
			break;
		case EVENT_REQUEST_MODULE:
			load_module(event.info);
			break;
		case EVENT_IRLAN_START:
			start_service("network", event.devname, 0);
			break;
		case EVENT_IRLAN_STOP:
			stop_service("network", event.devname);
			break;
		case EVENT_IRLPT_START:
			minor = lookup_dev("irlpt_server");
			start_service("printer", event.devname, minor);
                       break;
		case EVENT_IRLPT_STOP:
			stop_service("printer", event.devname);
                        break;
		default:	
			syslog(LOG_INFO, "Unknown event\n");
			break;
		}
	}
	close(fd);

	return 0;
}




