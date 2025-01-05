#ifndef __HASH_BASE__H__
#define __HASH_BASE__H__

#include <stdlib.h>

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef int int_least16_t;


class HashBase
{
public:
    enum HashFunc
    {
        SHA1,
    };
protected:
    HashBase(HashFunc h) : m_HashFunc(h)
    {
    }
public:
    virtual ~HashBase(void)
    {
    }
public:
    enum HashStatus
    {
        Success = 0,
        Null,            /* Null pointer parameter */
        InputTooLong,    /* input data too long */
        StateError       /* called Input after Result */
    };

    virtual size_t GetHashSize() const = 0;
    virtual size_t GetBlockSize() const = 0;

    virtual HashStatus Reset() = 0;
    virtual HashStatus Input(const uint8_t *, unsigned int) = 0;
    virtual HashStatus Result(uint8_t * const Message_Digest) = 0;

protected:
    HashFunc m_HashFunc;
};


#endif /* #ifndef __HASH_BASE__H__ */
