
<h2>Linux-IrDA Development</h2>

<f2>
While Linux already enjoy an Open Source implementation of
IrDA stack there is a real lack of knowledge developing
infrared aware applications. This document is a step by
step guide to Linux-IrDA programming that make an easy introduction
for unexperienced programmers. Alltough, an experienced
one will find usefull the paralell between IrDA and TCP/IP
protocols.
</f2>

<h2>Starting IrDA stack</h2>
<f2>
IrDA is a protocol stack similar to TCP/IP. It's not surprising that
applications to employ IrDA make use of <tt>socket()</tt> system call. I
remember that among TCP/IP, there are many other protocols stacks under Linux:
IPX/SPX, DecNet, NetAtalk. Execute:
</f2><pre>
man socket
</pre><f2>
As you may see <tt>socket()</tt> function have 3 parameters. First one specify
what protocol stack we want to use. For our case is <tt>AF_IRDA</tt>
(address family IrDA). This is a number. More precisely 23, but who care. Some
protocols offer many type of services for applications: reliable data streams,
data packets without confirmation etc. The second paramater encode type of
service. <tt>SOCK_STREAM</tt> and <tt>SOCK_DGRAM</tt> are most common ones.
Third parameter tell protocol used. In almost all cases the value is 0.<br>
<br>
A <tt>socket()</tt> call try to initialize a binding between application and
IrDA stack from kernel. It's like opening a file before trying to read or write
from it. The value returned is a descriptor which refer to our opened connection
with stack. Bellow is <i>ir-tut1.c</i> which illustrate what I said above:
</f2><pre>
\#include &lt;stdio.h&gt;
\#include &lt;stdlib.h&gt;
\#include &lt;sys/socket.h&gt;
\#include &lt;linux/types.h&gt;
\#include &lt;linux/irda.h&gt;

int main(int argc, char **argv) {
  int fd;
  fd=socket(AF_IRDA,SOCK_STREAM,0);
  if (fd<0) {
    perror("socket");
    exit(-1);
  }
  return(0);
}
</pre><f2>
Compile  and execute it:
</f2><pre>
clauc:tutorial# gcc -I/usr/include -o ir-tut1 ir-tut1.c
clauc:tutorial# ./ir-tut1
socket: Address family not supported by protocol
clauc:tutorial#
</pre><f2>
Our program failed. This is good because IrDA stack is not
in kernel. This happen if you use an usual Linux distribution.
IrDA code is compiled as modules and loaded on demand or
at user request. We manually load the IrDA stack and reexecute
our program.
</f2><pre>
clauc:tutorial# modprobe irda
clauc:tutorial# ./ir-tut1
clauc:tutorial#
</pre><f2>
Well, we can see that <i>ir-tut1</i> has completed succefully by doing nothing.
Because for the moment nothing is complicated in first IrDA
application lets make a tour arround what Linux IrDA stack
show us in virtual <tt>/proc</tt> filesystem.
</f2><pre>
clauc:tutorial# ls -1 /proc/net/irda
discovery
irias
irlap
irlmp
irttp
clauc:tutorial#
</pre>

<f2>
You can observe 5 files. Information in these files is dynamic
and change according to stack functionality. When another
IrDA protocol components are loaded, more files will be
present here. Just for curiosity look on files using an
ordinary text viewer.
</f2><pre>
clauc:tutorial# ls -1 /proc/sys/net/irda
devname
discovery
discovery_slots
discovery_timeout
max_baud_rate
max_inactive_time
slot_timeout
clauc:tutorial#
</pre><f2>
Above you can view the files that control some IrDA stack parameters. Of some
importance when tweaking operation with external IrDA aware devices can be 
<i>discovery_timeout</i> and/or <i>slot_timeout</i>. I'll not develop an 
exhaustive discussion about these parameters or what informational files in 
<i>/proc/net/irda/</i> contain, but some more info are provided below as need 
arise.
</f2>

<h2>Playing with local IrIAS</h2>

