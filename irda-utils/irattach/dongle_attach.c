/*********************************************************************
 *                
 * Filename:      dongle_attach.c
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Mon May 10 23:58:45 1999
 * Modified at:   Wed Oct  6 20:22:48 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1999 Dag Brattli, All Rights Reserved.
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *     MA 02111-1307 USA
 *     
 ********************************************************************/

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <asm/byteorder.h>

#include <net/if.h>
#include <linux/types.h>

#ifndef AF_IRDA
#define AF_IRDA 23
#endif /* AF_IRDA */

#define SIOCSDONGLE     SIOCDEVPRIVATE

/* These are the currently known dongles */
typedef enum {
	TEKRAM_DONGLE,
	ESI_DONGLE,
	ACTISYS_DONGLE,
	ACTISYS_PLUS_DONGLE,
	GIRBIL_DONGLE,
	LITELINK_DONGLE,
	AIRPORT_DONGLE,
} DONGLE_T;

/*
 * Function main (argc, )
 *
 *    Initialize and try to receive test frames
 *
 */
int main(int argc, char *argv[])
{
	struct ifreq ifr;
	char dev[10];
	int dongle = -1;
	int c;
	int fd;

	if (argc < 3) {
		printf("Usage: dongle_attach <device> -d <dongle>\n");
		printf("\nExample: dongle_attach irda0 -d esi\n\n");
		exit(-1);
	}
	
	strcpy(dev, argv[1]);

	while ((c = getopt(argc, argv, "d:")) != -1) {
		switch (c) {
		case 'd':
			if (strcmp(optarg, "esi") == 0)
				dongle = ESI_DONGLE;
			else if (strcmp(optarg, "tekram") == 0)
				dongle = TEKRAM_DONGLE;
			else if (strcmp(optarg, "actisys") == 0)
				dongle = ACTISYS_DONGLE;
			else if (strcmp(optarg, "actisys+") == 0)
				dongle = ACTISYS_PLUS_DONGLE;
			else if (strcmp(optarg, "girbil") == 0)
				dongle = GIRBIL_DONGLE;
			else if (strcmp(optarg, "litelink") == 0)
				dongle = LITELINK_DONGLE;
			else if (strcmp(optarg, "airport") == 0)
				dongle = AIRPORT_DONGLE;
			
			if (dongle == -1) {
				printf("Sorry, dongle not supported yet!\n");
				exit(-1);
			}
			break;
		default:
			break;
		} 
	}
	/* Create socket */
	fd = socket(AF_IRDA, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket");
		exit(-1);
        }

        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	ifr.ifr_data = (void *) dongle;

	if (ioctl(fd, SIOCSDONGLE, &ifr) < 0) {
		perror("ioctl");
		exit(-1);
	}
	return 0;
}
