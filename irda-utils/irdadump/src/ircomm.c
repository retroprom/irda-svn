/*********************************************************************
 *                
 * Filename:      ircomm.c
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sun Jun  6 13:40:30 1999
 * Modified at:   Fri Jun 11 10:47:08 1999
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

#include "irdadump.h"
#include "ircomm.h"

static char *ircomm_service_type[] = 
{
	"N/A",
	"THREE_WIRE_RAW",
	"THREE_WIRE",
	"NINE_WIRE",
	"CENTRONICS"
};

static char *ircomm_port_type[] = 
{
	"Unknown",
	"SERIAL",
	"PARALLEL"
};

void parse_ircomm_params(guint8 clen, GNetBuf *buf, GString *str)
{
	guint pi, pl;
	guint pv_byte;
	guint n = 0;

	while (clen) {
		pi = buf->data[n] & 0x7f; /* Remove critical bit */
		pl = buf->data[n+1];
		
		switch (pi) {
		case SERVICE_TYPE:
			pv_byte = buf->data[3];
			g_string_sprintfa(str, "Service Type=%s ",
					  ircomm_service_type[pv_byte]);
			break;
		case PORT_TYPE:
			pv_byte = buf->data[3];
			g_string_sprintfa(str, "Port Type=%s ",
					  ircomm_port_type[pv_byte]);
			break;
		default:
			break;
		}
		clen -= pl+2;
		n += pl+2;
	}
	g_netbuf_pull(buf, clen);
}

void parse_ircomm_connect(struct lsap_state *conn, GNetBuf *buf, GString *str)
{
	parse_ircomm(conn, buf, str);
}

void parse_ircomm(struct lsap_state *conn, GNetBuf *buf, GString *str)
{
	guint8 clen;

	g_string_append(str, "\n\tIrCOMM ");

	return;
	clen = buf->data[0];

	g_netbuf_pull(buf, 1);

	if (clen)
		parse_ircomm_params(clen, buf, str);
}
