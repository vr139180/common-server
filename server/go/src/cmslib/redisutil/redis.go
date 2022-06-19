//redis util

package redisutil

import (
	"cmslib/logx"
	"context"
	"fmt"
	"strconv"
	"time"

	"github.com/go-redis/redis/v8"
)

type RedisUtil struct {
	rdv *redis.Client
	ctx context.Context
}

func NewRedisUtil(ip string, port int, pwd string, db int) (*RedisUtil, error) {
	ru := RedisUtil{}

	ru.ctx = context.Background()
	ru.rdv = redis.NewClient(&redis.Options{
		Addr:     ip + ":" + strconv.Itoa(port),
		Password: pwd,
		DB:       db,
	})

	_, err := ru.rdv.Ping(ru.ctx).Result()
	if err != nil {
		logx.Errorf("init redis[%s:%d db:%d] client failed", ip, port, db)
		return nil, err
	}

	logx.Infof("init redis[%s:%d db:%d] client success", ip, port, db)
	return &ru, nil
}

func (r *RedisUtil) Expire(key string, t time.Duration) (bool, error) {
	return r.rdv.Expire(r.ctx, key, t*time.Second).Result()
}

func (r *RedisUtil) KeyExist(key string) (bool, error) {
	cnt, err := r.rdv.Exists(r.ctx, key).Result()
	if err != nil {
		return false, err
	} else {
		return (cnt > 0), nil
	}
}

// add k v
func (r *RedisUtil) Add(key string, val string) error {
	_, err := r.rdv.Set(r.ctx, key, val, 0).Result()
	return err
}

func (r *RedisUtil) AddNX(key string, val interface{}, t time.Duration) bool {
	rt, err := r.rdv.SetNX(r.ctx, key, val, t).Result()
	if err != nil {
		return false
	}
	return rt
}

func (r *RedisUtil) AddEX(key string, val interface{}, t time.Duration) bool {
	rt, err := r.rdv.SetEX(r.ctx, key, val, t*time.Second).Result()
	if err != nil {
		return false
	}
	return rt == "OK"
}

func (r *RedisUtil) GetKeys(keys []string) (map[string]interface{}, error) {
	dat := make(map[string]interface{})
	d, err := r.rdv.MGet(r.ctx, keys...).Result()
	if err != nil {
		return dat, err
	}

	for i, k := range keys {
		dat[k] = d[i]
	}

	return dat, nil
}

func (r *RedisUtil) Eval(sc string, keys []string, args ...interface{}) ([]string, error) {
	return r.rdv.Eval(r.ctx, sc, keys, args...).StringSlice()
}

func (r *RedisUtil) Get(key string) (string, error) {
	return r.rdv.Get(r.ctx, key).Result()
}

func (r *RedisUtil) DelZsetLessThen(key string, val int64) (int64, error) {
	return r.rdv.ZRemRangeByScore(r.ctx, key, "0", strconv.FormatInt(val, 10)).Result()
}

func (r *RedisUtil) GetZsetAllMOfKey(key string) ([]string, error) {
	return r.rdv.ZRange(r.ctx, key, 0, -1).Result()
}

func (r *RedisUtil) GetZsetGreatMOfKey(key string, score string) ([]string, error) {
	return r.rdv.ZRangeByScore(r.ctx, key, &redis.ZRangeBy{Min: ("(" + score), Max: "+inf"}).Result()
}

func (r *RedisUtil) AddZset(key string, member string, score float64) (int64, error) {
	return r.rdv.ZAdd(r.ctx, key, &redis.Z{Member: member, Score: score}).Result()
}

func (r *RedisUtil) ZsetDel(key string, members []interface{}) (int64, error) {
	return r.rdv.ZRem(r.ctx, key, members...).Result()
}

func (r *RedisUtil) BuildKey(f string, val ...interface{}) string {
	return fmt.Sprintf(f, val...)
}

func (r *RedisUtil) HGet(key string, field string) (string, error) {
	return r.rdv.HGet(r.ctx, key, field).Result()
}

func (r *RedisUtil) HGetAll(key string) (map[string]string, error) {
	return r.rdv.HGetAll(r.ctx, key).Result()
}

func (r *RedisUtil) HSet(key string, field string, val interface{}) (int64, error) {
	return r.rdv.HSet(r.ctx, key, field, val).Result()
}

func (r *RedisUtil) HDel(key string, field []string) (int64, error) {
	return r.rdv.HDel(r.ctx, key, field...).Result()
}
