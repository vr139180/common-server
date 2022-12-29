#boost

bootstrap.bat

.\b2 install cxxflags="--std=c++11" --prefix=./stages --build-type=complete --no-cmake-config --without-python --without-wave --without-graph --without-graph_parallel --without-nowide --without-test toolset=msvc link=static threading=multi runtime-link=shared address-model=64 debug release

#openssl
perl Configure debug-VC-WIN64A no-asm threads -static --prefix=D:\workspace\common_server\server\sdk-src\openssl-OpenSSL_1_1_1-stable/stages

perl Configure VC-WIN64A no-asm threads -static --prefix=D:\workspace\common_server\server\sdk-src\openssl-OpenSSL_1_1_1-stable/stages
nmake
nmake install

#mysql-connector-cpp

cmake -DBOOST_ROOT=D:\workspace\common_server\server\sdk-src\boost_1_78_0\stages\include\boost-1_78 -DCMAKE_INSTALL_PREFIX=./stages -DMYSQLCLIENT_STATIC_LINKING=ON ./
vs2017 open MYSQLCPPCONN.sln
编译 mysqlcppconn-static 项目 /MD

#hiredis

cmake -DDISABLE_TESTS=ON -DCMAKE_INSTALL_PREFIX=./stages -G "Visual Studio 15" -A x64 ./
vs2017 打开 hiredis.sln 修改成lib静态库

#redis-plus-plus

cmake -DCMAKE_PREFIX_PATH=D:\workspace\common_server\server\sdk-src\hiredis-1.0.2\stages\include -DCMAKE_INSTALL_PREFIX=./stages -DREDIS_PLUS_PLUS_CXX_STANDARD=11 -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_SHARED=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC_WITH_PIC=OFF -G "Visual Studio 15" -A x64 ./

vs2017 open redis++.sln
添加 D:\workspace\common_server\server\services\src\sdk\include include路径

#curl
projects\generate.bat
注销ldap
curl_setup.h 212 line
#  ifndef CURL_DISABLE_LDAPS
#    define CURL_DISABLE_LDAPS
#  endif

vs2017 open projects\Windows\VC15\lib\libcurl.sln
build LibDebug LibRelease

#zlib
vs2017 open contrib\vstudio\vc14\zlibvc.sln
修改zconf.h 文件，注释掉 //#define Z_HAVE_UNISTD_H
build zlibstat

#protobuff
下载并安装CMake（Window版本）， 官方网站 cmake.org

执行安装好的CMake,选择protobuf源代码目录，选择编译输出的目录，
然后点击【Configure】按钮，生成工具选择VC++ 2017,
再点击【Generate】按钮，CMake会生成VS项目文件
如果没有错误，那么【Open Project】按钮可用，点击按钮则会调用VS 2017打开Cmake生成的Protobuf工程,

----------------------------------------------------------------linux-----------------------------------------------
ar/nm 
#boost
./b2 install cxxflags="--std=c++11" --prefix=./stages --no-cmake-config --without-python --without-wave --without-graph --without-graph_parallel --without-nowide --without-test link=static threading=multi runtime-link=static address-model=64 release
./b2 install --prefix=./stages --no-cmake-config --without-test link=static threading=multi runtime-link=static address-model=64 release

#mysql-connector-cpp
cmake -DBOOST_ROOT=../../services/src/sdk/include/boost -DMYSQLCLIENT_STATIC_LINKING=NO -DCMAKE_INSTALL_PREFIX=./stages -DBUILD_STATIC=ON -DCMAKE_BUILD_TYPE=Release -DWITH_SSL=system ./
cmake --build . --target install --config Release

#hiredis
make

#redis-plus-plus
cmake -DCMAKE_PREFIX_PATH=../../services/src/sdk/include/. -DCMAKE_INSTALL_PREFIX=./stages -DREDIS_PLUS_PLUS_CXX_STANDARD=11 -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_SHARED=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC_WITH_PIC=OFF .
make

#curl
cmake -DBUILD_SHARED_LIBS=NO -DCURL_ENABLE_SSL=NO -DCMAKE_USE_LIBSSH2=NO .

#protobuff
./configure --disable-shared --prefix/

修改configure文件，添加-fPIC
if test "x${ac_cv_env_CFLAGS_set}" = "x"
then :
  CFLAGS="-fPIC"
fi
if test "x${ac_cv_env_CXXFLAGS_set}" = "x"
then :
  CXXFLAGS="-fPIC"
fi

#mysqlclient install
rpm -ivh https://repo.mysql.com/mysql80-community-release-el7-7.noarch.rpm
rpm -ivh https://repo.mysql.com//mysql80-community-release-el8-4.noarch.rpm
rpm --import https://repo.mysql.com/RPM-GPG-KEY-mysql-2022

yum module disable mysql
yum install mysql-community-server
yum install mysql-community-client
yum install mysql-community-devel

cd /usr/lib64/
ln ./mysql/libmysqlclient.so.20.3.25 libmysqlclient.so

#gdb install
vi /etc/yum.repos.d/CentOS-Linux-Debuginfo.repo
修改 enabled=1
yum debuginfo-install libgcc-8.5.0-4.el8_5.x86_64 libstdc++-8.5.0-4.el8_5.x86_64 mysql-libs-8.0.26-1.module_el8.4.0+915+de215114.x86_64 openssl-libs-1.1.1k-5.el8_5.x86_64 zlib-1.2.11-17.el8.x86_64

