
#include "sthread.h"
#include <iostream>

#include "../src/log.h"

using namespace std;


extern Logrec gLogrec;

#define LOG(aLevel) gLogrec.MeetsLevel(aLevel) && TLog(aLevel, getId(), &gLogrec).ContentStream()
#define LOGS(aLevel) gLogrec.MeetsLevel(aLevel) && TLog(aLevel, string(), &gLogrec).ContentStream()



//Need to actually "allocate" static member
pthread_mutex_t SessionThread::sMutex;

SessionThread::SessionThread(const string& aOwrId): mOwrId(aOwrId) {
}

int SessionThread::Create(void *Callback, void *args) {
    int res = 0;
    //Supercreepy typecast
    res = pthread_create(&this->mTid, NULL, (void *(*)(void *))Callback, args);
    if (res != 0) {
        LOG(EErr) << "Failed creating threads, err: " << res;
        return res;
    } else {
        //LOG(EDbg) << "Thread successfully created" << endl;
        return 0;
    }
}

int SessionThread::Join() {
    pthread_join(this->mTid, NULL);
    return 0;
}

int SessionThread::InitMutex() {
    if (pthread_mutex_init(&SessionThread::sMutex, NULL) < 0) {
        LOGS(EErr) << "Failed initializing mutex";
        return -1;
    } else {
        return 0;
    }
}

/*
   LockMutex():
   Blocks until mutex becomes available
   */
int SessionThread::LockMutex(const string& aId) {
    //cout << identifier << " is trying to acquire the lock..." << endl;
    if (pthread_mutex_lock(&SessionThread::sMutex) == 0) {
        //cout << identifier << " acquired the lock!" << endl;
        return 0;
    } else {
        LOGS(EErr) << "Error while client [" << aId << "] was trying to acquire the lock" << endl;
        return -1;
    }
}

int SessionThread::UnlockMutex(const string& aId) {
    //cout << identifier << " is trying to release the lock..." << endl;
    if (pthread_mutex_unlock(&SessionThread::sMutex) == 0) {
        //cout << identifier << " released the lock!" << endl;
        return 0;
    } else {
        LOGS(EErr) << "Error while client [" << aId << "] was trying to release the lock" << endl;
        return -1;
    }
}

string SessionThread::getId() const
{
    return mOwrId + ".THR";
}


