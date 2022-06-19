package main

import (
	"cmslib/server"
	"cmslib/utilc"
	"flag"
	"loginservice/g"
	"loginservice/loginservice"
)

func main() {
	//utilc.GDebugPath = "D:\\workspace\\common_server\\server\\go\\bin\\loginservice"
	var debugPath = flag.String("debug_path", "", "in debug mode, use to set workspace base path.")
	flag.Parse()
	if debugPath != nil {
		utilc.GDebugPath = *debugPath
	}

	app := new(loginservice.LoginService)
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
