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

void version() {
  fprintf(stdout,"%s, %s\n",PACKAGE_NAME,PACKAGE_VERSION);
}

void no_irda() {
  fprintf(stderr,"No IrDA support in kernel or IrDA service was not started.\n");
}


void main_help() {
  fprintf(stdout,
    "Usage: %s [ OPTIONS ] OBJECT { COMMAND | help }\n"
    "where OBJECT := { lap | lmp | ias | ttp }\n"
    "      OPTIONS := { -V[version] | -h[elp] | [ -c count ] [ -d delay ] \n"
    "                 [ -H ] [ -C ] [ -S separator ] [ -D ] }\n",
    PACKAGE_NAME    
  );
}

void main_no_option(char *option) {
  fprintf(stderr,"Option \"%s\" is unknown, try \"%s -help\".\n",option,PACKAGE_NAME);
}

void main_no_object(char *object) {
  fprintf(stderr,"Object \"%s\" is unknown, try \"%s help\".\n",object,PACKAGE_NAME);
}

void main_no_action() {
  fprintf(stderr,"No action given, try \"%s help\".\n",PACKAGE_NAME);
}

void main_err_param(char *param) {
  fprintf(stderr,"Invalid parameter value \"%s\".\n",param);
}

void main_no_complete() {
  fprintf(stderr,"Command line is not complete. Try \"%s help\".\n",PACKAGE_NAME);
}


void cleanup() {
  fflush(stdout);
  exit(0);
}

void timeout(int signo) {
  if (count) {
    if (m_func) m_func();
    count--;
  } else {
    cleanup();
  }
}

void register_timeout(sighandler_t func) {
  struct sigaction sa;
  struct itimerval it;
  
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = timeout;
  (void) sigaction(SIGALRM, &sa, NULL);

  /* set the function which will be repeatedly called by timeout */
  m_func=func;

  it.it_interval.tv_sec=delay;
  it.it_interval.tv_usec=0;
  it.it_value.tv_sec=delay;
  it.it_value.tv_usec=0;
  setitimer(ITIMER_REAL, &it, NULL);
  
}

int main(int argc, char**argv) {
  struct sigaction sa;
  
  arg_index=1;
  m_argc = argc;
  m_argv = argv;  
  m_func = NULL;
  delay = 1;
  count = 0;
  fmt_column = 0;
  fmt_no_header = 0;
  fmt_sep=':';
  debug=0;

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
        case 'D':
          debug++;
        break;
        case 'C':
          fmt_column=1;
        break;
        case 'H':
          fmt_no_header=1;
        break;
        case 'S': {
          if (arg_index+1==m_argc) {
            main_no_complete();
            exit(255);
          }
          arg_index++;
          fmt_sep=m_argv[arg_index][0]; 
        }
        break;
        case 'd': {
          if (arg_index+1==m_argc) {
            main_no_complete();
            exit(255);
          } else {
            const char *nptr;
            char *endptr;
          
            arg_index++;
            nptr = m_argv[arg_index];
            endptr = NULL;
            delay = (unsigned int) abs(strtoul(nptr,&endptr,0));
            if (!nptr || !*nptr || !endptr || *endptr) {
              main_err_param(m_argv[arg_index]);
              exit(255);
            }
          }
        }
        break;
        case 'c': {
          if (arg_index+1==m_argc) {
            main_no_complete();
            exit(255);
          } else {
            const char *nptr;
            char *endptr;
         
            arg_index++;
            nptr = m_argv[arg_index];
            endptr = NULL;
            count = (unsigned int) abs(strtoul(nptr,&endptr,0));
            if (!nptr || !*nptr || !endptr || *endptr) {
              main_err_param(m_argv[arg_index]);
              exit(255);
            }
          }
        }
        break;
        default:
          main_no_option(m_argv[arg_index]);
          exit(255);
      }
    }
    arg_index++;
  } 

  if (arg_index==m_argc) {
    main_no_action();
    exit(255);
  }

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = cleanup;
  (void) sigaction(SIGTERM, &sa, NULL);
  (void) sigaction(SIGINT, &sa, NULL);
  (void) sigaction(SIGHUP, &sa, NULL);

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

