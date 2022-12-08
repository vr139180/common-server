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

package channel

import (
	"cmslib/protocolx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
)

type PreAllocChannel struct {
	channelBase
}

func newPreAllocChannel(t ChannelType, cid int64, cind int) (pc *PreAllocChannel) {
	pc = new(PreAllocChannel)
	pc.InitBase(t, cid, cind)

	return
}

func (cb *PreAllocChannel) UserSay(pro *protocolx.NetProtocol) {
	say := cb.getUserOfMsg(pro)
	if say == nil {
		return
	}

	msg := pro.Msg.(*gpro.Chat_UserMsgSay)
	item := cb.saveMessage(say.GetUserIid(), msg)

	tnow := utilc.GetTimestamp()
	for iu := cb.usersLink.GetHeadElement(); iu != nil; {
		uinfo := iu.(*UserInfo)
		if !uinfo.IsActived(tnow) {
			break
		}

		uinfo.SendChatMsg(item, cb)

		iu = iu.DLGetNextObj()
	}
}
