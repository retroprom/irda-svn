# Makefile configuration - processed by automake.
#
# General automake options.
AUTOMAKE_OPTIONS = foreign no-dependencies
#ACLOCAL_AMFLAGS = -I m4

# The list of programs that are built.
bin_PROGRAMS = smcinit tosh2450-smcinit tosh1800-smcinit

configure.ac: VERSION
	perl -pi -e 'BEGIN { open V, "$<" and $$version = <V>; chomp $$version; } { s/$$2/$$version/ if m/(AC_INIT\(.*,\s*)(.*)(\s*,.*)/ }' $@


tosh2450_smcinit_SOURCES = tosh2450-smcinit.c
tosh1800_smcinit_SOURCES = tosh1800-smcinit.c
smcinit_SOURCES = smcinit.c

man_MANS = smcinit.7  smcinit.8  tosh1800-smcinit.8

# Additional files to be distributed.
#EXTRA_DIST = autogen.sh autoclean.sh ChangeLog CHANGELOG.Peri 

specs = smcinitredhat.spec smcinitredhat.spec.in smcinitsuse.spec \
	smcinitsuse.spec.in makerpmredhat.sh makerpmsuse.sh

sysvinit = boot.smcinit boot.smcinit.rhmdk smcinit.sysconfig

EXTRA_DOCS = AUTHORS CHANGELOG.Peri RobMiller-irda.html \
	     smcinit.lsm smcinit.lsm.in VERSION tosh-superio.h
EXTRA_DIST = ${man_MANS} ${EXTRA_DOCS} ${specs} ${sysvinit}

# COPYING tosh1800-smcinit.c tosh2450-smcinit.c tosh-superio.h VERSION

MAINTAINERCLEANFILES = *~

