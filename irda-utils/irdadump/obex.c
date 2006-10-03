/*********************************************************************
 *                
 * Filename:      obex.c
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Wed Mar 31 14:09:47 1999
 * Modified at:   Wed Apr 21 15:52:03 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1999 Dag Brattli, All Rights Reserved.
 *     Copyright (c) 1999 Jean Tourrilhes, All Rights Reserved.
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

#include "irdadump.h"
#include "obex.h"

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>		/* ntohs */

/* 
 * We keep track of the last command since we need to discriminate the
 * CONNECT case.
 */
static guint8 last_command = 0xff;

void unicode_to_char(guint8 *buf)
{
	guint8 *buf2 = buf;
	int n=0;
	
	while ((buf2[n] = buf[n*2+1]))
		n++;
}

/*
 * Function parse_obex_header (buf)
 *
 *    
 *
 */
int parse_obex_header(GNetBuf *buf, GString *str, int istext)
{
	guint8 string[255];
	guint32 obex_int;
	guint16 string_len;
	int len = 0;

	switch (buf->data[0] & OBEX_HI_MASK) {
	case OBEX_UNICODE:
		memcpy(&string_len, buf->data + 1, 2); /* Align value */
		string_len = GINT16_FROM_BE(string_len) - 3;
		len += 3;
		if (string_len) {
			memcpy(string, buf->data + 3, string_len);
			unicode_to_char(string);
			len += string_len;
		}
		string[string_len] = 0;
		g_string_sprintfa(str, "\"%s\" ", string);
		break;
	case OBEX_BYTE_STREAM:
                /* g_print("OBEX_BYTE_STREAM");fflush(stdout); */
		memcpy(&string_len, buf->data + 1, 2); /* Align value */
		string_len = GINT16_FROM_BE(string_len) - 3;
		len += string_len + 3;
		if(istext) {
			if (string_len) {
				memcpy(string, buf->data + 3, string_len);
				string[string_len] = 0;
				g_string_sprintfa(str, "\"%s\" ", string);
			}
		} else
			g_string_sprintfa(str, "[%d bytes] ", string_len);
		break;
	case OBEX_BYTE:
		/* g_print("OBEX_BYTE");fflush(stdout); */
		g_string_sprintfa(str, "%d ", buf->data[0]);
                len += 2;
		break;
	case OBEX_INT:
		memcpy(&obex_int, buf->data + 1, 4); /* Align value */
		len += 5;
		/* printf("%ld ", ntohl(tmp_int));fflush(stdout); */
		g_string_sprintfa(str, "%d ", GINT32_FROM_BE(obex_int));
		break;
	default:
		g_print("******");fflush(stdout);
		break;
	}
	
	return len;
}

/*
 * Function parse_obex_headers (buf)
 *
 *    Parse OBEX headers
 *
 */
inline void parse_obex_headers(GNetBuf *buf, GString *str, guint8 header_offset)
{
	struct obex_minimal_frame *frame;
	int final;
	guint16 size;
	int len;

	frame = (struct obex_minimal_frame *) buf->data;

	/* We know it's a put frame, but we have to check if it's the final */
	final = frame->opcode & OBEX_FINAL;

	/* Length of this frame */
	size = ntohs(frame->len);
	
	/* Remove the OBEX common header */
	g_netbuf_pull(buf, header_offset);

	g_string_sprintfa(str, "final=%d len=%d ", final >> 7, size);

	/* Parse all headers */
	while (buf->len > 0) {
		/* Read the header identifier */
		/* g_print("hi=%02x", buf->data[0]);fflush(stdout); */
		switch (buf->data[0]) {
		case HEADER_NAME:
		case HEADER_ANAME:
			g_string_append(str, "Name=");
			len = parse_obex_header(buf, str, 1);
			break;
		case HEADER_DESCRIPTION:
			g_string_append(str, "Description=");
			len = parse_obex_header(buf, str, 0);
			break;
		case HEADER_LENGTH:
			g_string_append(str, "Lenght=");
			len = parse_obex_header(buf, str, 0);
			break;
		case HEADER_TYPE:
			g_string_append(str, "Type=");
			len = parse_obex_header(buf, str, 1);
			break;
		case HEADER_TARGET:
			g_string_append(str, "Target=");
			len = parse_obex_header(buf, str, 1);
			break;
		case HEADER_BODY:
			g_string_append(str, "body=");
			len = parse_obex_header(buf, str, 0);
			break;
		case HEADER_BODY_END:
			g_string_append(str, "body-end=");
			len = parse_obex_header(buf, str, 0);
			break;
		case HEADER_CONN_ID:
			g_string_append(str, "Connection ID=");
			len = parse_obex_header(buf, str, 0);
			break;
		default:
			g_string_sprintfa(str, "custom(0x%x)=", buf->data[0]);
			len = parse_obex_header(buf, str, 0);
			break;
		}
                /* g_print("len=%d\n", len);fflush(stdout); */
		if(g_netbuf_pull(buf, len) == NULL) {
			g_string_append(str, "{unterminated} ");
			break;
		}
	}
}

