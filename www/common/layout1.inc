
#include <tags.inc>

#include <pre-h.inc>
<table border="0" width="100%" cellspacing="0" cellpadding="0">
  <tr>
    <td><img src="$(ROOT)/img/title1.gif" bortder="0" height="60" width="500"></td>
  </tr>
  <tr>
    <td><px height="10" width="1"></td>
  </tr>
  <tr>
    <td>
      <table border="0" width="100%" cellspacing="0" cellpadding="0">
        <tr>
          <!-- BEGIN right column -->
          <td width="200" valign="top">
<img src="$(ROOT)/img/menu/top.gif" border="0"><br>
<navbar:render name="mainmenu" select="$(selected)">
<img src="$(ROOT)/img/menu/bot.gif" border="0"><br>
<dump RIGHT>
          </td>
          <!-- END   right column -->
          <td width="10"><px height="10" width="10"></td>
          <!-- BEGIN middle column -->
          <td width="100%" valign="top">
<dump MIDDLE>
          </td>
          <!-- END   middle column -->
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td><px height="10" width="1"></td>
  </tr>
  <tr>
    <td align="center"><img src="$(ROOT)/img/bar1.gif" border="0"></td>
  </tr>
  <tr>
    <td><px height="10" width="1"></td>
  </tr>
</table>
#include <post-h.inc>

