#!/bin/sh

autoreconf -i -f &&
./configure --enable-maintainer-mode --disable-static $@
#./configure --enable-maintainer-mode --disable-static --with-alternate-optimization="-O3 -funroll-all-loops" $@
