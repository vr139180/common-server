local totlenum = tonumber(ARGV[4])
local gnum = tonumber(ARGV[5])
local hkey = ARGV[2]..'#'..ARGV[1]
local tokenx = tonumber(ARGV[3])
local btoken = redis.call('ZSCORE',KEYS[3],hkey)
btoken = tonumber(btoken)
if(not btoken)
then
	return {'1'}
end
if(btoken ~= tokenx)
then
	return {'2'}
end
redis.call('ZREM',KEYS[3],hkey)
if(not redis.call('ZSCORE',KEYS[8],ARGV[2]))
then
	return {'3'}
end
if(not redis.call('ZSCORE',KEYS[9],ARGV[1]))
then
	return {'3'}
end
redis.call('HMSET',KEYS[4],ARGV[1],ARGV[2],ARGV[2],ARGV[1])
local cnt = redis.call('ZCOUNT',KEYS[5],0,999999999999999)
if(cnt >= gnum)
then
	redis.call('ZADD',KEYS[6],totlenum,hkey)
else
	redis.call('ZADD',KEYS[5],totlenum,hkey)
end
redis.call('INCRBY',KEYS[7],totlenum)
return {'0'}