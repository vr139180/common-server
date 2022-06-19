#!/bin/sh

echo 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx begin ewo building xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'

aclocal

echo 'call autoconf'

autoconf

echo 'call automake'

automake -a

echo 'call configure'

./configure CXXFLAGS='-O2 -DNDEBUG' CFLAGS='-O2 -DNDEBUG'

echo '------------------------------------ begin make ------------------------------------'

make -j 8

echo '------------------------------------ end make ------------------------------------'

echo 'copy all application to server direction'

rm -f ../../server/ewwASPolicy/ewwASPolicy

objcopy --strip-debug ./ewwASPolicy/ewwASPolicy ../../server/ewwASPolicy/ewwASPolicy

rm -f ../../server/ewwCenter/ewwCenter

objcopy --strip-debug ./ewwCenter/ewwCenter ../../server/ewwCenter/ewwCenter

rm -f ../../server/ewwDBCache/ewwDBCache

objcopy --strip-debug ./ewwDBCache/ewwDBCache ../../server/ewwDBCache/ewwDBCache

rm -f ../../server/ewwGateway/ewwGateway

objcopy --strip-debug ./ewwGateway/ewwGateway ../../server/ewwGateway/ewwGateway

rm -f ../../server/ewwGlobalSvr/ewwGlobalSvr

objcopy --strip-debug ./ewwGlobalSvr/ewwGlobalSvr ../../server/ewwGlobalSvr/ewwGlobalSvr

rm -f ../../server/ewwGMTServer/ewwGMTServer

objcopy --strip-debug ./ewwGMTServer/ewwGMTServer ../../server/ewwGMTServer/ewwGMTServer

rm -f ../../server/ewwGS/ewwDGS

objcopy --strip-debug ./ewwGS/ewwDGS ../../server/ewwDGS/ewwDGS

rm -f ../../server/ewwLoginSvr/ewwLoginSvr

objcopy --strip-debug ./ewwLoginSvr/ewwLoginSvr ../../server/ewwLoginSvr/ewwLoginSvr

rm -f ../../server/ewwService/ewwService

objcopy --strip-debug ./ewwService/ewwService ../../server/ewwService/ewwService

echo 'everything done'

echo 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx end ewo building xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
