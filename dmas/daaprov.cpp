
#include "daaprov.h"
#include "daapxprov.h"
#include "sysdm.h"
#include "renvadp.h"
#include "ownedpx.h"


/** Native agents factory registry */
const ProvBase::TFReg DaaProv::mReg ( {
        Item<SystDm>()
});

const ProvBase::TIfiFReg DaaProv::mIfiReg ( {
        IfiItem<MIpxProv, DaaPxProv>()
});

const ProvBase::TIfiFRegH DaaProv::mIfiRegH ( {
        IfiItemH<MIpxProv, DaaPxProv>()
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
