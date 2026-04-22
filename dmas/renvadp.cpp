
#include "../src/log.h"
#include "renvadp.h"


#define LOG(aLevel) mEnv->Logger()->MeetsLevel(aLevel) && TLog(aLevel, Uid(), mEnv->Logger()).ContentStream()


//// OwnedPpx

OwdRenvAdp::OwnedPpx::OwnedPpx(OwdRenvAdp* aHost, const string& aContext):
    DaaProxy(aHost->mEnv, &aHost->mPxMgr, aContext), mHost(aHost), mOwsCp(this) {}

OwdRenvAdp::OwnedPpx::~OwnedPpx() 
{}

MIface* OwdRenvAdp::OwnedPpx::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    return res;
}



//// OwdRenvAdp

OwdRenvAdp::OwdRenvAdp(const string& aName, MEnv* aEnv): mName(aName), mEnv(aEnv),
    mPxMgr(aEnv, this, mRenvClient)
{
}

OwdRenvAdp::~OwdRenvAdp()
{
}

MIface* OwdRenvAdp::MOwdRenvAdp_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif(aId, mMOwdRenvAdp));
    else if (aId == MOwned::idHash()) {
        return dynamic_cast<MOwned*>(mOwdPpx);
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
        }
        LOG(EInfo) << "Getting remote env root, resp: " << rroot;
        // Create MOwned proxy to remote root
        // The proxy created is actually primary proxy - specific proxy assosiated to Renv adapter
        mOwdPpx = new OwnedPpx(this, rroot);
        mPxMgr.RegisterProxy(mOwdPpx);

        res = true;
        } while (false);
        return res;
    }

string OwdRenvAdp::getOid() const
{
    return string();
}


