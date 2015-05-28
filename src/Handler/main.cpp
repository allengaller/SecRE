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

//time compute
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
    cout<<"©§                                          Controller Command List                                     ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§     Input Command Below :                                                                            ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         0 Exit                                                                                       ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         1 Init SSECuckoo And Put Data                                                                ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         2 Encrypt Index                                                                              ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         3 Test the Get Operator                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         4 Test one Get Operator                                                                      ©§"<<endl;
    cout<<"©§                                                                                                      ©§"<<endl;
    cout<<"©§         5 Init Index On Share Memory                                                                 ©§"<<endl;
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
                cout<<"Begin to Init the Index"<<endl;
                uiNum = PrintAndGet<uint32_t>("Please input the number of records you wants to put in Index. [ Then Press Enter ]");
                dLoad = PrintAndGet<double>("Please input the load of Index. [ Then Press Enter ]");
                uiB = PrintAndGet<uint32_t>("Please input the number of entrys per bucket. [ Then Press Enter ]");
                uiC = PrintAndGet<uint32_t>("Please input the split number of data value. [ Then Press Enter ]");
                //This work for demo. To Set the split slice.
                uint32_t uiIndexNum = uiNum * uiC;
                uint64_t uiAllMem = oSSECuckoo.Size(uiIndexNum, dLoad, uiB, DEF_DATA_SIZE);
                cout<<"We work out the memory size : "<<uiAllMem<<endl;
                if(NULL != pMem)
                {
                    delete [] pMem;
                }
                pMem = new char[uiAllMem];
                uiAllMem = oSSECuckoo.Init(pMem, uiIndexNum, dLoad, uiB, DEF_DATA_SIZE, DEF_MAX_KICKOUT);
                cout<<"In fact, All Memory size : "<<uiAllMem<<endl;
                cout<<"Finish Init."<<endl;

                cout<<"Init Random Data to prepare insert."<<endl;

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
                cout<<"Begin to Insert the Data"<<endl;

                TimeDiff::DiffTimeInMicroSecond();
                for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                {
                    if(0 != oSSECuckoo.Put(parKeys[uiCur], DEF_KEY_SIZE, uiC, (char**)pVal[uiCur], DEF_DATA_SIZE))
                    {
                        //cout<<"Error for Put"<<uiCur<<"Kickout : "<<oSSECuckoo.GetKick()<<endl;
                    }
                }
                uint32_t uiTime = TimeDiff::DiffTimeInMicroSecond();
                cout<<"Build Plaintext Index : "<<uiTime<<endl;
                cout<<"All Kickout Time : "<<oSSECuckoo.GetKick()<<endl;

                break;
            }
        case 50:
            {
                //2
                cout<<"Begin to encrypt the index."<<endl;

                TimeDiff::DiffTimeInMicroSecond();
                oSSECuckoo.Encrypt();
                uint32_t uiTime = TimeDiff::DiffTimeInMicroSecond();
                cout<<"Build Plaintext Index : "<<uiTime<<endl;

                break;
            }
        case 51:
            {
                //3
                cout<<"Waiting...No Implementation Now."<<endl;

                uint32_t uiTestNum = PrintAndGet<uint32_t>("Please input the number of Get Operator you wants. [ Then Press Enter ]");

                cout<<"Begin to Test Every Get Operator"<<endl;

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
                cout<<"Test Equal operator"<<endl;
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
                cout<<"Init Index in Share Memory."<<endl;


                cout<<"Begin to Init the Index"<<endl;
                uiNum = PrintAndGet<uint32_t>("Please input the number of records you wants to put in Index. [ Then Press Enter ]");
                dLoad = PrintAndGet<double>("Please input the load of Index. [ Then Press Enter ]");
                uiB = PrintAndGet<uint32_t>("Please input the number of entrys per bucket. [ Then Press Enter ]");
                uiC = PrintAndGet<uint32_t>("Please input the split number of data value. [ Then Press Enter ]");
                //This work for demo. To Set the split slice.
                uint32_t uiIndexNum = uiNum * uiC;
                uint64_t uiAllMem = oSSECuckoo.Size(uiIndexNum, dLoad, uiB, DEF_DATA_SIZE);
                cout<<"We work out the memory size : "<<uiAllMem<<endl;
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
                cout<<"In fact, All Memory size : "<<uiAllMem<<endl;
                cout<<"Finish Init."<<endl;

                cout<<"Init Random Data to prepare insert."<<endl;

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
                cout<<"Begin to Insert the Data"<<endl;

                TimeDiff::DiffTimeInMicroSecond();
                for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
                {
                    if(0 != oSSECuckoo.Put(parKeys[uiCur], DEF_KEY_SIZE, uiC, (char**)pVal[uiCur], DEF_DATA_SIZE))
                    {
                        cout<<"Error for Put"<<uiCur<<"Kickout : "<<oSSECuckoo.GetKick()<<endl;
                    }
                }
                uint32_t uiTime = TimeDiff::DiffTimeInMicroSecond();
                cout<<"Build Plaintext Index : "<<uiTime<<endl;
                cout<<"All Kickout Time : "<<oSSECuckoo.GetKick()<<endl;

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

    cout<<"Good Bye !"<<endl;

    return 0;
}
