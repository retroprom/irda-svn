#!/bin/sh

Version=$(< VERSION)
aclocal
autoheader
autoconf
automake -a -c
