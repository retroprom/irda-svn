/*********************************************************************
 *                
 * Filename:      irdadump.c
 * Version:       0.6.1
 * Description:   irdadump sniffs IrDA frames, and is inspired by tcpdump
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sun Oct  4 20:33:05 1998
 * Modified at:   Wed Jan 19 11:03:32 2000
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1998-2000 Dag Brattli, All Rights Reserved.
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

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <net/if_arp.h>
#include <net/if_packet.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <netpacket/packet.h>

#include <stdint.h>
#include <irda.h>

#include "irdadump.h"

int config_print_diff = 0;
int config_dump_frame = 0;
int config_print_irlmp = 1;
int config_print_lost_frames = 0;

int self_nr = 0;
int self_ns = 0;
int peer_nr = 0;
int peer_ns = 0;

int verbose = 0;
int snaplen = 32;
int ifindex = 0;

GNetBuf *buf;
struct sockaddr_ll from;
struct timeval time1, time2;
struct timeval *curr_time, *prev_time, *tmp_time;
int fromlen;
int fd;

struct ias_query  last_ias;
struct lsap_state conn[MAX_CONNECTIONS];
int conn_cache = -1;

/*
 * Function print_time (tvp)
 *
 *    Print current time
 *
 */
inline void print_time(const struct timeval *time, GString *str)
{
        int s;
	
	s = (time->tv_sec) % 86400;
	g_string_sprintfa(str, "%02d:%02d:%02d.%06u ", 
			  s / 3600, (s % 3600) / 60, 
			  s % 60, (u_int32_t) time->tv_usec);
}

/*
 * Function print_diff_time (time, prev_time)
 *
 *    Print the difference in time between this frame and the previous one
 *
 */
inline void print_diff_time(struct timeval *time, struct timeval *prev_time, 
			    GString *str)
{
	float diff;
	
	if (prev_time->tv_usec > time->tv_usec) {
		time->tv_usec += 1000000;
		time->tv_sec--;
	}
	prev_time->tv_usec = time->tv_usec - prev_time->tv_usec;
	prev_time->tv_sec  = time->tv_sec - prev_time->tv_sec;
	
	diff = ((float) prev_time->tv_sec * 1000000 + prev_time->tv_usec)
		/ 1000.0;
	
	g_string_sprintfa(str, "(%07.2f ms) ", diff);
}

/*
 * Function find_connection (slsap_sel, dlsap_sel)
 *
 *    
 *
 */
inline int find_connection(guint8 slsap_sel, guint8 dlsap_sel)
{
	int i;

	/* Try cache first */
	if (conn_cache != -1) {
		if ((conn[conn_cache].slsap_sel == slsap_sel) &&
		    (conn[conn_cache].dlsap_sel == dlsap_sel))
			return conn_cache;
	}
	
	/* Just have to do linear search then */
	for (i=0;i<MAX_CONNECTIONS;i++) {
		if ((conn[i].slsap_sel == slsap_sel) &&
		    (conn[i].dlsap_sel == dlsap_sel))
			return (conn_cache = i);
	}
	return (conn_cache = -1);
}

/*
 * Function find_free_connection ()
 *
 *    Find free slot for a new connection
 *
 */
inline int find_free_connection(void)
{
	gint i;
	
	for (i=0;i<MAX_CONNECTIONS;i++) {
		if (!conn[i].valid)
			return i;
	}
	return -1;
}

/*
 * Function parse_irttp (buf, str)
 *
 *    Parse IrTTP data frame
 *
 */
inline void parse_irttp(GNetBuf *buf, GString *str) 
{
	g_string_sprintfa(str, "TTP credits=%d ", buf->data[0] & 0x7f);

	if (buf->data[0] & 0x80)
		g_string_append(str, "More ");

	/* Remove TTP header */
	g_netbuf_pull(buf, 1);
}

/*
 * Function parse_irttp_connect (buf, str)
 *
 *    Parse IrTTP connect frame
 *
 */
