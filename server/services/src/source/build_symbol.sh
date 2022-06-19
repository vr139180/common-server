#!/bin/sh

echo 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx begin ewo building xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'

make clean

aclocal

echo 'call autoconf'

autoconf

echo 'call automake'

automake -a

echo 'call configure'

./configure CXXFLAGS='-O2 -g -DNDEBUG' CFLAGS='-O2 -g -NDEBUG'

echo '------------------------------------ begin make ------------------------------------'

make -j 8

echo '------------------------------------ end make ------------------------------------'

echo 'copy all application symbol to server direction'

rm -f ../../server/ewwASPolicy/ewwASPolicy.symbol

objcopy --only-keep-debug ./ewwASPolicy/ewwASPolicy ../../server/ewwASPolicy/ewwASPolicy.symbol

rm -f ../../server/ewwCenter/ewwCenter.symbol

objcopy --only-keep-debug ./ewwCenter/ewwCenter ../../server/ewwCenter/ewwCenter.symbol

rm -f ../../server/ewwDBCache/ewwDBCache.symbol

objcopy --only-keep-debug ./ewwDBCache/ewwDBCache ../../server/ewwDBCache/ewwDBCache.symbol

rm -f ../../server/ewwGateway/ewwGateway.symbol

objcopy --only-keep-debug ./ewwGateway/ewwGateway ../../server/ewwGateway/ewwGateway.symbol

rm -f ../../server/ewwGlobalSvr/ewwGlobalSvr.symbol

objcopy --only-keep-debug ./ewwGlobalSvr/ewwGlobalSvr ../../server/ewwGlobalSvr/ewwGlobalSvr.symbol

rm -f ../../server/ewwGS/ewwDGS.symbol

objcopy --only-keep-debug ./ewwGS/ewwDGS ../../server/ewwDGS/ewwDGS.symbol

rm -f ../../server/ewwLoginSvr/ewwLoginSvr.symbol

objcopy --only-keep-debug ./ewwLoginSvr/ewwLoginSvr ../../server/ewwLoginSvr/ewwLoginSvr.symbol

rm -f ../../server/ewwService/ewwService.symbol

objcopy --only-keep-debug ./ewwService/ewwService ../../server/ewwService/ewwService.symbol

echo 'everything done'

echo 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx end ewo building xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
