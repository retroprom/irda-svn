/*********************************************************************
 *                
 * Filename:      irmanager.h
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sat Oct 17 10:50:14 1998
 * Modified at:   Thu May  6 09:50:13 1999
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

#ifndef IRMANAGER_H
#define IRMANAGER_H

#define IRMGR_IOC_MAGIC 'm'
#define IRMGR_IOCTNPC     _IO(IRMGR_IOC_MAGIC, 1)
#define IRMGR_IOC_MAXNR   1 

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef enum {
	S_PNP,
	S_PDA,
	S_COMPUTER,
	S_PRINTER,
	S_MODEM,
	S_FAX,
	S_LAN,
	S_TELEPHONY,
	S_COMM,
	S_OBEX,
} SERVICE;

/*
 *  Events that we pass to the user space manager
 */
typedef enum {
	EVENT_DEVICE_DISCOVERED = 0,
	EVENT_REQUEST_MODULE,
	EVENT_IRLAN_START,
	EVENT_IRLAN_STOP,
	EVENT_IRLPT_START,
	EVENT_IRLPT_STOP,
	EVENT_IROBEX_START,
	EVENT_IROBEX_STOP,
	EVENT_IRDA_STOP,
	EVENT_NEED_PROCESS_CONTEXT,
} IRMGR_EVENT;

struct irmanager_event {
	IRMGR_EVENT event;
	char devname[10];
	char info[32];
	int service;
	int saddr;
	int daddr;
};

#endif

