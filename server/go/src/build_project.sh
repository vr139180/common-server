#!/bin/sh

echo '--------------go build mailservice------------'
cd mailservice
go build -o ../../bin/mailservice/mailservice mailservice
cd ..

echo '--------------go build friendservice------------'
cd friendservice
go build -o ../../bin/friendservice/friendservice friendservice
cd ..

echo '--------------go build chatservice------------'
cd chatservice
go build -o ../../bin/chatservice/chatservice chatservice
cd ..

echo '--------------go build matchmaking service------------'
cd matchmaking
go build -o ../../bin/matchmaking/matchmaking matchmaking
cd ..

echo '--------------go build datasync------------'
cd datasync
go build -o ../../bin/datasync/datasync datasync
cd ..