/*
 * Filename: main.c
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

#include "irdamain.h"
#include "irias.h"

void main_help() {
  fprintf(stdout,
    "Usage: %s [ OPTIONS ] OBJECT { COMMAND | help }\n"
    "where OBJECT := { lap | lmp | ias | ttp }\n"
    "      OPTIONS := { -V[version] | -h[elp] | -c[count] <repeat> }\n",
    PACKAGE_NAME    
  );
}

void version() {
  fprintf(stdout,"%s, %s\n",PACKAGE_NAME,PACKAGE_VERSION);
}

void main_no_option(char *option) {
  fprintf(stderr,"Option \"%s\" is unknown, try \"%s -help\".\n",option,PACKAGE_NAME);
}

void main_no_object(char *object) {
  fprintf(stderr,"Object \"%s\" is unknown, try \"%s help\".\n",object,PACKAGE_NAME);
}


int main(int argc, char**argv) {
  arg_index=1;
  m_argc = argc;
  m_argv = argv;  
  
  if (m_argc<2) {
    main_help();
    exit(255);
  }

  /* parse general options if any */
  while (arg_index<m_argc && m_argv[arg_index][0]=='-') {
    if (strlen(m_argv[arg_index])>=2) {
      switch (m_argv[arg_index][1]) {
        case 'V':
          version();
          exit(0);
        break;
        case 'h':
          main_help();
          exit(255);
        break;
        default:
          main_no_option(m_argv[arg_index]);
          exit(255);
      }
    }
    arg_index++;
  } 
    
  /* parse top level options */ 
  while (arg_index<m_argc) {
    if (!strncmp("he",m_argv[arg_index],2)) {
      main_help();
      exit(255);
    } else if (!strncmp("ia",m_argv[arg_index],2)) {
      irias_main();
      exit(0);
    } else {
      main_no_object(m_argv[arg_index]);
      exit(255);
    }
    arg_index++;
  } 
  return(0);
}

