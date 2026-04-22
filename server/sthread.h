#ifndef _SESSIONTHREAD_H_
#define _SESSIONTHREAD_H_


#include <string>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

class SessionThread {
    public:
        pthread_t mTid;
    private:
        static pthread_mutex_t sMutex;
    public:
        SessionThread(const string& aOwrId);
        int Create(void *Callback, void *args);
        int Join();
        static int InitMutex();
        static int LockMutex(const string& aId);
        static int UnlockMutex(const string& aId);
    protected:
        string getId() const;
    protected:
        string mOwrId;
};
#endif
