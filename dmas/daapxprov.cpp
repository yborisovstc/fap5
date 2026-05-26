
#include "daapxprov.h"
#include "ownedpx.h"
#include "nodepx.h"



DaaPxProv::DaaPxProv(const string& aName, MEnv* aEnv): mName(aName), mEnv(aEnv)
{
}

DaaPxProv::~DaaPxProv() {
}

MIface* DaaPxProv::MIpxProv_getLif(TIdHash aTid)
{
    MIface* res = nullptr;
    if (res = checkLif(aTid, mMIpxProv));
    return res;
}

MProxy* DaaPxProv::createProxy(const string& aId, MProxyMgr* aMgr, const string& aContext) const
{
    MProxy* res = NULL;
    if (aId == MOwned::idStr()) {
	res = new OwnedPx(mEnv, aMgr, aContext);
    } else if (aId == MNode::idStr()) {
	res = new OwnedPx(mEnv, aMgr, aContext);
    } else {
        assert(false);
    }
    return res;
}

MProxy* DaaPxProv::createProxy(TIdHash aIfaceId, MProxyMgr* aMgr, const string& aContext) const
{
    MProxy* res = nullptr;
    if (aIfaceId == MOwned::idHash()) {
	res = new OwnedPx(mEnv, aMgr, aContext);
    } else if (aIfaceId == MNode::idHash()) {
	res = new NodePx(mEnv, aMgr, aContext);
    } else {
        assert(false);
    }
    return res;

}
