
RELDIR=../

HTML_WML_FILES= index.wml home.wml news.wml sitemap.wml   \
                about.wml credits.wml download.wml        \
                apps.wml irda-utils/index.wml smcinit/index.wml                        \
                resources.wml companies.wml chipsets.wml dongles.wml devices.wml       \
                docs/index.wml docs/index2.wml docs/index3.wml docs/index4.wml         \
                docs/developer.wml docs/status.wml                                     \
                contact.wml


COMMON = $(RELDIR)common/tags.inc       \
         $(RELDIR)common/layout1.inc    \
         $(RELDIR)common/pre-h.inc      \
         $(RELDIR)common/post-h.inc     \
         $(RELDIR)common/menu.inc       \
         $(RELDIR)data/inside-head.inc  \
         $(RELDIR)data/inside-bottom.inc 

include $(RELDIR)Makefile.cfg
include $(RELDIR)Makefile

SUBDIRS = 

WWWDIRS = img docs irda-utils smcinit 

all-local: wwwdirs html
 
clean-local:  clean-html clean-dirs

$(HTML_FILES): $(COMMON)

$(RELDIR)html/home.html:       $(RELDIR)common/layout2.inc $(RELDIR)common/news.pl $(RELDIR)data/news/*.data
$(RELDIR)html/news.html:       $(RELDIR)common/layout2.inc $(RELDIR)common/news.pl $(RELDIR)data/news/*.data
$(RELDIR)html/sitemap.html:    $(RELDIR)common/layout2.inc
$(RELDIR)html/about.html:      $(RELDIR)common/layout2.inc
$(RELDIR)html/download.html:   $(RELDIR)common/layout2.inc

$(RELDIR)html/apps.html:              $(RELDIR)common/layout2.inc
$(RELDIR)html/smcinit/index.html:     $(RELDIR)common/layout2.inc
$(RELDIR)html/irda-utils/index.html:  $(RELDIR)common/layout2.inc

$(RELDIR)html/credits.html:    $(RELDIR)data/credits.inc

$(RELDIR)html/contact.html:    $(RELDIR)data/contact.inc

$(RELDIR)html/resources.html:  $(RELDIR)data/resources.inc $(RELDIR)common/layout2.inc
$(RELDIR)html/companies.html:  $(RELDIR)data/companies.inc
$(RELDIR)html/chipsets.html:   $(RELDIR)data/chipsets/*.data $(RELDIR)/common/chipsets.pl
$(RELDIR)html/dongles.html:    $(RELDIR)data/dongles.inc
$(RELDIR)html/devices.html:    $(RELDIR)data/devices.inc


$(RELDIR)html/docs/index.html: $(RELDIR)data/docs/index.inc
$(RELDIR)html/docs/index2.html: $(RELDIR)data/docs/index2.inc
$(RELDIR)html/docs/index3.html: $(RELDIR)data/docs/index3.inc
$(RELDIR)html/docs/index4.html: $(RELDIR)data/docs/index4.inc
$(RELDIR)html/docs/developer.html: $(RELDIR)data/docs/developer.inc
$(RELDIR)html/docs/status.html: $(RELDIR)data/docs/status.inc

