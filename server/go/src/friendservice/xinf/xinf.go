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
