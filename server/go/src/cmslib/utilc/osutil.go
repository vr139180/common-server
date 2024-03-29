// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

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
