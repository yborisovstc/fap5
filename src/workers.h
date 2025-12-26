
#ifndef __FAP5_WORKERS_H
#define __FAP5_WORKERS_H

#include <list>
//#include <mutex>
#include <thread>
#include <condition_variable>

#include "menv.h"
#include "mworkers.h"
#include "mdes.h"

/** @brief Thread job
 * */
class TUpdateJob: public MJob
{
    public:
        TUpdateJob(MJobOwner* aOwner, MDesSyncable* aSyncable): mOwner(aOwner), mSyncable(aSyncable) {}
        virtual ~TUpdateJob() = default;
        // From MJob
        virtual std::string id() const { return mSyncable->Uid();}
        const MJobOwner* owner() const override { return mOwner;}
        void perform() override;
        void complete() override;
    protected:
        MJobOwner* mOwner;
        MDesSyncable* mSyncable;
        
};

/*
class Dworker
{
    public:
        Dworker() {}
        virtual ~Dworker();
        bool addJob(MJob* aJob) {
            mJobs.push_back(aJob);
            return true;
        }
        int jobsCount() const { return mJobs.size();}
    protected:
        std::list<MJob*> mJobs;
};
*/


class Dworker
{
    public:
        class MObserver {
            public:
                virtual void onCompleted(Dworker* aWorker) = 0;
        };
    public:
        Dworker(MObserver* aObserver, int aId);
        virtual ~Dworker();
        bool takeJob(MJob* aJob);
        void run();
        const MJob* job() const { return mJob;}
        MJob* job() { return mJob;}
        int id() { return mId;}
        void resetJob();
    protected:
        int mId = 0;
        MJob* mJob = nullptr;
        bool mStop;
        MObserver* mObserver = nullptr;
        std::mutex mMutex;
        std::thread mThread;
        std::condition_variable mCv;
};

class ThreadPool: public MThreadPool, Dworker::MObserver
{
    public:
        using TActiveElem = std::pair<MJob*, Dworker*>;
    public:
        ThreadPool(MEnv* aEnv, int aSize): mEnv(aEnv), mSize(aSize) {}
        virtual ~ThreadPool(); 
        bool takeJob(MJob* aJob) override;
        // From MObse
        void onCompleted(Dworker* aWorker) override;
    protected:
        MEnv* mEnv;
        int mSize;
        std::list<MJob*> mWaitingJobs;
        std::map<MJob*, Dworker*> mActive;
        std::list<Dworker*> mIdle;
        std::mutex mMutex;
        int mWorkerCount = 0;
};

#endif

