#ifndef __SHA1SUM__H__
#define __SHA1SUM__H__

#include "HashBase.h"


class SHA1Sum : public HashBase
{
public:
    SHA1Sum() : HashBase(HashBase::SHA1)
    {
    }

    static const int SHA1HashSize = 20;
    static const int SHA1BlockSize = 64;
    virtual size_t GetHashSize() const { return SHA1HashSize; }
    virtual size_t GetBlockSize() const { return SHA1BlockSize; }

private:
    uint32_t Intermediate_Hash[SHA1HashSize/4]; /* Message Digest */
    uint32_t Length_Low;                        /* Message length in bits */
    uint32_t Length_High;                       /* Message length in bits */
    int_least16_t Message_Block_Index;          /* Index into message block array */
    uint8_t Message_Block[SHA1BlockSize];                  /* 512-bit message blocks */
    bool Computed;                              /* Is the digest computed? */
    bool Corrupted;                             /* Is the message digest corrupted? */

public:
    virtual HashStatus Reset();
    virtual HashStatus Input(const uint8_t *, unsigned int);
    virtual HashStatus Result(uint8_t * const Message_Digest);
private:
    void SHA1PadMessage();
    void SHA1ProcessMessageBlock();
};

#endif /* #ifndef __SHA1SUM__H__ */
