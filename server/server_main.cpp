#include <iostream>
#include "../server/sserver.h"

using namespace std;

int main() {
    cout << "Running fap5 server!" << endl;
    Server *s;
    s = new Server();
    //Main loop
    s->AcceptAndDispatch();
    return 0;
}
