/*********************************************************************
 *                
 * Filename:      capture.c
 * Version:       0.1
 * Description:   Generate libpcap log files for Ethereal
 * Status:        Experimental.
 * Author:        Jan Kiszka
 * Created at:    Thu Jul 17 16:32:35 PDT 2003
 * Modified at:   Thu Jul 17 16:32:35 PDT 2003
 * Modified by:   Jean Tourrilhes
 * 
 *     Copyright (c) 1999 Jan Kiszka, All Rights Reserved.
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* write */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "netbuf.h"
#include "capture.h"

#include <netpacket/packet.h>

#include <linux/types.h>

#define SENT_FRAME  0x0100
#define RECV_FRAME  0x0101

struct pcaprec_hdr {
	__u32 ts_sec;
	__u32 ts_usec;
	__u32 incl_len;
	__u32 orig_len;
};

struct pseudo_hdr {
	__u8  not_used1;
	__u8  not_used2;
	__u16 type;
};

struct pcap_hdr {
	__u32 magic;
	__u16 version_major;
	__u16 version_minor;
	__u32 thiszone;
	__u32 sigfigs;
	__u32 snaplen;
	__u32 network;
};

struct pcap_hdr fileHeader = {
	magic:		0xa1b2c3d4,
	version_major:	2,
	version_minor:	0,
	thiszone:	0,
	sigfigs:	0,
	snaplen:	2050 + sizeof(struct pseudo_hdr),
	network:	123
};


/*
 * Function capture_open ()
 *
 *    Open capture file and return its fd
 *
 */
int capture_open(char *	capfilename)
{
	int capturefile;

	/* Open the capture file */
	capturefile = open(capfilename, O_WRONLY | O_CREAT | O_TRUNC, 00640);
	if (capturefile < 0)
		perror("Opening capture file");

	return capturefile;
}

/*
 * Function capture_close ()
 *
 *    Close capture file if needed
 *
 */
void capture_close(int capturefile)
{
	close(capturefile);
}

/*
 * Function capture_init ()
 *
 *    Write capture header in file
 *
 */
int capture_init(int capturefile)
{
	/* Write capture file header */
	if (write(capturefile, &fileHeader, sizeof(struct pcap_hdr)) < 0)
		return -1;

	return 0;
}

/*
 * Function capture_dump ()
 *
 *    Write capture header in file
 *
 */
int capture_dump(int capturefile,
		 GNetBuf *frame_buf,
		 int len,
		 struct sockaddr_ll *from,
		 struct timeval *curr_time)
{
	struct pcaprec_hdr rec_hdr;
	struct pseudo_hdr  psd_hdr;

	rec_hdr.ts_sec    = curr_time->tv_sec;
	rec_hdr.ts_usec   = curr_time->tv_usec;
	rec_hdr.orig_len  = len + sizeof(psd_hdr);
	rec_hdr.incl_len  = rec_hdr.orig_len;

	psd_hdr.not_used1 = 0;
	psd_hdr.not_used1 = 0;
	psd_hdr.type      = (from->sll_pkttype) ? SENT_FRAME : RECV_FRAME;

	if ((write(capturefile, &rec_hdr,
			sizeof(rec_hdr)) < 0) ||
		(write(capturefile, &psd_hdr,
			sizeof(psd_hdr)) < 0) ||
		(write(capturefile, frame_buf->head, len) < 0)) {
		perror("write capture file");
		exit(-1);
	}

	return 0;
}
