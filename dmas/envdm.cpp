
#include "../src/log.h"

#include "envdm.h"
//#include "mrenvadp.h"
#include "renvadp.h"


#define LOG(aLevel) (Logger()->MeetsLevel(aLevel)) && TLog(aLevel, "Env", mLogger).ContentStream()


EnvDm::EnvDm(const string& aSpecFileName, const string& aLogFileName): Env(aSpecFileName, aLogFileName)
{
}

EnvDm::EnvDm(bool aOpt, const string& aSpec, const string& aLogFileName): Env(aOpt, aSpec, aLogFileName)
{
}

EnvDm::~EnvDm()
{
}

bool EnvDm::constructSystem()
{
    bool res = false;
    do {
        res = Env::constructSystem();
        if (!res) {
            LOG(EErr) << "Failed constructing system";
            break;
        }
        // Create owning remote env adapter
        mRenva = new OwgRenvAdp("RenvAdp", this);
        if (!mRenva) {
            LOG(EErr) << "Failed creating remote env adapter";
            break;
        }

        // Bind to remote env (owning)
        string psid, puid, peid;
        res = getEVar("PrimarySID", psid);
        res = res && getEVar("PrimaryEID", peid);
        res = res && getEVar("PrimaryUid", puid);
        if (!res) {
            LOG(EInfo) << "Cannot get primary env IDs, consider self as primary";
            res = true;
            break;
        }
        res = mRenva->bindRenv(psid, peid, puid);
        if (!res) {
            LOG(EErr) << "Failed binding remote environment";
            break;
        }
        // Get owner primary proxy
        auto* owrPx = mRenva->lIft<MOwner>();
        if (!owrPx) {
            LOG(EErr) << "Failed getting owner primary proxy";
            break;
        }
        auto* owrRoot = mRoot->lIft<MOwned>();
        res = owrPx->owrAttachOwned(owrRoot);
        if (!res) {
            LOG(EErr) << "Failed attaching root to owner primary proxy";
            break;
        }
        res = true;
    } while (false);
    return res;
}
