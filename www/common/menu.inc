
<navbar:define name="mainmenu" imgstar="std:sel:ovr">
  <navbar:header><table border="0" cellpadding="0" cellspacing="0" border="0"><tr></navbar:header>
  <navbar:footer></tr></table></navbar:footer>

  <navbar:prolog><td></navbar:prolog>
  <navbar:epilog pos=1><img src="$(ROOT)img/part5.png" width="2" height="30"></td></navbar:epilog>
  <navbar:epilog pos=2><img src="$(ROOT)img/part6.png" width="2" height="30"></td></navbar:epilog>
  <navbar:epilog pos=3><img src="$(ROOT)img/part5.png" width="2" height="30"></td></navbar:epilog>
  <navbar:epilog pos=4><img src="$(ROOT)img/part5.png" width="2" height="30"></td></navbar:epilog>
  <navbar:epilog pos=5></td></navbar:epilog>

  <navbar:button id="about"      txt="About"          img="$(ROOT)img/about-*.gif"       url="$(ROOT)about.html">
  <navbar:button id="contact"    txt="Contact"        img="$(ROOT)img/contact-*.gif"     url="$(ROOT)contact.html">
  <navbar:button id="docs"       txt="Documentation"  img="$(ROOT)img/docs-*.gif"        url="$(ROOT)docs/index.html">
  <navbar:button id="download"   txt="Download"       img="$(ROOT)img/download-*.gif"    url="$(ROOT)download.html">
  <navbar:button id="resources"  txt="Resources"      img="$(ROOT)img/resources-*.gif"   url="$(ROOT)resources.html">
</navbar:define>


<navbar:define name="mainmenu2" imgstar="std:sel:ovr">
  <navbar:header></navbar:header>
  <navbar:footer></navbar:footer>

  <navbar:prolog><tr><td></navbar:prolog>
  <navbar:epilog pos=1><br><img src="$(ROOT)img/i2.png" width="100" height="5"></td></tr></navbar:epilog>
  <navbar:epilog pos=2><br><img src="$(ROOT)img/i2.png" width="100" height="5"></td></tr></navbar:epilog>
  <navbar:epilog pos=3><br><img src="$(ROOT)img/i2.png" width="100" height="5"></td></tr></navbar:epilog>
  <navbar:epilog pos=4><br><img src="$(ROOT)img/i2.png" width="100" height="5"></td></tr></navbar:epilog>
  <navbar:epilog pos=5><br><img src="$(ROOT)img/i2.png" width="100" height="5"></td></tr></navbar:epilog>
  <navbar:epilog pos=6><br><img src="$(ROOT)img/i2.png" width="100" height="5"></td></tr></navbar:epilog>
  <navbar:epilog pos=7></td></tr></navbar:epilog>

  <navbar:button id="home"       txt="Home"           img="$(ROOT)img/home-*.gif"        url="$(ROOT)home.html">
  <navbar:button id="about"      txt="About"          img="$(ROOT)img/about-*.gif"       url="$(ROOT)about.html">
  <navbar:button id="contact"    txt="Contact"        img="$(ROOT)img/contact-*.gif"     url="$(ROOT)contact.html">
  <navbar:button id="docs"       txt="Documentation"  img="$(ROOT)img/docs-*.gif"        url="$(ROOT)docs/index.html">
  <navbar:button id="apps"       txt="Applications"   img="$(ROOT)img/apps-*.gif"        url="$(ROOT)apps.html">
  <navbar:button id="download"   txt="Download"       img="$(ROOT)img/download-*.gif"    url="$(ROOT)download.html">
  <navbar:button id="resources"  txt="Resources"      img="$(ROOT)img/resources-*.gif"   url="$(ROOT)resources.html">
</navbar:define>



<navbar:define name="tmainmenu">
  <navbar:header></navbar:header>
  <navbar:footer></navbar:footer>

  <navbar:prolog></navbar:prolog>
  <navbar:epilog>&nbsp;|&nbsp;</navbar:epilog>
  <navbar:epilog pos=7>&nbsp;</navbar:epilog>

  <navbar:button id="home"       txt="Home"           url="$(ROOT)home.html">
  <navbar:button id="about"      txt="About"          url="$(ROOT)about.html">
  <navbar:button id="contact"    txt="Contact"        url="$(ROOT)contact.html">
  <navbar:button id="docs"       txt="Documentation"  url="$(ROOT)docs/index.html">
  <navbar:button id="apps"       txt="Applications"   url="$(ROOT)apps.html">
  <navbar:button id="download"   txt="Download"       url="$(ROOT)download.html">
  <navbar:button id="resources"  txt="Resources"      url="$(ROOT)resources.html">
</navbar:define>


############################################################################
#             S U B M E N U S
############################################################################

