package verification

/**
*key:
USERLOGIN_ONLINE_INFO,USERLOGIN_ONLINE_TOKEN,USERLOGIN_ONLINE_LASTUPDATE
*values:
useriid,token,nowtimestamp
*result
0:succ 1:user online
**/
const REDIS_LOGIN_SC = `
local tnow = tonumber(ARGV[3])
local lt = redis.call('HGET',KEYS[1],KEYS[3])
if(lt)
then
	if(tonumber(lt)+15000 > tnow)
	then
		return {'1'}
	end
end
redis.call('HMSET',KEYS[1],KEYS[2],ARGV[2],KEYS[3],ARGV[3])
redis.call('EXPIRE',KEYS[1],604800)
return {'0'}
`
