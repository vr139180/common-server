#! /bin/sh

declare -a cpp_dir

cpp_dir[0]='attributecalculator/*.cpp'
cpp_dir[1]='staticdata/*.cpp'
cpp_dir[2]='test/*.cpp'

for (( ii =0; ii < ${#cpp_dir[*]} ; ii++ )) ; do

for file in $( ls ${cpp_dir[ $ii ]}); do
    echo -e $file' \'
done

done
