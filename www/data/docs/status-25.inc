
Other Linux-IrDA patches included in <B>2.5.13</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir258_wait_event_fixes-2.diff">Fix races waiting for
events</a> : use proper logic to wait for events instead of obsoleted
kernel calls, to avoid race conditions. Most of those races were
theoretical or had workaround. Original patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ircomm_export.diff">IrCOMM export rules for 2.5.X</a> :
Add proper export rules to IrCOMM modules to make it work properly in
kernel 2.5.10 and later. Patch from Kai Germaschews.

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.16</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_long_set_bit.diff">set_bit fixes</a> : change all
arguments of set_long to "unsigned long". Patch from Paul Mackerras.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_lsap_cache_fix.diff">LSAP cache fix</a> : replace
the global LSAP cache with LSAP caches private to each LAP. Fix a bug
where two simultaneous connections from two devices using the same
LSAPs would get mixed up. Should also improve performance in similar
cases. Patch from Christoph Bartelmus.

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.24</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_usb_cleanup-2.diff">USB driver cleanup</a>&nbsp;:
Handle properly failure of URB with new speed settings, and various
other cleanups.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_headers_init-2.diff">Init & Headers cleanup for
2.5.X</a>&nbsp;: Cleanup the header includes (should speed up
dependancies & compile) and module init (should enable static
compiles). <I>new version for 2.5.22</I> (use "patch -l" if needed).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_cache_wait_data-2.diff">LSAP cache and socket
receive fixes</a>&nbsp;: fix one instance were we forgot to clear LSAP
cache, and fix a bogus conversion to wait_event() where the socket
closure would never propagate to the app waiting on a socket. This
fixes bugs introduced in 2.5.13 and 2.5.16. Also reserve enough skb
header space in IrSOCK so that drivers don't complain. <I>new version
for 2.5.22</I> (use "patch -l" if needed).

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.39</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_hashbin_fixes-4.diff">HashBins fixes</a>&nbsp;: fix
kernel crash after removal of IAS object or IAS attribute with
duplicated name, simplify LMP registration to avoid using random
generator in static init, and make all hashbin uses 64 bit safe.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_lap_unique_saddr.diff">Unique LAP
address</a>&nbsp;: Make sure LAP address is sane, which mean not NULL,
not BROADCAST and not already in use by another link.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_nsc_speed_both.diff">NSC speed fix</a>&nbsp;: try
to fix the NSC driver so that it changes speed back to 9600
properly. Aggregate of both original patches.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_hashbin_locking_fixes-4.diff">Hashbin locking
fixes</a>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after two
first patches above.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_ircomm_locking_fixes-4.diff">IrCOMM/IrLAN locking
fixes</a>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after
locking hashbin fixes.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_drivers_locking_fixes-3.diff">Driver locking
fixes</a>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after
locking hashbin fixes & nsc speed patch.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_ali_locking_fixes-3.diff">ALI locking
fixes</a>&nbsp;: Fix locking for 2.5.32. Final patch. Apply after
locking hashbin fixes.

</UL>

<p>
Other Linux-IrDA patches included in <B>2.5.42</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_vlsi_v4-2.diff">VLSI driver v0.4</a>&nbsp;: major
update of the VLSI driver. Patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_donauboe-2.diff">Donauboe driver</a>&nbsp;: new
driver for Toshiba IrDA chip. Patch from Christian Gennerat & Martin
Lucina. <I>Updated version</I>.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_export_crc-2.diff">Export CRC-16 routine</a>&nbsp;:
export to drivers the routine doing CRC calculations. Original patch
from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_ircomm_uninit_fix-6.diff">Fixes chat over IrCOMM
problem</a>&nbsp;: workaround a bug in the kernel (TTY layer)
triggered by chat brokeness by accepting data before IrCOMM is
initialised. Also remove the IrCOMM-IAS entry to accept multiple
incomming connections.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_qos_param-2.diff">Handle invalid QoS
parameter</a>&nbsp;: handle better case where the peer feed us invalid
or oversized parameters.  Also add various warning so that users knows
when the IrDA peer is buggy (and doesn't start blaming
Linux-IrDA). Also add a new sysctl, <B>max_tx_window</B>. <I>new
patch, complete fix</I>.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir251_lmp_timer_race-2.diff">LMP timer race
condition</a>&nbsp;: fix a race condition when starting the IrLMP
watchdog timer, and improve the debugging messages.

