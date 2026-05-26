
#include "../src/log.h"
#include "renvadp.h"


#define LOG(aLevel) mEnv->Logger()->MeetsLevel(aLevel) && TLog(aLevel, MRenvAdp::Uid(), mEnv->Logger()).ContentStream()


//// OwnedPpx

OwdRenvAdp::OwnedPpx::OwnedPpx(OwdRenvAdp* aHost, const string& aContext):
    OwnedPx(aHost->mEnv, &aHost->mPxMgr, aContext), mHost(aHost) {}

OwdRenvAdp::OwnedPpx::~OwnedPpx() 
{}

MIface* OwdRenvAdp::OwnedPpx::MProxy_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    res = OwnedPx::MProxy_getLif(aId);
    return res;
}



//// RenvAdp
//
RenvAdp::RenvAdp(const string& aName, MEnv* aEnv): mName(aName), mEnv(aEnv),
    mPxMgr(aEnv, this, mRenvClient)
{
}

RenvAdp::~RenvAdp()
{
}

MIface* RenvAdp::MRenvAdp_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif(aId, mMRenvAdp));
    return res;
}

string RenvAdp::getOid() const
{
    return string();
}

bool RenvAdp::connect(const string& aSrvUri)
{
    bool res = true;
    assert(mRmtSrvUri.empty());
    mRmtSrvUri = aSrvUri;
    try {
        mRenvClient.Connect(mRmtSrvUri);
    } catch (exception& e) {
        LOG(EErr) << "Failed connecting to server " << mRmtSrvUri;
        res = false;
    }
    LOG(EDbg) << "Connected to server " << mRmtSrvUri;
    return res;
}


//// OwdRenvAdp

OwdRenvAdp::OwdRenvAdp(const string& aName, MEnv* aEnv): RenvAdp(aName, aEnv)
{
}

OwdRenvAdp::~OwdRenvAdp()
{
}

MIface* OwdRenvAdp::MOwdRenvAdp_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (aId == MOwned::idHash()) {
        return dynamic_cast<MOwned*>(mOwdPpx);
    } else {
        res = MRenvAdp_getLif(aId);
    }
    return res;
}


/*
string OwdRenvAdp::ProxyMgrOwner::getOid() const
{
    return string();
}
*/

void OwdRenvAdp::onOwnerAttached()
{
}

bool OwdRenvAdp::createRenv(const string& aRmtSrvUid, const string& aPrmUid, const ChromoNode& aChromo)
{
    bool res = false;
    assert(mRmtSrvUri.empty());
    mRmtSrvUri = aRmtSrvUid;
    do {
        try {
            mRenvClient.Connect(mRmtSrvUri);
        } catch (exception& e) {
            LOG(EErr) << "Failed connecting to server " << aRmtSrvUid;
        }
        LOG(EDbg) << "Connected to server " << aRmtSrvUid;
        // Create remote env
        string env, spec;
        aChromo.ToString(spec);
        bool cres = mRenvClient.Request("MEnvProvider", "CreateEnv,1," + spec, env);
        if (!cres) {
            LOG(EErr) << "Failed creating remote model environment: " << env;
            break;
        }
        // Set server id, env id and this agent Uid to remote env as Primary SID, EID and UID
        // TODO redesign getting SID, EID. Using env variable is not suitable for multi-server solution.
        string sid, eid;
        cres = mEnv->getEVar("SID", sid);
        cres = res && mEnv->getEVar("EID", eid);
        //=====
        sid = "TestSID";
        eid = "TestEID";
        cres = true;
        //=====
        string resp;
        if (cres) {
            // Set server id, env id and this agent Uid to remote env as PrimarySid and PrimaryUid
            cres = mRenvClient.Request(env, "setEVar,1,PrimarySID," + sid, resp);
            cres = cres && mRenvClient.Request(env, "setEVar,1,PrimaryEID," + eid, resp);
            cres = cres && mRenvClient.Request(env, "setEVar,1,PrimaryUid," + aPrmUid, resp);
        }
        if (!cres) {
            LOG(EErr) << "Failed setting primary environment IDs";
            break;
        }
        string rsid;
        cres = cres && mRenvClient.Request(env, "getEVar,1,SSID", rsid);
        if (!cres) {
            LOG(EErr) << "Failed getting SSID";
            break;
        }
        mRenvClient.SetRmtSID(rsid);
        // Create remote model
        cres = mRenvClient.Request(env, "constructSystem", resp);
        if (!cres) {
            LOG(EErr) << "Failed adding node to remote env [ " << mRmtSrvUri << "], resp [" << resp << "]";
            break;
        }
        // Get remote root
        string rroot;
        cres = mRenvClient.Request(env, "Root", rroot);
        if (!cres) {
            LOG(EErr) << "Failed getting remote env root, resp: " << rroot;
            break;
        }
        string rrootOwd;
        string req = Ifu::CombineIcSpec("MNode_getLif", "1", Ifu::FromIdHash(MOwned::idHash()));
        cres = mRenvClient.Request(rroot, req, rrootOwd);
        if (!cres) {
            LOG(EErr) << "Failed getting remote env root owned, resp: " << rrootOwd;
            break;
        }
        LOG(EInfo) << "Getting remote env root owned, resp: " << rrootOwd;
        // Create MOwned proxy to remote root
        // The proxy created is actually primary proxy - specific proxy assosiated to Renv adapter
        mOwdPpx = new OwnedPpx(this, rrootOwd);
        mPxMgr.RegisterProxy(mOwdPpx);

        res = true;
    } while (false);
    return res;
}


