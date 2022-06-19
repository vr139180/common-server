local hgid = ARGV[2]..'#'..ARGV[1]
--gateid in waitlist
if(not redis.call('ZSCORE',KEYS[3],hgid))
then
	return {'1'}
end
redis.call('ZREM',KEYS[3],hgid)
if(redis.call('ZSCORE',KEYS[5],ARGV[1]))
then
	redis.call('SADD',KEYS[1],ARGV[1])
end
if(redis.call('ZSCORE',KEYS[4],ARGV[2]))
then
	redis.call('SADD',KEYS[2],ARGV[2])
end
return {'0'}