#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(smcinit, 0.5-1, irda-users@lists.sf.net)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpci':
AC_CHECK_LIB([pci], [pci_alloc])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h syslog.h unistd.h])
AC_CHECK_HEADERS([sys/io.h sys/types.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_CHECK_FUNCS([strtoul])

AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
