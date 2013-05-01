#!/bin/sh

rm -f depcomp
rm -f config.cache
rm -rf autom4te.cache
rm -f config.log
rm -f config.h
rm -f aclocal.m4
rm -f Makefile.in
rm -f config.status
rm -f install-sh
rm -f missing
rm -f Makefile.in
rm -f configure

if [ -d m4 ]; then
        echo "Looking in m4 directory for macros."
        aclocal -I m4
else
        echo "Looking in current directory for macros."
        aclocal -I .
fi

echo "Running autoconf"
autoconf
echo "Running autoheader"
autoheader
echo "Running automake"
automake -a

exit
