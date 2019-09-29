#!/bin/sh

set -e

PLATFORM=or1000
CONF=Release
MAKE_ARGS="CONF=$CONF CND_PLATFORM=$PLATFORM CXX=or1k-linux-musl-g++"
JORCONSOLE=/mnt/e/git/jorconsole
STAGE=stage/$PLATFORM
DEST=`readlink -f .`/alisp

if [ "$1" = "clean" ]; then
	echo make $MAKE_ARGS clean
	make $MAKE_ARGS clean
	exit 0
fi

make $MAKE_ARGS -j2
mkdir -p $STAGE
cp dist/$CONF/$PLATFORM/alisp $STAGE
cp -r lisp $STAGE
(cd $STAGE && tar cf $DEST alisp lisp)
cp $DEST $JORCONSOLE/jor1k-sysroot/fs/home/user/alisp
(cd $JORCONSOLE && node index)