inline void parse_irttp_connect(GNetBuf *buf, GString *str)
{
	guint8 plen, pi, pl;
	guint16 tmp_cpu;

	g_string_sprintfa(str, "TTP credits=%d", buf->data[0] & 0x7f);

	if (buf->data[0] & 0x80) {
		plen = buf->data[1];
		pi   = buf->data[2];
		pl   = buf->data[3];

		memcpy(&tmp_cpu, buf->data+4, 2); /* Align value */
		tmp_cpu = GUINT16_FROM_BE(tmp_cpu); /* Convert to host order */

		g_netbuf_pull(buf, plen);

		g_string_sprintfa(str, "MaxSduSize=%d ", tmp_cpu);
	}
}
/*
 * Function parse_hints (hint)
 *
 *    Parse and print the names of the various hint bits if they are set
 *
 */
inline void parse_hints(guint8 *hint, GString *str)
{
	g_string_append(str, "[ ");
	if (hint[0] & HINT_PNP)
		g_string_append(str, "PnP ");
	if (hint[0] & HINT_PDA)
		g_string_append(str, "PDA/Palmtop ");
	if (hint[0] & HINT_COMPUTER)
		g_string_append(str, "Computer ");
	if (hint[0] & HINT_PRINTER)
		g_string_append(str, "Printer ");
	if (hint[0] & HINT_MODEM)
		g_string_append(str, "Modem ");
	if (hint[0] & HINT_FAX)
		g_string_append(str, "Fax ");
	if (hint[0] & HINT_LAN)
		g_string_append(str, "LAN Access ");
	
	if (hint[1] & HINT_TELEPHONY)
		g_string_append(str, "Telephony ");
	if (hint[1] & HINT_FILE_SERVER)
		g_string_append(str, "File Server ");       
	if (hint[1] & HINT_COMM)
		g_string_append(str, "IrCOMM ");
	if (hint[1] & HINT_OBEX)
		g_string_append(str, "IrOBEX ");
	g_string_append(str, "] ");
}

/*
 * Function parse_xid_frame (command, pf, buf, len)
 *
 *    Exchange station identification frame
 *
 */
inline void parse_xid_frame(guint8 command, guint8 pf, int type, 
			    GNetBuf *buf, GString *str)
{
	struct xid_frame *frame = (struct xid_frame *) buf->data;
	guint8 S,s;
	guint32 saddr, daddr;
	char *info = NULL;
	guint8 hint[2];
	
	hint[0] = hint[1] = 0;
	
	saddr = GINT32_FROM_LE(frame->saddr);
	daddr = GINT32_FROM_LE(frame->daddr);
	
	switch(frame->flags) {
	case 0x00:
		S = 1;
		break;
	case 0x01:
		S = 6;
		break;
	case 0x02:
		S = 8;
		break;
	case 0x03:
		S = 16;
		break;
	default:
		S = 0;
	}
	
	s = frame->slotnr;
	
	/* 
	 * The last of the discovery command frames, and response frames 
	 * contains info 
	 */
	if ((s == 0xff) || (!command)) {
		hint[0] = frame->discovery_info[0];
		if (hint[0] & 0x80) {
			hint[1] = frame->discovery_info[1];
			info = (char *) frame->discovery_info+3;
		} else
			info = (char *) frame->discovery_info+2;
		
		/* Terminate string */
		buf->data[buf->len] = '\0';
	}
	if (type)
		g_string_sprintfa(str, "xid:%s %08x > %08x S=%d ", 
				  command?"cmd":"rsp", saddr, daddr, S);
	else
		g_string_sprintfa(str, "xid:%s %08x < %08x S=%d ", 
				  command?"cmd":"rsp", daddr, saddr, S);
	
	/* Printing a * instead of 255 makes things more aligned */
	if (s == 0xff)
		g_string_append(str, "s=* ");
	else
		g_string_sprintfa(str, "s=%d ", s);
	
	/* Print info if any */
	if (info) {
		g_string_sprintfa(str, "%s hint=%02x%02x ", info, hint[0], 
				  hint[1]);
		parse_hints(hint, str);
	}
}

