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

//fnv1-32hash算法
func HashFNV1_32(str string) int32 {
	var p int32 = 16777619
	var hash int32 = 0
	{
		vv := 2166136261
		hash = int32(vv)
	}

	for _, s := range []byte(str) {
		hash = (hash ^ int32(s)) * p
	}

	hash += hash << 13
	hash ^= hash >> 7
	hash += hash << 3
	hash ^= hash >> 17
	hash += hash << 5

	if hash < 0 {
		hash *= -1
	}

	return hash
	/*
		h := fnv.New32()
		h.Write([]byte(str))
		return int32(h.Sum32())
	*/
}
