<br>
<small>
<navbar:render name="docs2menu" select="advanced"><br>
</small>

<h2>Advanced Linux-IrDA</h2>

<p>
This tutorial was written by Jean Tourrilhes and adapted by me, Claudiu
Costin. Currently any major Linux distribution have already IrDA support,
tools to manage it and some utilities to communicate with various compatible
IrDA devices. The following documentation is for die hard kernel &amp;
software hackers which want to be on bleeding edge and even to contribute 
to Linux-IrDA project in general.



<h3>Understanding IrDA</h3>

<p>
A lot of IrDA novices mix up the low level and high level of the IrDA
stack. A few words...

<ul>
	<li><b>Low level of Linux-IrDA</B> : this is the part of the IrDA
	stack dealing with the IrDA hardware on your computer and managing
	it. The keywords are <i>IrDA driver</i>, <i>IrDA ports</i> (either
	<i>SIR</i>, <i>MIR</i> or <i>FIR</i>), <i>IrDA dongles</i> and 
	<i>TTY/serial port</i>.

	<li><b>High level of Linux-IrDA</b> : this is the part of the
	IrDA stack dealing with communicating with the other IrDA devices and
	exchanging the data (the protocol). The keywords are <i>protocol</i>,
	<i>IrCOMM</i>, <i>IrLPT</i>, <i>OBEX</i>, <i>IrLAN</i> and
	<i>IrNET</i>.
</ul>

The low level and high level are totally independent of each other,
however each need to be configured properly for what you want to do.
The procedure to get IrDA working looks usually like this:

<ul>
	<li><a href="#config">Compilation and configuration</a>
	<li><a href="#driver">Set up the low level to recognise your IrDA hardware</a>
	<li><a href="#checking">Verify that the low level works</a>
	<li><a href="#apps">Set up your application on top of the proper high
	level protocol</a>
</ul>

I also offer various <a href="#debug">debugging tips</a> at the end of
this document.


<a name="config">
<h3>Compilation and configuration</h3>
</a>

Here are the steps to build your own kernel with IrDA paramaters of your 
choice.

<ul>
	<li>Get kernel of your choice, unpack the source in <tt>/usr/src</tt>
	or where you feel have enough space to compile it.
	<li>To do this configure the IrDA stack as a module, enable all options (as
	module if possible) and the driver/protocols you need (as modules).
	Prefferably all drivers because don't hurt.
	You can configure serial port as a module and static is ok as well, except
	for the NSC or FIR setup. You're warned.
	<li>Compile kernel iamge, modules and install them:
<pre>&gt; make bzImage
&gt; make modules
&gt; make modules_install</pre>
	<li>Install <a href="http://irda.sourceforge.net/ftp/irda-utils/irda-utils-0.9.15.tar.gz">irda-utils-0.9.15</a> or later
	<li>Add the following stuff in <tt>/etc/modules.conf</tt>:
<pre>
# IrDA stuff...
alias tty-ldisc-11 irtty
alias char-major-161 ircomm-tty  # if you want IrCOMM support
\# These values are hard-coded in irattach (not instance order)
alias irda-dongle-0 tekram       # Tekram IrMate IR-210B
alias irda-dongle-1 esi          # ESI JetEye
alias irda-dongle-2 actisys      # Actisys IR-220L
alias irda-dongle-3 actisys      # Actisys IR-220L+
alias irda-dongle-4 girbil       # Greenwich GIrBIL
alias irda-dongle-5 litelink     # Parallax LiteLink
alias irda-dongle-6 airport      # Adaptec Airport 1000 and 2000
\# IrNET module...
alias char-major-10-187 irnet    # Official allocation of IrNET</pre>
	<li>Create the IrDA devices:
<pre>&gt; mknod /dev/ircomm0 c 161 0
&gt; mknod /dev/ircomm1 c 161 1
&gt; mknod /dev/irlpt0 c 161 16
&gt; mknod /dev/irlpt1 c 161 17
&gt; mknod /dev/irnet c 10 187
&gt; chmod 664 /dev/ir*</pre>
	<li>Edit <tt>/etc/lilo.conf</tt> if you use LILO bootloader or
	<tt>/boot/grub/menu.lst</tt> if you use GRUB bootloader and add
	your new compiled kernel. Then reboot the machine.