/*
 * Function parse_i_frame (buf, len, u_char)
 *
 *    Information frames
 *
 */
inline void parse_i_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			  GNetBuf *buf, GString *str)
{
	int nr, ns;

	nr = buf->data[1] >> 5;          /* Next to receive */
	ns = (buf->data[1] >> 1) & 0x07; /* Next to send */

	/* Remove IrLAP header */
	g_netbuf_pull(buf, 2);

	g_string_sprintfa(str, "i:%s  %s ca=%02x pf=%d nr=%d ns=%d ", 
			  cmd ? "cmd" : "rsp", 
			  type ? ">" : "<", caddr, pf, nr, ns);
	
	/* Check if we should print IrLMP information */
	if (config_print_irlmp)
		parse_irlmp(buf, str, type, cmd);


	if (config_print_lost_frames) {
		if (type) {
			self_nr = nr;
			if (self_nr != ((peer_ns + 1) % 8)) {
				g_string_sprintfa(str, 
						  "** %d frame(s) lost ** ",
						  (peer_ns + 1 > self_nr) ? 
						  peer_ns + 1 - self_nr : 
						  peer_ns + 9 - self_nr);
			}
			if (ns != ((self_ns + 1) % 8)) {
				g_string_append(str, "** retransmit ** ");
			}
			self_ns = ns;		
		} else {
			peer_nr = nr;
			if (peer_nr != ((self_ns + 1) % 8)) {
				g_string_sprintfa(str, 
						  "** %d frame(s) lost ** ",
						  (self_ns + 1 > peer_nr) ? 
						  self_ns + 1 - peer_nr : 
						  self_ns + 9 - peer_nr);
			}
			if (ns != ((peer_ns + 1) % 8)) {
				g_string_append(str, "** retransmit **");
			}
			peer_ns = ns;
		}
	}
}

/*
 * Function parse_ui_frame (buf, len, u_char)
 *
 *    Information frames
 *
 */
inline void parse_ui_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			   GNetBuf *buf, GString *str)
{
	/* Remove IrLAP header */
	g_netbuf_pull(buf, 2);
	
	g_string_sprintfa(str, "ui:%s %s ca=%02x pf=%d  ", 
			  cmd ? "cmd" : "rsp", 
			  type ? ">" : "<", caddr, pf);
	
	/* Check if we should print IrLMP information */
	if (config_print_irlmp)
		parse_ui_irlmp(buf, str, type);
}

/*
 * Function parse_frmr_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Frame reject frame
 *
 */
inline void parse_frmr_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			     GNetBuf *buf, GString *str)
{
	int nr, ns;
	guint8 ctrl;

	ctrl = buf->data[1];
	nr = buf->data[2] >> 5;          /* Next to receive */
	ns = (buf->data[2] >> 1) & 0x07; /* Next to send */

	g_string_sprintfa(str, 
			  "frmr:%s  %s ca=%02x pf=%d ctrl=%02x, nr=%d ns=%d ",
			  cmd ? "cmd" : "rsp", type ? ">" : "<", caddr, ctrl, 
			  nr, pf, ns);
}

/*
 * Function parse_rr_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Receive ready frame
 *
 */
inline void parse_rr_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			   GNetBuf *buf, GString *str)
{
	int nr;
	
	nr = buf->data[1] >> 5;          /* Next to receive */
	
	g_string_sprintfa(str, "rr:%s %s ca=%02x pf=%d nr=%d ", 
			  cmd ? "cmd" : "rsp", type ? ">" : "<", caddr, pf, 
			  nr);

	if (config_print_lost_frames) {
		if (type) {
			self_nr = nr;
			if (self_nr != ((peer_ns + 1) % 8)) {
				g_string_sprintfa(str, 
						  "** %d frame(s) lost ** ",
						  (peer_ns + 1 > self_nr) ? 
						  peer_ns + 1 - self_nr : 
						  peer_ns + 9 - self_nr);
			}
		} else {
			peer_nr = nr;
			if (peer_nr != ((self_ns + 1) % 8)) {
				g_string_sprintfa(str, 
						  "** %d frame(s) lost ** ",
						  (self_ns + 1 > peer_nr) ? 
						  self_ns + 1 - peer_nr : 
						  self_ns + 9 - peer_nr);
			}
		}
	}
}

