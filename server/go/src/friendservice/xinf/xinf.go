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


package xinf

type IFriendProcessor interface {
	ProcessFriend()
}

type IDBProcessor interface {
	RunInDBProcessor()
}

//0b0000000000000000000001111111111111111111111111111111111111111111
const USERGATE_MASK_ZERO_H = uint64(0x7FFFFFFFFFF)

//0b1111111111111111111110000000000000000000000000000000000000000000
const USERGATE_MASK_ZERO_L = uint64(0xFFFFF80000000000)

func ParseUserGate(uidgid uint64) (gateid int64, userid int64) {
	gateid = int64((uidgid & USERGATE_MASK_ZERO_L) >> 43)
	userid = int64((uidgid & USERGATE_MASK_ZERO_H))

	return
}
