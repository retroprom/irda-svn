#!/bin/sh

Version=$(< VERSION)
aclocal
autoconf
automake -a -c
