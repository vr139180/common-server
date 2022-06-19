package main

import (
	"chatservice/chatservice"
	"chatservice/g"
	"cmslib/server"
	"cmslib/utilc"
	"flag"
)

func main() {
	//utilc.GDebugPath = "D:\\workspace\\common_server\\server\\go\\bin\\chatservice"
	var debugPath = flag.String("debug_path", "", "in debug mode, use to set workspace base path.")
	flag.Parse()
	if debugPath != nil {
		utilc.GDebugPath = *debugPath
	}

	app := new(chatservice.ChatService)
	g.InitAppGlobal(app)

	svr := server.NewServerWrapper(app)
	err := svr.InitApp()
	if err != nil {
		panic(err)
	}

	svr.Run()

	svr.UnIntApp()
	g.UnInitAppGlobal()
}
