/*********************************************************************
 *                
 * Filename:      main.c
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sun Mar 28 12:28:51 1999
 * Modified at:   Wed Jan 19 11:05:52 2000
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1999-2000 Dag Brattli, All Rights Reserved.
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

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>	/* strdup */

#include <glib.h>

extern int irdadump_init(char *);
extern int irdadump_loop(GString *);

extern int config_print_diff;
extern int config_print_irlap;
extern int config_dump_frame;
extern int config_snaplen;
extern int config_dump_bytes;
extern int config_snapcols;
extern int config_force_ttp;
extern int config_force_obex;

#define VERSION "0.9.16 (10.10.2002) Dag Brattli/Jean Tourrilhes"

int packets = 0;

void cleanup(int signo)
{

	/* Kill "unused" warning */
	signo = signo;

	fflush(stdout);
	putc('\n', stdout);

	printf("%d packets received by filter\n", packets);
	
	exit(0);
}

int main(int argc, char *argv[])
{
	GString *line;
	char *ifdev = NULL;
	int fd, c;

	while ((c = getopt(argc, argv, "i:bc:dxs:ptolv?")) != -1) {
		switch (c) {
		case 'b': /* Dumb bytes */
			config_dump_bytes = 1;
			break;
		case 'c': /* set snapcols for byte printing */
 			c = atoi(optarg);
 			if (c <= 0) {
				config_snapcols = 16;
			} else {
 				config_snapcols = c ;
 			}
			break;
		case 'd': /* Print diffs */
			config_print_diff = 1;
			break;
		case 'x': /* Dump frame */
			config_dump_frame = 1;
			break;
		case 's': /* set snaplen for printing */
 			c = atoi(optarg);
 			if (c <= 0) {
				config_snaplen = 2050;
			} else {
 				config_snaplen = c ;
 			}
		case 'p': /* Disable IrDA frame parsing, in case they
			   * are garbage... */
			config_print_irlap = 0;
			break;
		case 't': /* Force TTP decoding of unknown connections */
			config_force_ttp = 1;
			break;
 		case 'l': /* Set linebuffering */
 			setlinebuf(stdout);
 			break;
 		case 'v': /* version */
			printf("Version: %s\n", VERSION);
			exit(0);
		case 'i': /* Interface */
			ifdev = (char *) strdup(optarg);
			printf("Using interface: %s\n", ifdev);
			break;
 		case '?': /* usage */
			fprintf(stderr,"Usage: %s [-d] [-x] [-b] [-s <n>] [-c <n>] [-p] [-i device]\n", 
				argv[0]);
 			fprintf(stderr,"\t-d\tPrint diffs\n");
 			fprintf(stderr,"\t-l\tSet line buffering on output file.\n");
			fprintf(stderr,"\t-s <n>\tSet snaplen for -x & -b\n");
 			fprintf(stderr,"\t-x\tDump frame (bytes + ascii)\n");
 			fprintf(stderr,"\t-b\tDump bytes in columns\n");
			fprintf(stderr,"\t-c <n>\tSet number of colums for -b\n");
			fprintf(stderr,"\t-p <n>\tDisable parsing/decoding\n");
 			exit(1);
		default:
			break;
		} 
	}
	signal(SIGTERM, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGHUP, cleanup);
	
	fd = irdadump_init(ifdev);
	if (fd < 0) {
	    perror(argv[0]);
	    return fd;
	}
	line = g_string_sized_new(1024);

	while (1) {
		if (irdadump_loop(line) == -1)
			continue;

		packets++;
		
		puts(line->str);
		
		/* Recycle line */
		g_string_truncate(line, 0);
	}
	return 0;
}