</ul>

Now, it's time to check which hardware you want to make run...

<a name="driver">
<h3>Low level drivers</h3>
</a>

This really depend on the IrDA hardware that you have. I describe a
few of the options below. The two safest options are 
Laptop in SIR mode and Serial dongle.


<h4>Serial dongle</h4>
<p>
The dongle I use is the Actisys 220L+, and the dongle driver is called
<u>actisys</u> (see list above). The setup for other dongles should be
very similar. I'm also using the first serial port in this example
(ttyS0), you may need to adapt to your case.

<ul>
	<li>Attach dongle to the serial port, and check which serial port it is.
	<li>Check if the serial config is ok:
<pre>&gt; setserial /dev/ttyS0
/dev/ttyS0, UART: 16550A, Port: 0x03f8, IRQ: 4</pre>
	<li>Start the IrDA stack:
<pre>&gt; irattach /dev/ttyS0 -d actisys -s</pre>
	<li>If you want to unload/stop the IrDA stack just kill all the IrDA
	applications and do:
<pre>&gt; killall irattach
&gt; rmmod irtty actisys
&gt; rmmod irda</pre>
</ul>

Note: all the modern ESI dongles work better with the <u>litelink</u> driver.

<h4>Laptop port in SIR mode</h4>
<p>
SIR (Serial Infrared) is not fast but always work and is easy to
set-up, so it's a safe bet. It will work <b>only if the BIOS is set to
SIR mode</b>, so don't bother otherwise.

<ul>
	<li>Go in the BIOS of the laptop, enable the Infrared port, and set it
	to SIR mode.
	<li>While in the BIOS, note the IO address and IRQ assigned to it.
	<li>Using <b>setserial</bb>, find the ttyS* that correspond to this
	port. Let's assume <tt>/dev/ttyS1</tt> (as in my laptop).
	<li>Start the IrDA stack:
<pre>&gt; irattach /dev/ttyS1 -s</pre>
	<li>Refer to previous example for more
</ul>

Now, you just need to figure out on which side of the laptop if the
IrDA port...


<h4>HP Omnibook 6000 in FIR mode</h4>
<p>
It seems that each laptop has its quirk when it come to FIR mode. I've
managed to get my OB6000 to work. Other laptops
will be different (different driver, different settings).

<ul>
	<li>Go in the BIOS of the laptop, enable the Infrared port, and set it
	to FIR mode.
	<li>Add the following stuff in <tt>/etc/modules.conf</tt>:
<pre># NSC FIR chipset in the OB6000
alias irda0 nsc-ircc
options nsc-ircc dongle_id=0x08</pre>
	<li>Remove the serial driver that gets in the way:
<pre>&gt; setserial /dev/ttyS1 uart none
&gt; rmmod serial</pre>
	<li>Start the IrDA stack:
<pre>&gt; irattach irda0 -s</pre>
</ul>

The NSC driver gives me some pretty good performance.

<h4>Other laptops in FIR mode</h4>
<p>
There is <b>different FIR hardwares</b> included in the various
laptops. Linux-IrDA support some of them (not all) in various degrees
(from good to bad). Moreover, it seems that each laptop has its quirk,
so it's difficult to list everything here.
For this reason, I recommend to make it work first in <b>SIR
mode</b>. After that, you can experiement, check the HowTo and query
the mailing list...
As a rule of thumb, the <i>NSC</i> driver seems to be the most
functional (if you set the proper <i>dongle_id</i>, which is either
<tt>0x8</tt> or <tt>0x9</tt>), and the <i>SMC</i> driver the 
most problematic.

<h4>USB FIR dongles</h4>
<p>
This driver is included in recent kernel. It's not as efficient as
other FIR hardware, but at least is supported and is relatively easy
to get working. Also, all the current products are based on the same
hardware, and we know most of its bugs.
The latest version of the driver has been tested with <i>usb-uhci</i>
and <i>usb-ohci</i>.

<ul>
	<li>Start the USB stack. If you have an UHCI hardware, it looks like:
<pre>&gt; modprobe usb-uhci</pre>
	<li>Load USB driver and start the IrDA stack:
