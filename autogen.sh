#!/bin/sh
set -x
aclocal -I m4
libtoolize --force
autoheader
autoconf
automake -a --foreign
./configure --enable-maintainer-mode
