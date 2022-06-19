package channel

import (
	"cmslib/datas"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"sync/atomic"
)

type UserActiveChannelProcessor struct {
	ch              *ChannelHolder
	ctype           ChannelType
	cid             int64
	channelNotExist bool
	uinfo           *UserInfo
}

//-----xinf.IChatProcessor-----
func (ua *UserActiveChannelProcessor) ProcessChat() {
	ct := ua.ch.typeChannels[ua.ctype]

	ct.userActive(ua.cid, ua.ch.loopIndex, ua.uinfo)

	//对应的减去预增加的值
	if ua.channelNotExist {
		ct.decPreNum()
	}
}

type UserSayProcessor struct {
	ch    *ChannelHolder
	ctype ChannelType
	cid   int64
	msg   *gpro.Chat_UserMsgSay
}

//-----xinf.IChatProcessor-----
func (us *UserSayProcessor) ProcessChat() {
	ct := us.ch.typeChannels[us.ctype]
	cl := ct.GetChannelByIid(us.cid)
	if cl == nil {
		return
	}

	cl.UserSay(us.msg)
}

//-------------------------------------------------------------------------
//存放各个类型的channel信息，只在processor的goroutine进行存取
type channelsOfType struct {
	ctype ChannelType

	//double link和hashmap组合的channel数据
	channels map[int64]IChannel
	//根据活动情况排列的 双向列表
	//方便对不活跃的channel进行维护
	channelsLink *datas.DoubleLinkUnSafe

	//channel数量
	nums int
	//预先分配的数值，只在分配规则使用
	//新建之后需要减少
	pre_nums int32
}

func newChannelsOfType(c ChannelType) (ct *channelsOfType) {
	ct = new(channelsOfType)
	ct.ctype = c
	ct.channels = make(map[int64]IChannel)
	ct.channelsLink = datas.NewDoubleLinkUnSafe()

	ct.nums = 0
	ct.pre_nums = 0

	return
}

func (ct *channelsOfType) GetChannelByIid(cid int64) IChannel {
	d, ok := ct.channels[cid]
	if !ok {
		return nil
	}

	return d
}

func (ct *channelsOfType) GetCalcNums() int {
	return ct.nums + int(atomic.LoadInt32(&ct.pre_nums))
}

func (ct *channelsOfType) incPreNum() {
	atomic.AddInt32(&ct.pre_nums, 1)
}

func (ct *channelsOfType) decPreNum() {
	nv := atomic.AddInt32(&ct.pre_nums, -1)
	if nv < 0 {
		atomic.StoreInt32(&ct.pre_nums, 0)
	}
}

func (ct *channelsOfType) userActive(cid int64, cindex int, uinfo *UserInfo) {
	cl := ct.GetChannelByIid(cid)
	if cl == nil {
		cl = NewOneChannel(ct.ctype, cid, cindex)
		dl := cl.(datas.IDoubleLink)

		ct.channels[cid] = cl
		ct.channelsLink.AddHeadElement(dl)
		ct.nums = len(ct.channels)
	}

	tnow := utilc.GetTimestamp()

	cl.UserActive(uinfo, tnow)

	if cl.NeedMoveChannel(tnow) {
		dl := cl.(datas.IDoubleLink)
		ct.channelsLink.ElementMoveToHead(dl)
	}
}

func (ct *channelsOfType) oneDayMaintance(tnow int64) (delcids []int64) {
	delcids = []int64{}
	//从尾部开始
	for e := ct.channelsLink.GetTailElement(); e != nil; {
		cc := e.(IChannel)

		//强制维护
		if !cc.IsActiveTimeout(tnow) {
			break
		}

		cid := cc.GetChannelId()
		delete(ct.channels, cid)
		ct.channelsLink.DelElement(e)

		delcids = append(delcids, cid)

		e = ct.channelsLink.GetTailElement()
	}

	//从头部开始强制维护
	for e := ct.channelsLink.GetHeadElement(); e != nil; e = e.DLGetNextObj() {
		cc := e.(IChannel)
		cc.MaintanceOfflineUsers(tnow, true)
	}

	return
}

//--------------------------------------------------------------------------------------------
//对应processor.loop的channel存储结构
type ChannelHolder struct {
	//对应processor.中的loop index
	loopIndex int

	//长度为ChannelTypeMax_Index的数组
	typeChannels []*channelsOfType
	parent       *ChannelCtrl
}

func newChannelHolder(l int, p *ChannelCtrl) (h *ChannelHolder) {
	h = new(ChannelHolder)
	h.loopIndex = l
	h.parent = p

	for ind := ChannelTypeGlobal_Indx; ind < ChannelTypeMax_Index; ind++ {
		tc := newChannelsOfType(ind)
		h.typeChannels = append(h.typeChannels, tc)
	}

	return
}

func (ch *ChannelHolder) GetChannelBy(t ChannelType, cid int64) IChannel {
	if t <= ChannelTypeInvalide_index || t >= ChannelTypeMax_Index {
		return nil
	}

	return ch.typeChannels[t].GetChannelByIid(cid)
}

func (ch *ChannelHolder) GetChannelNumOfType(t ChannelType) int {
	return ch.typeChannels[t].GetCalcNums()
}

func (ch *ChannelHolder) IncPreNum(t ChannelType) {
	if t <= ChannelTypeInvalide_index || t >= ChannelTypeMax_Index {
		return
	}

	ch.typeChannels[t].incPreNum()
}

func (ch *ChannelHolder) DecPreNum(t ChannelType) {
	if t <= ChannelTypeInvalide_index || t >= ChannelTypeMax_Index {
		return
	}

	ch.typeChannels[t].decPreNum()
}

func (ch *ChannelHolder) OneDayMaintance() {
	tnow := utilc.GetTimestamp()
	for _, ct := range ch.typeChannels {
		dls := ct.oneDayMaintance(tnow)
		//调整映射关系
		if len(dls) > 0 {
			ch.parent.UnBindChannelId(ct.ctype, dls)
		}
	}
}
