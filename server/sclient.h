#ifndef _sessionclient_h_
#define _sessionclient_h_

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include "../src/env.h"
#include "../src/ifu.h"
#include "fapcsbase.h"
#include "sthread.h"

using namespace std;


class SessionThread;
class SessionClient : public CSessionBase {
    public:
        SessionClient(const string& aSrvId);
        SessionClient(int sock, const string& aSrvId);
	virtual ~SessionClient();
        static void* RunSession(void *args);
    protected:
        virtual void Send(string const& aMsg);
        virtual bool Run();
    protected:
        SessionThread *mThread;
};


#endif
