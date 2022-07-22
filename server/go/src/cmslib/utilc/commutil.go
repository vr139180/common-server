package utilc

import (
	"cmslib/logx"
	"encoding/hex"
	"math/rand"
	"strconv"
	"strings"
	"time"
	"unsafe"

	uuid "github.com/satori/go.uuid"
	"google.golang.org/protobuf/proto"
)

func DeleteElementFromSlice(source []interface{}, e interface{}) (r []interface{}) {
	var ind int = -1
	for i, v := range source {
		if v == e {
			ind = i
			break
		}
	}

	if ind == -1 {
		return source
	}

	if len(source) == 1 {
		r = []interface{}{}
		return
	}

	if ind == 0 {
		r = source[ind+1:]
	} else if ind == len(source)-1 {
		r = source[:ind]
	} else {
		r = append(source[:ind], source[ind+1:])
	}

	return
}

func SpliteStrToInt64(src string, sep string) []int64 {
	ret := []int64{}
	hdstr := strings.Split(src, sep)
	for _, val := range hdstr {
		iv, err := strconv.ParseInt(val, 10, 64)
		if err == nil {
			ret = append(ret, iv)
		}
	}

	return ret
}

func UuidGen() string {
	u1 := uuid.NewV4()
	buf := make([]byte, 32)
	hex.Encode(buf[0:], u1[0:])
	return string(buf)
}

//[0, rmax)
func CMSRandMax(rmax int) int {
	rand.Seed(time.Now().UnixNano())
	return rand.Intn(rmax)
}

func GetTimestamp() int64 {
	return time.Now().UTC().UnixMilli()
}

func GetTimeSecond() int32 {
	return int32(time.Now().UTC().Unix())
}

func GetTimeSecond2(t time.Time) int32 {
	if t.IsZero() {
		return 0
	}

	return int32(t.UTC().Unix())
}

func SecondToTime(ts int32) (t time.Time) {
	if ts <= 0 {
		return
	}

	t = time.Unix(int64(ts), int64(0))
	return
}

/*
func StringToByte(str string) []byte {
	return *(*[]byte)(unsafe.Pointer(&str))
}
*/

func I64ToString(v int64) string {
	return strconv.FormatInt(v, 10)
}

func StringToI64(s string, def int64) int64 {
	v, err := strconv.ParseInt(s, 10, 64)
	if err != nil {
		return def
	}
	return v
}

func ProtoToBytes(msg proto.Message) []byte {
	dat, err := proto.Marshal(msg)
	if err != nil {
		logx.Errorf("********************protobuff Marshal failed:%s", err.Error())
		return nil
	}

	return dat
}

func ProtoToString(msg proto.Message) string {
	dat, err := proto.Marshal(msg)
	if err != nil {
		logx.Errorf("********************protobuff Marshal failed:%s", err.Error())
		return ""
	}

	return *(*string)(unsafe.Pointer(&dat))
}

func BytesToProto(d []byte, msg proto.Message) error {
	err := proto.Unmarshal(d, msg)
	if err != nil {
		logx.Errorf("********************protobuff Unmarshal failed:%s", err.Error())
	}

	return err
}
