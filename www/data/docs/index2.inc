<br>
<f2>
<navbar:render name="docs2menu" select="starting"><br>
</f2>

<h2>Starting Linux-IrDA stack</h2>
Most Linux distributions come with IrDA support by default. Following 
are instructions to configure IrDA and your device. If you don't find
explanations for your distro, please send contact us on 
<a href="http://lists.sourceforge.net/lists/listinfo/irda-users">Linux-IrDA mailing list</a>.

<h3>Debian GNU/Linux 2.2 and 3.0</h3>

You at least must install the <tt>irda-common</tt> package. To have at hand the
<b>irdadump</b>, then get and install <tt>irda-tools</tt> package. The easy way
is:
<pre>&gt; apt-get install irda-common
&gt; apt-get install irda-tools</pre>

As of August 2002, these utilities are based on <tt>irda-utils 0.9.14</tt>. 
To start IrDA driver and stack at boot time, you should run:

<pre>&gt; update-rc.d irda defaults</pre>

To start or stop it manually, run:

<pre>&gt; /etc/init.d/irda start
&gt; /etc/init.d/irda stop</pre>

Before you run the <tt>irda</tt> script you must configure the 
<tt>/etc/irda.conf</tt>. Bellow are the variables used in this file:
<ul>
  <li><tt>IRDADEV</tt>: path to device where IrDA dongle is attached. It's 
	for serial attached dongles. Default is <tt>/dev/ttyS1</tt>, but you must figure
	where you connect the physical device and change value acordingly.
	
	<li><tt>DONGLE</tt>: this efectively tell which device driver to be used
	between physical device and IrDA stack. Look on dongle documentation to
	figure out. For laptops with FIR built-in, please consult ...
	(<i>fixme: must point to a list of laptops, devices and drivers</i>)
	
	<li><tt>DISCOVERY</tt>: if blank, discovery is not started. If set to
	<tt>-s</tt>, the continuos discovery will be started and any IrDA device 
	in range will be discovered and made available in discovery log.
	
	<li><tt>ENABLE</tt>: default is <tt>no</tt> and mean that IrDA is not
	started at boot or by issuing <tt>/etc/init.d/irda start</tt>. To activate
	it you must set this variable to <tt>yes</tt>.
	
</ul>


<h3>Mandrake Linux 8.1 and 8.2</h3>

This section must be written.

<h3>RedHat Linux 7.1 and 7.2</h3>

This section must be written.

<h3>Slackware 8.0 and 8.1</h3>

The Slackware user have no IrDA support by default, except the one provided
by kernel. You must grab the latest 
<a href="http://irda.sourceforge.net/ftp/irda-utils/">irda-utils</a> source
and compile yourself. For instructions see 
<a href="$(ROOT)docs/index4.html">Advanced section</a>.

<h2>Common pitfalls</h2>

There is many way to get the IrDA stack to not run properly. Not
following instructions seems to be one of the most guaranteed way to
reach that goal.
Here are <b>mistakes</b> I've seen user make:

<ul>
  <li>IrDA ports not properly aligned
  <li><b>irmanager</b> running (it's obsolete, get rid of it;
    let the init scripts of the distribution do the job)
  <li><b>irdaping</b> running (prevent the LAP connection)
  <li>obsolete irda-utils
  <li>using the wrong driver (<i>FIR/irtty/irport</i>)
  <li>using the wrong port (<tt>/dev/ttyS0</tt> vs <tt>/dev/ttyS1</tt>)
  <li>wrong permissions on the serial port (<B>irattach</B> must run as root)
  <li>some other application (such as <b>pppd</b>) grabbing the serial port
  <li>some other driver (most likely a PCMCIA card) using the IRQ that
  the IrDA driver needs (for Pcmcia drivers, play with "exclude"
  directives in <tt>/etc/pcmcia/config.opts</tt>)
  <li>wrong irattach command (<tt>irattach /dev/ttySX</tt> for SIR vs
  <tt>irattach irdaX</tt> for FIR, where "X" is a number, i.e. 0)
  <li>multiple instances of <b>irattach</b> for the same port (staying
  in background)
  <li>wrong modules names (should be <b>ircomm-tty</b>, not <b>ircomm_tty</b>)
  <li>IrDA modules not loaded
  <li>mixing up <tt>/dev/ttySX</tt> and <tt>/dev/ircommX</tt>. The stack
  runs on top of <tt>/dev/ttySX</tt> and provides serial emulation through
  <tt>/dev/ircommX</tt>, so serial apps should run on top of
  <tt>/dev/ircommX</tt>.
  <li>compiling the IrDA stack static (i.e. non module - it may work,
  but this is not what the developpers work with)
  <li>compiling the IrDA stack without the IRDA_DEBUG option. This
  option enable extra checks that prevent your kernel to crash.
  <li>mixing modules belonging to different versions of the IrDA stack
  <li>unclean IrDA source (failed patch)
  <li>mixing modules belonging to different kernel versions
  <li>trying to load an IrDA module on a stack wich has this code
  compiled static (or loading module twice)
  <li>wrong version of modules tools (for kernel 2.4)
  <li>failed to do depmod -a (usually automatic at reboot)
  <li>using a version of the kernel which is not used and recommended by
  developpers.
</ul>

