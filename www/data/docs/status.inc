Willing or not, it seem that I'm the one responsible of making sure
the Linux-IrDA stack work properly. Here is a quick status report...

<UL>
<LI>The archive for the old mailing list at <A
HREF="http://www.pasta.cs.uit.no/pipermail/linux-irda/">pasta</A> is
erratic, please use the new mailing list at <A
HREF="http://sourceforge.net/mail/?group_id=5616">SourceForge</A>
(otherwise I may not be able to read your e-mail).

<LI>It is my belief that <I>all known issue</I> regarding the IrDA
stack are <I>resolved</I> (I'm not talking here about drivers and
installation, which are know to be problematic). Don't prove me
wrong&nbsp;;-)

<LI>What's the <B>latest version</B>&nbsp;? Things are always
evolving, so I recommend to get a very recent kernel 2.4.X (like the
latest one) and apply some patches from this page. Note the the
development of Linux-IrDA in 2.4.X is <B>stopped</B>, and I will no
longer accept major patches for it, development is now happening only
in <B>2.5.X</B>, see below.

<LI>The latest essential update of 2.4.X was in kernel <B>2.4.21</B>,
which fixed various race conditions and connection issues, therefore I
don't even want to hear about version prior to <B>2.4.21</B>. And
don't mention 2.2.X&nbsp;;-)

<LI>Kernel <B>2.4.18</B> has essential fixes for <B>IrLAN</B> and
<B>IrLPT</B>. I don't normally use them, but they now seem to
work.

<LI>Kernel <B>2.4.19</B> has important fixes to <B>IrLAP</B>, that
avoid desequenced packet. This is essential for long communications
using any higher level protocol, such as with <I>IrCOMM</I>.

<LI>Kernel <B>2.4.20</B> fixes a bunch of race conditions in LAP,
<I>IrCOMM</I> and <I>IrLAN</I> (enable Linux as IrCOMM/IrLAN server),
fixes the <I>USB</I> driver unplug code, fixes <I>IrNET</I> retries
and add various new <I>dongle</I> drivers (mcp2120, act200l & ma600).

<LI>Kernel <B>2.4.21</B> has a new socket scheduler (improve fairness
between IrDA sockets) and a partial rewrite of IrTTP (kill zillion of
bugs and race conditions, and improve performance). It also includes a
fix for when two IrNET peers connect to each other simultaneously, and
adds the Donauboe FIR driver.

<LI>Kernel <B>2.4.22</B> fixes some IrCOMM problems with chat, add
workarounds for interoperability with buggy devices and fix a common
memory leak. This is the last Linux-IrDA update for 2.4.X, no more
updates are planned.

<LI>The IrDA stack in <B>2.5.X</B> contains a large number of fixes
and improvement over what is available in 2.4.X (list below), and this
is where new development is happening so you are welcome to try
it. Usually later version are better.

<LI>Kernel <B>2.5.28</B> broke the IrDA stack, and that was only
repaired in <B>2.5.39</B> with a massive patch to fix SMP
locking. Also included were fix to avoid duplicated hashbin entries
and fix on NSC change speed.

<LI>Kernel <B>2.5.42</B> fixes some IrCOMM problems, adds the Donauboe
driver and update the VLSI driver. Kernel <B>2.5.47</B> adds the new
IrTTY driver (to replace the old one which is now broken - not all
dongle drivers are converted yet). Kernel <B>2.5.61</B> features a
partial rewrite of the SIR wrapper (smaller & faster). Kernel
<B>2.5.67</B> fix locking in the discovery code and improve the
performance at SIR. Kernel <B>2.5.70</B> has some fixes for
<B>irtty-sir</B> and <B>irport</B>, add the new <B>smsc2-ircc</B>
driver and fix a large number of sbk memory leaks.

<LI>My changes to IrTTP and the addition of the Socket scheduler (see
above) are pretty big changes. I've tested the performance of it with
<A HREF="http://www.hpl.hp.com/personal/Jean_Tourrilhes/IrDA/IrDA.html/perf.txt">irtty</A> and 
<A HREF="http://www.hpl.hp.com/personal/Jean_Tourrilhes/IrDA/IrDA.html/perf4.txt">nsc-ircc</A>. You
can see that unidirectional small packet performance has increased by
a factor 3 at 4Mb/s. Also, the maximum number of Tx buffers queued in
the IrDA stack decreased from 21 to 10, which improves
latency. Lastly, it seems that the TTY layer is becoming a bottleneck.

<LI>The new <B>irtty-sir</B> driver in kernel <B>2.5.X</B> replace the
old irtty driver. It fixes some architectural issue in the interaction
with the serial driver. The downside is that all <B>dongle drivers</B>
need to be updated to the new dongle infrastructure. Martin has
<A HREF="http://www.hpl.hp.com/personal/Jean_Tourrilhes/IrDA/IrDA.html/irda_dongle_api.html">documented that</A>.

<LI>The <B>SMC</B> driver will soon be replaced with the <A
HREF="http://www.csai.unipa.it/peri/smsc-ircc2/">new SMC
driver</A>. Please help testing it.

<LI>The <B>toshoboe</B> driver will soon be replaced with the <A
HREF="http://libxg.free.fr/irda/lib-irda.html">donauboe
driver</A>. Please help testing it.

<LI>The <B>irport</B> driver is in need of a maintainer. Not me.

<LI><B>irda-utils 0.9.15</B> is out. Don't use anything older than
that. I don't want to look at bug reports using old irdadump, because
they don't show some common problems, I really want <B>irdadump
0.9.15</B>.

</UL>

