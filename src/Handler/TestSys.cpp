#include <stdio.h>
#include <string>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>

#include "../Caravel/TimeDiff.h"
#include "../Caravel/AES.h"
#include "../Caravel/SSECuckoo.h"
#include "../Caravel/ShmCtl.h"

#include <stdlib.h>
#include <time.h>

//Config
#include "conf.h"

using namespace caravel;
using namespace std;


int main(int argc, char **argv)
{

    if(argc != 5)
    {
        cout<<"usage : ./TestSys [A] [B] [C] [D]"<<endl;
        cout<<"The seed is [A] ."<<endl;
        cout<<"The test will last [B] seconds"<<endl;
        cout<<"The configure file name [C]"<<endl;
        cout<<"The test will begin at [D] ."<<endl;
        return 0;
    }

    uint32_t uiBeg, uiTime, uiSeed;
    
    sscanf(argv[1], "%u", &uiSeed);
    sscanf(argv[2], "%u", &uiTime);
    sscanf(argv[4], "%u", &uiBeg);
    
    string sFileName;
    sFileName.assign(argv[3]);

    uint32_t uiNum;
    double dLoad;
    uint32_t uiB;
    uint32_t uiC;

    //Read Params from File
    ifstream myfile;
    myfile.open(sFileName);
    if (myfile.is_open())
    {
        myfile >> uiNum;
        myfile >> dLoad;
        myfile >> uiB;
        myfile >> uiC;
    }
    myfile.close();
    
    /*
    cout<<"Params uiNum : "<<uiNum<<endl;;
    cout<<"Params dLoad : "<<dLoad<<endl;;
    cout<<"Params uiB : "<<uiB<<endl;;
    cout<<"Params uiC : "<<uiC<<endl;;
    */

    srand(uiSeed);

    //Time
    uint32_t uiCurTime;
    uint32_t uiBegTime = uiBeg;
    uint32_t uiEndTime = uiBegTime + uiTime;
    uint32_t uiCnt = 0;

    char **pVal = NULL;
    pVal = new char*[uiC];
    for(uint32_t uiCur = 0; uiCur < uiC; uiCur++)
    {
        pVal[uiCur] = new char[DEF_DATA_SIZE];
    }
    char **parKeys = NULL;
    parKeys = new char*[uiNum];
    for(uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
    {
        //init the key
        parKeys[uiCur] = new char[DEF_KEY_SIZE];
        memset(parKeys[uiCur], 0, DEF_KEY_SIZE);
        *(uint32_t*)parKeys[uiCur] = uiCur;
    }

    //Index
    
    SSECuckoo oSSECuckoo;
    uint32_t uiIndexNum = uiNum * uiC;
    uint64_t uiAllMem = oSSECuckoo.Size(uiIndexNum, dLoad, uiB, DEF_DATA_SIZE);

    char *pMem = NULL;

    key_t kKey = DEF_SHM_KEY;
    //Get Shm Memory
    if (!ShmCtl::GetShm((void**)&pMem, kKey, uiAllMem))
    {
        cout<<"ERROR : Get Shm False!"<<endl;
        return false;
    }

    uiAllMem = oSSECuckoo.Init(pMem, uiIndexNum, dLoad, uiB, DEF_DATA_SIZE, DEF_MAX_KICKOUT, false);

    while(true)
    {

        uiCurTime = time(NULL);

        if(uiCurTime < uiBegTime)
        {
            continue;
        }

        if(uiCurTime >= uiEndTime)
        {
            break;
        }

        oSSECuckoo.Get(parKeys[rand() % uiNum], DEF_KEY_SIZE, uiC, pVal);

        uiCnt++;


    }

    cout<<uiCnt<<endl;
    
    return 0;






}


