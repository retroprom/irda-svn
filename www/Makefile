##############################################################################
#
# $Id$
#
# Descriere: Acesta este fisierul Makefile avand chestii comune
#
##############################################################################



# secvente ANSI pentru a evidentia anumiti pasi ai instalarii
BOLD=\033\133\61\155
NORM=\033\133\155
# hack in caz ca folosesc 'locale' diferit de implicit
LANG=C
LC_ALL=C

# optiuni WML
WML        = wml
WMLDEFINE  =
WMLOPTIONS = -Idata -Iinclude $(WMLOPTIONSEXTRA)

# optiuni INSTALL
INSTALL      = install
INSTALLPLAIN = -p -D 
INSTALLDIR   = -p -d
INSTALLDATA  = -p -D -m 0644
INSTALLEXEC  = -p -D -m 0755

# programe pentru generare documentatie
POD2HTML       = pod2html
POD2TEXT       = pod2text
POD2MAN        = pod2man
POD2LATEX      = pod2latex
POD2HTMLOPTS   = --noindex --title "Bumerang"
POD2TEXTOPTS   = 
POD2MANOPTS    = --noindex --title "Bumerang"
POD2LATEXTOPTS = 
LATEX          = latex
DVIPS          = dvips
DVIPSOPTS      = -t A4
PS2PDF         = ps2pdf
FIG2DEV        = fig2dev
FIG2DEVOPTS1   = -L eps
FIG2DEVOPTS2   = -L png
                                                                                



## reguli pentru generarea de fisiere template si html din wml
$(DIR_WWW)/%.php:  %.wml
	$(WML) -o $@ $<

$(DIR_WWW)/%.tpl:  %.tpl.wml
	$(WML) -o $@ $<

$(DIR_WWW)/%.style:  %.style.wml
	$(WML) -o $@ $<
 
$(DIR_WWW)/%.html: %.wml
	$(WML) -o $@ $<

$(DIR_WWW)/%.inc: %.wml
	$(WML) -o $@ $<

## reguli pentru generarea de scripturi
% : %.in
	@sedvars="`set | grep -e '^SED_' | sed 's,SED_,,'`" ; \
	sedcommand=""; \
	for i in $$sedvars ; do \
	    var=`echo $$i | cut -d= -f 1`; \
	    val=`echo $$i | cut -d= -f 2`; \
	    sedcommand="$$sedcommand s,@$$var@,$$val,g;" ; \
	done ; \
	cat $<  | sed "$$sedcommand" > $@

## FIXME: acesta e un hack!
%.sh : %.sh.in
	@sedvars="`set | grep -e '^SED_' | sed 's,SED_,,'`" ; \
	sedcommand=""; \
	for i in $$sedvars ; do \
	    var=`echo $$i | cut -d= -f 1`; \
	    val=`echo $$i | cut -d= -f 2`; \
	    sedcommand="$$sedcommand s,@$$var@,$$val,g;" ; \
	done ; \
	cat $<  | sed "$$sedcommand" > $@

## FIXME: acesta e un hack!
%.php : %.php.in
	@sedvars="`set | grep -e '^SED_' | sed 's,SED_,,'`" ; \
	sedcommand=""; \
	for i in $$sedvars ; do \
	    var=`echo $$i | cut -d= -f 1`; \
	    val=`echo $$i | cut -d= -f 2`; \
	    sedcommand="$$sedcommand s,@$$var@,$$val,g;" ; \
	done ; \
	cat $<  | sed "$$sedcommand" > $@

## reguli pentru generarea de documentatie
%.html: %.pod
	$(POD2HTML) $(POD2HTMLOPTS) $< > $@

%.txt: %.pod
	$(POD2TEXT) $(POD2TEXTOPTS) $< > $@

%.man: %.pod
	$(POD2MAN) $(POD2TEXTOPTS) $< > $@

%.tex: %.pod
	$(POD2LATEX) $(POD2LATEXOPTS) $< 

%.dvi: %.tex
	$(LATEX) $< 

%.ps: %.dvi
	$(DVIPS) $(DVIPSOPTS) -o $@ $< 

%.pdf: %.ps
	$(PS2PDF) $< $@

