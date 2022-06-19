#!/bin/sh

echo 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx begin cms building xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'

aclocal

echo 'call autoconf'

autoconf

echo 'call automake'

automake -a

echo 'call configure'

./configure

echo '------------------------------------ begin make ------------------------------------'

make -j 8

echo '------------------------------------ end make ------------------------------------'

echo 'copy all application to server direction'

cp -f ./ewwASPolicy/ewwASPolicy ../../server/ewwASPolicy/

cp -f ./ewwCenter/ewwCenter ../../server/ewwCenter/

cp -f ./ewwDBCache/ewwDBCache ../../server/ewwDBCache/

cp -f ./ewwGateway/ewwGateway ../../server/ewwGateway/

cp -f ./ewwGlobalSvr/ewwGlobalSvr ../../server/ewwGlobalSvr/

cp -f ./ewwGS/ewwDGS ../../server/ewwDGS/

cp -f ./ewwLoginSvr/ewwLoginSvr ../../server/ewwLoginSvr/

cp -f ./ewwService/ewwService ../../server/ewwService/

echo 'everything done'

echo 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx end cmd building xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
