
#include "mnt.h"


// Content: env variable kept mounted model path
const string KCntEnvVarId = "EnvVar";


AMntp::AMntp(const string &aType, const string &aName, MEnv* aEnv): Syst(aType, aName, aEnv), mMdlEnv(nullptr)
{
} 

AMntp::~AMntp() 
{
    if (mMdlEnv) {
	delete mMdlEnv;
	mMdlEnv = nullptr;
    }
}

int AMntp::contCount() const
{
    return Syst::contCount() + 1;
}

bool AMntp::getContentId(int aIdx, string& aRes) const
{
    bool res = false;
    if (aIdx < Syst::contCount()) {
        aRes = Syst::getContentId(aIdx, aRes);
        res = true;
    } else if (aIdx == Syst::contCount()) {
        aRes = KCntEnvVarId;
        res = true;
    }
    return res;
}

bool AMntp::getContent(const string& aId, string& aRes) const
{
    bool res = false;
    if (aId == KCntEnvVarId) {
        aRes = mCntEnvVar;
        res = true;
    } else {
        Syst::getContent(aId, aRes);
    }
    return res;
}

bool AMntp::setContent(const string& aId, const string& aData)
{
    bool res = false;
    if (aId == KCntEnvVarId) {
        mCntEnvVar = aData;
        res = true;
    } else {
        res = Syst::setContent(aId, aData);
    }
    return res;
}


void AMntp::onContentChanged(const string& aId)
{
    if (aId == KCntEnvVarId) {
        string envvid;
        bool res = getContent(aId, envvid);
        if (res) {
            string mpath;
            bool eres = mEnv->getEVar(envvid, mpath);
            if (eres) {
                Logger()->Write(EInfo, this, "Model path from evar: %s", mpath.c_str());
                bool cres = CreateModel(mpath);
                if (cres) {
                    cres = attachOwned(mMdlEnv->Root());
                    if (!cres) {
                        Logger()->Write(EErr, this, "Cannot attach the model [%s]", mpath.c_str());
                    }
                } else {
                    Logger()->Write(EErr, this, "Failed creating the model [%s]", mpath.c_str());
                }
            } else {
                Logger()->Write(EErr, this, "Cannot find env variable [%s]", envvid.c_str());
            }
        } else {
            LOGN(EDbg, "Cannot get content [" + aId + "] data");
        }
    }
    Syst::onContentChanged(aId);
}

bool AMntp::CreateModel(const string& aSpecPath)
{
    bool res = true;
    if (mMdlEnv) {
        delete mMdlEnv;
        mMdlEnv = nullptr;
    }
    // Create model
    mMdlEnv = new Env(aSpecPath, aSpecPath + ".log");
    assert(mEnv);
    mMdlEnv->constructSystem();
    if (!mMdlEnv->Root()) {
        Logger()->Write(EErr, this, "Cannot create model [%s]", aSpecPath.c_str());
	res = false;
    }
    return res;
}

MIface* AMntp::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMMntpPtr));
    else res = Syst::MNode_getLif(aId);
    return res;
}

MNode* AMntp::root() const
{
    return mMdlEnv ? mMdlEnv->Root() : nullptr;
}

MNode* AMntp::getMag()
{
    return mMdlEnv ? mMdlEnv->Root() : nullptr;
}
