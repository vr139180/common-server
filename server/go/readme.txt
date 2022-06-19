#环境变量设置

GO111MODULE=on
go env -w GOPROXY=https://goproxy.cn
GOPROXY=https://goproxy.cn,direct

#go mod常用命令
	go mod init: 初始化modules
    go mod download: 下载依赖的module到本地cache
    go mod edit: 编辑go.mod文件，选项有-json、-require和-exclude，可以使用帮助go help mod edit
    go mod graph: 以文本模式打印模块需求图
    go mod tidy: 检查，删除错误或者不使用的modules，以及添加缺失的模块
    go mod vendor: 生成vendor目录，将依赖复制到vendor目录下面
    go mod verify: 验证依赖是否正确
    go mod why： 解释为什么需要依赖


    go list -m： 查看主模块的路径
    go list -m -f={{.Dir}}： 查看主模块的根目录
    go list -m all： 查看当前的依赖和版本信息
	
#gomodule代码查看
	settings->搜索 docsTool
	->Go: Docs Tool 改为gogetdoc
	
	settings->搜索 use language server
	->Go: Use Language Server 改为非选中状态
	
#protobuff gen-go安装
	go install google.golang.org/protobuf/cmd/protoc-gen-go@latest
	
githua token :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