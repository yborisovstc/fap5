#include "sserver.h"
#include "sclient.h"

#include "../src/log.h"

using namespace std;

extern Logrec gLogrec;


#define LOG(aLevel) gLogrec.MeetsLevel(aLevel) && TLog(aLevel, mId, &gLogrec).ContentStream()

Server::Server(const string& aId, int aPort): mId(aId), mPort(aPort)
{
    Construct();
}

Server::Server(const string& aId): mId(aId), mPort(0)
{
    Construct();
}

void Server::Construct()
{
    //Initialize static mutex from SessionThread
    SessionThread::InitMutex();
    //For setsock opt (REUSEADDR)
    int yes = 1;
    //Init serverSock and start listen()'ing
    mServerSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&mServerAddr, 0, sizeof(sockaddr_in));
    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_addr.s_addr = INADDR_ANY;
    mServerAddr.sin_port = htons(mPort == 0 ? PORT : mPort);
    //Avoid bind error if the socket was not close()'d last time;
    setsockopt(mServerSock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    int res = bind(mServerSock, (struct sockaddr *) &mServerAddr, sizeof(sockaddr_in)) < 0;
    if (res < 0) {
        cerr << "Failed to bind server socket, err: " << errno << endl;
    }
    listen(mServerSock, 5);
}

// Main loop: Blocks at accept(), until a new connection arrives.
// When it happens, create a new thread to handle the new client.
void Server::AcceptAndDispatch() {
    socklen_t cliSize = sizeof(sockaddr_in);
    while (1) {
        //Blocks here;
        int sock = accept(mServerSock, (struct sockaddr *) &mClientAddr, &cliSize);
        if (sock < 0) {
            cerr << "Error on accept";
        } else {
            new SessionClient(sock, mId);
            //cout << "Server [p:" << htons(mServerAddr.sin_port) <<
	    // "], accepted client [p:" << htons(mClientAddr.sin_port) << "]" << endl;
	    LOG(EDbg) << "Server [p:" << htons(mServerAddr.sin_port) <<
		"], accepted client [p:" << htons(mClientAddr.sin_port) << "]";
        }
    }
}
