#! /bin/sh

if [ -z "$CC" ]; then
  CC=$(which gcc)
fi

CFLAGS="$CFLAGS -std=c99 -O -Wall -Wextra -Wundef"

$CC $CFLAGS \
  vdi.c \
  vdidump.c