/*
 * Function parse_rnr_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Receive not ready frame
 *
 */
inline void parse_rnr_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			    GNetBuf *buf, GString *str)
{
	int nr;

	nr = buf->data[1] >> 5;          /* Next to receive */

	g_string_sprintfa(str, "rnr:%s %s ca=%02x pf=%d nr=%d ", 
			  cmd ? "cmd" : "rsp", type ? ">" : "<", caddr, pf, 
			  nr);
}

/*
 * Function parse_rej_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Reject frame
 *
 */
inline void parse_rej_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			    GNetBuf *buf, GString *str)
{
	int nr;

	nr = buf->data[1] >> 5;          /* Next to receive */

	g_string_sprintfa(str, "rej:%s %s ca=%02x pf=%d nr=%d ", 
			  cmd ? "cmd" : "rsp", type ? ">" : "<", caddr, pf, 
			  nr);
}

/*
 * Function parse_srej_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Selective reject frame
 *
 */
inline void parse_srej_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			     GNetBuf *buf, GString *str)
{
	int nr;
	
	nr = buf->data[1] >> 5;          /* Next to receive */

	g_string_sprintfa(str, "srej:%s %s ca=%02x pf=%d nr=%d ", 
			  cmd ? "cmd" : "rsp", type ? ">" : "<", caddr, pf, 
			  nr);
}

/*
 * Function parse_ua_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Unnumbered acknowledgement frame
 *
 */
inline void parse_ua_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			   GNetBuf *buf, GString *str)
{
	struct ua_frame *frame = (struct ua_frame *) buf->data;
	gint32 saddr, daddr;
	
	saddr = GINT32_FROM_LE(frame->saddr);
	daddr = GINT32_FROM_LE(frame->daddr);

	if (type)
		g_string_sprintfa(str, "ua:%s ca=%02x pf=%d %08x > %08x ", 
				  cmd?"cmd":"rsp", caddr, pf, saddr, daddr);
	else
		g_string_sprintfa(str, "ua:%s ca=%02x pf=%d %08x < %08x ", 
				  cmd?"cmd":"rsp", caddr, pf, daddr, saddr);
}

/*
 * Function parse_disc_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Disconnect frame
 *
 */
inline void parse_disc_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			     GNetBuf *buf, GString *str)
{
	g_string_sprintfa(str, "disc:%s %s ca=%#02x pf=%d ", 
			  cmd ? "cmd" : "rsp", type ? ">" : "<", caddr, pf);
}

/*
 * Function parse_dm_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Disconnected mode frame
 *
 */
inline void parse_dm_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			   GNetBuf *buf, GString *str)
{
	g_string_sprintfa(str, "dm:%s %s ca=%#02x pf=%d ", cmd?"cmd":"rsp", 
			  type ? ">" : "<", caddr, pf);	
}

/*
 * Function parse_rd_frame (caddr, cmd, pf, type, buf, str)
 *
 *    Request disconnect frame
 *
 */
inline void parse_rd_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			   GNetBuf *buf, GString *str)
{
	g_string_sprintf(str, "rd:%s %s ca=%#02x pf=%d ", cmd?"cmd":"rsp", 
			 type ? ">" : "<", caddr, pf);	
}

/*
 * Function parse_test_frame (caddr, cmd, pf, type, buf, str)
 *
 *    Test frame
 *
 */
inline void parse_test_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			     GNetBuf *buf, GString *str)
{
	struct test_frame *frame = (struct test_frame *) buf->data;
        gint32 saddr, daddr;

	saddr = GINT32_FROM_LE(frame->saddr);
	daddr = GINT32_FROM_LE(frame->daddr);

	g_string_sprintfa(str, "test:%s ca=%#02x pf=%d %08x %s %08x ", 
			  cmd?"cmd":"rsp", caddr, pf, saddr, type ? ">" : "<", 
			  daddr);	
}

