Linux-IrDA patches pending for <B>even later</B>, for 2.5.X unless
specified&nbsp;:
<UL>

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/future/ir257_stir4200-1.tgz">SigmaTel STIr4200 USB
driver</A>&nbsp;: Experimental support for the SigmaTel STIr4200 USB
based dongles. This driver is still alpha quality and you will need to
set both max_baud_rate to 115200 and max_tx_window to 1 (otherwise the
hardware will corrupt/drop packets). Original code from Paul Stewart.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/future/ir254_via_ircc-2.diff">VIA FIR driver</A>&nbsp;: driver
for the IrDA module of some VIA chipsets. This driver need more work
(PCI probe, timing loop, testing). Written by Frank Liu (VIA).

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/future/ir241_dynamic_fix-2.diff">Dynamic window fixes
(2.4.X)</A>&nbsp;: fix the dynamic window code to properly send the pf
bit - patch for 2.4.X kernel (already in 2.5.67).

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/future/irXXX_ep7211_ir.diff">ep7211_ir driver locking
fix</A>&nbsp;: fix locking in the ep7211_ir driver. Patch from Chris
Wilson.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/future/irXXX_sa1100_ir.diff">sa1100_ir driver locking
fix</A>&nbsp;: fix locking in the sa1100_ir driver. Patch from Chris
Wilson.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/future/irXXX_donauboe_comments.diff">donauboe comments
cleanup</A>&nbsp;: Patch from Pavel Machek.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/future/ir252_lap_race.diff">Fix LAP race condition</A>&nbsp;:
improve previous fix for IrLAP race condition to protect in more
cases. This patch is not totally safe and need a rewrite.

</UL>

