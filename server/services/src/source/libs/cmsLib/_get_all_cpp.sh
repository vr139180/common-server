#! /bin/sh

declare -a cpp_dir

cpp_dir[0]='base/*.cpp'
cpp_dir[1]='condition/*.cpp'
cpp_dir[2]='config/*.cpp'
cpp_dir[3]='encryption/md5/*.cpp'
cpp_dir[4]='gamedata/*.cpp'
cpp_dir[5]='httpcurl/*.cpp'
cpp_dir[6]='lua_src/*.c'
cpp_dir[7]='luabind_src/*.cpp'
cpp_dir[8]='net/*.cpp'
cpp_dir[9]='os/linux/*.cpp'
cpp_dir[10]='prolib/*.cpp'
cpp_dir[11]='script/*.cpp'
cpp_dir[12]='serialization/base64/*.cpp'
cpp_dir[13]='serialization/cjson/*.c'
cpp_dir[14]='system/*.cpp'
cpp_dir[15]='tinyxml/*.cpp'
cpp_dir[16]='util/*.cpp'
cpp_dir[17]='./*.cpp'

for (( ii =0; ii < ${#cpp_dir[*]} ; ii++ )) ; do

for file in $( ls ${cpp_dir[ $ii ]}); do
    echo -e $file' \'
done

done