<pre>&gt; modprobe irda-usb
&gt; irattach irda0 -s</pre>
</ul>

If you have already some other IrDA hardware configured on the PC, the
driver won't load as <i>irda0</i>, so check the message log with
<b>dmesg</b>. Also, the driver can manage up to 4 IrDA-USB dongles per
PC (that can be increased in the source).<br>
<br>
Recently a new type of USB dongle from <b>SigmaTel</b> has appeared on
the market which is <b>not</b> compliant with the IrDA-USB
specification, and therefore doesn't work with this driver. On the
other hand, <b>SigmaTel</b> has made available the <a
href="http://www.sigmatel.com/technical_docs.htm#wireless">full
technical specification</a>, so writing a driver for it is possible.<br>
<br>
<b>Important note</b>: in recent kernels, the USB team has added
a driver called <b>ir-usb</b>. Not only this driver is <b>not</b>
compatible with the IrDA stack (the IrDA driver is called
<b>irda-usb</b>), but this driver will load automatically before
<b>irda-usb</b>, therefore preventing you to use
it. <b>Solution</b>: get rid of <b>ir-usb</b>.

<h4>SIR with irport</h4>

The standard SIR driver is <u>irtty</u>, which uses the standard
serial driver and TTY layer. This is the easiest and safest way to get
IrDA working.
However, the TTY layer adds some overhead and doesn't understand the
IrDA protocol, which make it unsuitable in some case (dongle without
echo cancelation) and less performant in others (small packets). That
is why there is a second driver, <u>irport</u>, which allow the IrDA
stack direct access to the serial port.
Unfortunately, the procedure to use <u>irport</u> is more complicated
and less well tested. Actually, I personally never managed to make
irport work reliably on any of my systems.

<ul>
	<li>Remove the serial driver that gets in the way:
<pre>&gt; setserial /dev/ttyS0 uart none
&gt; rmmod serial</pre>
	<li>Load the irport driver and attach the dongle driver.
<pre>&gt; modprobe irport io=0x3f8 irq=4
&gt; dongle_attach irda0 -d actisys+</pre>
	<li>Start the IrDA stack:
<pre>&gt; irattach irda0 -s</pre>
</ul>

<h3>Checking that it work</h3>

