Linux-IrDA patches included in <B>2.4.20-pre2</B>&nbsp;:
<UL>

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_sys_max_tx-2.diff">Max-Tx sysctl</A> : Fix a
contradiction in IrDA spec regarding max frame size, necessary for
FIR operation using IrComm or some socket applications.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_irnet_disc_ind_again.diff">IrNET disconnect fix</A>
: Fix IrNET to re-enable passive mode and retries (bug added in latest
kernel patches).

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_trivial_fixes-3.diff">Very important fixes</A> :
fix a race condition in IrLAP, fix "unkillable" application in IrSock,
and fix transmit before ready in ircomm.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_discovery_fixes.diff">Discovery fixes</A> : ignore
passive discoveries in IrComm and IrLAN (to avoid simultaneous
connections, mostly useful as server) and force log expiry on LAP
disconnect.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_nsc_ob6100.diff">NSC OB6100</A> : Enable the
nsc-ircc driver to recognise the FIR chip on the HP OmniBook
6100. Contributed by Kevin Thayer.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_irtty_stats.diff">irtty stat fixes</A> : increment
irtty tx counter at the right time. Contributed by Frank Becker.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_usb_disconnect-2.diff">USB disconnect fixes</A> :
fix race conditions in USB disconnect code. Allow you to unplug the
dongle even with heavy transfer going on.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_expiry_fix.diff">Discovery expiry fix</A>&nbsp;:
Make discovery expiry work properly for non default discovery
period/timeout.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_mcp2120_act200l_ma600_drivers-2.diff">MCP2120
driver, ActiSys 200L and MA600 driver for 2.4.X</A> : MCP2120 dongle
driver contributed by Felix Tang, ActiSys 200L dongle driver
contributed by Shimizu Takuja (patch from Gerhard Bertelsmann) and
MA600 dongle driver contributed by Leung.

</UL>

<p>
Linux-IrDA patches included in <B>2.4.21-pre3</B>&nbsp;:
<UL>

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_flow_sched_lap_lmp-6.diff">Socket scheduler</A> :
add socket packet scheduler for fairness between socket, add LAP flow
control to minimse LAP Tx buffer queue (10 => 2). Need IrTTP fixes.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_flow_sched_ttp-6.diff">IrTTP big fixes</A> : fix
3 race conditions (start timer, give credit, flow ind), fix Rx queue
flow control (could become infinite), protect Rx credit update, fix
TTP flow control, implement packet scheduler, reduce Tx queue max size
(11 => 8), reduce latency (by skipping timer), optimise threshold and
flow parameters. Need socket scheduler. This seem to have fixed my all
my IrNET Oopses and weirdness...

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_dongle_locking.diff">Fix dongle autoloading</A> :
avoid loading dongle modules with irq disabled in irtty.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_lsap_lap_close-2.diff">Minor fixes</A> : Always
close LAP on LSAP closure, disable LAP timer on accept socket, and
header fix for Alpha (by Felix Tang).

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_irnet_simult_race-2.diff">Simultaneous IrNET
race</A> : fix race condition when the two peer simultaneously create
an IrNET connection to each other (thanks to Martin Diehl for
reporting this race). As opposed to the IrCOMM/IrLAN discovery
workaround, this fixes the race properly and cleanly.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_smc_msg.diff">SMC small fixes</A> : Fix SMC
driver removal code and various messages. Patch from Jeff Snyder.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_donauboe-2.diff">Donauboe driver</A>&nbsp;: new
driver for Toshiba IrDA chip. Patch from Christian Gennerat & Martin
Lucina. <I>Updated version</I>.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_checker.diff">Four small bugs in /proc</A>&nbsp;:
fix four cases where interrupts would not be re-enabled on
failure. Found by the Stanford checker.

</UL>

<p>
Linux-IrDA patches included in <B>2.4.22-pre2</B>&nbsp;:
<UL>

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_ircomm_uninit_fix-6.diff">Fixes chat over IrCOMM
problem</A>&nbsp;: workaround a bug in the kernel (TTY layer)
triggered by chat brokeness by accepting data before IrCOMM is
initialised. Also remove the IrCOMM-IAS entry to accept multiple
incomming connections.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_qos_param-2.diff">Handle invalid QoS
parameter</A>&nbsp;: handle better case where the peer feed us invalid
or oversized parameters.  Also add various warning so that users knows
when the IrDA peer is buggy (and doesn't start blaming
Linux-IrDA). Also add a new sysctl, <B>max_tx_window</B>.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_lmp_timer_race-2.diff">LMP timer race
condition</A>&nbsp;: fix a race condition when starting the IrLMP
watchdog timer, and improve the debugging messages.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_export_crc-3.diff">Export CRC-16 routine</A>&nbsp;:
export to drivers the routine doing CRC calculations. Original patch
from Martin Diehl.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_secondary_rr.diff">Fix secondary nack
code</A>&nbsp;: fix the secondary function to send RR and frames
without the poll bit when it detect packet losses.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_iriap_skb_leak.diff">IrIAP SKB leak</A>&nbsp;: fix
an obvious SKB leak in IrIAP. This is only a localised fix, and the
same fix need to be applied to all Linux-IrDA state machines. Original
patch from Jan Kiszka.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_usb_cleanup-4.diff">USB driver cleanup</A>&nbsp;:
Handle properly failure of URB with new speed settings, fix endianess
and various other cleanups.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_caddr_mask.diff">caddr fix</A>&nbsp;: ignore the
C/R bit in the LAP connection address. Patch from Jan Kiszka.

<LI><li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir241_static_init.diff">static driver fix</A>&nbsp;: fix
some obvious static init bugs.

</UL>

