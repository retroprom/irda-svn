Other Linux-IrDA patches included in <B>2.5.13</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir258_wait_event_fixes-2.diff">Fix races waiting for
events</A> : use proper logic to wait for events instead of obsoleted
kernel calls, to avoid race conditions. Most of those races were
theoretical or had workaround. Original patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ircomm_export.diff">IrCOMM export rules for 2.5.X</A> :
Add proper export rules to IrCOMM modules to make it work properly in
kernel 2.5.10 and later. Patch from Kai Germaschews.

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.16</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_long_set_bit.diff">set_bit fixes</A> : change all
arguments of set_long to "unsigned long". Patch from Paul Mackerras.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_lsap_cache_fix.diff">LSAP cache fix</A> : replace
the global LSAP cache with LSAP caches private to each LAP. Fix a bug
where two simultaneous connections from two devices using the same
LSAPs would get mixed up. Should also improve performance in similar
cases. Patch from Christoph Bartelmus.

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.24</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_usb_cleanup-2.diff">USB driver cleanup</A>&nbsp;:
Handle properly failure of URB with new speed settings, and various
other cleanups.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_headers_init-2.diff">Init & Headers cleanup for
2.5.X</A>&nbsp;: Cleanup the header includes (should speed up
dependancies & compile) and module init (should enable static
compiles). <I>new version for 2.5.22</I> (use "patch -l" if needed).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_cache_wait_data-2.diff">LSAP cache and socket
receive fixes</A>&nbsp;: fix one instance were we forgot to clear LSAP
cache, and fix a bogus conversion to wait_event() where the socket
closure would never propagate to the app waiting on a socket. This
fixes bugs introduced in 2.5.13 and 2.5.16. Also reserve enough skb
header space in IrSOCK so that drivers don't complain. <I>new version
for 2.5.22</I> (use "patch -l" if needed).

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.39</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_hashbin_fixes-4.diff">HashBins fixes</A>&nbsp;: fix
kernel crash after removal of IAS object or IAS attribute with
duplicated name, simplify LMP registration to avoid using random
generator in static init, and make all hashbin uses 64 bit safe.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_lap_unique_saddr.diff">Unique LAP
address</A>&nbsp;: Make sure LAP address is sane, which mean not NULL,
not BROADCAST and not already in use by another link.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_nsc_speed_both.diff">NSC speed fix</A>&nbsp;: try
to fix the NSC driver so that it changes speed back to 9600
properly. Aggregate of both original patches.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_hashbin_locking_fixes-4.diff">Hashbin locking
fixes</A>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after two
first patches above.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_ircomm_locking_fixes-4.diff">IrCOMM/IrLAN locking
fixes</A>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after
locking hashbin fixes.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_drivers_locking_fixes-3.diff">Driver locking
fixes</A>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after
locking hashbin fixes & nsc speed patch.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_ali_locking_fixes-3.diff">ALI locking
fixes</A>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after
locking hashbin fixes.

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.42</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_vlsi_v4-2.diff">VLSI driver v0.4</A>&nbsp;: major
update of the VLSI driver. Patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_donauboe-2.diff">Donauboe driver</A>&nbsp;: new
driver for Toshiba IrDA chip. Patch from Christian Gennerat & Martin
Lucina. <I>Updated version</I>.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_export_crc-2.diff">Export CRC-16 routine</A>&nbsp;:
export to drivers the routine doing CRC calculations. Original patch
from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_ircomm_uninit_fix-6.diff">Fixes chat over IrCOMM
problem</A>&nbsp;: workaround a bug in the kernel (TTY layer)
triggered by chat brokeness by accepting data before IrCOMM is
initialised. Also remove the IrCOMM-IAS entry to accept multiple
incomming connections.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_qos_param-2.diff">Handle invalid QoS
parameter</A>&nbsp;: handle better case where the peer feed us invalid
or oversized parameters.  Also add various warning so that users knows
when the IrDA peer is buggy (and doesn't start blaming
Linux-IrDA). Also add a new sysctl, <B>max_tx_window</B>. <I>new
patch, complete fix</I>.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_lmp_timer_race-2.diff">LMP timer race
condition</A>&nbsp;: fix a race condition when starting the IrLMP
watchdog timer, and improve the debugging messages.

</UL>