/*
 * Function parse_snrm_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Set normal response mode frame
 *
 */
inline void parse_snrm_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			     GNetBuf *buf, GString *str)
{
	struct snrm_frame *frame = (struct snrm_frame *) buf->data;
	gint32 saddr, daddr;
	guint8 new_caddr;

	saddr = GINT32_FROM_LE(frame->saddr);
	daddr = GINT32_FROM_LE(frame->daddr);

	new_caddr = frame->ncaddr;

	/* Which direction? */
	if (type)
	       g_string_sprintfa(str, "snrm:%s ca=%02x pf=%d %08x > "
				 "%08x new-ca=%02x ", cmd ? "cmd" : "rsp", 
				 caddr, pf, saddr, daddr, new_caddr);
	else
	       g_string_sprintfa(str, "snrm:%s ca=%02x pf=%d %08x < "
				 "%08x new-ca=%02x ", cmd ? "cmd" : "rsp", 
				 caddr, pf, daddr, saddr, new_caddr);
}

/*
 * Function parse_rnrm_frame (caddr, cmd, pf, type, frame, len)
 *
 *    Request normal response mode
 *
 */
inline void parse_rnrm_frame(guint8 caddr, guint8 cmd, guint8 pf, int type, 
			     GNetBuf *buf, GString *str)
{
	gint32 saddr, daddr;
	struct rnrm_frame *frame = (struct rnrm_frame *) buf->data;

	saddr = GINT32_FROM_LE(frame->saddr);
	daddr = GINT32_FROM_LE(frame->daddr);

	g_string_sprintfa(str, "snrm:%s ca=%02x pf=%d %08x %s %08x ", 
			  cmd?"cmd":"rsp", caddr, pf, saddr, type?">":"<", 
			  daddr);
}

/*
 * Function parse_irda_frame (buf, len)
 *
 *    Find out what kind of IrDA frame we have received, and dispatch the 
 *    right function to handle the frame
 *
 */
inline void parse_irda_frame(int type, GNetBuf *buf, GString *str)
{
	guint8 ctrl;
	guint8 cmd;
	guint8 caddr;
	guint8 pf;

	cmd   = buf->data[0] & CMD_FRAME;
	caddr = buf->data[0] & CBROADCAST;

	pf    = (buf->data[1] & PF_BIT) >> 4;
	ctrl  = buf->data[1] & ~PF_BIT; /* Mask away poll/final bit */

	/*  
	 *  Optimize for the common case and check if the frame is an
	 *  I(nformation) frame. Only I-frames have bit 0 set to 0
	 */
	if (~ctrl & 0x01) {
		parse_i_frame(caddr, cmd, pf, type, buf, str);
		return;
	}

	/*
	 *  We now check is the frame is an S(upervisory) frame. Only 
	 *  S-frames have bit 0 set to 1 and bit 1 set to 0
	 */
	if (~ctrl & 0x02) {
		/* 
		 *  Received S(upervisory) frame, check which frame type it is
		 *  only the first nibble is of interest
		 */
		switch (ctrl & 0x0f) {
		case RR:
			parse_rr_frame(caddr, cmd, pf, type, buf, str);
			break;
		case RNR:
			parse_rnr_frame(caddr, cmd, pf, type, buf, str);
			break;
		case REJ:
			parse_rej_frame(caddr, cmd, pf, type, buf, str);
			break;
		case SREJ:
			parse_srej_frame(caddr, cmd, pf, type, buf, str);
			break;
		default:
			g_string_sprintfa(str,
					  "Unknown S frame %02x received!\n", 
					  ctrl);
			break;
		}
		return;
	}
	/* 
	 *  This must be a C(ontrol) frame 
	 */
	switch(ctrl) {
	case XID_RSP:
		parse_xid_frame(cmd, pf, type, buf, str);
		break;
	case XID_CMD:
		parse_xid_frame(cmd, pf, type, buf, str);
		break;
	case SNRM_CMD:
		if (cmd)
			parse_snrm_frame(caddr, cmd, pf, type, buf, str);
		else
			parse_rnrm_frame(caddr, cmd, pf, type, buf, str);
		break;
	case DM_RSP:
		parse_dm_frame(caddr, cmd, pf, type, buf, str);
		break;
	case DISC_CMD:
	        if (cmd)
			parse_disc_frame(caddr, cmd, pf, type, buf, str);
		else
			parse_rd_frame(caddr, cmd, pf, type, buf, str);
		break;
	case TEST_CMD:
		parse_test_frame(caddr, cmd, pf, type, buf, str);
		break;
	case UA_RSP:
		parse_ua_frame(caddr, cmd, pf, type, buf, str);
		break;
	case FRMR_RSP:
		parse_frmr_frame(caddr, cmd, pf, type, buf, str);
		break;
	case UI_FRAME:
		parse_ui_frame(caddr, cmd, pf, type, buf, str);
		break;
	default:
	        g_string_sprintfa(str, "Unknown frame %02x received!\n", 
				  ctrl);
		break;
	}
}

