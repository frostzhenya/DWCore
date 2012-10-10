
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include "..\Common.h"

class BigNumber;

class Sha1Hash
{
    public:
        Sha1Hash();
        ~Sha1Hash();

        void UpdateBigNumbers(BigNumber *bn0, ...);

        void UpdateData(const unsigned char *dta, int len);
        void UpdateData(const std::string &str);

        void Initialize();
        void Finalize();

        unsigned char *GetDigest(void) { return mDigest; };
        int GetLength(void) { return SHA_DIGEST_LENGTH; };

    private:
        SHA_CTX mC;
        unsigned char mDigest[SHA_DIGEST_LENGTH];
};