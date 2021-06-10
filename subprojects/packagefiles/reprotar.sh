#!/bin/sh

# stolen from https://reproducible-builds.org/docs/archives/
# requires GNU Tar 1.28+ (Ubuntu >=16.04)

exec tar --sort=name \
    --mtime="@0" \
    --owner=0 --group=0 --numeric-owner \
    --pax-option=exthdr.name=%d/PaxHeaders/%f,delete=atime,delete=ctime \
    "$@"
