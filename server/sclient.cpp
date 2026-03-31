#include <stdexcept> 
#include "sclient.h"
#include "requests.h"
#include <stdlib.h>
#include <ifu.h>
#include <guri.h>
#include <sstream>
//#include "../dmas/daaprov.h"

using namespace std;

const int KBufSize = 2048;


SessionClient::SessionClient(): CSessionBase()  {
}

SessionClient::SessionClient(int sock): CSessionBase(sock), mThread(NULL)
{
    mThread = new SessionThread();
    // Adding client in Static sClients registry (Critical section!)
    SessionThread::LockMutex(mId.c_str());
    SetId(sClients.size());
    cout << "Adding client with id: " << mId << endl;
    sClients.push_back(this);
    SessionThread::UnlockMutex(mId.c_str());
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
	    cout << "SessionClient [" << mId << "] disconnected" << endl;
	    close(mSock);
	    //Remove client in Static sClients <vector> (Critical section!)
	    SessionThread::LockMutex(mId.c_str());
	    index = FindSessionIndex(this);
	    cout << "Removed client session, id: " << sClients[index]->mId << endl;
	    sClients.erase(sClients.begin() + index);
	    SessionThread::UnlockMutex(mId.c_str());
	    delete this;
	    break;
	} else if (n < 0) {
	    cerr << "Error while receiving message from client: " << mId << endl;
	} else {
	    //Message received.
	    HandleMessage(string(buffer));
	}
    }
    return false;
}

//#endif
