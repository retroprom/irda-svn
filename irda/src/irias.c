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


void irias_help() {
  fprintf(stdout,
    "Usage: %s ias list\n"
    "       %s ias get [ DEST ] class CLASS attrib ATTRIB\n"
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

void irias_list_entry(struct irda_ias_set *query) {
  const char *charset[] = { "ASCII", "ISO-8859-1"};

  fprintf(stdout,"Class: %s\nAttribute: %s\n"
          ,query->irda_class_name,query->irda_attrib_name);
  switch (query->irda_attrib_type) {
    case IAS_MISSING:
    break;
    case IAS_INTEGER:
    break;
    case IAS_OCT_SEQ:
    break;
    case IAS_STRING:
      fprintf(stdout,"Attribute Type: String (length=%d, charset=%s)\n",
             query->attribute.irda_attrib_string.len,
             charset[query->attribute.irda_attrib_string.charset]);
      fprintf(stdout,"Value: \"%s\"\n",query->attribute.irda_attrib_string.string);
    break;
    default:
      fprintf(stderr,"%s: Invalid IAS attribute type received from kernel.",PACKAGE_NAME);
      exit(1);
  }
  
}


void irias_get() {
  struct irda_ias_set query;
  int ias_daddr, ret, is_local=1, fd, len=sizeof(struct irda_ias_set);
  char *ias_class=0, *ias_attrib=0;
 
  /* get the options */
  arg_index++;
  while (arg_index<m_argc) {
    if (!strncmp("lo",m_argv[arg_index],2)) {
      is_local=1;
    } else if (!strncmp("re",m_argv[arg_index],2)) {
      is_local=0;
      if (arg_index+1==m_argc) {
        irias_no_complete();
        if (ias_class) free(ias_class);
        if (ias_attrib) free(ias_attrib);
        exit(255);
      } else {
        arg_index++;
        if (sscanf(m_argv[arg_index],"%X",&ias_daddr) != 1) {
          irias_err_daddr(m_argv[arg_index]);
          if (ias_class) free(ias_class);
          if (ias_attrib) free(ias_attrib);
          exit(1);
        }
      }
    } else if (!strncmp("cl",m_argv[arg_index],2)) {
      if (arg_index+1==m_argc) {
        irias_no_complete();
        if (ias_attrib) free(ias_attrib);
        exit(255);
      } else {
        arg_index++;
        ias_class=strndup(m_argv[arg_index],IAS_EXPORT_CLASSNAME);
        if (!ias_class) {
          perror(PACKAGE_NAME);
          if (ias_attrib) free(ias_attrib);
          exit(errno);
        }
       }
    } else if (!strncmp("at",m_argv[arg_index],2)) {
      if (arg_index+1==m_argc) {
        irias_no_complete();
        if (ias_class) free(ias_class);
        exit(255);
      } else {
        arg_index++;
        ias_attrib=strndup(m_argv[arg_index],IAS_EXPORT_ATTRIBNAME);
        if (!ias_attrib) {
          perror(PACKAGE_NAME);
          if (ias_class) free(ias_class);
          exit(errno);
        }
      }
    } else {
      irias_no_option(m_argv[arg_index]);
      if (ias_class) free(ias_class);
      if (ias_attrib) free(ias_attrib);
      exit(255);
    }
    arg_index++;
  }

  if (ias_class==0) {
    irias_no_iasclass();
    exit(255);
  }
  if (ias_attrib==0) {
    irias_no_iasattrib();
    exit(255);
  }

  strncpy(query.irda_class_name,ias_class,IAS_EXPORT_CLASSNAME);
  strncpy(query.irda_attrib_name,ias_attrib,IAS_EXPORT_ATTRIBNAME);
  if (ias_class) free(ias_class);
  if (ias_attrib) free(ias_attrib);
  
  fd=socket(AF_IRDA,SOCK_STREAM,0);
  if (fd<0) {
    perror(PACKAGE_NAME);
    exit(errno);
  }

  if (is_local) {
    ret=getsockopt(fd,SOL_IRLMP,IRLMP_IAS_GET,&query,&len);
  } else {
    query.daddr=ias_daddr;
    ret=getsockopt(fd,SOL_IRLMP,IRLMP_IAS_QUERY,&query,&len);
  }
  if (ret) {
    perror(PACKAGE_NAME);
    exit(1);
  } 
  
  irias_list_entry(&query);  
}

void irias_set() {
}

void irias_list() {
  printf("mumu\n");
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

