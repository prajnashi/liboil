#!/bin/sh
set -x
set -e
autoreconf -i -f
./configure --enable-maintainer-mode --disable-static $@
