#!/bin/sh
#

set -xe

CFLAGS="-Wall -Wextra -pedantic"

cc $CFLAGS fuzzy.c -o fuzzy