The first test is to check if the <b>discovery</b> is happening
properly. If the IrDA driver is properly configured, the Linux-IrDA
will discover other IrDA devices in range. If the discover doesn't
work, this indicate that the low level is not configured properly (and
you don't need to go any further).
You can check if there is any device listed in the discovery log
with:
<pre>&gt; cat /proc/net/irda/discovery 
IrLMP: Discovery log:
nickname: Jean Tourrilhes, hint: 0x8220, saddr: 0x913b1bbc, daddr: 0x5619b45e</pre>

You can also check various other files in <tt>/proc</tt>, or use
<b>irdadump</b>, check the <a href="#debug">debugging section</a>.<br>
<br>
Then, you might want to use a simple aplication, such as
<b>e-Squirt</b> to verify that everything works fine. Or you can skip
directly to the next section.
The big advantage of e-Squirt is that it is a really simple protocol,
doesn't stress the IrDA stack too much and we have implementation for
<a href="http://www.cooltown.hp.com/code.htm">various platforms</a>,
so that you can test your setup with almost anything on the other side
(Linux, Win32, WinCE or Palm).
Compile the <a href="http://www.hpl.hp.com/personal/Jean_Tourrilhes/IrDA/squirt.html">Linux e-Squirt library</a>
and the test programs on all Linux computers, and go in the <tt>tests</tt>
directory. On other platforms, load and start the relevant the
e-Squirt application.
If you want to use Linux as a <i>receiver</i>, just do:
<pre>&gt; ./squirt</pre>
To use Linux as a <i>sender</i>, you can do:
<pre>&gt; ./ultra_beacon http://cooltown.hp.com/
&gt; ./socket_squirt http://cooltown.hp.com/</pre>
With that, you should be able to exchange back and forth URLs and
check that your IrDA stack works. If not, continue to read below.
One caveat: Most implementations have two exclusive receiving
modes, IrDA and Ultra, and they switch between these (either as a
preference setting, or automatically triggerd by discovery
packets). Linux is an exception and can listen to both at the same
time. This means that unless you do a Linux-Linux test, <b>only one of
the two sender tests</b> listed above will work properly.


<a name="apps">
<h3>Apps and protocols on top of the IrDA stack</h3>
</a>

If you want to run e-Squirt applications, you are done, and you just
need to run the application themselves, they should work.
Other applications and protocols you may want to run&nbsp;:

<ul>
	<li><a href="http://www.hpl.hp.com/personal/Jean_Tourrilhes/IrDA/IrNET.html">TCP/IP over IrNET</a> (external link)
	<li><a href="#irterm">Terminal over IrCOMM</a>
	<li><a href="#irphone">Connect to a mobile phones</a>
	<li><a href="#irlan">TCP/IP over IrLAN</a>
	<li><a href="http://sourceforge.net/projects/openobex/">Obex</a> (external link) to
	exchange files/objects with PDAs, Phones and other Obex devices
</ul>

Note that I don't use IrCOMM and IrLAN, so I can't help much with that...

<a name="irterm">
<h4>Terminal over IrComm</h4>
</a>

This is a simple test to check that <b>IrComm</b> is working between
two PCs. After that, you can try more complex applications such as
<b>PPP</b>.<br>
<b>Server side</b>: Start the terminal server
<pre>&gt; getty ircomm0 DT19200 vt100	 # Red-Hat syntax</pre>
or
<pre>&gt; getty -L ircomm0 19200 vt100  # Debian syntax</pre>
At this point, your text terminal should get reset and you
come back to a login prompt. That's normal. I don't know what happen
in X.<br>
<br>
<b>Client side with kermit</b>: Start the terminal emulator
<pre>&gt; kermit
&gt; &gt; set line /dev/ircomm0
&gt; &gt; set speed 19200
&gt; &gt; connect
&gt; stty sane	# Get backspace to work ok
</pre>
The prompt shouls appear after <i>connect</i>. Also, you need to
ignore the following message: "<i>Warning: no access to tty
(Inappropriate ioctl for device).  Thus, no job control enabled</i>",
and "<i>Can't open terminal /dev/tty</i>".<br>
<br>
<b>Client side with minicom</b>: Minicom is a bit more problematic, and 
I'm still fighting with it. I still don't understand how to connect. 
I managed to make it work like this:

<ul>
	<li>start minicom
	<li>Configure (Ctrl+A O)
	<li>sub-menu serial
	<li>set to <tt>/dev/ircomm0</tt>
	<li>set speed to 19200
	<li>exit
	<li>sub-menu "save as dft"
	<li>exit
	<li>Exit (Ctrl+A X)
	<li>restart minicom
</ul>

<a name="irphone">
<h4>IrDA and mobile phones</h4>
</a>

I don't have any <i>mobile phone</i>, and I don't use <i>IrCOMM</i>,
so I can't help...
There is many people using this feature, and some of them have put
instructions in their web pages. The people doing <a
href="http://gnokii.org/">Gnokii</a> are also quite knowledgeable in
this area, so you may ask advice on their mailing list, but please
report IrDA bugs in the IrDA mailing lists.
If I can't reproduce your problem, I can't debug it, so I can't fix
it. If I can't see anything obvious in the <b>irdadump</b> log, I
won't bother. You may also want to try to reproduce the problem
<i>between two Linux boxes</i> (because I may be able to reproduce
that).

<a name="irlan">
<h4>TCP/IP over IrLAN</h4>
</a>

I don't use <b>IrLAN</b> any longer, because I'm only using
<b>IrNET</b>. I just did a refresh on the original instructions that I
sent on the mailing list (removing mentions of irmanager).
IrLAN as an access option, which can be 1 (direct mode), 2 (peer to
peer) and 3 (hosted). Basically, you would use 2 if you connect to
another PC, 1 if you connect to a transparent access point, and 3 if
you are the access point.  The HP
Netbeamer is an access point, but it accept connections only if the PC
is in peer mode. Go figure...<br>
<br>
<b>PC -&gt; HP NetBeamer</b>:
<br>
Here is how to hook to the NetBeamer... After aligning the IrDA port
or after starting <b>irattach</b>, the light of the NetBeamer should
flash. If it doesn't, you may want to play with the <tt>slot_timeout</tt>
value.
<pre>&gt; insmod irlan access=2
&gt; ifconfig irlan0 10.0.0.1 netmask 255.255.255.0 broadcast 10.0.0.255</pre>
At this point, the light goes solid green. Link is on, you can ping
and everybody is happy. You may want to add a gateway with:<br>
"<tt>route add default gw ...</tt>".<br>
<br>
<b>PC -&gt; PC</b>:
<br>
Not everybody has a NetBeamer, so here is a step by step on
how to create a link between two PCs.<br>
<br>
<i>On the first PC</i>:
<pre>&gt; insmod irlan access=2
&gt; ifconfig irlan0 10.0.0.1 netmask 255.255.255.0 broadcast 10.0.0.255</pre>
<i>On the second PC</i>:
<pre>&gt; insmod irlan access=2
&gt; ifconfig irlan0 10.0.0.2 netmask 255.255.255.0 broadcast 10.0.0.255</pre>
After that, you should be able to ping and telnet...<br>
<br>
By default, <tt>/etc/irda/network.opts</tt> is not used. In the previous
example, we ifconfig-ure irlan by hand. If you have a Red-Hat/Mandrake
distribution, <b>irmanager</b> can do the job automatically at the condition
that you create a file <tt>/etc/sysconfig/network-scripts/ifcfg-irlan0</tt> and
set the right values in there... There might be more needed, but I'm
not totally expert on this...<br>
For other distribution (like Debian),
you need to replace the file <tt>/etc/irda/network</tt> with possibly something
from a Pcmcia package, and with some editing you might get it to load
network.opts...<br>
You might also want to add in your <tt>/etc/conf.modules</tt> a 
"<i>option irlan access=2</i>". So, if you use <b>modprobe</b> instead of 
<b>insmod</b>, you won't have to specify <i>access=2</i> on the command line.

<a name="debug">
<h3>Checking Linux-IrDA state and debugging</h3>
</a>

Of course, I'm sure that you won't get things smooth the first
time. Actually, I'm pretty sure you will struggle a little bit.
If you get the Obex stuff out of the loop (so, using Ultra or Socket,
as described above), the e-Squirt stuff is so simple that if anything
doesn't work you can bet that it's the IrDA stack.
The first trick is to check is the modules are loaded:
<pre>&gt; cat /proc/modules 
actisys          1652   1 (autoclean)
irtty            7524   2 (autoclean)
irda           151905  11 (autoclean) [actisys irtty]</pre>

This is what a serial dongle setup would look like. If the modules
don't show up, check you modules configuration and check the error
messages in the log (with <b>dmesg</b>).
Then, check the bunch of files in <tt>/proc/net/irda</tt>:
<pre>&gt; cat /proc/net/irda/discovery 
IrLMP: Discovery log:

nickname: Jean, hint: 0x8220, saddr: 0x913b1bbc, daddr: 0x5619b45e

&gt; cat /proc/net/irda/irlap
irlap0 state: LAP_NDM
  device name: irda0, hardware name: ttyS0
  caddr: 0x52, saddr: 0x913b1bbc, daddr: 0x5619b45e
  win size: 1, win: 1, line capacity: 4800, bytes left: 4800
  tx queue len: 0 win queue len: 0 rbusy: FALSE mbusy: FALSE
  retrans: 0 vs: 2 vr: 2 va: 0
  qos   bps     maxtt   dsize   winsize addbofs mintt   ldisc   comp
  tx    9600    0       64      1       12      0       0
  rx    9600    0       64      1       12      0       0
&gt; cat /proc/net/irda/irias 
LM-IAS Objects:
name: hp:esquirt, id=76371435
 - Attribute name: "IrDA:TinyTP:LsapSel", value[IAS_INTEGER]: 96

name: OBEX:ESquirt, id=76371435
 - Attribute name: "IrDA:TinyTP:LsapSel", value[IAS_INTEGER]: 95

name: Device, id=0
 - Attribute name: "IrLMPSupport", value[IAS_OCT_SEQ]: octet sequence

 - Attribute name: "DeviceName", value[IAS_STRING]: "lagaffe"

name: hp:beacon, id=76371435
 - Attribute name: "IrDA:TinyTP:LsapSel", value[IAS_INTEGER]: 97</pre>

There, you can see that the IrDA stack has discovered my Palm V, that
my IrDA port is ttyS0, that I'm not connected, and you can also see
that I have an e-Squirt application running that has opened a bunch of
server sockets (of course, if you haven't started e-Squirt, the IAS
won't contains all those sockets).<br>
<br>
The ultimate debugging tool is <b>irdadump</b>. You should run
irdadump <i>while attempting to connect</i> and check what's
happening. A normal irdadump log with a IrDA device in front of the
port (not connected) should show something like this 
(note that "<font color="red">&gt;&gt;</font>" mean breaking long lines):
<pre>&gt; irdadump
22:04:48.000713 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=0 (14)
22:04:48.090705 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=1 (14)
22:04:48.180714 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=2 (14)
22:04:48.270734 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=3 (14)
22:04:48.270698 xid:rsp 6f1e8511 &lt; fb48d412 S=6 s=2 Jean <font color="red">&gt;&gt;</font>
  Tourrilhes hint=8220 [ PDA/Palmtop IrOBEX ] (32)
22:04:48.360742 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=4 (14)
22:04:48.450733 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=5 (14)
22:04:48.540762 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=* weblab10 <font color="red">&gt;&gt;</font>
  hint=0400 [ Computer ] (24)</pre>

You see my Palm V answering the discoveries of Linux. The Palm shows
the infamous "Waiting for sender" pop-up.
On the other hand, if the stack is not properly configured (wrong
port, wrong driver), or if the device in front is not active, you will
get something like this:

<pre>22:02:47.988983 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=0 (14)
22:02:48.078981 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=1 (14)
22:02:48.168992 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=2 (14)
22:02:48.258995 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=3 (14)
22:02:48.349018 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=4 (14)
22:02:48.439035 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=5 (14)
22:02:48.529063 xid:cmd 6f1e8511 &gt; ffffffff S=6 s=* weblab10 <font color="red">&gt;&gt;</font>
  hint=0400 [ Computer ] (24)</pre>
As you can see, nobody answer us...
After that, send a good bug report to the <a
href="http://lists.sourceforge.net/lists/listinfo/irda-users">Linux-IrDA mailing list</a>.

<a name="hang">
<h4>The connection just "hang"</h4>
</a>

One classical problem is the connection hanging just after beeing
negociated. The IrDA dump looks like the following:

<pre>18:03:28.766071 xid:cmd ffffffff &lt; af28ca67 S=6 s=0 (14) 
18:03:28.856067 xid:cmd ffffffff &lt; af28ca67 S=6 s=1 (14) 
18:03:28.947685 xid:cmd ffffffff &lt; af28ca67 S=6 s=2 (14) 
18:03:29.037383 xid:cmd ffffffff &lt; af28ca67 S=6 s=3 (14) 
18:03:29.037549 xid:rsp 977f612c &gt; af28ca67 S=6 s=3 lagaffe <font color="red">&gt;&gt;</font>
  hint=4400 [ Computer LAN Access ] (23) 
18:03:29.126099 xid:cmd ffffffff &lt; af28ca67 S=6 s=4 (14) 
18:03:29.216071 xid:cmd ffffffff &lt; af28ca67 S=6 s=5 (14) 
18:03:29.316257 xid:cmd ffffffff &lt; af28ca67 S=6 s=* tanguy <font color="red">&gt;&gt;</font>
  hint=4400 [ Computer LAN Access ] (22) 
18:03:29.316433 snrm:cmd ca=fe pf=1 977f612c &gt; af28ca67 new-ca=ba (32) 
18:03:29.417508 ua:rsp ca=ba pf=1 977f612c &lt; af28ca67 (31) 
18:03:29.417646 rr:cmd &gt; ca=ba pf=1 nr=0 (2) 
18:03:29.666173 rr:cmd &gt; ca=ba pf=1 nr=0 (2)</pre>

If you are on the primary, you will see a series of <b>rr:cmd</b> until it
times-out. On the secondary, you won't see anything after the
<b>ua:rsp</b> and it will eventually timeout.
What most likely happening is that the negociated connection
parameters don't match. Usually, one end doesn't implement properly
the speed that is beeing negociated, so the two nodes can't hear each
other after changing speed. And most likely it happens at FIR speeds.
Of course, it would be nice to fix the driver, but in the short term
the solution is to force the IrDA stack to negociate a lower
speed:

<pre>&gt; echo 115200 &gt; /proc/sys/net/irda/max_baud_rate</pre>

You can of course try lower values, and there is also other parameters
you can tweak in this directory.
There is another hang similar to that. You may see the IrDA stack
"hanging" on transmitting a large packet. This seems due to a bug in
the IrDA-USB dongles.

<pre>18:03:30.458569 i:rsp  &lt; ca=ba pf=1 nr=6 ns=5 LM <font color="red">&gt;&gt;</font>
  slsap=12 dlsap=10 CONN_CMD TTP credits=0(12) 
18:03:30.458740 i:cmd  &gt; ca=ba pf=1 nr=6 ns=6 LM <font color="red">&gt;&gt;</font>
   slsap=10 dlsap=12 CONN_RSP TTP credits=0(12) 
18:03:30.466399 rr:rsp &lt; ca=ba pf=1 nr=7 (2) 
18:03:30.516548 rr:cmd &gt; ca=ba pf=1 nr=6 (2) 
18:03:30.537423 i:rsp  &lt; ca=ba pf=1 nr=7 ns=6 LM <font color="red">&gt;&gt;</font>
  slsap=12 dlsap=10 TTP credits=0 (29) 
18:03:30.537663 rr:cmd &gt; ca=ba pf=1 nr=7 (2) 
18:03:30.547328 rr:rsp &lt; ca=ba pf=1 nr=7 (2) 
18:03:30.555025 i:cmd  &gt; ca=ba pf=1 nr=7 ns=7 LM <font color="red">&gt;&gt;</font>
  slsap=10 dlsap=12 TTP credits=1 (2050) 
18:03:30.566804 i:cmd  &gt; ca=ba pf=1 nr=7 ns=7 LM <font color="red">&gt;&gt;</font>
  slsap=10 dlsap=12 TTP credits=1 (2050) 
18:03:30.596405 i:cmd  &gt; ca=ba pf=1 nr=7 ns=7 LM <font color="red">&gt;&gt;</font>
  slsap=10 dlsap=12 TTP credits=1 (2050)</pre>

It may look a bit different for you, but you get the idea, the packet
doesn't goes through and is retried, and the communication just dies
there.
As we can't fix the hardware, the solution is to force the IrDA stack
to transmit smaller packets:

<pre>&gt; echo 2000 &gt; /proc/sys/net/irda/max_tx_data_size</pre>

<a name="tcsetattr">
<h4>irattach print "tcsetattr" in the log</h4>
</a>

People using FIR drivers (nsc-ircc, smc-ircc...) are often confronted
to this simple problem. When they start <b>irattach</b>, it doesn't
work and the following message (or similar) is printed in the
log&:
<pre>irattach: tcsetattr: Invalid argument</pre>

This is due to a conflict between the Linux-IrDA <i>FIR driver</i> and
the regular Linux <i>serial driver</i>. Both want to manage the same
hardware, the serial driver has registered the FIR port as a pseudo
serial port and is <i>owning</i> it, and the kernel rightly prevent
the FIR driver to get ownership of it (it's first come first serve).
The solution is simple. You need to tell the serial driver that it
should not manage this port.
The safest way is to remove the serial driver:

<pre>&gt; rmmod serial</pre>

Unfortunately, the trick above doesn't always work (non-modular
driver, another serial port in use). Another way is to declare the
port invalid:

<pre>&gt; setserial /dev/ttyS1 uart none</pre>


<h4>Compilation problems</h4>

Sometimes, when you compile the IrDA stack or some various IrDA
package, you may have the compiler complaining the things such as
<i>IRLMP_HINT_MASK_SET</i> or <i>IRDAPROTO_ULTRA</i> are not defined.
This is because of a mess related to kernel headers and the way most
distributions deal with it. If you have the 2.4.X kernel source lying
around, the fix is simple. Just copy the header <tt>irda.h</tt> from the
kernel to your <i>include</i> directory:

<pre>cp /usr/src/linux/include/linux/irda.h /usr/include/linux</pre>

That should fix it.




