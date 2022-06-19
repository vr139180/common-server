#! /bin/sh

spath=.

if [ $# -gt 0 ]; then
   spath=$1
fi

for file in $( ls $spath/*.cpp); do
    echo -e $file' \'
done

