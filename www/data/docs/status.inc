
<h2>Linux-IrDA Current Status [August 2002]</h2>

<f2>
Here is what Jean Tourrilhes say about Linux-IrDA current status:<br>
<ul>
<li>State of the art is 2.5.13, all versions before have known
bugs. The latest essential update was in kernel 2.4.16, which
fixed various race conditions, memory leaks and endianess issues,
therefore I don't even want to hear about version prior to
2.4.16. And don't mention 2.2.X 

<li>Kernel 2.4.18 has essential fixes for IrLAN and
IrLPT. I don't normally use them, but they now seem to work.

<li>Kernel 2.5.6 is the first really usable kernel in 2.5.X
series, and also contains approximately the same Linux-IrDA stack as
2.4.19.

<li>Kernel 2.4.19 and 2.5.6 have important fixes to
IrLAP, that avoid desequenced packet. This is essential for
long communications using any higher level protocol, such as with
IrCOMM. It's highly recommended to have those fixes, so grab a
pre version.

<li>Kernel 2.5.8 fixes a bunch of race conditions in LAP,
IrCOMM and IrLAN (enable connect between two Linux),
fixes the USB driver unplug code and fixes IrNET
retries.

<li>Kernel 2.5.13 has a new socket scheduler (improve fairness
between IrDA sockets) and a partial rewrite of IrTTP (kill zillion of
bugs and race conditions, and improve performance). It also includes a
fix for when two IrNET peers connect to each other simultaneously, and
adds the mcp2120 dongle driver.

<li>Kernel 2.5.16 fixes the LSAP cache for multi-port
communications (plus minor fixes).

<li>My changes to IrTTP and the addition of the Socket scheduler (see
above) are pretty big changes. I've tested the performance of it with
irtty and nsc-ircc. You
can see that unidirectional small packet performance has increased by
a factor 3 at 4Mb/s. Also, the maximum number of Tx buffers queued in the IrDA stack decreased from 21 to 10, which improves
latency. Lastly, it seems that the TTY layer is becoming a bottleneck.

</ul>

</f2>

<h2>Pending Patches</h2>
<f2>
Linux-IrDA patches pending for <b>2.4.20</b>:<br>

<ul>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_sys_max_tx-2.diff">Max-Tx sysctl</a> : Fix a
contradiction in IrDA spec regarding max frame size, necessary for
FIR operation using IrComm or some socket applications.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir256_irnet_disc_ind_again.diff">IrNET disconnect fix</a>
: Fix IrNET to re-enable passive mode and retries (bug added in latest
kernel patches).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_trivial_fixes-3.diff">Very important fixes</a> :
fix a race condition in IrLAP, fix "unkillable" application in IrSock,
and fix transmit before ready in ircomm. <I>new version</I>.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_discovery_fixes.diff">Discovery fixes</a> : ignore
passive discoveries in IrComm and IrLAN (to avoid simultaneous
connections, mostly useful as server) and force log expiry on LAP
disconnect.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_nsc_ob6100.diff">NSC OB6100</a> : Enable the
nsc-ircc driver to recognise the FIR chip on the HP OmniBook
6100. Contributed by Kevin Thayer.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_irtty_stats.diff">irtty stat fixes</a> : increment
irtty tx counter at the right time. Contributed by Frank Becker.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_usb_disconnect_atomic-2.diff">USB disconnect fixes
for 2.5.X</a> : fix race conditions in USB disconnect code. Allow you
to unplug the dongle even with heavy transfer going on. Also include
the patch from Martin Diehl to fix usb_submit().

<li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240_mcp2120_act200l_ma600_drivers.diff">MCP2120 driver,
ActiSys 200L and MA600 driver for 2.4.X</a> : MCP2120 dongle driver
contributed by Felix Tang, ActiSys 200L dongle driver contributed by
Shimizu Takuja (patch from Gerhard Bertelsmann) and MA600 dongle
driver contributed by Leung.


</ul>

Linux-IrDA patches pending for <b>2.4.21</b>:<br>

<ul>
<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir258_flow_sched_lap_lmp-6.diff">Socket scheduler</a> :
add socket packet scheduler for fairness between socket, add LAP flow
control to minimse LAP Tx buffer queue (10 => 2). Need IrTTP fixes.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir258_flow_sched_ttp-6.diff">IrTTP big fixes</a> : fix 3
race conditions (start timer, give credit, flow ind), fix Rx queue
flow control (could become infinite), protect Rx credit update, fix
TTP flow control, implement packet scheduler, reduce Tx queue max size
(11 => 8), reduce latency (by skipping timer), optimise threshold and
flow parameters. Need socket scheduler. This seem to have fixed my
all my IrNET Oopses and weirdness...

