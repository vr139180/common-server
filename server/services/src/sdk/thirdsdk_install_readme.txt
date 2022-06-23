#boost

.\b2 install cxxflags="--std=c++11" --prefix=./stages --build-type=complete --no-cmake-config --without-python --without-wave --without-graph --without-graph_parallel --without-nowide --without-test toolset=msvc link=static threading=multi runtime-link=static address-model=64 debug release

.\b2 install --prefix=./stages --build-type=complete --no-cmake-config --without-python --without-wave --without-graph --without-test --show-libraries toolset=msvc link=static threading=multi runtime-link=shared address-model=64 debug releasemd

#openssl
perl Configure VC-WIN64A no-asm -static --prefix=D:\workspace\common_server\server\sdk-src\openssl-OpenSSL_1_1_1-stable/stages
nmake
nmake install

#mysql-connector-cpp
cmake -DCMAKE_BUILD_TYPE=Debug/Release -DCMAKE_INSTALL_PREFIX=./stages -G "Visual Studio 15" -A x64 -DBUILD_STATIC=ON -DWITH_JDBC=OFF -DWITH_SSL=D:\workspace\common_server\server\sdk-src\openssl-OpenSSL_1_1_1-stable\stages -DSTATIC_MSVCRT=ON ./

cmake --build . --target install --config Debug/Release


cmake -DBOOST_ROOT=D:\workspace\common_server\server\sdk-src\boost_1_78_0\stages\include\boost-1_78 -DCMAKE_INSTALL_PREFIX=./stages -DMYSQLCLIENT_STATIC_LINKING=ON ./
cmake --build . --target install --config Debug/Release

#hiredis

cmake -DDISABLE_TESTS=ON -DCMAKE_INSTALL_PREFIX=./stages -G "Visual Studio 15" -A x64 ./
cmake --build . --target install --config Debug/Release

#redis-plus-plus

cmake -DCMAKE_PREFIX_PATH=D:\workspace\common_server\server\sdk-src\hiredis-1.0.2\stages\include -DCMAKE_INSTALL_PREFIX=./stages -DREDIS_PLUS_PLUS_CXX_STANDARD=11 -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_SHARED=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC_WITH_PIC=OFF -G "Visual Studio 15" -A x64 ./

cmake --build . --target install --config Debug/Release

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
rpm -ivh https://repo.mysql.com//mysql57-community-release-el7-11.noarch.rpm
rpm --import https://repo.mysql.com/RPM-GPG-KEY-mysql-2022
yum install mysql-community-client.x86_64 -y
cd /usr/lib64/
ln ./mysql/libmysqlclient.so.20.3.25 libmysqlclient.so