//// OwgRenvAdp::OwnerPpx


OwgRenvAdp::OwnerPpx::OwnerPpx(OwgRenvAdp* aHost, const string& aContext):
    DaaProxy(aHost->mEnv, &aHost->mPxMgr, aContext), mHost(aHost), mOwsCp(this) {}

OwgRenvAdp::OwnerPpx::~OwnerPpx() 
{}

MIface* OwgRenvAdp::OwnerPpx::MOwner_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    return res;
}

MOwned* OwgRenvAdp::OwnerPpx::getOwned(const GUri& aUri, const MOwned* aReq) const
{
    return nullptr;
}

void OwgRenvAdp::OwnerPpx::ownerGetUri(GUri& aUri, const MOwner* aBase) const
{
}

void OwgRenvAdp::OwnerPpx::onOwnedMutated(const MOwned* aOwned, const ChromoNode& aMut, const MutCtx& aCtx)
{
}

void OwgRenvAdp::OwnerPpx::onOwnedAttached(MOwned* aOwned)
{
}

void OwgRenvAdp::OwnerPpx::onOwnedDetached(MOwned* aOwned)
{
}

MNode* OwgRenvAdp::OwnerPpx::getParent(const GUri& aUri)
{
    return nullptr;
}

bool OwgRenvAdp::OwnerPpx::isOwned(const MOwned* mOwned) const
{
    return false;
}

bool  OwgRenvAdp::OwnerPpx::owrAttachOwned(MOwned* aOwned)
{
    bool res = mOwsCp.connect(aOwned->ownedCp());
    return res;
}


//// OwgRenvAdp

OwgRenvAdp::OwgRenvAdp(const string& aName, MEnv* aEnv): RenvAdp(aName, aEnv)
{
}

OwgRenvAdp::~OwgRenvAdp()
{
    if (mOwrPpx) {
        delete mOwrPpx;
    }
}

MIface* OwgRenvAdp::MOwgRenvAdp_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (aId == MOwner::idHash()) {
        return dynamic_cast<MOwner*>(mOwrPpx);
    } else {
        res = MRenvAdp_getLif(aId);
    }
    return res;
}

bool OwgRenvAdp::bindRenv(const string& aPSid, const string& aPEid, const string& aPUid)
{
    bool res = false;
    do {
        // Connect to remote env
        res = connect(aPSid);
        if (!res) {
            LOG(EErr) << "Failed connecting to primary environment server";
            break;
        }
        // Create owner proxy of owner node in remote env
        mOwrPpx = new OwnerPpx(this, aPUid);
        mPxMgr.RegisterProxy(mOwrPpx);
        res = true;
    } while (false);
    return res;
}


