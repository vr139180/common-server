package mailbox

import "gamelib/protobuf/gpro"

type MailType int32

const (
	MailTypeInvalide_index = MailType(-1)
	MailTypeSystem_Indx    = MailType(0)
	MailTypeUnion_Index    = MailType(1)
	MailTypeMax_Index      = MailType(2)
)

func GetMailType(ct gpro.MailType) (t MailType) {
	switch ct {
	case gpro.MailType_MailType_System:
		t = MailTypeSystem_Indx
	case gpro.MailType_MailType_Union:
		t = MailTypeUnion_Index
	default:
		t = MailTypeInvalide_index
	}

	return
}

func GetCTypeOfMailType(t MailType) (ct gpro.MailType) {
	switch t {
	case MailTypeSystem_Indx:
		ct = gpro.MailType_MailType_System
	case MailTypeUnion_Index:
		ct = gpro.MailType_MailType_Union
	default:
		ct = gpro.MailType_MailType_System
	}

	return
}

func IsValidateMailType(t MailType) bool {
	return (t > MailTypeInvalide_index && t < MailTypeMax_Index)
}

type MailBoxType int

const (
	MailBoxType_System = MailBoxType(0)
	MailBoxType_User   = MailBoxType(1)
)

//---------------------------redis key define----------------------------------------
