#ifndef _RSAUTIL_H__
#define _RSAUTIL_H__
/**
#include "cmsLib/GlobalSettings.h"

#include <openssl/pem.h>
#include <string>

class RSADecryptFromPrivKey
{
public:
	RSADecryptFromPrivKey();
	virtual ~RSADecryptFromPrivKey();

	bool init_rsa( const char* privkey);

	std::string decrypt( const unsigned char* txt, int length );

private:
	RSA* rsa_privatekey;
};

class RSACryptFromPubKey
{
public:
	RSACryptFromPubKey();
	virtual ~RSACryptFromPubKey();

	bool init_rsa( const char* pubkey);

	std::string crypt( const char* txt);

private:
	RSA* rsa_pubkey;
};
*/
#endif	//_RSAUTIL_H__