<navbar:define name="aboutmenu" imgstar="std:sel:ovr">
  <navbar:header><table border="0" cellpadding="0" cellspacing="0" border="0"></navbar:header>
  <navbar:footer></table></navbar:footer>

  <navbar:prolog><tr><td></navbar:prolog>
  <navbar:epilog></td></tr></navbar:epilog>

  <navbar:button id="about"      txt="About"          img="$(ROOT)img/about/menu0-*.gif"     url="$(ROOT)about.html">
  <navbar:button id="credits"    txt="Credits"        img="$(ROOT)img/about/menu1-*.gif"     url="$(ROOT)credits.html">
</navbar:define>

<navbar:define name="docsmenu" imgstar="std:sel:ovr">
  <navbar:header><table border="0" cellpadding="0" cellspacing="0" border="0"></navbar:header>
  <navbar:footer></table></navbar:footer>

  <navbar:prolog><tr><td></navbar:prolog>
  <navbar:epilog></td></tr></navbar:epilog>

  <navbar:button id="docs"      txt="Documentation"  img="$(ROOT)img/docs/menu0-*.gif"     url="$(ROOT)docs/index.html">
  <navbar:button id="developer" txt="Developer"      img="$(ROOT)img/docs/menu1-*.gif"     url="$(ROOT)docs/developer.html">
  <navbar:button id="status"    txt="Status"         img="$(ROOT)img/docs/menu3-*.gif"     url="$(ROOT)docs/status.html">
</navbar:define>


<navbar:define name="resourcesmenu" imgstar="std:sel:ovr">
  <navbar:header><table border="0" cellpadding="0" cellspacing="0" border="0"></navbar:header>
  <navbar:footer></table></navbar:footer>

  <navbar:prolog><tr><td></navbar:prolog>
  <navbar:epilog></td></tr></navbar:epilog>

  <navbar:button id="resources"  txt="Resources"      img="$(ROOT)img/resources/menu0-*.gif"     url="$(ROOT)resources.html">
  <navbar:button id="companies"  txt="Companies"      img="$(ROOT)img/resources/menu1-*.gif"     url="$(ROOT)companies.html">
  <navbar:button id="chipsets"   txt="Chipsets"       img="$(ROOT)img/resources/menu2-*.gif"     url="$(ROOT)chipsets.html">
  <navbar:button id="dongles"    txt="Dongles"        img="$(ROOT)img/resources/menu3-*.gif"     url="$(ROOT)dongles.html">
  <navbar:button id="devices"    txt="Devices"        img="$(ROOT)img/resources/menu4-*.gif"     url="$(ROOT)devices.html">
</navbar:define>


<navbar:define name="docs2menu" imgstar="std:sel:ovr">
  <navbar:header></navbar:header>
  <navbar:footer></navbar:footer>

  <navbar:prolog></navbar:prolog>
  <navbar:epilog>&nbsp;|&nbsp;</navbar:epilog>
  <navbar:epilog pos=4>&nbsp;</navbar:epilog>

  <navbar:button id="intro"     txt="Intro"          url="$(ROOT)docs/index.html">
  <navbar:button id="starting"  txt="Starting IrDA"  url="$(ROOT)docs/startirda.html">
  <navbar:button id="comm"      txt="Communicating"  url="$(ROOT)docs/communicating.html">
  <navbar:button id="advanced"  txt="Advanced"       url="$(ROOT)docs/advanced.html">
</navbar:define>


<navbar:define name="homemenu" imgstar="std:sel:ovr">
  <navbar:header><table border="0" cellpadding="0" cellspacing="0" border="0"></navbar:header>
  <navbar:footer></table></navbar:footer>

  <navbar:prolog><tr><td></navbar:prolog>
  <navbar:epilog></td></tr></navbar:epilog>

  <navbar:button id="home"      txt="Home"      img="$(ROOT)img/home/menu0-*.gif"     url="$(ROOT)home.html">
  <navbar:button id="news"      txt="News"      img="$(ROOT)img/home/menu1-*.gif"     url="$(ROOT)news.html">
  <navbar:button id="sitemap"   txt="Site Map"  img="$(ROOT)img/home/menu2-*.gif"     url="$(ROOT)sitemap.html">
  <navbar:button id="splash"    txt="Splash"    img="$(ROOT)img/home/menu3-*.gif"     url="$(ROOT)index.html">
</navbar:define>

<navbar:define name="appsmenu" imgstar="std:sel:ovr">
  <navbar:header><table border="0" cellpadding="0" cellspacing="0" border="0"></navbar:header>
  <navbar:footer></table></navbar:footer>

  <navbar:prolog><tr><td></navbar:prolog>
  <navbar:epilog></td></tr></navbar:epilog>

  <navbar:button id="home"        txt="Home"       img="$(ROOT)img/apps/menu0-*.gif"     url="$(ROOT)apps.html">
  <navbar:button id="irdautils"   txt="Irda-utils" img="$(ROOT)img/apps/menu1-*.gif"     url="$(ROOT)irda-utils/index.html">
  <navbar:button id="smcinit"     txt="Smcinit"    img="$(ROOT)img/apps/menu2-*.gif"     url="$(ROOT)smcinit/index.html">
</navbar:define>



