// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

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