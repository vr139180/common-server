package main

import (
	"cmslib/server"
	"cmslib/utilc"
	"datasync/datasync"
	"datasync/g"
	"flag"
)

func main() {
	var debugPath = flag.String("debug_path", "", "in debug mode, use to set workspace base path.")
	flag.Parse()
	if debugPath != nil {
		utilc.GDebugPath = *debugPath
	}

	app := new(datasync.DataSync)
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
