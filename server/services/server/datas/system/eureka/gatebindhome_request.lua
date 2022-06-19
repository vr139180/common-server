local btoken = ARGV[3]
--gateid in waitlist
if(redis.call('SISMEMBER',KEYS[1],ARGV[1]) == 0)
then
	return {'1'}
end
--homeid exists
if(not redis.call('ZSCORE',KEYS[4],ARGV[2]))
then
	return {'2'}
end
local hgid = ARGV[2]..'#'..ARGV[1]
redis.call('SREM',KEYS[1],ARGV[1])
redis.call('ZADD',KEYS[3],btoken,hgid)
return {'0'}