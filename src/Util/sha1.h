/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: sha1.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#ifndef _AUTH_SHA1_H
#define _AUTH_SHA1_H

#include "Common.h"
#include <openssl/sha.h>
#include <openssl/crypto.h>

class BigNumber;

class Sha1Hash
{
    public:
        Sha1Hash();
        ~Sha1Hash();

        void UpdateBigNumbers(BigNumber *bn0, ...);

        void UpdateData(const uint8 *dta, int len);
        void UpdateData(const std::string &str);

        void Initialize();
        void Finalize();

        uint8 *GetDigest(void) { return mDigest; };
        int GetLength(void) { return SHA_DIGEST_LENGTH; };

    private:
        SHA_CTX mC;
        uint8 mDigest[SHA_DIGEST_LENGTH];
};
#endif