/*
 * Filename: irias.c
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

struct {
  int fd, is_local, ias_daddr;
  struct irda_ias_set query;
  char filename[PATH_MAX];
} ias_data;

void irias_help() {
  fprintf(stdout,
    "Usage: %s ias list\n"
    "       %s ias get [ DEST ] class CLASS attrib ATTRIB [ filename FILE ]\n"
    "DEST := { local | remote DEVADDR }\n",
    PACKAGE_NAME, PACKAGE_NAME    
  );
}

void irias_no_option(char *option) {
  fprintf(stderr,"Command \"%s\" is unknown, try \"%s ias help\".\n",option,PACKAGE_NAME);
}

void irias_no_complete() {
  fprintf(stderr,"Command line is not complete. Try \"%s ias help\".\n",PACKAGE_NAME);
}

void irias_no_iasclass() {
  fprintf(stderr,"\"class\" attribute is not present. Try \"%s ias help\".\n",PACKAGE_NAME);
}

void irias_no_iasattrib() {
  fprintf(stderr,"\"attrib\" attribute is not present. Try \"%s ias help\".\n",PACKAGE_NAME);
}

void irias_err_daddr(char *daddr) {
  fprintf(stderr,"Invalid device address \"%s\". Example: 0xabcd.\n",daddr);
}

void irias_show_octetseq(struct irda_ias_set *query) {
  int i;
  
  for(i=0; i<query->attribute.irda_attrib_octet_seq.len; i++) 
    fprintf(stdout,"%02x ",query->attribute.irda_attrib_octet_seq.octet_seq[i]);
}

void irias_list_entry(struct irda_ias_set *query) {
  const char *charset[] = { "ASCII", "ISO-8859-1", "ISO-8859-2", "ISO-8859-3", "ISO-8859-4",
                            "ISO-8859-5", "ISO-8859-6", "ISO-8859-7", "ISO-8859-8", 
                            "ISO-8859-9", "UNICODE" };

  if (fmt_column==1) {
    fprintf(stdout,"%s%c%s",query->irda_class_name,fmt_sep,query->irda_attrib_name);
  } else {
    fprintf(stdout,"Class: \"%s\"\nAttribute: \"%s\"\n"
            ,query->irda_class_name,query->irda_attrib_name);
  }
  switch (query->irda_attrib_type) {
    case IAS_MISSING:
    break;
    case IAS_INTEGER:
      if (fmt_column==1) {
        fprintf(stdout,"%cinteger%c%d\n",fmt_sep,fmt_sep,query->attribute.irda_attrib_int);
      } else {
        fprintf(stdout,"Attribute Type: Integer\nValue: %d\n",
              query->attribute.irda_attrib_int);
      }
    break;
    case IAS_OCT_SEQ:
      if (fmt_column==1) {
        fprintf(stdout,"%coctetseq%c",fmt_sep,fmt_sep);
      } else {
        fprintf(stdout,"Attribute Type: Octet Sequence (length=%d)\n",
                query->attribute.irda_attrib_octet_seq.len);
      }
      if (strlen(ias_data.filename)) {
        int fh;
        if ((fh=open(ias_data.filename,O_WRONLY|O_CREAT,0644)) != -1) {
          int wlen;
          wlen=write(fh,query->attribute.irda_attrib_octet_seq.octet_seq,
                   query->attribute.irda_attrib_octet_seq.len);
          if (wlen != query->attribute.irda_attrib_octet_seq.len) {
            perror(ias_data.filename);
          }
          close(fh);
          if (fmt_column==1) {
            fprintf(stdout,"(%s)\n",ias_data.filename);
          } else {
            fprintf(stdout,"Value: (file=%s)\n",ias_data.filename);
          }
        } else {
          perror(ias_data.filename);
        }
     } else {
        if (fmt_column==1) {
          irias_show_octetseq(query);
          fprintf(stdout,"\n");
        } else {
          fprintf(stdout,"Value: ");
          irias_show_octetseq(query);
          fprintf(stdout,"\n");
        }
      }
    break;
    case IAS_STRING:
      if (fmt_column==1) {
        fprintf(stdout,"%cstring%c%s\n",fmt_sep,fmt_sep,query->attribute.irda_attrib_string.string);
      } else {
        fprintf(stdout,"Attribute Type: String (length=%d, charset=%s)\nValue: \"%s\"\n",
              query->attribute.irda_attrib_string.len,
              charset[query->attribute.irda_attrib_string.charset],
              query->attribute.irda_attrib_string.string);
      }
    break;
    default:
      fprintf(stderr,"%s: Invalid IAS attribute type received. type=%d",PACKAGE_NAME,query->irda_attrib_type);
      exit(1);
  }
  
}

void irias_get_exec() {
  struct irda_ias_set m_query;
  int ret, m_len=sizeof(struct irda_ias_set);

  m_query=ias_data.query;
  if (ias_data.is_local) {
    ret=getsockopt(ias_data.fd,SOL_IRLMP,IRLMP_IAS_GET,&m_query,&m_len);
  } else {
    ret=getsockopt(ias_data.fd,SOL_IRLMP,IRLMP_IAS_QUERY,&m_query,&m_len);
  }
  
  if (ret) {
    perror(PACKAGE_NAME);
  } else {
    irias_list_entry(&m_query);
  }
}

void irias_get() {
  int have_ias_class=0, have_ias_attrib=0;

  ias_data.is_local=1;
  ias_data.filename[0]=0;

  /* get the options */
  arg_index++;
  while (arg_index<m_argc) {
    if (!strncmp("lo",m_argv[arg_index],2)) {
      ias_data.is_local=1;
    } else if (!strncmp("re",m_argv[arg_index],2)) {
      ias_data.is_local=0;
      if (arg_index+1==m_argc) {
        irias_no_complete();
        exit(255);
      } else {
        const char *nptr;
        char * endptr;
        
        arg_index++;
        nptr = m_argv[arg_index];
        endptr = NULL;
        ias_data.query.daddr = (unsigned int) strtoul(nptr,&endptr,16);
        if (!nptr || !*nptr || !endptr || *endptr) {
          irias_err_daddr(m_argv[arg_index]);
          exit(1);
        }
      }
    } else if (!strncmp("cl",m_argv[arg_index],2)) {
      if (arg_index+1==m_argc) {
        irias_no_complete();
        exit(255);
      } else {
        arg_index++;
        strncpy(ias_data.query.irda_class_name, m_argv[arg_index], IAS_EXPORT_CLASSNAME);
        have_ias_class++;
      }
    } else if (!strncmp("at",m_argv[arg_index],2)) {
      if (arg_index+1==m_argc) {
        irias_no_complete();
        exit(255);
      } else {
        arg_index++;
        strncpy(ias_data.query.irda_attrib_name, m_argv[arg_index], IAS_EXPORT_ATTRIBNAME);
        have_ias_attrib++;
      }
    } else if (!strncmp("fi",m_argv[arg_index],2)) {
      if (arg_index+1==m_argc) {
        irias_no_complete();
        exit(255);
      } else {
        arg_index++;
        strncpy(ias_data.filename, m_argv[arg_index], PATH_MAX);
      }
    } else {
      irias_no_option(m_argv[arg_index]);
      exit(255);
    }
    arg_index++;
  }

  if (!have_ias_class) { 
    irias_no_iasclass();
    exit(255);
  }
  if (!have_ias_attrib) {
    irias_no_iasattrib();
    exit(255);
  }

  ias_data.fd=socket(AF_IRDA,SOCK_STREAM,0);
  if (ias_data.fd<0) {
    perror(PACKAGE_NAME);
    exit(errno);
  }

  if (fmt_column==1) {
    if (fmt_no_header==0) {
      fprintf(stdout,"class%cattribute%ctype%cvalue\n",fmt_sep,fmt_sep,fmt_sep);
    }
  }
  if (count) {
    int m_sleep;
    
    if (count+1 < count) {
      m_sleep=count;
    } else {
      m_sleep=count+1;
    }
    register_timeout(irias_get_exec);
    while (1) sleep(m_sleep);
  } else {
    irias_get_exec();
  }
}

