
#include "daaprov.h"



DaaProv::DaaProv(const string &aName, MEnv* aEnv): Provider(aName, aEnv)
{
}


MIface* DaaProv::createIfi(const string& aType, const string& aName, MEnv* aEnv)
{
    MIface* res = nullptr;
    return res;
}