%.eps: %.fig
	$(FIG2DEV) $(FIG2DEVOPTS1) $< > $@

%.png: %.fig
	$(FIG2DEV) $(FIG2DEVOPTS2) $< $@


## tintele generale
all: all-recursive all-local

install: all install-recursive install-local

uninstall: uninstall-recursive uninstall-local

clean: clean-local clean-recursive

admin: admin-recursive admin-local

admin-install: admin admin-install-recursive admin-install-local

admin-uninstall: admin-uninstall-recursive admin-uninstall-local

admin-clean: admin-clean-recursive admin-clean-local

## definiri de fisiere generate
SH_FILES    = $(SH_IN_FILES:%.sh.in=%.sh)
HTML_FILES  = $(HTML_WML_FILES:%.wml=$(DIR_WWW)/%.html)
HTML_FILES2 = $(HTML_WML_FILES2:%.wml=$(DIR_WWW)/%.html)
PHP_FILES2  = $(PHP_WML_FILES:%.wml=%.php)
STYLE_FILES = $(STYLE_WML_FILES:%.style.wml=%.style)
TPL_FILES   = $(TPL_WML_FILES:%.tpl.wml=%.tpl)
PHP_FILES   = $(PHP_IN_FILES:%.php.in=%.php)
INC_FILES   = $(INC_WML_FILES:%.wml=%.inc)
MYSQL_FILES = $(MYSQL_IN_FILES:%.mysql.in=%.mysql)
CRON_FILES  = $(CRON_IN_FILES:%.cron.in=%.cron)
HTML_DOCS   = $(POD_FILES:%.pod=%.html)
MAN_DOCS    = $(POD_FILES:%.pod=%.man)
PS_DOCS     = $(POD_FILES:%.pod=%.ps)
PDF_DOCS    = $(POD_FILES:%.pod=%.pdf)
TXT_DOCS    = $(POD_FILES:%.pod=%.txt)
EPS_DOCS    = $(FIG_FILES:%.fig=%.eps)
PNG_DOCS    = $(FIG_FILES:%.fig=%.png)

## tintele corespunzatoare fisierelor generate
sh:        $(SH_FILES)
html:      $(HTML_FILES) $(HTML_FILES2)
style:     $(STYLE_FILES)
tpl:       $(TPL_FILES)
php:       $(PHP_FILES) $(PHP_FILES2)
inc:       $(INC_FILES)
mysql:     $(MYSQL_FILES)
cron:      $(CRON_FILES)
html-docs: png-docs $(HTML_DOCS)
man-docs:  $(MAN_DOCS)
txt-docs:  $(TXT_DOCS)
ps-docs:   eps-docs $(PS_DOCS)
pdf-docs:  eps-docs $(PDF_DOCS)
eps-docs:  $(EPS_DOCS)
png-docs:  $(PNG_DOCS)

## implementarea tintelor generale
install-htdocs:
	@( test -d $(APP_DEST) ) || mkdir -p $(APP_DEST) ; \
	static='htdocs/';  \
	for infile in `find $$static` ; do \
	outfile=`echo $$infile | sed "s,^$$static,,"`; \
	if echo $$infile | grep -v -q CVS ; then  \
	   if test -d $$infile ; then \
	      mkdir -p $(APP_DEST)/$$outfile ; \
	   else \
	      cp $$infile $(APP_DEST)/$$outfile ; \
	   fi ; \
	fi ; \
	done 

uninstall-htdocs:
	@( test -d $(APP_DEST) ) || exit ; \
	static='htdocs/';  \
	for infile in `find $$static` ; do \
	outfile=`echo $$infile | sed "s,^$$static,,"`; \
	if echo $$infile | grep -v -q CVS ; then  \
	   if test -f $$infile ; then \
	      rm -f $(APP_DEST)/$$outfile ; \
	   fi ; \
	fi ; \
	done ; \
	for infile in `find $$static` ; do \
	outfile=`echo $$infile | sed "s,^$$static,,"`; \
	if echo $$infile | grep -v -q CVS ; then  \
	   if test -d $$infile ; then \
	      rmdir --ignore-fail-on-non-empty $(APP_DEST)/$$outfile ; \
	   fi ; \
	fi ; \
	done ; true