/*
 * Function irdadump_init ()
 *
 *    
 *
 */
int irdadump_init(char *ifdev)
{
	struct ifreq ifr;

	memset(&last_ias, 0, sizeof(struct ias_query));
	memset(&conn, 0, sizeof(struct lsap_state));
	
	curr_time = &time1;
	prev_time = &time2;

	/* Get time, so the first time diff is right */
	gettimeofday(prev_time, (struct timezone*) 0);

        /* 
	 * Create socket, we must use SOCK_DGRAM to get the link level header 
	 * that we are interested in 
	 */
	fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));
	if (fd < 0) {
		return fd;
        }

	if (ifdev) {
		strncpy(ifr.ifr_name, ifdev, IFNAMSIZ);
		if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
			perror("ioctl");
			exit(-1);
		}
		ifindex = ifr.ifr_ifindex;
	}

	buf = g_netbuf_new(MAX_FRAME_SIZE);

	return fd;
}

/*
 * Function irdadump_loop ()
 *
 *    
 *
 */
int irdadump_loop(GString *str) 
{
	int len;

	fromlen = sizeof(struct sockaddr_ll);
	
	g_netbuf_recycle(buf);
	
	len = recvfrom(fd, buf->data, MAX_FRAME_SIZE, 0, 
		       (struct sockaddr *) &from, &fromlen);
	if (len < 0) {
		g_message("recvfrom");
		exit(-1);
	}

	/* Filter away all non IrDA frames */
	if (from.sll_protocol != ntohs(ETH_P_IRDA))
		return -1;

	/* Filter away frames from other IrDA interfaces */
	if (ifindex && (from.sll_ifindex != ifindex))
		return -1;

	/* Filter away empty frames (forced speed change) */
	if (len == 0)
		return -1;

	/* Data should be fine now */
	g_netbuf_put(buf, len);

	/* Get time from packet */
	if (ioctl(fd, SIOCGSTAMP, curr_time) < 0) {
		perror("ioctl");
		exit(-1);
	}
 	print_time(curr_time, str);

	if (config_print_diff) {
		print_diff_time(curr_time, prev_time, str);
		
		/* Swap */
		tmp_time = prev_time;
		prev_time = curr_time;
		curr_time = tmp_time;
	}
	parse_irda_frame(from.sll_pkttype, buf, str);
	
        g_string_sprintfa(str, "(%d) ", len);

	if (config_dump_frame) {
		int i, maxlen;
		char c;

		g_string_append(str, "\n\t");
		maxlen = (len < snaplen) ? len : snaplen;
		
		for (i=0;i<maxlen;i++)
			g_string_sprintfa(str, "%02x", buf->head[i]);
		g_string_append(str, "\n\t");
		
		for (i=0;i<maxlen;i++) {
			c = buf->head[i];
			if (c < 32 || c > 126) 
				c='.';
			g_string_sprintfa(str, " %c", c);
		}
	}
	return 0;
}




