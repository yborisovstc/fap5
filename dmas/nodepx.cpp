
#include "nodepx.h"


NodePx::NodePx(MEnv* aEnv, MProxyMgr* aMgr, const string& aContext):
    DaaProxy(aEnv, aMgr, aContext)
{
}

NodePx::~NodePx()
{
}

MIface* NodePx::MProxy_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif(aId, mMNode));
    else DaaProxy::MProxy_getLif(aId);
    return res;
}

MIface* NodePx::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif(aId, mMNode));
    return res;
}

string NodePx::MNode_Uid() const
{
    return Rpc<string>(__func__);
}

const string& NodePx::name() const
{
    auto* self = const_cast<NodePx*>(this);
    bool res = mMgr->Request(mContext, "name", self->mName);
    return mName;
}

GUri NodePx::parentUri() const
{
    string resp;
    bool res = mMgr->Request(mContext, "parentUri", resp);
    return resp;
}

vector<GUri> NodePx::parentsUris() const
{
    return vector<GUri>();
}

MOwned* NodePx::getOwned(const string& aId) const
{
    return RpcPx<MOwned>(__func__, aId);
}

const MNode* NodePx::getNode(const GUri& aUri) const
{
    return RpcPx<MNode>(__func__, aUri);
}

MNode* NodePx::getNode(const GUri& aUri)
{
    return RpcPx<MNode>(__func__, aUri);
}

MNode* NodePx::getNodeS(const char* aUri)
{
    return RpcPx<MNode>(__func__, aUri);
}

MNode* NodePx::getNode(const GUri& aUri, const TNs& aNs)
{
    return RpcPx<MNode>(__func__, aNs);
}

void NodePx::getUri(GUri& aUri, const MNode* aBase) const
{
    GUri res = Rpc<GUri, const MNode*>(__func__, aBase);
    aUri = res;
}

void NodePx::setCtx(MOwner* aContext)
{
    Rpcv<MIface*>(__func__, aContext);
}

void NodePx::mutate(const ChromoNode& aMut, bool aChange /*false*/, const MutCtx& aCtx, bool aTreatAsChromo /*false*/, bool aLocal /*false*/)
{
    Rpcv<ChromoNode, bool, const MutCtx, bool>(__func__, aMut, aChange, aCtx, aTreatAsChromo);
}

bool NodePx::attachOwned(MNode* aOwned)
{
    bool res = Rpc<bool, MNode*>(__func__, aOwned);
    return res;
}

MNode::TOwnerCp* NodePx::ownerCp()
{
    assert(false);
    return nullptr;
}

const MNode::TOwnerCp* NodePx::ownerCp() const
{
    assert(false);
    return nullptr;
}

void NodePx::MNode_doDump(int aLevel, int aIdt, ostream& aOs) const
{
    Rpc<int, int>(__func__, aLevel, aIdt);
}
