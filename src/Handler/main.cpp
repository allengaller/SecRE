#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdint.h>

//getch
#include <termios.h>
#include <unistd.h>

#include "../Caravel/SSECuckoo.h"

#include "../Caravel/ShmCtl.h"

//time 
#include "../Caravel/TimeDiff.h"

//Config
#include "conf.h"

using namespace std;
using namespace caravel;

int getch(){
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

template<typename T>
T PrintAndGet(char *szMsg)
{
    T tRet;
    cout<<szMsg<<endl<<endl;
    cin>>tRet;
    return tRet;
}

void PrintCmdList(){
    system("clear");
    cout<<"©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                          Request Handler Command List                                     ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§     Input Command Below :                                                                            ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         0 Exit                                                                                       ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         1 Init SSECuckoo and Insert Data                                                                ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         2 Encrypt Index                                                                              ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         3 Test Batch Get Requests                                                                     ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         4 Test Single Get Request                                                                     ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         5 Init Index in Shared Memory                                                                 ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                                                                                     ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿"<<endl;
    return;
}

int main(int argc, char **argv)
{
    srand(0);

    SSECuckoo oSSECuckoo;
    char *pMem = NULL;

    uint32_t uiNum;
    double dLoad;
    uint32_t uiB;
    uint32_t uiC;

    char **parKeys = NULL;
    char ***pVal = NULL;

    system("clear");
    int iCmd;
    do{
        PrintCmdList();
        iCmd = getch();
        switch(iCmd)
        {
        case 49:
            {
                //1
                cout<<"Begin to init the index"<<endl;
                uiNum = PrintAndGet<uint32_t>("Please input the number of data records that you want to put in the index. [ Then Press Enter ]");
                dLoad = PrintAndGet<double>("Please input the load factor of index. [ Then Press Enter ]");
                uiB = PrintAndGet<uint32_t>("Please input the number of entries per bucket. [ Then Press Enter ]");
                uiC = PrintAndGet<uint32_t>("Please input the number of chunks for each data record. [ Then Press Enter ]");
                //This work for demo. To Set the split slice.
                uint32_t uiIndexNum = uiNum * uiC;
                uint64_t uiAllMem = oSSECuckoo.Size(uiIndexNum, dLoad, uiB, DEF_DATA_SIZE);
                cout<<"Required memory size : "<<uiAllMem<<endl;
                if(NULL != pMem)
                {
                    delete [] pMem;
                }
                pMem = new char[uiAllMem];
                uiAllMem = oSSECuckoo.Init(pMem, uiIndexNum, dLoad, uiB, DEF_DATA_SIZE, DEF_MAX_KICKOUT);
                cout<<"The allocated memory size: "<<uiAllMem<<endl;
                cout<<"Finish Init."<<endl;

                cout<<"Init random data records for insertion."<<endl;

                //Check parKeys [Key]
                if(NULL != parKeys)
                {
                    for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                    {
                        delete [] parKeys[uiCur];
                    }
                    delete [] parKeys;
                }
                parKeys = new char*[uiNum];
                //Check pVal [Value]
                if(NULL != pVal)
                {
                    for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                    {
                        for(uint32_t uiIdx = 0; uiIdx < uiC; uiIdx++)
                        {
                            delete [] pVal[uiCur][uiIdx];
                        }
                        delete [] pVal[uiCur];
                    }
                    delete [] pVal;
                }
                pVal = new char**[uiNum];
                //Init the data
                for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                {
                    //init the key
                    parKeys[uiCur] = new char[DEF_KEY_SIZE];
                    memset(parKeys[uiCur], 0, DEF_KEY_SIZE);
                    *(uint32_t*)parKeys[uiCur] = uiCur;
                    //init the value
                    pVal[uiCur] = new char*[uiC];
                    for(uint32_t uiIdx = 0; uiIdx < uiC; uiIdx++)
                    {
                        pVal[uiCur][uiIdx] = new char[DEF_DATA_SIZE];
                        *(uint32_t*)pVal[uiCur][uiIdx] = uiCur * 100 + uiIdx;
                    }
                }

                //Put the data
                cout<<"Begin insertion"<<endl;

                TimeDiff::DiffTimeInMicroSecond();
                for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                {
                    if(0 != oSSECuckoo.Put(parKeys[uiCur], DEF_KEY_SIZE, uiC, (char**)pVal[uiCur], DEF_DATA_SIZE))
                    {
                        //cout<<"Error for Put"<<uiCur<<"Kickout : "<<oSSECuckoo.GetKick()<<endl;
                    }
                }
                uint32_t uiTime = TimeDiff::DiffTimeInMicroSecond();
                cout<<"The time of insertion procedure: "<<uiTime<<endl;
                cout<<"Total number of Kickout operations: "<<oSSECuckoo.GetKick()<<endl;

                break;
            }
        case 50:
            {
                //2
                cout<<"Begin to encrypt the index."<<endl;

                TimeDiff::DiffTimeInMicroSecond();
                oSSECuckoo.Encrypt();
                uint32_t uiTime = TimeDiff::DiffTimeInMicroSecond();
                cout<<"The time of encryption procedure: "<<uiTime<<endl;

                break;
            }
        case 51:
            {
                //3
                uint32_t uiTestNum = PrintAndGet<uint32_t>("Please input the number of Get requests. [ Then Press Enter ]");

                cout<<"Begin to test every Get request"<<endl;

                for(uint32_t uiCur = 0; uiCur < uiTestNum; uiCur++)
                {
                    TimeDiff::DiffTimeInMicroSecond();

                    if(!oSSECuckoo.Get(parKeys[uiCur], DEF_KEY_SIZE, uiC, pVal[uiCur]))
                    {
                        //cout<<"Error for Get"<<endl;
                    }

                    uint32_t uiTime = TimeDiff::DiffTimeInMicroSecond();
                    cout<<uiTime<<endl;
                }

                break;
            }
        case 52:
            {
                //4
                cout<<"Equality checking in buckets"<<endl;
                for(uint32_t uiCur = 0; uiCur < 10; uiCur++)
                {
                    if(!oSSECuckoo.Get(parKeys[uiCur], DEF_KEY_SIZE, uiC, pVal[uiCur]))
                    {
                        cout<<"Error for Get"<<uiCur<<endl;
                    }
                    uint32_t uiGetVal = *(uint32_t*)pVal[uiCur][0];
                    cout<<"The Val of "<<uiCur<<" is "<<uiGetVal<<endl;
                }
                break;
            }
        case 53:
            {
                //5
                cout<<"Init index in shared memory."<<endl;


                cout<<"Begin to init the index"<<endl;
                uiNum = PrintAndGet<uint32_t>("Please input the number of data records in the index. [ Then Press Enter ]");
                dLoad = PrintAndGet<double>("Please input the load factor of Index. [ Then Press Enter ]");
                uiB = PrintAndGet<uint32_t>("Please input the number of entries per bucket. [ Then Press Enter ]");
                uiC = PrintAndGet<uint32_t>("Please input the number of chunks for each data record. [ Then Press Enter ]");
                //This work for demo. To Set the split slice.
                uint32_t uiIndexNum = uiNum * uiC;
                uint64_t uiAllMem = oSSECuckoo.Size(uiIndexNum, dLoad, uiB, DEF_DATA_SIZE);
                cout<<"Required memory size : "<<uiAllMem<<endl;
                if(NULL != pMem)
                {
                    delete [] pMem;
                }

                key_t kKey = DEF_SHM_KEY;
                //Get Shm Memory
                if (!ShmCtl::GetShm((void**)&pMem, kKey, uiAllMem))
                {
                    cout<<"ERROR : Get Shm False!"<<endl;
                    return false;
                }

                uiAllMem = oSSECuckoo.Init(pMem, uiIndexNum, dLoad, uiB, DEF_DATA_SIZE, DEF_MAX_KICKOUT);
                cout<<"The allocated memory size : "<<uiAllMem<<endl;
                cout<<"Finish Init."<<endl;

                cout<<"Init random data records for insertion."<<endl;

                //Check parKeys [Key]
                if(NULL != parKeys)
                {
                    for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                    {
                        delete [] parKeys[uiCur];
                    }
                    delete [] parKeys;
                }
                parKeys = new char*[uiNum];
                //Check pVal [Value]
                if(NULL != pVal)
                {
                    for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                    {
                        for(uint32_t uiIdx = 0; uiIdx < uiC; uiIdx++)
                        {
                            delete [] pVal[uiCur][uiIdx];
                        }
                        delete [] pVal[uiCur];
                    }
                    delete [] pVal;
                }
                pVal = new char**[uiNum];
                //Init the data
                for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                {
                    //init the key
                    parKeys[uiCur] = new char[DEF_KEY_SIZE];
                    memset(parKeys[uiCur], 0, DEF_KEY_SIZE);
                    *(uint32_t*)parKeys[uiCur] = uiCur;
                    //init the value
                    pVal[uiCur] = new char*[uiC];
                    for(uint32_t uiIdx = 0; uiIdx < uiC; uiIdx++)
                    {
                        pVal[uiCur][uiIdx] = new char[DEF_DATA_SIZE];
                        *(uint32_t*)pVal[uiCur][uiIdx] = uiCur * 100 + uiIdx;
                    }
                }

                //Put the data
                cout<<"Begin to insert the data"<<endl;

                TimeDiff::DiffTimeInMicroSecond();
                for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                {
                    if(0 != oSSECuckoo.Put(parKeys[uiCur], DEF_KEY_SIZE, uiC, (char**)pVal[uiCur], DEF_DATA_SIZE))
                    {
                        cout<<"Error for Put"<<uiCur<<"Kickout : "<<oSSECuckoo.GetKick()<<endl;
                    }
                }
                uint32_t uiTime = TimeDiff::DiffTimeInMicroSecond();
                cout<<"The time of insertion procedure: "<<uiTime<<endl;
                cout<<"Total number of Kickout operations: "<<oSSECuckoo.GetKick()<<endl;

                break;
            }
        case 54:
            {
                //6
                cout<<"Waiting...No Implementation Now."<<endl;
                break;
            }
        default:
            {
                continue;
                break;
            }
        };

        cout<<"Press [Enter] to continue..."<<endl;
        getch();
        getch();
        
    }while(48 != iCmd);

    cout<<"Good Bye!"<<endl;

    return 0;
}
