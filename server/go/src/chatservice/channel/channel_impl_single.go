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

type SingleChannel struct {
	channelBase
}

func newSingleChannel(cid int64, cind int) (pc *SingleChannel) {
	pc = new(SingleChannel)
	pc.InitBase(ChannelTypeSingle_Index, cid, cind)

	return
}

func (cb *SingleChannel) IsPreDefined() bool {
	return false
}

func (cb *SingleChannel) UserSay(pro *protocolx.NetProtocol) {

	rid := pro.GetRoleIid()

	msg := pro.Msg.(*gpro.Chat_UserMsgSay)
	item := cb.saveMessage(rid, msg)

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
