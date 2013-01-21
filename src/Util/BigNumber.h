/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: BigNumber.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "Common.h"

struct bignum_st;

class BigNumber
{
    public:
        BigNumber();
        BigNumber(const BigNumber &bn);
        BigNumber(uint32);
        ~BigNumber();

        void SetDword(uint32);
        void SetQword(uint64);
        void SetBinary(const uint8 *bytes, int len);
        void SetHexStr(const char *str);

        void SetRand(int numbits);

        BigNumber operator=(const BigNumber &bn);

        BigNumber operator+=(const BigNumber &bn);
        BigNumber operator+(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t += bn;
        }
        BigNumber operator-=(const BigNumber &bn);
        BigNumber operator-(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t -= bn;
        }
        BigNumber operator*=(const BigNumber &bn);
        BigNumber operator*(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t *= bn;
        }
        BigNumber operator/=(const BigNumber &bn);
        BigNumber operator/(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t /= bn;
        }
        BigNumber operator%=(const BigNumber &bn);
        BigNumber operator%(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t %= bn;
        }

        bool isZero() const;

        BigNumber ModExp(const BigNumber &bn1, const BigNumber &bn2);
        BigNumber Exp(const BigNumber &);

        int GetNumBytes(void);

        struct bignum_st *BN() { return _bn; }

        uint32 AsDword();
        uint8* AsByteArray(int minSize = 0, bool reverse = true);

        const char *AsHexStr();
        const char *AsDecStr();

    private:
        void ClearArraysMap();
        struct bignum_st *_bn;
        std::map<int, uint8*> arraysMap;
};