void irias_set() {
}

#define LSIZE 4096

void irias_list() {
  FILE *fp;
  char line[LSIZE];
  int in_class=0;
  
  if ((fp=fopen(PROC_IRIAS,"r")) == NULL) {
    perror("Unable to open the " PROC_IRIAS " file");
  } else {
    regex_t *rx_class, *rx_attrib;
    int rx_ret;
    
    rx_ret=regcomp(rx_class,"^\\s*name:\\s*([^\\s]+)\\s*,", REG_EXTENDED | REG_NEWLINE);
    if (rx_ret) {
      regerror(rx_ret,rx_class,line,LSIZE-1);
      exit(2);
    }
    rx_ret=regcomp(rx_attrib,"ttribute name: ", REG_EXTENDED | REG_NEWLINE);
    if (rx_ret) {
      regerror(rx_ret,rx_attrib,line,LSIZE-1);
      exit(2);
    }
     
    while (fgets(line,LSIZE-1,fp) != NULL) {
      fprintf(stdout,"l=%s\n",line); 

    }

    regfree(rx_class);
    regfree(rx_attrib);
    fclose(fp);
  }
}

void irias_main() {
  arg_index++;

  if (arg_index==m_argc) {
    /* execute default action */
    irias_list();
    exit(0);
  }
  
  if (!strncmp("he",m_argv[arg_index],2)) {
    irias_help();
    exit(255);
  } else if (!strncmp("ge",m_argv[arg_index],2)) {
    irias_get();
    exit(0);
  } else if (!strncmp("se",m_argv[arg_index],2)) {
    irias_set();
    exit(0);
  } else if (!strncmp("ls",m_argv[arg_index],2) ||
             !strncmp("sh",m_argv[arg_index],2) ||
             !strncmp("li",m_argv[arg_index],2)) {
    irias_list();
    exit(0);
  } else {
    irias_no_option(m_argv[arg_index]);
    exit(255);
  }
}

