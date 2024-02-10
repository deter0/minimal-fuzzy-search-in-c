#!/bin/sh
#

set -xe

CFLAGS="-Wall -Wextra -pedantic -DFUZZY_TESTING"

cc $CFLAGS fuzzy.c -o fuzzy