</UL>


<p>
Other Linux-IrDA patches included in <B>2.5.47</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_packet_type.diff">Fix packet type</a>&nbsp;:
migrate the IrDA stack to the multithreaded receive path of the
networking stack and cleanup the packet type definition. This should
fix an annoying warning on SMP.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_donauboe_init.diff">Fix donauboe init</a>&nbsp;:
fix donauboe driver function names to avoid clashes with toshoboe
driver.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_irtty-sir-1.diff">New irtty driver
(2.5.44)</a>&nbsp;: new IrTTY driver to go with the new serial driver
in 2.5.X. Original patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir255_irtty-sir-1.diff">New irtty driver
(2.5.45)</a>&nbsp;: new IrTTY driver to go with the new serial driver
in 2.5.X. Patch from Martin Diehl.

</UL>

<p>
Linux-IrDA patches included in <B>2.5.61</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_driver_module_fixes-2.diff">IrDA driver module
fixes</a>&nbsp;: Use SET_MODULE_OWNER() in various IrDA drivers.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_new_wrapper-3.diff">Partial rewrite of the SIR
wrapper</a>&nbsp;: optimise the SIR wrapper (stuff/unstuff bytes) for
better performance. Will only decrease the CPU overhead, not the
network overhead. <I>Improved version</I>. Also add the optional
ZeroCopy-Rx mechanism for drivers that support it (like this <a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir256_sir-dev_wrapper-3.diff">experimental patch for
sir-dev</a>).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_secondary_rr.diff">Fix secondary nack
code</a>&nbsp;: fix the secondary function to send RR and frames
without the poll bit when it detect packet losses.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir254_ircomm_dce.diff">IrCOMM status line fix</a>&nbsp;: fix
status line initialisation in IrCOMM. Patch from Jan Kiszka.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_namespace_iriap.diff">IrIAP namespace
cleanup</a>&nbsp;: reduce namespace pollution in IrIAP. Patch from
Arnd Bergmann.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_namespace_qos.diff">IrQoS namespace
cleanup</a>&nbsp;: reduce namespace pollution in IrDA QoS. Patch from
Arnd Bergmann.

</UL>

<p>
Linux-IrDA patches included in <B>2.5.67</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_discovery_locking-2.diff">Discovery fixes and
cleanup</a>&nbsp;: fix the last locking problems in discovery and
expiry, simplify, cleanup and optimise the discovery/expiry code
(i.e. smaller and faster).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_dynamic_fix-2.diff">Dynamic window fixes</a>&nbsp;:
fix the dynamic window code to properly send the pf bit. Up to 40%
improvement from large packets at SIR (i.e. most bulk transfer at
SIR). Performance unchanged for FIR or small packet. More complete
performance analysis later on.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_usb_rx_skb-2.diff">IrDA-USB cleanups</a>&nbsp;:
cleanup the Rx path, the USB registration and others in the IrDA-USB
driver.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_sir-dev_wrapper-4.diff">Sir-dev ZeroCopy</a>&nbsp;:
Enable ZeroCopy Rx in irtty-sir (provided by the new SIR wrapper).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_timer_mod.diff">Minor timer optimisation</a>&nbsp;:
make IrDA timers use mod_timer (safer & faster).

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir253_irnet_mod_hints-2.diff">IrNET update</a>&nbsp;: Fix
module refcounting and add hints to discovery.

</UL>

