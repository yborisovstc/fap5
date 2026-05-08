
#include "daaprov.h"
#include "sysdm.h"
#include "renvadp.h"


/** Native agents factory registry */
const ProvBase::TFReg DaaProv::mReg ( {
        Item<SystDm>()
});

const ProvBase::TIfiFReg DaaProv::mIfiReg ( {
        /*IfiItem<OwdRenvAdp>()*/
});

/** Data factory registry */
const ProvBase::TDtFReg DaaProv::mDtReg ( {
});

DaaProv::DaaProv(const string &aName, MEnv* aEnv): ProvBase(aName, aEnv)
{
}

/*
MIface* DaaProv::createIfi(const string& aType, const string& aName, MEnv* aEnv)
{
    MIface* res = nullptr;

    return res;
}
*/