<f2>
Frankly, our stack is not ready for normal use, but most concepts can be fixed 
in mind before we start real work. Lets introduce IAS - Information Access
Service. IAS is an IrDA built-in application and on top of IrLMP. It's similar
to DNS - Domain Name System counterpart. Instead of resolving names to IP
addresses, it resolve service names to IrLMP selectors. Oh, lot of complicated
info. I'll try to take these step by step. First, an advised person know that 
DNS not only map names to IPs, but many types of informations. It's like a 
database. You can put anything you like to make your website, mail server etc. 
need to function properly and users be happy. Likewise, IAS can store different 
type o informations: numbers, strings and byte sequences. With these building 
blocks you or your application can store or read informations in/from IAS, 
structured by classes (like database records). Every object contain one or more 
entries of different type.<br>
<br>
What a LMP selector it is? Well, it's easy to associate with a TCP or UDP port. 
A host connected to network have an IP address and in this host a specific 
service act (listen) on specific port. That way a host can provide multiple 
services on one data link. Similary, LMP offer a meaning to multiplex many data 
channels on one infrared link. TCP/IP have a maximum of 65535 ports. IrDA allow 
256, but more are reserved and some are special. For example IAS listen on 0x00 
selector all the time. Seletor 0x7f, when open, will allow exclusive access to 
IrDA link. Your application can use selectors in 0x01-0x7e range. IANA, an 
Internet authority define known services which listen on specific ports. For 
example mail servers using SMTP protocol use port 25, web servers using HTTP 
use 80. It's not going to happen with IrDA. Because communications are temporary 
and selectors are a limited resource, applications which act as a server 
temporary register their presence in IAS. The listening LMP selector is made 
known in IAS, along with another informations if application want to provide.<br>
<br>
In this lesson you will learn how to set and query informations from IAS. 
Please keep in mind that every entity which employ IrDA have an IAS service. 
So, we talk about two IAS entities when come to have a data link: our IAS and 
peer IAS. We have need for two new system calls: <tt>getsockopt()</tt> and 
<tt>setsockopt()</tt>. To accomodate with it, run:
</f2><pre>
man getsockopt
man setsockopt
</pre><f2>
These functions control various IrDA stack parameters or are used to do other
than transfering data. Our case is working with IAS. To point in using IrLMP
facilities, <tt>level</tt> argument is set to <tt>SOL_IRLMP</tt>, which is a
number defined in <i>irda.h</i>.
Take a look in that file to view various socket
options available for Linux IrDA stack. This lesson will make use of
<tt>IRLMP_IAS_GET</tt> and <tt>IRLMP_IAS_SET</tt> to query and set our IAS
database. To query peer IAS there is a <tt>IRLMP_IAS_QUERY</tt> but for the
moment we don't use it because yet is no IrDA link with other device.
<tt>getsockopt()</tt> take a data structure as one of arguments:
<tt>irda_ias_set</tt>.
</f2><pre>
struct irda_ias_set {
  char irda_class_name[IAS_EXPORT_CLASSNAME];
  char irda_attrib_name[IAS_EXPORT_ATTRIBNAME];
  unsigned int irda_attrib_type;
  union {
    unsigned int irda_attrib_int;
    struct {
      unsigned short len;
      __u8 octet_seq[IAS_MAX_OCTET_STRING];
    } irda_attrib_octet_seq;
    struct {
      __u8 len;
      __u8 charset;
      __u8 string[IAS_MAX_STRING];
    } irda_attrib_string;
  } attribute;
  __u32       daddr;    /* Address of device (for some queries only) */
};
</pre><f2>
Before calling <tt>getsockopt()</tt> we must fill <tt>irda_class_name</tt> and
<tt>irda_attrib_name</tt> fields. After call remaining fields will be filled
with result if any. This system call corespond with <tt>GetValueByClass</tt>
primitive from IrDA stack description. Looking to <tt>irda_attrib_type</tt> we 
can tell what type of data had returned: <tt>IAS_MISSING</tt> (nothing), 
<tt>IAS_INTEGER</tt> (an unsigned integer number), <tt>IAS_OCT_SEQ</tt> 
(an octet sequence) or <tt>IAS_STRING</tt> (a string).<br>
<br>
<b>[TO BE CONTINUED]</b>

</f2>
