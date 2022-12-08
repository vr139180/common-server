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

import "gamelib/protobuf/gpro"

type ChannelType int

const (
	ChannelTypeInvalide_index = ChannelType(-1)
	ChannelTypeGlobal_Indx    = ChannelType(0)
	ChannelTypeChannel_Index  = ChannelType(1)
	ChannelTypeUnion_Index    = ChannelType(2)
	ChannelTypeSingle_Index   = ChannelType(3)
	ChannelTypeCustom_Index   = ChannelType(4)
	ChannelTypeMax_Index      = ChannelType(5)
)

func GetChannelType(ct gpro.ChatChannelType) (t ChannelType) {
	switch ct {
	case gpro.ChatChannelType_ChatChannelT_Global:
		t = ChannelTypeGlobal_Indx
	case gpro.ChatChannelType_ChatChannelT_Channel:
		t = ChannelTypeChannel_Index
	case gpro.ChatChannelType_ChatChannelT_Union:
		t = ChannelTypeUnion_Index
	case gpro.ChatChannelType_ChatChannelT_Single:
		t = ChannelTypeSingle_Index
	case gpro.ChatChannelType_ChatChannelT_Custom:
		t = ChannelTypeCustom_Index
	default:
		t = ChannelTypeInvalide_index
	}

	return
}

func GetCTypeOfChannelType(t ChannelType) (ct gpro.ChatChannelType) {
	switch t {
	case ChannelTypeGlobal_Indx:
		ct = gpro.ChatChannelType_ChatChannelT_Global
	case ChannelTypeChannel_Index:
		ct = gpro.ChatChannelType_ChatChannelT_Channel
	case ChannelTypeUnion_Index:
		ct = gpro.ChatChannelType_ChatChannelT_Union
	case ChannelTypeSingle_Index:
		ct = gpro.ChatChannelType_ChatChannelT_Single
	case ChannelTypeCustom_Index:
		ct = gpro.ChatChannelType_ChatChannelT_Custom
	default:
		ct = gpro.ChatChannelType_ChatChannelT_Custom
	}

	return
}

func IsValidateChannelType(t ChannelType) bool {
	return (t > ChannelTypeInvalide_index && t < ChannelTypeMax_Index)
}
