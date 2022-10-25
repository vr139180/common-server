#include <cmsLib/hash/HashUtil.h>

#include <map>

S_INT_32 HashUtil::fnv1_32_hash(const std::string& str)
{
	const S_INT_32 p = 16777619;
	S_INT_32 hash = (S_INT_32)2166136261L;
	for (int i = 0; i < str.length(); i++) {
		hash = (hash ^ str[i]) * p;
	}
	hash += hash << 13;
	hash ^= hash >> 7;
	hash += hash << 3;
	hash ^= hash >> 17;
	hash += hash << 5;

	if (hash < 0) {
		hash = std::abs(hash);
	}
	return hash;
}

S_INT_32 HashUtil::int_hash(S_INT_32 key)
{
	key += ~(key << 15);
	key ^= (key >> 10);
	key += (key << 3);
	key ^= (key >> 6);
	key += ~(key << 11);
	key ^= (key >> 16);
	return key;
}

S_INT_32 HashUtil::rs_hash(const std::string& str)
{
	S_INT_32 b = 378551;
	S_INT_32 a = 63689;
	S_INT_32 hash = 0;

	for (int i = 0; i < str.length(); i++)
	{
		hash = hash * a + str[i];
		a = a * b;
	}

	return (hash & 0x7FFFFFFF);
}
