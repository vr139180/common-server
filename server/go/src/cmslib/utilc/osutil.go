package utilc

import (
	"os"
	"path/filepath"
)

//TODO:
var GDebugPath = ""

// 获取当前工作目录
func GetBinPath() (string, error) {
	ex, err := os.Executable()
	if err != nil {
		return "", err
	}

	fp := filepath.Dir(ex)

	realPath, err := filepath.EvalSymlinks(fp)
	if err != nil {
		return "", err
	}

	// TODO:debug
	if len(GDebugPath) > 0 {
		realPath = GDebugPath
	}

	return realPath, nil
}
