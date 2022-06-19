package dtask

import (
	"cmslib/logx"
	"cmslib/redisutil"
	"cmslib/utilc"
	"encoding/json"
	"errors"
	"gamelib/redis"
	"time"
)

//redis实现的简单分布式任务
type DTaskRedis struct {
	//service id
	ServiceIid int64 `json:"iid"`
	//start time
	StartTime int64 `json:"start"`
	//task name
	Name string `json:"name"`
	//task step, is second
	TaskStep int `json:"step"`
}

func newDTaskRedis(name string, sid int64, step int) (task *DTaskRedis) {
	task = new(DTaskRedis)

	task.Name = name
	task.ServiceIid = sid
	task.TaskStep = step
	task.StartTime = utilc.GetTimestamp()

	return
}

//task, is own task, error
func BuildDTaskRedis(name string, sid int64, step int, redisx *redisutil.RedisUtil) (*DTaskRedis, bool, error) {
	taskkey := redisx.BuildKey(redis.GLOBAL_DTASK_PREFIX, name)
	str, err := redisx.Get(taskkey)
	if err == nil {
		var obj DTaskRedis
		err = json.Unmarshal([]byte(str), &obj)
		if err != nil {
			logx.Errorf("DTask pase redis content[]%s failed", str)
			return nil, false, err
		}

		return &obj, false, nil
	}

	//save
	td := newDTaskRedis(name, sid, step)
	dt, err := td.ToJsonString()
	if err != nil {
		return nil, false, err
	}

	if redisx.AddNX(taskkey, dt, time.Duration(step*1000)*time.Millisecond) {
		return td, true, nil
	}

	return nil, false, errors.New("no dtask error")
}

func (t *DTaskRedis) IsLoopTask() bool {
	return t.TaskStep > 0
}

//randscope is second
func (t *DTaskRedis) NextTaskTimer(randscope int) (dt int64) {
	dt = 0
	if t.TaskStep <= 0 {
		return
	}

	tx := utilc.GetTimestamp() - t.StartTime - int64(t.TaskStep*1000)
	var nt int64 = 0
	if tx >= 0 {
		nt = 1
	} else {
		nt = (tx * -1)
	}

	if randscope > 0 {
		nt += int64(utilc.CMSRandMax(randscope*10) * 100)
	}

	dt = nt
	return
}

func (t *DTaskRedis) ToJsonString() ([]byte, error) {
	return json.Marshal(t)
}
