
#use wml::std::tags
#use wml::std::case global=lower
#use wml::des::navbar
#use wml::des::gfont

#include <menu.inc>

# define current website maintainer email address
<define-tag webmaster>\
<protect><a href="mailto:claudiuc@geocities.com?subject=Suggestions%20about%20Linux-IrDA%20website">webmaster</a></protect>\
</define-tag>

<define-tag yes>\
<img src="$(ROOT)img/yes.gif" width="16" height="13" align="bottom">\
</define-tag>

<define-tag gear>\
<img src="$(ROOT)img/gear.gif" width="13" height="13" align="bottom">\
</define-tag>


<define-tag f2 endtag=required>\
<font face="helvetica,arial" size="2">%body</font>\
</define-tag>

<define-tag f1 endtag=required>\
<font face="helvetica,arial" size="1">%body</font>\
</define-tag>

<define-tag red endtag=required>\
<font color="red">%body</font>\
</define-tag>

<define-tag green endtag=required>\
<font color="green">%body</font>\
</define-tag>

<define-tag blue endtag=required>\
<font color="blue">%body</font>\
</define-tag>


#<define-tag head1 endtag=required>\
#<gfont face="Helvetica" size="24">%body</gfont>\
#</define-tag>

<define-tag head1 endtag=required>\
<h1 align="left" style="font-size: 24pt; font-family: helvetica,arial,sans-serif; color:#606060;">%body</h1>\
</define-tag>

<define-tag head2 endtag=required>\
<h2 align="left" style="font-size: 20pt; font-family: helvetica,arial,sans-serif; color:#808080;">%body</h2>\
</define-tag>


<define-tag px>
<preserve height />\
<preserve width />\
<set-var %attributes>\
<img src="$(ROOT)img/pix.gif" width="<get-var width />" height="<get-var height />">\
<restore width />\
<restore height />\
</define-tag>

<define-tag t0>\
<px width="16" height="16" alt="">\
</define-tag>

<define-tag t1>\
<img src="$(ROOT)img/tree1.gif" width="16" height="16" alt="">\
</define-tag>

<define-tag t2>\
<img src="$(ROOT)img/tree2.gif" width="16" height="16" alt="">\
</define-tag>

<define-tag t3>\
<img src="$(ROOT)img/tree3.gif" width="16" height="16" alt="">\
</define-tag>

<define-tag t4>\
<img src="$(ROOT)img/tree4.gif" width="16" height="16" alt="">\
</define-tag>

<define-tag t5>\
<img src="$(ROOT)img/tree5.gif" width="16" height="16" alt="">\
</define-tag>

