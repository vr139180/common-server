protoc --proto_path=./proto --cpp_out=./cpp ./proto/*.proto

protoc --proto_path=./proto --java_out=./java ./proto/*.proto

protoc --proto_path=./proto --go_out=./go ./proto/*.proto

protoc --proto_path=./proto --csharp_out=./csharp db_internal.proto global_internal.proto proto_iid.proto user_login.proto user_building.proto user_pet.proto user_chat.proto mail_system.proto item_warehouse.proto friend_system.proto task_system.proto matchmaking_system.proto