<p>
Other Linux-IrDA patches included in <B>2.5.47</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_packet_type.diff">Fix packet type</A>&nbsp;:
migrate the IrDA stack to the multithreaded receive path of the
networking stack and cleanup the packet type definition. This should
fix an annoying warning on SMP.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_donauboe_init.diff">Fix donauboe init</A>&nbsp;:
fix donauboe driver function names to avoid clashes with toshoboe
driver.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_irtty-sir-1.diff">New irtty driver
(2.5.44)</A>&nbsp;: new IrTTY driver to go with the new serial driver
in 2.5.X. Original patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir255_irtty-sir-1.diff">New irtty driver
(2.5.45)</A>&nbsp;: new IrTTY driver to go with the new serial driver
in 2.5.X. Patch from Martin Diehl.

</UL>

<p>
Linux-IrDA patches included in <B>2.5.61</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_driver_module_fixes-2.diff">IrDA driver module
fixes</A>&nbsp;: Use SET_MODULE_OWNER() in various IrDA drivers.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_new_wrapper-3.diff">Partial rewrite of the SIR
wrapper</A>&nbsp;: optimise the SIR wrapper (stuff/unstuff bytes) for
better performance. Will only decrease the CPU overhead, not the
network overhead. <I>Improved version</I>. Also add the optional
ZeroCopy-Rx mechanism for drivers that support it (like this <a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir256_sir-dev_wrapper-3.diff">experimental patch for
sir-dev</A>).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_secondary_rr.diff">Fix secondary nack
code</A>&nbsp;: fix the secondary function to send RR and frames
without the poll bit when it detect packet losses.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_ircomm_dce.diff">IrCOMM status line fix</A>&nbsp;: fix
status line initialisation in IrCOMM. Patch from Jan Kiszka.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_namespace_iriap.diff">IrIAP namespace
cleanup</A>&nbsp;: reduce namespace pollution in IrIAP. Patch from
Arnd Bergmann.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_namespace_qos.diff">IrQoS namespace
cleanup</A>&nbsp;: reduce namespace pollution in IrDA QoS. Patch from
Arnd Bergmann.

</UL>

<p>
Linux-IrDA patches included in <B>2.5.67</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_discovery_locking-2.diff">Discovery fixes and
cleanup</A>&nbsp;: fix the last locking problems in discovery and
expiry, simplify, cleanup and optimise the discovery/expiry code
(i.e. smaller and faster).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_dynamic_fix-2.diff">Dynamic window fixes</A>&nbsp;:
fix the dynamic window code to properly send the pf bit. Up to 40%
improvement from large packets at SIR (i.e. most bulk transfer at
SIR). Performance unchanged for FIR or small packet. More complete
performance analysis later on.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_usb_rx_skb-2.diff">IrDA-USB cleanups</A>&nbsp;:
cleanup the Rx path, the USB registration and others in the IrDA-USB
driver.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_sir-dev_wrapper-4.diff">Sir-dev ZeroCopy</A>&nbsp;:
Enable ZeroCopy Rx in irtty-sir (provided by the new SIR wrapper).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_timer_mod.diff">Minor timer optimisation</A>&nbsp;:
make IrDA timers use mod_timer (safer & faster).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_irnet_mod_hints-2.diff">IrNET update</A>&nbsp;: Fix
module refcounting and add hints to discovery.

</UL>

<p>
Linux-IrDA patches included in <B>2.5.70</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_skb_get-7.diff">skb leak audit</A>&nbsp;: Fix many
real and potential skb (packet buffer) memory leaks in the IrDA
stack. This patch is so good that over time the number of skb used
decrease&nbsp;;-) Thanks to Jan Kiszka for pointing out the issue.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_irnet_bh.diff">IrNET context fix</A>&nbsp;: fix
IrNET locking to be compatible with PPP, fix a couple of crash in
2.5.67+.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_caddr_mask.diff">caddr fix</A>&nbsp;: ignore the
C/R bit in the LAP connection address. Patch from Jan Kiszka.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_sir_kthread_Morton-2.diff">sir_kthread
comment</A>&nbsp;: add comment to explain task state, new module stuff
in irtty-sir.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_irport-6.diff">irport fixes #3</A>&nbsp;: various
fixes and optimisation for irport.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_smsc-ircc2-6.diff">New SMSC driver #6</A>&nbsp;:
new SMSC driver, this one works, even in FIR (but still need
testing). Original patch from Daniele Peri. <I>Improved version, for
2.5.69</I>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_trans_start-4.diff">trans_start fixes</A>&nbsp;:
minor fix in various drivers.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_tty_api_fix.diff">TTY API fix</A>&nbsp;: Add an
extra call to the TTY API so that irtty-sir can work again. Russell
eventually implemented something similar.

</UL>

