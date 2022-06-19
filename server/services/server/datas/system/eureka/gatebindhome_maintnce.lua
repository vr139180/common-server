local totlenum = tonumber(ARGV[4])
local gnum = tonumber(ARGV[5])
local sid = ARGV[1]
local typ = tonumber(ARGV[2])
local homeid
local gateid
local hgid
redis.call('SREM',KEYS[1],sid)
redis.call('SREM',KEYS[2],sid)
if( typ == 1)
then
	homeid = redis.call('HGET',KEYS[4],sid)
	if(homeid)
	then
		gateid = redis.call('HGET',KEYS[4],homeid)
		redis.call('SADD',KEYS[2],homeid)
	end
else
	gateid = redis.call('HGET',KEYS[4],sid)
	if(gateid)
	then
		homeid = redis.call('HGET',KEYS[4],gateid)
		redis.call('SADD',KEYS[1],gateid)
	end
end
if(homeid or gateid)
then
	hgid = homeid..'#'..gateid
	redis.call('HDEL',KEYS[4],homeid,gateid)
end
if(hgid)
then
	local wn = redis.call('ZSCORE',KEYS[5],hgid)
	if(not wn)
	then
		wn = 0
	end
	local fn = redis.call('ZSCORE',KEYS[6],hgid)
	if(not fn)
	then
		fn = 0
	end
	redis.call('ZREM',KEYS[5],hgid)
	redis.call('ZREM',KEYS[6],hgid)
	redis.call('DECRBY',KEYS[3],(wn+fn))
	return {'0',homeid,gateid}
else
	return {'1','0','0'}
end