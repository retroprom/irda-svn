#!/bin/sh

# get the current version & release
version=`cat  VERSION | cut -d- -f 1`
release=`cat  VERSION | cut -d- -f 2`

# make sure there's no objects and binaries arround
make clean

# reconfigure things just to make sure rpmspec is good
./configure

# make a new snapshot
mkdir -p ../smcinit-$version-$release
cp -r . ../smcinit-$version-$release/

# jump a step behind
cd ..

# make archive from it and then delete local copy
tar cfz smcinit-$version-$release.tar.gz smcinit-$version-$release
rm -r smcinit-$version-$release

# build the RPM
cp -f  smcinit/smcinitredhat.spec /usr/src/redhat/SPECS
cp -f  smcinit-$version-$release.tar.gz /usr/src/redhat/SOURCES
rpmbuild -ba /usr/src/redhat/SPECS/smcinitredhat.spec
rm -f /usr/src/redhat/SPECS/smcinitredhat.spec
rm -f /usr/src/redhat/SOURCES/smcinit-$version-$release.tar.gz

# back to work...
cd smcinit