<li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240b_dongle_locking.diff">Fix dongle autoloading</a> : 
avoid loading dongle modules with irq disabled in irtty.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240b_lsap_lap_close-2.diff">Minor fixes</a> :
Always close LAP on LSAP closure, disable LAP timer on accept socket, and 
header fix for Alpha (by Felix Tang).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir258_irnet_simult_race-2.diff">Simultaneous IrNET
race</a> : fix race condition when the two peer simultaneously create
an IrNET connection to each other (thanks to Martin Diehl for
reporting this race). As opposed to the IrCOMM/IrLAN discovery
workaround, this fixes the race properly and cleanly.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240b_smc_msg.diff">SMC small fixes</a> :
Fix SMC driver removal code and various messages. Patch from Jeff Snyder.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240b_expiry_fix.diff">Discovery expiry fix</a> :
Make discovery expiry work properly for non default discovery
period/timeout.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240b_donauboe.diff">Donauboe driver</a> :
New driver for Toshiba IrDA chip. Patch from Christian Gennerat &amp; Martin
Lucina.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.4.x/ir240b_checker.diff">Four small bugs in /proc</a> :
Fix four cases where interrupts would not be re-enabled on
failure. Found by the Stanford checker.

</ul>


Other Linux-IrDA patches included in <b>2.5.13</b> and pending <b>for 2.4.22</b>:<br>

<ul>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir258_wait_event_fixes-2.diff">Fix races waiting for events</a> :
Use proper logic to wait for events instead of obsoleted
kernel calls, to avoid race conditions. Most of those races were
theoretical or had workaround. Original patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ircomm_export.diff">IrCOMM export rules for 2.5.X</a> :
Add proper export rules to IrCOMM modules to make it work properly in
kernel 2.5.10 and later. Patch from Kai Germaschews.

</ul>


Other Linux-IrDA patches included in <b>2.5.16</b> and pending <b>for 2.4.22</b>:<br>

<ul>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_long_set_bit.diff">set_bit fixes</a> :
Change all arguments of set_long to "unsigned long". Patch from Paul Mackerras.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_lsap_cache_fix.diff">LSAP cache fix</a> :
Replace
the global LSAP cache with LSAP caches private to each LAP. Fix a bug
where two simultaneous connections from two devices using the same
LSAPs would get mixed up. Should also improve performance in similar
cases. Patch from Christoph Bartelmus.

</ul>



Linux-IrDA patches pending for <b>2.5.24</b> and <b>2.4.22</b>:<br>

<ul>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_usb_cleanup-2.diff">USB driver cleanup</a>&nbsp;:
Handle properly failure of USB with new speed settings, and various
other cleanups.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_headers_init-2.diff">Init &amp; Headers cleanup for
2.5.X</a> : Cleanup the header includes (should speed up
dependancies &amp; compile) and module init (should enable static
compiles). <i>new version for 2.5.22</i> (use "patch -l" if needed).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_cache_wait_data-2.diff">LSAP cache and socket
receive fixes</a> : fix one instance were we forgot to clear LSAP
cache, and fix a bogus conversion to wait_event() where the socket
closure would never propagate to the app waiting on a socket. This
fixes bugs introduced in 2.5.13 and 2.5.16. <i>new version for
2.5.22</i> (use "patch -l" if needed).

</ul>


Linux-IrDA patches pending for <b>2.5.30</b> and <b>2.4.22</b>:<br>

<ul>
<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_hashbin_fixes.diff">HashBins fixes</a>&nbsp;: fix
kernel crash after removal of IAS object or IAS attribute with
duplicated name, simplify LMP registration to avoid using random
generator in static init, and make all hashbin uses 64 bit safe.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/vlsi_ir-2.5.24-v0.4-patch">VLSI driver v0.4</a>&nbsp;: 
major update of the VLSI driver. Patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir255_donauboe.diff">Donauboe driver</a>&nbsp;: 
new driver for Toshiba IrDA chip. Patch from Christian Gennerat & Martin Lucina

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir255_checker.diff">Two small bugs in /proc</a>&nbsp;: 
fix two cases where interrupts would not be re-enabled on
failure. Found by the Stanford checker

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir255_nsc_speed-4.diff">NSC speed fix</a>&nbsp;: 
try to fix the NSC driver so that it changes speed back to 9600 properly

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir255_nsc_speed_again-2.diff">NSC speed fix (part 2)</a> : 
Fix my previous patch that contained serious errors
(which amazingly didn't prevent the driver to work). Thanks to Martin
Diehl to force me to think harder at the whole thing.


<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir255_export_crc-2.diff">Export CRC-16 routine</a> : 
Export to drivers the routine doing CRC calculations. Original patch
from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir_ali_cleanup.diff">ALI driver cleanup</a> : 
Remove compilation warning. Patch from "sandeen".

</ul>


</f2>
