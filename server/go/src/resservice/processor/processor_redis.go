package processor

/**
*key:
HOMEGATE_WATERSLOT,HOMEGATE_FLEXSLOT,HOMEGATE_USERSLOT_AUTH,GLOBAL_USERSLOT_FREE
*values:
session,timestamp,0,0
*result
0:succ hid,type 1:no slot
**/
const USERSLOT_REQUEST_SCRIPT = `
local ts=tonumber(ARGV[2])
local d1 = redis.call('ZREVRANGEBYSCORE',KEYS[1],'+inf',1,'LIMIT',0,1)
local len = table.getn(d1)
local hid
local type
if(len > 0)
then
	hid = d1[1]
	type = '1'
else
	local d2 = redis.call('ZRANGEBYSCORE',KEYS[1],1,'+inf','LIMIT',0,1)
	len = table.getn(d2)
	if(len > 0)
	then
		hid = d2[1]
		type = '2'
	end
end
if(hid)
then
	redis.call('ZINCRBY',KEYS[1],-1,hid)
	local ky = string.format(KEYS[3],hid)
	redis.call('ZADD',ky,ts,ARGV[1])
	redis.call('DECR',KEYS[4])
	return {'0',hid,type}
else
	return {'1','0','0'}
end
`

/**
*key:
HOMEGATE_USERSLOT_AUTHX
*values:
session
*result
0:succ 1:faild
**/
const USERSLOT_CONFIRM_SCRIPT = `
local del = redis.call('ZREM',KEYS[1],ARGV[1])
if(del == 0)
then
	return {'1'}
else
	return {'0'}
end
`

/*
*key:
HOMEGATE_WATERSLOT,HOMEGATE_FLEXSLOT,HOMEGATE_USERSLOT_AUTHX,GLOBAL_USERSLOT_FREE
*values:
succ[0:succ 1:fail],session,type[1:waterslot 2:flexslot],homeid+gateid
*result
0:succ 1:faild

const USERSLOT_CONFIRM_SCRIPT = `
local rt = tonumber(ARGV[1])
local ty = tonumber(ARGV[3])
local nums = '0'
local del = redis.call('ZREM',KEYS[3],ARGV[2])
if(del == 0)
then
	return {'1','0'}
end
if(ty == 1)
then
	if(rt ~= 0)
	then
		if(redis.call('ZSCORE',KEYS[1],ARGV[4]))
		then
			nums = redis.call('ZINCRBY',KEYS[1],1,ARGV[4])
			redis.call('INCR',KEYS[4])
		end
	end
else
	if(rt ~= 0)
	then
		if(redis.call('ZSCORE',KEYS[2],ARGV[4]))
		then
			nums = redis.call('ZINCRBY',KEYS[2],1,ARGV[4])
			redis.call('INCR',KEYS[4])
		end
	end
end
return {'0',nums}
`
*/

/**
*key:
SERVICE_DETAIL_INFO,HOMEGATE_WATERSLOT,HOMEGATE_FLEXSLOT,GLOBAL_USERSLOT_FREE
*values:
freeslot,sequence,hid
*result
0:succ 1:gate不存在 2:sequence舍弃 3:gate home不存在
**/
const GATEFREESLOT_SYNC = `
local fs = ARGV[1]
local sq = tonumber(ARGV[2])
local gt = redis.call('HGET',KEYS[1],'ext:slotseq')
if( not gt)
then
	return {'1','0'}
end
if(tonumber(gt) >= sq)
then
	return {'2','0'}
end
local nfsx = 0
local nfs = redis.call('ZSCORE',KEYS[2],ARGV[3])
if( not nfs)
then
	nfs = redis.call('ZSCORE',KEYS[3],ARGV[3])
	if(not nfs)
	then
		return {'3','0'}
	else
		nfsx = tonumber(nfs)
		redis.call('ZADD',KEYS[3],fs,ARGV[3])
	end
else
	nfsx = tonumber(nfs)
	redis.call('ZADD',KEYS[2],fs,ARGV[3])
end
redis.call('HSET',KEYS[1],'ext:slotseq',sq)
local fx = fs - nfsx
local ic = redis.call('INCRBY',KEYS[4],fx)
return {'0',ic..''}
`
