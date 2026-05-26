
#include "ownedpx.h"



OwnedPx::OwnedPx(MEnv* aEnv, MProxyMgr* aMgr, const string& aContext):
    DaaProxy(aEnv, aMgr, aContext), mOwsCp(this)
{
}

OwnedPx::OwnedPx(MEnv* aEnv, const string& aContext): DaaProxy(aEnv, aContext),
    mOwsCp(this)
{
}

OwnedPx::~OwnedPx()
{
}

MIface* OwnedPx::MProxy_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif(aId, mMOwned));
    else DaaProxy::MProxy_getLif(aId);
    return res;
}

string OwnedPx::ownedId() const
{
    string resp;
    bool res = mMgr->Request(mContext, __func__, resp);
    return resp;
}


MIface* OwnedPx::MOwned_getLif(TIdHash aId)
{
    return RpcPxNh(__func__, aId);
}

string OwnedPx::MOwned_Uid() const
{
    string resp;
    bool res = mMgr->Request(mContext, __func__, resp);
    return resp;
}

void OwnedPx::deleteOwned()
{
    string resp;
    bool res = mMgr->Request(mContext, __func__, resp);
}

void OwnedPx::onOwnerAttached()
{
    string resp;
    bool res = mMgr->Request(mContext, __func__, resp);
}

void OwnedPx::onOwnerDetached()
{
    string resp;
    bool res = mMgr->Request(mContext, __func__, resp);
}


