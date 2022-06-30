#!/bin/sh

echo '--------------go build resservice------------'
cd resservice
go build -o ../../bin/resservice/resservice.a resservice
cd ..

echo '--------------go build loginservice------------'
cd loginservice
go build -o ../../bin/loginservice/loginservice.a loginservice
cd ..

echo '--------------go build mailservice------------'
cd mailservice
go build -o ../../bin/mailservice/mailservice.a mailservice
cd ..

echo '--------------go build friendservice------------'
cd friendservice
go build -o ../../bin/friendservice/friendservice.a friendservice
cd ..

echo '--------------go build chatservice------------'
cd chatservice
go build -o ../../bin/chatservice/chatservice.a chatservice
cd ..

echo '--------------go build datasync------------'
cd datasync
go build -o ../../bin/datasync/datasync.a datasync
cd ..