admin-install-htdocs:
	@( test -d $(ADM_DEST) ) || mkdir -p $(ADM_DEST) ; \
	static='htdocs/';  \
	for infile in `find $$static` ; do \
	outfile=`echo $$infile | sed "s,^$$static,,"`; \
	if echo $$infile | grep -v -q CVS ; then  \
	   if test -d $$infile ; then \
	      mkdir -p $(ADM_DEST)/$$outfile ; \
	   else \
	      cp $$infile $(ADM_DEST)/$$outfile ; \
	   fi ; \
	fi ; \
	done 

admin-uninstall-htdocs:
	@( test -d $(ADM_DEST) ) || exit ; \
	static='htdocs/';  \
	for infile in `find $$static` ; do \
	outfile=`echo $$infile | sed "s,^$$static,,"`; \
	if echo $$infile | grep -v -q CVS ; then  \
	   if test -f $$infile ; then \
	      rm -f $(ADM_DEST)/$$outfile ; \
	   fi ; \
	fi ; \
	done ; \
	for infile in `find $$static` ; do \
	outfile=`echo $$infile | sed "s,^$$static,,"`; \
	if echo $$infile | grep -v -q CVS ; then  \
	   if test -d $$infile ; then \
	      rmdir --ignore-fail-on-non-empty $(ADM_DEST)/$$outfile ; \
	   fi ; \
	fi ; \
	done ; true


all-recursive install-recursive uninstall-recursive clean-recursive admin-recursive admin-install-recursive admin-clean-recursive admin-uninstall-recursive:
	@target=`echo $@ | sed 's/-recursive//'`;\
	dirs='$(SUBDIRS)' ;\
	for item in $$dirs ; do \
	  cd $$item ;\
	  echo -e "$(BOLD)Execut $$target in $$item$(NORM)" ;\
	  make $$target ;\
	  cd .. ;\
	done ;


clean-html: 
	@rm -f $(HTML_FILES) $(HTML_FILES2)
clean-style: 
	@rm -f $(STYLE_FILES)
clean-tpl: 
	@rm -f $(TPL_FILES)
clean-inc: 
	@rm -f $(INC_FILES)
clean-php: 
	@rm -f $(PHP_FILES)  $(PHP_FILES2)
clean-mysql: 
	@rm -f $(MYSQL_FILES)
clean-sh: 
	@rm -f $(SH_FILES)
clean-html-docs:
	@rm -f $(HTML_DOCS)
clean-ps-docs:
	@rm -f $(PS_DOCS)
clean-pdf-docs:
	@rm -f $(PDF_DOCS)
clean-txt-docs:
	@rm -f $(TXT_DOCS)
clean-man-docs:
	@rm -f $(MAN_DOCS)
clean-eps-docs:
	@rm -f $(EPS_DOCS)
clean-png-docs:
	@rm -f $(PNG_DOCS)
clean-garbage:
	@rm -f pod2html-* *.aux *.log *.tex *.toc


wwwdirs:
	@for i in $(WWWDIRS); do \
	if  test ! -d $(DIR_WWW)/$$i ; then  \
	 mkdir -p $(DIR_WWW)/$$i ; \
	fi; \
	done

clean-dirs:
	@for i in $(WWWDIRS); do \
	if  test -d $(DIR_WWW)/$$i ; then  \
	 rmdir -p --ignore-fail-on-non-empty $(DIR_WWW)/$$i ; \
	fi; \
	done


.PHONY:  html mysql sh tpl style  php clean-sh clean-php clean-tpl clean-inc \
         clean-dirs clean-mysql all all-local all-recursive   \
         install install-recursive install-local \
         uninstall uninstall-recursive uninstall-local \
         clean clean-local clean-recursive \
         admin admin-local admin-recursive \
         admin-install admin-install-local admin-install-recursive \
         admin-uninstall admin-uninstall-local admin-uninstall-recursive \
         admin-clean   admin-clean-local   admin-clean-recursive \
         html-docs ps-docs pdf-docs txt-docs man-docs eps-docs png-docs \
         admin-uninstall-htdocs