<p>
Linux-IrDA patches included in <B>2.5.70</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_skb_get-7.diff">skb leak audit</a>&nbsp;: Fix many
real and potential skb (packet buffer) memory leaks in the IrDA
stack. This patch is so good that over time the number of skb used
decrease&nbsp;;-) Thanks to Jan Kiszka for pointing out the issue.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_irnet_bh.diff">IrNET context fix</a>&nbsp;: fix
IrNET locking to be compatible with PPP, fix a couple of crash in
2.5.67+.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_caddr_mask.diff">caddr fix</a>&nbsp;: ignore the
C/R bit in the LAP connection address. Patch from Jan Kiszka.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_sir_kthread_Morton-2.diff">sir_kthread
comment</a>&nbsp;: add comment to explain task state, new module stuff
in irtty-sir.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_irport-6.diff">irport fixes #3</a>&nbsp;: various
fixes and optimisation for irport.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_smsc-ircc2-6.diff">New SMSC driver #6</a>&nbsp;:
new SMSC driver, this one works, even in FIR (but still need
testing). Original patch from Daniele Peri. <I>Improved version, for
2.5.69</I>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_trans_start-4.diff">trans_start fixes</a>&nbsp;:
minor fix in various drivers.

</UL>

<p>
Linux-IrDA patches pending for <B>2.5.75</B>&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir259_irda-usb_endian.diff">IrDA-USB endian
fixes</a>&nbsp;: Trivial fix for irda-usb to make it work on big
endian boxes (such as PPC). Original patch from Jacek Jakubowski.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_nsc_39x-1.diff">NSC '39x support</a>&nbsp;: Add
preliminary support for NSC PC8739x chipset, such as found in IBM R31
laptops. Original patch from Jan Frey.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_via_ircc-1.diff">VIA FIR driver</a>&nbsp;: driver
for the IrDA module of some VIA chipsets. Written by Joseph Chan.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_include_cleanup.diff">Header cleanup</a>&nbsp;:
cleanup the &lt;linux/irda.h&gt; header.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_irnet_cast.diff">IrNET cast fix</a>&nbsp;: remove
pointer casting in IrNET debug code. Patch suggested by David
S. Miller.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_irttp_leak.diff">More skb leaks</a>&nbsp;: fix two
additional potential skb leaks in IrTTP. Found by the Stanford
checker.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_setup_dma_fix-2.diff">setup_dma fix</a>&nbsp;: mark
all drivers that need setup_dma() as requiring CONFIG_ISA.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir250_struct_check.diff">Verify structs</a>&nbsp;: verify
the size of packed structures. Patch suggested by Russell King.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irda_sir_cleanup-2.5.70.diff">sir-dev cleanup</a>&nbsp;:
various cleanups in irtty-sir and sir-dev drivers. Patch from Martin
Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_vlsi-05.diff">vlsi driver update</a>&nbsp;: update
VLSI driver to version 0.5 (kernel 2.4.73). Patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_vlsi-05.diff">vlsi driver update</a>&nbsp;: update
VLSI driver to version 0.5 (kernel 2.4.74). Patch from Martin Diehl.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_frigo.diff">refrigerator support</a>&nbsp;: support
software suspend in irtty-sir. Patch from Neil Brown.

</UL>

<p>
Linux-IrDA patches pending for <B>even later</B>, for 2.5.X unless
specified&nbsp;:
<UL>

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_stir4200-1.tgz">SigmaTel STIr4200 USB
driver</a>&nbsp;: Experimental support for the SigmaTel STIr4200 USB
based dongles. This driver is still alpha quality and you will need to
set both max_baud_rate to 115200 and max_tx_window to 1. Original code
from Paul Stewart.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir241_dynamic_fix-2.diff">Dynamic window fixes
(2.4.X)</a>&nbsp;: fix the dynamic window code to properly send the pf
bit - patch for 2.4.X kernel.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_ep7211_ir.diff">ep7211_ir driver locking
fix</a>&nbsp;: fix locking in the ep7211_ir driver. Patch from Chris
Wilson.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_sa1100_ir.diff">sa1100_ir driver locking
fix</a>&nbsp;: fix locking in the sa1100_ir driver. Patch from Chris
Wilson.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/irXXX_donauboe_comments.diff">donauboe comments
cleanup</a>&nbsp;: Patch from Pavel Machek.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir257_tty_api_fix.diff">TTY API fix</a>&nbsp;: Add an
extra call to the TTY API so that irtty-sir can work again.

<li><a href="http://irda.sourceforge.net/ftp/patches/2.5.x/ir252_lap_race.diff">Fix LAP race condition</a>&nbsp;:
improve previous fix for IrLAP race condition to protect in more
cases. This patch is not totally safe and need a rewrite.

</UL>

