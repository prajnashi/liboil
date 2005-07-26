#!/bin/sh

cat <<-EOF
***
***  liboil CVS has moved.
***
***  The new project root is
***  :pserver:anoncvs@anoncvs.freedesktop.org:/cvs/liboil
***
***  Use the script
***  ./cvs-changeroot :pserver:anoncvs@anoncvs.freedesktop.org:/cvs/liboil
***  to automatically change to the new repository.
***
EOF

exit 0

autoreconf -i -f &&
./configure --enable-maintainer-mode --disable-static $@
