#include <stdexcept> 
#include "sclient.h"
#include "requests.h"
#include <stdlib.h>
#include <ifu.h>
#include <guri.h>
#include <sstream>
//#include "../dmas/daaprov.h"

#include "../src/log.h"

using namespace std;

const int KBufSize = 2048;

extern Logrec gLogrec;

#define LOG(aLevel) gLogrec.MeetsLevel(aLevel) && TLog(aLevel, getId(), &gLogrec).ContentStream()



SessionClient::SessionClient(const string& aOwrId): CSessionBase(aOwrId)  {
}

SessionClient::SessionClient(int sock, const string& aOwrId): CSessionBase(sock, aOwrId), mThread(NULL)
{
    // Adding client in Static sClients registry (Critical section!)
    SessionThread::LockMutex(mId);
    SetId(sClients.size());
    LOG(EDbg) << "Adding client" << endl;
    mThread = new SessionThread(getId());
    sClients.push_back(this);
    SessionThread::UnlockMutex(mId);
    mThread->Create((void*) RunSession, this);
}

SessionClient::~SessionClient()
{
    if (mThread != NULL) {
	delete mThread;
    }
}

void SessionClient::Send(string const& aMsg) {
    SessionThread::LockMutex("'Send()'");
    send(mSock, aMsg.c_str(), aMsg.size(), 0);
    SessionThread::UnlockMutex("'Send()'");
}

//Static
void *SessionClient::RunSession(void *args)
{
    SessionClient *c = (SessionClient *) args;
    c->Run();
    //End thread
    return NULL;
}

bool SessionClient::Run()
{
    char buffer[KBufSize-25];
    int index, n;
    while (1) {
	memset(buffer, 0, sizeof buffer);
	n = recv(mSock, buffer, sizeof buffer, 0);
	//SessionClient disconnected?
	if (n == 0) {
	    LOG(EDbg) << "Client session [" << mId << "] disconnected";
	    close(mSock);
	    //Remove client in Static sClients <vector> (Critical section!)
	    SessionThread::LockMutex(mId.c_str());
	    index = FindSessionIndex(this);
	    LOG(EDbg) << "Removed client session, id: " << sClients[index]->mId;
	    sClients.erase(sClients.begin() + index);
	    SessionThread::UnlockMutex(mId.c_str());
	    delete this;
	    break;
	} else if (n < 0) {
	    LOG(EErr) << "Error while receiving message from client: " << mId;
	} else {
	    //Message received.
	    HandleMessage(string(buffer));
	}
    }
    return false;
}

