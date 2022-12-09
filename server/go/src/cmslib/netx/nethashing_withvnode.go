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

package netx

import (
	"cmslib/utilc"
	"fmt"
	"strconv"

	rbt "github.com/emirpasic/gods/trees/redblacktree"
	utils "github.com/emirpasic/gods/utils"
)

type netVNodeWrap struct {
	Key             int64
	HashKey         int32
	VirtualSequence int32
}

func newNewVNodeWrap(k int64, s int32) *netVNodeWrap {
	v := new(netVNodeWrap)

	v.Key = k
	v.VirtualSequence = s

	str := fmt.Sprintf("%s#%d", strconv.FormatInt(v.Key, 10), v.VirtualSequence)
	v.HashKey = utilc.HashFNV1_32(str)

	return v
}

type NetHashingWithVNode struct {

	//虚拟节点数
	NetNodes *rbt.Tree

	RealNodes []*netVNodeWrap

	//虚拟节点数量
	vnodes int
}

func NewNetHashingWithVNode(vnodes int) *NetHashingWithVNode {
	node := new(NetHashingWithVNode)

	node.vnodes = vnodes
	node.NetNodes = rbt.NewWith(utils.Int32Comparator)

	return node
}

func (n *NetHashingWithVNode) AddRealNode(key int64) {
	vnode := newNewVNodeWrap(key, int32(0))
	n.RealNodes = append(n.RealNodes, vnode)
}

func (n *NetHashingWithVNode) ClearRealNode() {
	n.RealNodes = []*netVNodeWrap{}
}

func (n *NetHashingWithVNode) BuildNetHashing() {
	n.NetNodes.Clear()

	for _, v := range n.RealNodes {
		for ii := 0; ii < n.vnodes; ii++ {
			vnode := newNewVNodeWrap(v.Key, int32(ii))
			n.NetNodes.Put(v.HashKey, vnode)
		}
	}
}

func (n *NetHashingWithVNode) GetNetNodebyHash(h int32) int64 {
	if n.NetNodes.Size() == 0 {
		return 0
	}

	v, found := n.NetNodes.Ceiling(h)
	if !found {
		v = n.NetNodes.Root
	}

	vnode := v.Value.(*netVNodeWrap)

	return vnode.Key
}

func (n *NetHashingWithVNode) GetNetNodeByKey(k int64) int64 {
	h := utilc.HashFNV1_32(strconv.FormatInt(k, 10))
	return n.GetNetNodebyHash(h)
}
