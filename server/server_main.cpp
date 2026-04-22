#include <iostream>

#include "../server/sserver.h"
#include "../src/log.h"

using namespace std;

Logrec gLogrec("/dev/stdout");

// Default Id
static const string id = "DmnSrv";

#define LOG(aLevel) gLogrec.MeetsLevel(aLevel) && TLog(aLevel, id, &gLogrec).ContentStream()


int main() {
    LOG(EInfo) << "Starting server";
    Server* s = new Server(id);
    if (s) {
        //Main loop
        s->AcceptAndDispatch();
        delete s;
    } 
    return 0;
}
