Linux-IrDA patches included in <B>2.6.0-test1</B>&nbsp;:
<UL>

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir254_irda-usb_endian-2.diff">IrDA-USB endian
fixes</A>&nbsp;: Trivial fix for irda-usb to make it work on big
endian boxes (such as PPC). Original patch from Jacek Jakubowski. Add
new USB device ID.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir254_nsc_39x-1.diff">NSC '39x support</A>&nbsp;: Add
preliminary support for NSC PC8739x chipset, such as found in IBM
R31/A30/A31 laptops. This patch needs more work. Original patch from
Jan Frey.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir250_include_cleanup.diff">Header cleanup</A>&nbsp;:
cleanup the &lt;linux/irda.h&gt; header.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir250_irnet_cast.diff">IrNET cast fix</A>&nbsp;: remove
pointer casting in IrNET debug code. Patch suggested by David
S. Miller.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir250_irttp_leak.diff">More skb leaks</A>&nbsp;: fix two
additional potential skb leaks in IrTTP. Found by the Stanford
checker.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir254_setup_dma_fix-3.diff">setup_dma fix</A>&nbsp;: mark
all drivers that need setup_dma() as requiring CONFIG_ISA. Mark new
dongle driver as needing sir-dev.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir250_struct_check.diff">Verify structs</A>&nbsp;: verify
the size of packed structures. Patch suggested by Russell King.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir254_ircomm_devfs.diff">IrCOMM devfs fix</A>&nbsp;: make
sure that the devfs name for IrCOMM devices are not bogus. Original
patch suggested by Martin Diehl.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/irda_sir_cleanup-2.5.70.diff">sir-dev cleanup</A>&nbsp;:
various cleanups in irtty-sir and sir-dev drivers. Patch from Martin
Diehl.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/irXXX_frigo.diff">refrigerator support</A>&nbsp;: support
software suspend in irtty-sir. Patch from Neil Brown.

</UL>

<p>
Linux-IrDA patches pending for <B>2.6.0-test5</B>&nbsp;:
<UL>

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/irXXX_vlsi-05.diff">vlsi driver update</A>&nbsp;: update
VLSI driver to version 0.5 (kernel 2.4.73). Patch from Martin Diehl.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/irXXX_vlsi-05.diff">vlsi driver update</A>&nbsp;: update
VLSI driver to version 0.5 (kernel 2.4.74). Patch from Martin Diehl.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir260_donau_cleanup.diff">Donauboe probe</A>&nbsp;:
disable the probe in Donauboe driver by default and minor
cleanups. Patch from Christian Gennerat.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir260_usb_probe-2.diff">irda-usb probe</A>&nbsp;: minor
fix to the probe failure path of irda-usb. Patch from Oliver Neukum.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.6.x/ir260_lap_retry_count.diff">IrLAP retry count</A>&nbsp;:
add interoperability workaround for a bug in 2.4.X IrDA stacks : 2.4.X
secondary forget to set pf bit when reaching window byte capacity
without sending the max number of packets (ex: 115 kb/s, max packet
size, window=7), and we would eventually close the connection. This
bug was fixed in 2.5.67 (Dynamic window fixes).

</UL>
