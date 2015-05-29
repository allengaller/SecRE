#include <iostream>
#include <string>
#include <string.h>
#include <stdint.h>

#include "Digest.h"

using namespace std;

#define SSE_CUCKOO_KEY_LEN 256

namespace caravel{

class SSECuckoo
{

private:

    //Data structure Info

    char *m_pAll;

    //0 means the first  1 means the second
    char *m_Line[2];

    uint32_t *m_arTempXor;
    char *m_arMask;

    uint32_t m_uiLineCnt;
    uint32_t m_uiAllCnt;
    uint32_t m_uiSizePerBucket;
    uint32_t m_uiSizePerEntry;

    char *m_TempGet;

    uint32_t m_uiW;
    uint32_t m_uiB;
    uint32_t m_uiS;
    uint32_t m_uiMaxKick;

    uint32_t m_uiFlag;

    uint64_t m_ulMemSize;

    uint32_t m_uiCurKick;

    //Encryption Info

    char m_Key1[SHA256_DIGEST_LENGTH];
    char m_Key2[SHA256_DIGEST_LENGTH];
    char m_Key3[SHA256_DIGEST_LENGTH];
    
    
    //Insert Info

    bool _Move(uint32_t uiLine, uint32_t uiI, uint32_t uiB, uint32_t uiKick);

    inline char *_SetEntrys(char *pBucket, uint32_t &uiB);
    inline char *_GetEntrys(char *pBucket, char *pMask);

public:
    SSECuckoo(void);
    ~SSECuckoo(void);

    uint32_t GetKick();

    uint64_t Size();
    uint64_t Size(uint32_t uiNum, double dLoad, uint32_t uiB, uint32_t uiS);

    //Init the data structure and return the real size
    //p is the pointer of Memory that holds the Index
    //uiW is the capacity of buckets in one array
    //uiB is the number of entries in one bucket
    //uiS is the size of entry
    //uiMaxKick is the threshold for kickout operations.
    //return the memory size of the Index
    uint64_t Init(char *p, uint32_t uiW, uint32_t uiB, uint32_t uiS, uint32_t uiMaxKick, bool bReset = true);

    //uiNum is the number of all data records. dPercent is the load factor.
    uint64_t Init(char *p, uint32_t uiNum, double dLoad, uint32_t uiB, uint32_t uiS, uint32_t uiMaxKick, bool bReset = true);



    //MasterKey will generate all the key. Just for Demo ^_^.
    void SetKey(string sMasterKey);

    //Put Key and Value
    //pKey is the pointer of fingerprint, uiKey is the length of pointer for safe usage.
    //uiC is the number of data chunks. A data record is split into uiC chunks.
    //pVal is the pointer of value (pseudonym), uiVal is the length of pointer for safe usage.
    // *important* : uiVal can not be greater than uiS (the size of each entry).
    int Put(char *pKey, uint32_t uiKey, uint32_t uiCnt, char **parVal, uint32_t uiVal);

    bool Get(char *pKey, uint32_t uiKey, uint32_t uiCnt, char **parRet);

    int Encrypt();

};



}

