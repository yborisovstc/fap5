

#include "workers.h"

#include <sstream>

#define LOG(aLevel, aContent) \
    if (mEnv->Logger()->MeetsLevel(aLevel)) {\
	TLog rec(aLevel, nullptr, aContent);\
	mEnv->Logger()->Write(rec);\
    }\


Dworker::Dworker(MObserver* aObserver, int aId): mObserver(aObserver), mId(aId),
    mStop(false), mThread(&Dworker::run, this)
{}

Dworker::~Dworker()
{
    //std::cout << "Dworker#" << mId << " destructor" << std::endl;
    mStop = true;
    mThread.join();
}

bool Dworker::takeJob(MJob* aJob) {
    {
        std::lock_guard lk(mMutex);
        mJob = aJob;
    }
    //std::cout << "Dworker#" << mId << "-" << this << ", notify, job: " << aJob << std::endl;
    mCv.notify_one();
    return true;
}

void Dworker::run()
{
    //std::cout << "Dworker#" << mId << " run, mStop: " << mStop << std::endl;
    while (!mStop) {
        std::unique_lock lk(mMutex);
        //std::cout << "Dworker#" << mId << "-" << this << ", locked" << std::endl;
        mCv.wait(lk,
            [this] { /*std::cout << "W#" << mId << " check, job: " << mJob << std::endl;*/ return (mJob != nullptr);});
        lk.unlock();
        //std::cout << "Dworker#" << mId << ", perform" << std::endl;
        mJob->perform();
        //std::cout << "Dworker#" << mId << ", performed" << std::endl;
        mObserver->onCompleted(this);
    }
    //std::cout << "Dworker#" << mId << " STOPPED" << std::endl;
}

void Dworker::resetJob()
{
    std::lock_guard lk(mMutex);
    mJob = nullptr;
}

void TUpdateJob::perform()
{
    mSyncable->update();
}

void TUpdateJob::complete()
{
    //mOwner->onJobCompleted(this);
}


//// Dworker

/*
Dworker::~Dworker()
{
    while (!mJobs.empty()) {
        MJob* job = mJobs.back();
        delete job;
        mJobs.pop_back();
    }
}
*/


//// ThreadPool


ThreadPool::~ThreadPool()
{
    // TODO to check that there are not busy workers
    while (!mIdle.empty()) {
        Dworker* elem = mIdle.back();
        delete elem;
        mIdle.pop_back();
    }
}

bool ThreadPool::takeJob(MJob* aJob)
{
    bool res = false;
    mMutex.lock();
    std::ostringstream ss;
    ss << this_thread::get_id();
    //LOG(EDbg, std::string("ThreadPool[") + ss.str() + "] takeJob, id: " + aJob->id() + ", idle: " + to_string(mIdle.size()));
    Dworker* worker = nullptr;
    if (!mIdle.empty()) {
        worker = mIdle.back();
        //LOG(EDbg, "ThreadPool.takeJob, got idle worker#" + to_string(worker->id()));
        mIdle.pop_back();
    } else {
        if (mWorkerCount < mSize) {
            worker = new Dworker(this, mWorkerCount);
            mWorkerCount++;
            LOG(EDbg, "ThreadPool takeJob, created worker#" + to_string(worker->id()) + ", cnt: " + to_string(mWorkerCount));
        }
    }
    if (worker) {
        mActive.insert(TActiveElem(aJob, worker));
        worker->takeJob(aJob);
        ///LOG(EDbg, "ThreadPool takeJob, assigned job " + aJob->id());
    } else {
        mWaitingJobs.push_back(aJob);
        ///LOG(EDbg, "ThreadPool takeJob, put job for waiting, cnt: " + to_string(mWaitingJobs.size()));
    }
    mMutex.unlock();
    return res;
}

void ThreadPool::onCompleted(Dworker* aWorker)
{
    mMutex.lock();
    auto* job = aWorker->job();
    //LOG(EDbg, "ThreadPool.onCompleted, worker: " + to_string(aWorker->id()) + ", job: " + job->id());
    //job->complete();
    auto jit = mActive.find(job);
    mActive.erase(jit);
    if (!mWaitingJobs.empty()) {
        auto* wjob = mWaitingJobs.back();
        mWaitingJobs.pop_back();
        aWorker->takeJob(wjob);
        ///LOG(EDbg, "ThreadPool.onCompleted, assigned, worker: " + to_string(aWorker->id()) + ", job: " + wjob->id());
        mActive.insert(TActiveElem(wjob, aWorker));
    } else {
        ///LOG(EDbg, "ThreadPool.onCompleted, put to idle, worker#" + to_string(aWorker->id()));
        aWorker->resetJob();
        //aWorker->takeJob(nullptr);
        mIdle.push_back(aWorker);
    }
    mMutex.unlock();
}
