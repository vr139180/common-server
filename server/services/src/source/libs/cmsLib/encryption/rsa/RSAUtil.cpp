#include "cmsLib/encryption/rsa/RSAUtil.h"
/*
#include <openssl/bio.h>

RSADecryptFromPrivKey::RSADecryptFromPrivKey():
	rsa_privatekey( 0)
{
}

RSADecryptFromPrivKey::~RSADecryptFromPrivKey()
{
	if( rsa_privatekey != 0)
	{
		RSA_free( rsa_privatekey);
		rsa_privatekey =0;
	}
}

bool RSADecryptFromPrivKey::init_rsa(const char* privkey)
{
	BIO *bio = BIO_new_mem_buf( (void*)privkey, (int)strlen(privkey));
	rsa_privatekey = PEM_read_bio_RSAPrivateKey( bio, NULL, 0, NULL);
	BIO_free( bio);

	return rsa_privatekey != 0;
}

std::string RSADecryptFromPrivKey::decrypt( const unsigned char* txt, int length )
{
	if( rsa_privatekey == 0 || txt == 0 || length == 0 )
		return "";

	int len1 = length;
	if( len1 > 1024)
		return "";

	std::string ret ="";

	char* buf2 =new char[len1*2];
	int r =RSA_private_decrypt( len1, (const unsigned char*)txt, (unsigned char*)buf2,
		rsa_privatekey, RSA_PKCS1_PADDING);

	if( r >= 0)
	{
		buf2[r] ='\0';
		ret =buf2;
	}

	delete buf2;

	return ret;
}

//--------------------------------------------------------------------------------

RSACryptFromPubKey::RSACryptFromPubKey():
	rsa_pubkey( 0)
{
}

RSACryptFromPubKey::~RSACryptFromPubKey()
{
	if( rsa_pubkey != 0)
	{
		RSA_free( rsa_pubkey);
		rsa_pubkey =0;
	}
}

bool RSACryptFromPubKey::init_rsa(const char* pubkey)
{
	BIO *bio = BIO_new_mem_buf( (void*)pubkey, (int)strlen(pubkey));
	rsa_pubkey = PEM_read_bio_RSA_PUBKEY( bio, NULL, 0, NULL);
	BIO_free( bio);

	return rsa_pubkey != 0;
}

std::string RSACryptFromPubKey::crypt( const char* txt)
{
	if( rsa_pubkey == 0 || txt == 0 || strlen( txt) == 0)
		return "";

	int len1 =strlen( txt);
	if( len1 > 1024)
		return "";

	std::string ret ="";

	int maxSize = RSA_size( rsa_pubkey);
	char* buf2 =new char[maxSize+1];

	int r =RSA_public_encrypt( len1, (const unsigned char*)txt, (unsigned char*)buf2,
		rsa_pubkey, RSA_PKCS1_PADDING);

	if( r >= 0)
	{
		buf2[r] ='\0';
		ret =buf2;
	}

	delete buf2;

	return ret;
}

*/