inline void parse_obex_connect(GNetBuf *buf, GString *str)
{
	struct obex_connect_frame *frame;
	guint16 length;
	guint8 version;
	int flags;
	guint16 mtu;
	
	frame = (struct obex_connect_frame *) buf->data;

	length  = ntohs(frame->len);

	/* Check if it contains connection setup parameters - Jean II */
	if(length == 7) {
		version = frame->version;
		flags   = frame->flags;
		mtu     = ntohs(frame->mtu);

		g_string_sprintfa(str,
				  "CONNECT len=%d ver=%d.%d flags=%d mtu=%d ", 
				  length, ((version & 0xf0) >> 4),
				  version & 0x0f, flags, mtu);
	} else
		g_string_sprintfa(str, "CONNECT len=%d ", length);
}

/*
 * The first success frame contains the negociated Obex parameters
 * We also need to parse the anser to GET request properly
 * Jean II
 */
inline void parse_obex_success(GNetBuf *buf, GString *str, guint8 last_cmd)
{
	/* CONNECT response is different from all the other ones...*/
	if (last_cmd == OBEX_CONNECT) {
		struct obex_connect_frame *frame;
		guint8 version;

		frame = (struct obex_connect_frame *) buf->data;
		version = frame->version;

		g_string_sprintfa(str,
				  "SUCCESS len=%d ver=%d.%d flags=%d mtu=%d ", 
				  ntohs(frame->len), ((version & 0xf0) >> 4),
				  version & 0x0f, frame->flags,
				  ntohs(frame->mtu));
		parse_obex_headers(buf, str, sizeof(struct obex_connect_frame));
	} else {
		g_string_sprintfa(str, "SUCCESS (from 0x%x) ", last_cmd);
		parse_obex_headers(buf, str, sizeof(struct obex_minimal_frame));
	}
}

inline void parse_obex_setpath(GNetBuf *buf, GString *str)
{
	struct obex_setpath_frame *frame;
	guint16 length;

	frame = (struct obex_setpath_frame *)buf->data;
	length  = ntohs(frame->len);

	g_string_sprintfa(str,
			  "SETPATH flags=0x%x constants=%d ", 
			  frame->flags, frame->constants);

	parse_obex_headers(buf, str, sizeof(struct obex_setpath_frame));
}

/*
 * Function parse_obex (buf)
 *
 *    Parse OBEX commands and responses
 *
 */
inline void parse_obex(GNetBuf *buf, GString *str, int cmd)
{
	guint8	opcode;
	int	len;

	/* g_print(__FUNCTION__);fflush(stdout); */

	g_string_append(str, "\n\tOBEX ");

	/* Check for empty frames - Jean II */
	len = g_netbuf_get_len(buf);
	if(len == 0)
	  return;

	opcode = buf->data[0] & ~OBEX_FINAL; /* Remove final bit */

	/* Check if it's a command or response frame - Jean II */
	if (!cmd) {
		switch (opcode) {
		case OBEX_CONTINUE:
			g_string_append(str, "CONTINUE ");
			parse_obex_headers(buf, str,
					   sizeof(struct obex_minimal_frame));
			break;
		case OBEX_SWITCH_PRO:
			g_string_append(str, "SWITCH_PRO ");
			break;
		case OBEX_SUCCESS:
			parse_obex_success(buf, str, last_command);
			break;
		case OBEX_CREATED:
			g_string_append(str, "CREATED ");
			break;
		case OBEX_ACCEPTED:
			g_string_append(str, "ACCEPTED ");
			break;
		case OBEX_BAD_REQUEST:
			g_string_append(str, "BAD REQUEST ");
			break;
		case OBEX_FORBIDDEN:
			g_string_append(str, "FORBIDDEN ");
			break;
		case OBEX_CONFLICT:
			g_string_append(str, "CONFLICT ");
			break;
		default:
			g_string_sprintfa(str, "Unknown response %02x ", 
					  opcode);
			break;
		}
	} else {
		last_command = opcode;
		switch (opcode) {
		case OBEX_CONNECT:
			parse_obex_connect(buf, str);
			break;
		case OBEX_PUT:
			g_string_append(str, "PUT ");
			parse_obex_headers(buf, str,
					   sizeof(struct obex_minimal_frame));
			break;
		case OBEX_GET:
			g_string_append(str, "GET ");
			parse_obex_headers(buf, str,
					   sizeof(struct obex_minimal_frame));
			break;
		case OBEX_DISCONNECT:
			g_string_append(str, "DISC ");
			break;
		case OBEX_ABORT:
			g_string_append(str, "ABORT ");
			break;
		case OBEX_SETPATH:
			parse_obex_setpath(buf, str);
			break;
		default:
			if ((opcode > 0x04) && (opcode < 0x10))
				g_string_append(str, "RESERVED ");
			else if ((opcode > 0x0f) && (opcode < 0x20))
				g_string_append(str, "USER_DEFINED ");
			else 
				g_string_sprintfa(str, "unknown opcode %#x ",
						 opcode);
			break;
		}
	}
}
