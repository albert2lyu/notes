
----------------------------------------------
# gdbserver :1234 ./ipqdemo 
Process ./ipqdemo created; pid = 384
Listening on port 1234
Remote debugging from host 192.168.222.213
readchar: Got EOF
Remote side has terminated connection.  GDBserver will reopen the connection.
Listening on port 1234
----------------------------------------------


----------------------------------------------
gdb-7.1 for x86
border@wifihack:/work/cross/gdb-7.1$ ./configure --prefix=/opt/rootfs/cross/mips/mips-linux --target=mipsel-linux-uclibc --program-prefix=mipsel-linux-
make 
install
----------------------------------------------

补充：前面还有库文件错误的问题，这里可以通过下面这样的方法的来解决。需要说明的是，远程调试，可能程序动态库，我们可以这样设置：

set solib-absolute-prefix /opt/rootfs/filesystem/
set solib-search-path /opt/rootfs/filesystem /home/smg

调试找不到源代码，如下设置：
directory xfer-1.0.0/src/

---------------------------------------------
border@wifihack:/opt/rootfs/cross/mips/mips-linux/bin$ ./mipsel-linux-gdb /opt/rootfs/ipq/ipqdemo 
GNU gdb (GDB) 7.1
Copyright (C) 2010 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-pc-linux-gnu --target=mipsel-linux-uclibc".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from /opt/rootfs/ipq/ipqdemo...done.
(gdb) set solib-absolute-prefix /opt/rootfs/filesystem/
(gdb) set solib-search-path /opt/rootfs/filesystem /home/ipq
(gdb) target remote 192.168.222.20:1234
Remote debugging using 192.168.222.20:1234
Reading symbols from /opt/rootfs/filesystem/lib/ld-uClibc.so.0...(no debugging symbols found)...done.
Loaded symbols for /opt/rootfs/filesystem/lib/ld-uClibc.so.0
0x2aaa8fd0 in _start () from /opt/rootfs/filesystem/lib/ld-uClibc.so.0
(gdb) c
Continuing.
---------------------------------------------




----------------------------------------------
交叉编译到mips
需要文件

termcap-1.3.1.tar.gz
gdb-7.1.tar.bz2
编译termcap

./configure --prefix=/opt/rootfs/cross/mips/termcap/ --host=mipsel-linux
手动修改Makefile文件中的

CC = gcc
AR = ar
RANLIB = ranlib
修改为:

CC = mipsel-linux-gcc
AR = mipsel-linux-ar
RANLIB = mipsel-linux-ranlib
编译安装:

make
make install
编译gdb

tar xvf gdb-7.1.tar.bz2
cd gdb-7.1
mkdir libtermcap
拷贝刚刚编译好的termcap库到libtermcap目录下

cp /opt/rootfs/cross/mips/termcap/lib/libtermcap.a ./libtermcap/
cp /opt/rootfs/cross/mips/termcap/include/termcap.h ./libtermcap/
编译和安装

./configure --prefix=/opt/rootfs/cross/mips/termcap/ --host=mipsel-linux
make
make install
----------------------------------------------






1. http://blog.chinaunix.net/u3/104447/showart_2244531.html
2. http://lsec.cc.ac.cn/~hyu/cn/wiki/gdb.html

