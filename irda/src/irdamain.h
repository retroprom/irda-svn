/*
 * Filename: irdamain.h
 *
 * Description:
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __IRDAMAIN_H__
#define __IRDAMAIN_H__

#define _GNU_SOURCE

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <regex.h>
#include <asm/types.h>
#include <linux/irda.h>

#ifdef HAVE_CONFIG_H 
#include <config.h>
#endif

#define PROC_IRIAS "/proc/net/irda/irias"


int m_argc, arg_index;
char **m_argv;
unsigned int count, delay, fmt_column, fmt_no_header, debug;
char fmt_sep;
void (*m_func)();

void register_timeout(sighandler_t func);

#endif

