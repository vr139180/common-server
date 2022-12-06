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

package dbserialize

import (
	"cmslib/server"
)

type roleSerializeCmd struct {
	roleiid string
}

type serializeLoop struct {
	//单个的系统消息处理
	sysch chan interface{}
	dbcmd chan interface{}

	parent *DBSerializePool
}

func newSerializeLoop(p *DBSerializePool) (l *serializeLoop) {
	l = new(serializeLoop)
	l.sysch = make(chan interface{}, 1)
	l.dbcmd = make(chan interface{}, 1024)
	l.parent = p

	return
}

func (l *serializeLoop) run() {

	defer func() {
		l.parent.loopWG.Done()
	}()

	var err error
	for {
		select {
		case i := <-l.sysch:
			switch v := i.(type) {
			case error:
				err = v
			}
		case v := <-l.dbcmd:
			switch d := v.(type) {
			case roleSerializeCmd:
				l.doRoleService(d.roleiid)
			}
		}

		if err == server.ErrExitApp {
			break
		}
	}
}
