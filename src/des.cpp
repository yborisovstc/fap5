
#include <thread>

//#include "mlink.h"
#include "des.h"
#include "prof_ids.h"

// Enable verification of DES active registry. !Affect system performance
#define DES_RGS_VERIFY

/// CpStateInp

CpStateInp::CpStateInp(const string &aType, const string& aName, MEnv* aEnv):
    ConnPoint(aType, aName, aEnv, MDesInpObserver::idHash(), MDVarGet::idHash()),
    mBcp(mRifId, mPifId, dynamic_cast<MDVarGet*>(this))
{}

MIface* CpStateInp::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLifn(aId, mBcp.mPair ? mBcp.mPair->prov<MDesInpObserver>() : nullptr));
    else res = TBase::MVert_getLif(aId);
    return res;
}

void CpStateInp::onConnected(MVert* aPair)
{
    TBase::onConnected(aPair);
    // Notify inp updated
    if (mBcp.mPair) mBcp.mPair->prov<MDesInpObserver>()->onInpUpdated();
}

void CpStateInp::onDisconnected()
{
    // Notify inp updated
    if (mBcp.mPair) mBcp.mPair->prov<MDesInpObserver>()->onInpUpdated();
}

void CpStateInp::onBound(MVert* aPair)
{
    // Notify inp updated
    // TODO seems we don't need such notif. it's because binding is the
    // primary op followed by conn.
    if (mBcp.mPair) mBcp.mPair->prov<MDesInpObserver>()->onInpUpdated();
}

void CpStateInp::onUnbound(MVert* aPair)
{
    // Notify inp updated
    if (mBcp.mPair) mBcp.mPair->prov<MDesInpObserver>()->onInpUpdated();
}

string CpStateInp::VarGetIfid() const
{
    string res;
    /*
    auto* pair = (mPairs.size() == 1) ? mPairs.at(0) : nullptr; 
    MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
    res = pairDget ? pairDget->VarGetIfid() : nullptr;
    return res;
    */
    // Inp CP allows multiple pairs
    for (auto* pair : mPairs) {
        MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
        if (pairDget) {
            string ifid = pairDget->VarGetIfid();
            if (res.empty()) {
                res = ifid;
            } else if (ifid.empty()) {
                // No id from this pair, just ignore
            } else if (ifid != res) {
                // Id from this pair doesn't match current id - error
                res.clear();
                break;
            }
        }
    }
    return res;
}

const DtBase* CpStateInp::VDtGet(const string& aType)
{
    const DtBase* res = nullptr;
    auto* pair = (mPairs.size() == 1) ? mPairs.at(0) : nullptr; 
    MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
    res = pairDget ? pairDget->VDtGet(aType) : nullptr;
    return res;
}

void CpStateInp::VDtGet(const string& aType, MDVarGet::TData& aData)
{
    for (auto* pair : mPairs) {
        MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
        if (pairDget) {
            pairDget->VDtGet(aType, aData);
        }
    }
}


/// CpStateOutp

CpStateOutp::CpStateOutp(const string &aType, const string& aName, MEnv* aEnv):
    ConnPoint(aType, aName, aEnv, MDVarGet::idHash(), MDesInpObserver::idHash()),
    mBcp(mRifId, mPifId, dynamic_cast<MDesInpObserver*>(this))
{}

MIface* CpStateOutp::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLifn(aId, mBcp.mPair ? mBcp.mPair->prov<MDVarGet>() : nullptr));
    else res = ConnPoint::MVert_getLif(aId);
    return res;
}


void CpStateOutp::onInpUpdated()
{
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        MDesInpObserver* obs = (*it)->lIf(obs);
        if (obs) {
            obs->onInpUpdated();
        }
    }
}


#if 0

CpStateInpPin::CpStateInpPin(const string &aType, const string& aName, MEnv* aEnv):
    TBase(aType, aName, aEnv), mBcp(this)
{}

CpStateInpPin::~CpStateInpPin() {}

string CpStateInpPin::VarGetIfid() const
{
    string res;
    auto* pair = (mPairs.size() == 1) ? mPairs.at(0) : nullptr; 
    MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
    res = pairDget ? pairDget->VarGetIfid() : nullptr;
    return res;
}

void CpStateInpPin::onConnected(MVert* aPair)
{
    CpStateInp::onConnected(aPair);
}

void CpStateInpPin::onDisconnected()
{
}

const DtBase* CpStateInpPin::VDtGet(const string& aType)
{
    const DtBase* res = nullptr;
    auto* pair = (mPairs.size() == 1) ? mPairs.at(0) : nullptr; 
    MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
    res = pairDget ? pairDget->VDtGet(aType) : nullptr;
    return res;
}

void CpStateInpPin::onInpUpdated()
{
    for (auto it = mBcp.pairsBegin(); it != mBcp.pairsEnd(); it++) {
        it->provided()->onInpUpdated();
    }
}






CpStateOutpPin::~CpStateOutpPin() {}

string CpStateOutpPin::VarGetIfid() const
{
    return mProvidedPx ? mProvidedPx->VarGetIfid() : string();
}

const DtBase* CpStateOutpPin::VDtGet(const string& aType)
{
    const DtBase* res = nullptr;
    if (mProvidedPx) {
        // Binded
        mProvidedPx->VDtGet(aType);
    } else {
        // Not binded
        auto* owrn = owner();
        MVert* owrv = owrn ? owrn->lIf(owrv) : nullptr;
        if (owrv) {
            if (owrv->getExtd() == this) {
                // Extender internal point
                auto* pair = (owrv->pairsCount() == 1) ? owrv->getPair(0) : nullptr; 
                auto* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
                res = pairDget ? pairDget->VDtGet(aType) : nullptr;
            }
        } else {
            // TODO handle
        }
    }
    return res;
}

#endif


// CpStateInp direct extender

string ExtdStateInp::KIntName = "Int";

ExtdStateInp::ExtdStateInp(const string &aType, const string& aName, MEnv* aEnv): CpStateInp(aType, aName, aEnv)
{
}

void ExtdStateInp::Construct()
{
    mInt = new CpStateOutp(string(CpStateOutp::idStr()), KIntName, mEnv);
    assert(mInt);
    bool res = attachOwned(mInt);
    assert(res);
    res = mInt->bind(bP());
    assert(res);
}



#if 0
void CpStateInpPin::onDisconnected()
{
}

const DtBase* CpStateInpPin::VDtGet(const string& aType)
{
    const DtBase* res = nullptr;
    auto* pair = (mPairs.size() == 1) ? mPairs.at(0) : nullptr; 
    MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
    res = pairDget ? pairDget->VDtGet(aType) : nullptr;
    return res;
}

void CpStateInpPin::onInpUpdated()
{
    for (auto it = mBcp.pairsBegin(); it != mBcp.pairsEnd(); it++) {
        it->provided()->onInpUpdated();
    }
}
#endif


#if 0

// ExtdStateInp

ExtdStateInp::ExtdStateInp(const string &aType, const string& aName, MEnv* aEnv): Extd(aType, aName, aEnv)
{
}

void ExtdStateInp::Construct()
{
    mInt = new CpStateOutpPin(string(CpStateOutp::idStr()), KUriInt, mEnv);
    assert(mInt);
    bool res = attachOwned(mInt);
    assert(res);
}

MIface* ExtdStateInp::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = mInt ? mInt->checkLif2(aId, mMDesInpObserver) : nullptr);
    else if (res = mInt ? mInt->checkLif2(aId, mMDVarGet) : nullptr);
    else res = Vert::MVert_getLif(aId);
    return res;
}

#endif


/// CpStateOutp direct extender


string ExtdStateOutp::KIntName = "Int";

ExtdStateOutp::ExtdStateOutp(const string &aType, const string& aName, MEnv* aEnv): CpStateOutp(aType, aName, aEnv)
{
}

void ExtdStateOutp::Construct()
{
    mInt = new CpStateInp(string(CpStateInp::idStr()), KIntName, mEnv);
    assert(mInt);
    bool res = attachOwned(mInt);
    assert(res);
    res = mInt->bind(bP());
    assert(res);
}


#if 0
/// CpStateOutp direct extender with ifaces impl

ExtdStateOutpI::ExtdStateOutpI(const string &aType, const string& aName, MEnv* aEnv): ExtdStateOutp(aType, aName, aEnv)
{
}

MIface* ExtdStateOutpI::MNode_getLif(const char *aType)
{
    MIface* res = nullptr;
    if (res = checkLif2(aType, mMDVarGetPtr));
    else res = ExtdStateOutp::MNode_getLif(aType);
    return res;
}

void ExtdStateOutpI::onInpUpdated()
{
    // Rederect to call to pairs
    for (auto pair : mPairs) {
	MUnit* pe = pair->lIf(pe);
	auto* ifcs = pe ? pe->getTIfs<MDesInpObserver>() : nullptr;
	for (auto obs : *ifcs) {
	    // assert(obs);
	    obs->onInpUpdated();
	}
    }
}

const DtBase* ExtdStateOutpI::VDtGet(const string& aType)
{
    // Redirect to internal point
    MVert* intcp = getExtd();
    MUnit* intcpu = intcp ? intcp->lIf(intcpu) : nullptr;
    MDVarGet* inpvg = intcpu ? intcpu->getSif(inpvg) : nullptr;
    return inpvg ? inpvg->VDtGet(aType) : nullptr;
}

string ExtdStateOutpI::VarGetIfid() const
{
    // Redirect to internal point
    auto* self = const_cast<ExtdStateOutpI*>(this);
    MVert* intcp = self->getExtd();
    MUnit* intcpu = intcp ? intcp->lIf(intcpu) : nullptr;
    MDVarGet* inpvg = intcpu ? intcpu->getSif(inpvg) : nullptr;
    return inpvg ? inpvg->VarGetIfid() : string();
}

void ExtdStateOutpI::resolveIfc(const string& aName, MIfReq::TIfReqCp* aReq)
{
    MIface* ifr = MNode_getLif(aName.c_str()); // Local
    if (ifr) {
	addIfpLeaf(ifr, aReq);
    } else if (aName == MDesInpObserver::Type()) {
	// Enable MDesInpObserver resolution for input
	MVert* intcp = getExtd();
	MUnit* intcpu = intcp ? intcp->lIf(intcpu) : nullptr;
	MIfProvOwner* intcpo = intcpu ? intcpu->lIf(intcpo) : nullptr;
	if (intcpo && aReq->provided()->isRequestor(intcpo)) {
	    ifr = mMDesInpObserverPtr ? mMDesInpObserverPtr : (mMDesInpObserverPtr = dynamic_cast<MDesInpObserver*>(this));
	    addIfpLeaf(ifr, aReq);
	}
    } else {
	ExtdStateOutp::resolveIfc(aName, aReq);
    }
}


/// CpStateMnodeInp direct extender 

ExtdStateMnodeInp::ExtdStateMnodeInp(const string &aType, const string& aName, MEnv* aEnv): Extd(aType, aName, aEnv)
{
    MNode* cp = Provider()->createNode(CpStateMnodeOutp::Type(), Extd::KUriInt , mEnv);
    assert(cp);
    bool res = attachOwned(cp);
    assert(res);
}


/// CpStateMnodeOutp direct extender 

ExtdStateMnodeOutp::ExtdStateMnodeOutp(const string &aType, const string& aName, MEnv* aEnv): Extd(aType, aName, aEnv)
{
    MNode* cp = Provider()->createNode(CpStateMnodeInp::Type(), Extd::KUriInt , mEnv);
    assert(cp);
    bool res = attachOwned(cp);
    assert(res);
}
#endif



// State, ver. 2, non-inhritable, monolitic, direct data, switching updated-confirmed

const string State::KCont_Value = "";
const string State::KInpName = "Inp";

State::State(const string &aType, const string& aName, MEnv* aEnv): ConnPoint(aType, aName, aEnv, MDVarGet::idHash(), MDesInpObserver::idHash()),
    mDesSyncCp(this), mPdata(NULL), mCdata(NULL), mUpdNotified(false), mActNotified(false), mStDead(false), mInp(nullptr),
    mInpBp(mRifId, mPifId, dynamic_cast<MDesInpObserver*>(this))
{ }

void State::Construct()
{
    //addComp(string(CpStateInp::idStr()), KInpName);
    mInp = new CpStateInp(string(CpStateInp::idStr()), KInpName, mEnv);
    assert(mInp);
    bool res = attachOwned(mInp);
    assert(res);
    //mInp->MNode::lIft<MConnPoint>()->bind(&mInpBp);
    mInp->bind(&mInpBp);
}

State::~State()
{
    if (mPdata) {
	delete mPdata;
    }
    if (mCdata) {
	delete mCdata;
    }
    mStDead = true;
}

MIface* State::MNode_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesSyncable));
    else if (res = checkLif2(aId, mMDesInpObserver));
    else if (res = checkLif2(aId, mMDVarGet));
    else if (res = checkLif2(aId, mMDVarSet));
    else res = TBase::MNode_getLif(aId);
    return res;
}

MIface* State::MOwner_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesSyncable)); // ??
    else res = TBase::MOwner_getLif(aId);
    return res;
}

MIface* State::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDesSyncable));
    else res = TBase::MOwned_getLif(aId);
    return res;
}

MIface* State::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDVarGet));
    else if (res = checkLif2(aId, mMDVarSet));
    else res = TBase::MVert_getLif(aId);
    return res;
}

/*
MDesObserver* State::desObserver()
{
    MDesObserver* res = nullptr;
    auto it = mDesSyncCp.pairsBegin();
    if (it != mDesSyncCp.pairsEnd()) {
        res = it->provided();
    }
}
*/

bool State::getContentId(int aIdx, string& aRes) const
{
    bool res = true;
    if (aIdx < contCount()) {
        if (aIdx < Node::contCount()) {
            Node::getContentId(aIdx, aRes);
        } else {
            aRes = KCont_Value;
        }
    } else {
        res = false;
    }
    return res;
}

bool State::getContent(const string& aId, string& aRes) const
{
    bool res = true;
    if (aId == KCont_Value) {
        aRes = mCdata->ToString();
    } else {
	res = TBase::getContent(aId, aRes);
    }
    return res;
}

bool State::setContent(const string& aId, const string& aData)
{
    bool res = true;
    if (aId == KCont_Value)
	res = updateWithContValue(aData);
    else
	res = TBase::setContent(aId, aData);
    return res;
}

void State::setActivated()
{
    if (!mActNotified) {
        // Propagate activation to owner
        MDesObserver* obs = desObserver();
	if (obs) {
	    obs->onActivated(this);
	    mActNotified = true;
	}
    }
}

void State::update()
{
    if (mName == "St3") {
        LOGN(EDbg, "update");
    }
    PFL_DUR_STAT_START(PEvents::EDurStat_StUpdate);
    mActNotified = false;
    string dtype;
    if (mCdata) {
	dtype = mCdata->GetTypeSig();
    }
    MDVarGet* vget = GetInp();
    if (vget) {
	mInpValid = true;
	const DtBase* pdata = nullptr;
	try {
	    pdata = vget->VDtGet(dtype);
	} catch (std::exception e) {
	    Logger()->Write(EErr, this, "Unspecified error on update");
	}
	if (pdata) {
	    if (!mPdata) {
		mPdata = CreateData(pdata->GetTypeSig());
	    }
	    if (mPdata) {
		*mPdata = *pdata;
	    }
	} else {
            mInpValid = false;
	}
    } else {
	mInpValid = false;
    }
    PFL_DUR_STAT_REC(PEvents::EDurStat_StUpdate);
}

void State::confirm()
{
    //if (mName == "TupleRes2") {
    //    LOGN(EInfo, "confirm");
    //}
    mUpdNotified = false;
    bool changed = false;
    PFL_DUR_STAT_START(PEvents::EDurStat_StConfirm);
    if (mCdata) {
	string old_value;
	if (LOG_LEVEL(EDbg)) {
	    old_value = mCdata->ToString();
	}
	if (mInpValid) {
	    if (mPdata) {
		if (*mCdata != *mPdata) {
		    // Swap the data
		    auto ptr = mCdata;
		    mCdata = mPdata;
		    mPdata = ptr;
		    notifyInpsUpdated();
		    changed = true;
		}
	    } else {
		if (mCdata->IsValid()) {
		    mCdata->mValid = false;
		    changed = true;
		}
	    }
	}
	if (changed) {
	    LOGN(EDbg, "Updated [" + mCdata->ToString() + " <- " + old_value + "]");
	}
    } else {
	if (mPdata) {
	    mCdata = CreateData(mPdata->GetTypeSig());
	}
	if (mCdata) {
	    *mCdata = *mPdata;
	    notifyInpsUpdated();
	    LOGN(EInfo, "Updated [" + mCdata->ToString() + "]");
	} else {
	    LOGN(EInfo, "Not initialized");
	}
    }
    PFL_DUR_STAT_REC(PEvents::EDurStat_StConfirm);
}

void State::setUpdated()
{
    if (!mUpdNotified) {
        MDesObserver* obs = desObserver();
	if (obs) {
	    obs->onUpdated(this);
	    mUpdNotified = true;
	}
    }
}

void State::onInpUpdated()
{
    if (!mStDead) {
	setActivated();
    }
}

MDVarGet* State::GetInp()
{
      return mInpBp.pcount() ? mInpBp.pairAt(0)->prov<MDVarGet>() : nullptr;
}

string State::VarGetIfid() const
{
    return mCdata ? mCdata->GetTypeSig() : string();
}

string State::VarGetSIfid()
{
    return mCdata ? mCdata->GetTypeSig() : string();
}

const bool State::VDtSet(const DtBase& aData)
{
    bool res = false;
    if (mCdata && mPdata) {
	*mPdata = aData;
	*mCdata = aData;
	if (mCdata->IsChanged()) {
	    notifyInpsUpdated();
	}
    }
    return res;
}

DtBase* State::CreateData(const string& aType)
{
   return Provider()->createData(aType);
}

bool State::updateWithContValue(const string& aData)
{
    bool res = false;
    if (!mCdata) {
	string type;
	DtBase::ParseSigPars(aData, type);
	mCdata = CreateData(type);
	mPdata = CreateData(type);
    }
    if (mCdata) {
	res = true;
	mCdata->FromString(aData);
	mPdata->FromString(aData);
	if (mCdata->IsValid()) {
	    if (mCdata->IsChanged()) {
		LOGN(EVerbose, "Initialized:  " + mCdata->ToString(true) + "]");
		notifyInpsUpdated();
	    }
	}  else {
	    if (!mCdata->IsValid() && mCdata->IsDsError()) {
		LOGN(EErr, "Error on applying content [" + mName + "] value [" + aData + "]");
		mCdata->FromString(aData);
	    }
	}
    }
    return res;
}

DtBase* State::VDtGet(const string& aType)
{
    // Enable getting base data
    bool dataMatched = ((aType == mCdata->GetTypeSig()) || aType.empty());
    if (!dataMatched) {
        LOGN(EErr, "Requesting incompatible data [" + aType + "], supported [" + mCdata->GetTypeSig() + "]");
    } 
    return dataMatched ? mCdata : nullptr;
}

void State::VDtGet(const string& aType, MDVarGet::TData& aData)
{
    DtBase* dt = VDtGet(aType);
    aData.push_back(dt);
}

void State::notifyInpsUpdated()
{
    for (auto* pair : mPairs) {
        auto obs = pair->lIft<MDesInpObserver>();
        if (obs) obs->onInpUpdated();
    }
}


// --- BState - Binded State ---

const string BState::KCont_Value = "";

BState::BState(const string &aType, const string& aName, MEnv* aEnv): ConnPoint(aType, aName, aEnv, MDVarGet::idHash(), MDesInpObserver::idHash()),
    mDesSyncCp(this), mPdata(NULL), mCdata(NULL), mUpdNotified(false), mActNotified(false), mStDead(false),
    mInpBp(MDesInpObserver::idHash(), MDVarGet::idHash(), dynamic_cast<MDesInpObserver*>(this))
{ }

BState::~BState()
{
    if (mPdata) {
	delete mPdata;
    }
    if (mCdata) {
	delete mCdata;
    }
    mStDead = true;
}

MIface* BState::MNode_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesSyncable));
    else if (res = checkLif2(aId, mMDesInpObserver));
    else if (res = checkLif2(aId, mMDVarGet));
    else if (res = checkLif2(aId, mMDVarSet));
    else res = TBase::MNode_getLif(aId);
    return res;
}

MIface* BState::MOwner_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesSyncable)); // ??
    else res = TBase::MOwner_getLif(aId);
    return res;
}

MIface* BState::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDesSyncable));
    else res = TBase::MOwned_getLif(aId);
    return res;
}

bool BState::getContentId(int aIdx, string& aRes) const
{
    bool res = true;
    if (aIdx < contCount()) {
        if (aIdx < Node::contCount()) {
            Node::getContentId(aIdx, aRes);
        } else {
            aRes = KCont_Value;
        }
    } else {
        res = false;
    }
    return res;
}

bool BState::getContent(const string& aId, string& aRes) const
{
    bool res = true;
    if (aId == KCont_Value) {
        aRes = mCdata->ToString();
    } else {
	res = TBase::getContent(aId, aRes);
    }
    return res;
}

bool BState::setContent(const string& aId, const string& aData)
{
    bool res = true;
    if (aId == KCont_Value)
	res = updateWithContValue(aData);
    else
	res = TBase::setContent(aId, aData);
    return res;
}


void BState::setActivated()
{
    if (!mActNotified) {
        // Propagate activation to owner
        MDesObserver* obs = desObserver();
	if (obs) {
	    obs->onActivated(this);
	    mActNotified = true;
	}
    }
}

void BState::update()
{
    PFL_DUR_STAT_START(PEvents::EDurStat_StUpdate);
    mActNotified = false;
    string dtype;
    if (mCdata) {
	dtype = mCdata->GetTypeSig();
    }
    MDVarGet* vget = GetInp();
    if (vget) {
	mInpValid = true;
	const DtBase* pdata = nullptr;
	try {
	    pdata = vget->VDtGet(dtype);
	} catch (std::exception e) {
	    Logger()->Write(EErr, this, "Unspecified error on update");
	}
	if (pdata) {
	    if (!mPdata) {
		mPdata = CreateData(pdata->GetTypeSig());
	    }
	    if (mPdata) {
		*mPdata = *pdata;
	    }
	} else {
            mInpValid = false;
	}
    } else {
	mInpValid = false;
    }
    PFL_DUR_STAT_REC(PEvents::EDurStat_StUpdate);
}

void BState::confirm()
{
    mUpdNotified = false;
    bool changed = false;
    PFL_DUR_STAT_START(PEvents::EDurStat_StConfirm);
    if (mCdata) {
	string old_value;
	if (LOG_LEVEL(EDbg)) {
	    old_value = mCdata->ToString();
	}
	if (mInpValid) {
	    if (mPdata) {
		if (*mCdata != *mPdata) {
		    // Swap the data
		    auto ptr = mCdata;
		    mCdata = mPdata;
		    mPdata = ptr;
		    notifyInpsUpdated();
		    changed = true;
		}
	    } else {
		if (mCdata->IsValid()) {
		    mCdata->mValid = false;
		    changed = true;
		}
	    }
	}
	if (changed) {
	    LOGN(EDbg, "Updated [" + mCdata->ToString() + " <- " + old_value + "]");
	}
    } else {
	if (mPdata) {
	    mCdata = CreateData(mPdata->GetTypeSig());
	}
	if (mCdata) {
	    *mCdata = *mPdata;
	    notifyInpsUpdated();
	    LOGN(EInfo, "Updated [" + mCdata->ToString() + "]");
	} else {
	    LOGN(EInfo, "Not initialized");
	}
    }
    PFL_DUR_STAT_REC(PEvents::EDurStat_StConfirm);
}

void BState::setUpdated()
{
    if (!mUpdNotified) {
        MDesObserver* obs = desObserver();
	if (obs) {
	    obs->onUpdated(this);
	    mUpdNotified = true;
	}
    }
}

void BState::onInpUpdated()
{
    if (!mStDead) {
	setActivated();
    }
}

MDVarGet* BState::GetInp()
{
    return mInpBp.pcount() ? mInpBp.pairAt(0)->prov<MDVarGet>() : nullptr;
}

string BState::VarGetIfid() const
{
    return mCdata ? mCdata->GetTypeSig() : string();
}

string BState::VarGetSIfid()
{
    return mCdata ? mCdata->GetTypeSig() : string();
}

const bool BState::VDtSet(const DtBase& aData)
{
    bool res = false;
    if (mCdata && mPdata) {
	*mPdata = aData;
	*mCdata = aData;
	if (mCdata->IsChanged()) {
	    notifyInpsUpdated();
	}
    }
    return res;
}

DtBase* BState::CreateData(const string& aType)
{
   return Provider()->createData(aType);
}

bool BState::updateWithContValue(const string& aData)
{
    bool res = false;
    if (!mCdata) {
	string type;
	DtBase::ParseSigPars(aData, type);
	mCdata = CreateData(type);
	mPdata = CreateData(type);
    }
    if (mCdata) {
	res = true;
	mCdata->FromString(aData);
	mPdata->FromString(aData);
	if (mCdata->IsValid()) {
	    if (mCdata->IsChanged()) {
		LOGN(EVerbose, "Initialized:  " + mCdata->ToString(true) + "]");
		notifyInpsUpdated();
	    }
	}  else {
	    if (!mCdata->IsValid() && mCdata->IsDsError()) {
		LOGN(EErr, "Error on applying content [" + mName + "] value [" + aData + "]");
		mCdata->FromString(aData);
	    }
	}
    }
    return res;
}

DtBase* BState::VDtGet(const string& aType)
{
    // Enable getting base data
    bool dataMatched = ((aType == mCdata->GetTypeSig()) || aType.empty());
    if (!dataMatched) {
        LOGN(EErr, "Requesting incompatible data [" + aType + "], supported [" + mCdata->GetTypeSig() + "]");
    } 
    return dataMatched ? mCdata : nullptr;
}

void BState::notifyInpsUpdated()
{
    for (auto* pair : mPairs) {
        auto obs = pair->lIft<MDesInpObserver>();
        if (obs) obs->onInpUpdated();
    }
}


// Const

const string Const::KCont_Value = "";

Const::Const(const string &aType, const string& aName, MEnv* aEnv): ConnPoint(aType, aName, aEnv, MDVarGet::idHash(), MDesInpObserver::idHash()),
    mData(NULL), mStDead(false)
{ }


Const::~Const()
{
    if (mData) {
	delete mData;
    }
    mStDead = true;
}

MIface* Const::MNode_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDVarGet));
    else res = TBase::MNode_getLif(aId);
    return res;
}

MIface* Const::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDVarGet));
    else res = TBase::MVert_getLif(aId);
    return res;
}


MIface* Const::MOwner_getLif(TIdHash aId)
{
    MIface* res = NULL;
    res = TBase::MOwner_getLif(aId);
    return res;
}

MIface* Const::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    res = TBase::MOwned_getLif(aId);
    return res;
}

bool Const::getContentId(int aIdx, string& aRes) const
{
    bool res = true;
    if (aIdx < contCount()) {
        if (aIdx < Node::contCount()) {
            Node::getContentId(aIdx, aRes);
        } else {
            aRes = KCont_Value;
        }
    } else {
        res = false;
    }
    return res;
}

bool Const::getContent(const string& aId, string& aRes) const
{
    bool res = true;
    if (aId == KCont_Value) {
        aRes = mData->ToString();
    } else {
	res = TBase::getContent(aId, aRes);
    }
    return res;
}

bool Const::setContent(const string& aId, const string& aData)
{
    bool res = true;
    if (aId == KCont_Value)
	res = updateWithContValue(aData);
    else
	res = TBase::setContent(aId, aData);
    return res;
}

string Const::VarGetIfid() const
{
    return mData ? mData->GetTypeSig() : string();
}

DtBase* Const::CreateData(const string& aType)
{
   return Provider()->createData(aType);
}

bool Const::updateWithContValue(const string& aData)
{
    bool res = false;
    if (!mData) {
	string type;
	DtBase::ParseSigPars(aData, type);
	mData = CreateData(type);
    }
    if (mData) {
	res = true;
	mData->FromString(aData);
	if (mData->IsValid()) {
	    if (mData->IsChanged()) {
		LOGN(EVerbose, "Initialized:  " + mData->ToString(true) + "]");
		notifyInpsUpdated();
	    }
	}  else {
	    if (!mData->IsValid() && mData->IsDsError()) {
		LOGN(EErr, "Error on applying content [" + mName + "] value [" + aData + "]");
		mData->FromString(aData);
	    }
	}
    }
    return res;
}

DtBase* Const::VDtGet(const string& aType)
{
    // Enable getting base data
    return (mData && (aType == mData->GetTypeSig() || aType.empty())) ? mData : nullptr;
}

void Const::notifyInpsUpdated()
{
    for (auto* pair : mPairs) {
        auto obs = pair->lIft<MDesInpObserver>();
        if (obs) obs->onInpUpdated();
    }
}




/// DES

const GUri Des::KControlledUri = "Controlled";

Des::Des(const string &aType, const string &aName, MEnv* aEnv): Syst(aType, aName, aEnv), mDesSyncCp(this), mDesObsrCp(this), mUpdNotified(false), mActNotified(false),
mPaused(false)
{
}

MIface* Des::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDesSyncable));
    else if (res = checkLif2(aId, mMDesObserver));
    //else if (res = checkLif2(aType, mMDesAdapter));
    //else if (res = checkLif2(aType, mMDesManageablePtr));
    else res = Syst::MNode_getLif(aId);
    return res;
}

void Des::update()
{
    mUpd = true;
    if (!mPaused) {
        for (auto comp : *mActive) {
            try {
                comp->update();
            } catch (std::exception e) {
                LOGN(EErr, "Error on update [" + comp->Uid() + "]");
            }
        }
        // Swapping the lists
        auto upd = mUpdated;
        mUpdated = mActive;
        mActive = upd;
        mActive->clear();
    }
    mActNotified = false;
    mUpd = false;
}

void Des::confirm()
{
    // Using safe iteration because mUpdated can be changed via some comp confirm
    while (!mUpdated->empty()) {
        auto* comp = mUpdated->back();
	try {
	    comp->confirm();
	} catch (std::exception e) {
	    LOGN(EErr, "Error on confirm [" + comp->Uid() + "]");
	}
        mUpdated->pop_back();
    }
    if (mIsActive && !mActNotified) {
	mIsActive = false;
        notifyChanged(); // TODO do we need it?
    }
    mUpdNotified = false;
}

void Des::setUpdated()
{
    if (!mUpdNotified) {
        MDesObserver* obs = desObserver();
	if (obs) {
	    obs->onUpdated(this);
	    mUpdNotified = true;
	}
    }
}

void Des::setActivated()
{
    if (!mActNotified) {
	// Propagate activation to owner
        MDesObserver* obs = desObserver();
	if (obs) {
	    obs->onActivated(this);
	    mActNotified = true;
	    // TODO Improve notification design, ref ds_obsi
	    if (!mIsActive) {
		mIsActive = true;
		notifyChanged(); // TODO do we need it?
	    }
	}
    }
}

bool Des::isActive() const
{
    return mIsActive;
}

int Des::countOfActive(bool aLocal) const
{
    int res = mActive->size();
    if (!aLocal) {
	for (auto item : *mActive) {
	    res += item->countOfActive(aLocal);
	}
    }
    return res;
}

void Des::onActivated(MDesSyncable* aComp)
{
    assert(!mUpd);
    setActivated();
    if (aComp) {
#ifdef DES_RGS_VERIFY
	for (auto comp : *mActive) {
	    assert(aComp != comp);
	}
#endif
	mActive->push_back(aComp);
    }
}

void Des::onUpdated(MDesSyncable* aComp)
{
//    setUpdated();
}

void Des::RmSyncable(TScblReg& aReg, MDesSyncable* aScbl)
{
    /*
    aReg.remove(aScbl);
    */
    for (auto it = aReg.begin(); it != aReg.end(); it++) {
	if (*it == aScbl) {
	    aReg.erase(it);
	    break;
	}
    }
}

void Des::onOwnedAttached(MOwned* aOwned)
{
    if (aOwned->ownedId() == "DrpCtx") {
        LOGN(EDbg, "onOwnedAttached - DrpCtx");
    }
    Syst::onOwnedAttached(aOwned);
    MDesSyncable* os = aOwned->lIf(os);
    if (os) {
        mDesObsrCp.connect(os->desSyncableCp());
	os->setActivated();
    }
    /*
    if (MDesCtxCsm* csm = aOwned->lIf(csm)) {
        MDesCtxBinder* owrdcb = owner() ? owner()->lIf(owrdcb) : nullptr;
        if (owrdcb) {
            owrdcb->bindDesCtx(csm);
        }
    } else if (MDesCtxSpl* spl = aOwned->lIf(spl)) {
        for (auto pitr = ownerCp()->pairsBegin(); pitr != ownerCp()->pairsEnd(); pitr++) {
            auto owdCp = *pitr;
            MDesCtxBinder* bnd = owdCp->provided()->lIf(bnd);
            if (bnd) {
                bnd->bindDesCtx(spl);
            }
        }
    }
    */
}

void Des::onOwnedDetached(MOwned* aOwned)
{
    Syst::onOwnedDetached(aOwned);
    MDesSyncable* os = aOwned->lIf(os);
    if (os) {
	RmSyncable(*mActive, os);
	RmSyncable(*mUpdated, os);
    }
}

MIface* Des::MOwned_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesSyncable));
    else if (res = checkLif2(aId, mMDesCtxBinder));
    else res = Syst::MOwned_getLif(aId);
    return res;
}

MIface* Des::MOwner_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesObserver)); // Notifying from owned 
    else if (res = checkLif2(aId, mMDesCtxBinder));
    //else if (res = checkLif2(aId, mMDesAdapter));
    else res = Syst::MOwner_getLif(aId);
    return res;
}

void Des::MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const
{
    if (aLevel & Ifu::EDM_Opt1) {
        Ifu::offset(aIdt, aOs); aOs << "Active:" << endl;
        for (auto item : *mActive) {
            Ifu::offset(aIdt, aOs); aOs << item->Uid() << endl;
            if (aLevel & Ifu::EDM_Recursive) {
                item->MDesSyncable_doDump(aLevel, aIdt + 4, aOs);
            }
        }
    }
    if (aLevel & Ifu::EDM_Opt2) {
        Ifu::offset(aIdt, aOs); aOs << "Updated:" << endl;
        for (auto item : *mUpdated) {
            Ifu::offset(aIdt, aOs); aOs << item->Uid() << endl;
            if (aLevel & Ifu::EDM_Recursive) {
                item->MDesSyncable_doDump(aLevel, aIdt + 4, aOs);
            }
        }
    }
    if (aLevel & Ifu::EDM_Opt3) {
        Ifu::offset(aIdt, aOs); aOs << "Syncable:" << endl;
        auto self = const_cast<Des*>(this);
        for (auto it = self->mDesObsrCp.pairsBegin(); it != self->mDesObsrCp.pairsEnd(); it++) {
            MDesSyncable* item = (*it)->provided();
            if (item && item != this) {
                Ifu::offset(aIdt, aOs); aOs << item->Uid() << endl;
                if (aLevel & Ifu::EDM_Recursive) {
                    item->MDesSyncable_doDump(Ifu::EDM_Opt3, aIdt + 4, aOs);
                }
            }
        }
    }
}

void Des::MDesObserver_doDump(int aLevel, int aIdt, ostream& aOs) const
{
    MDesSyncable_doDump(aLevel, aIdt, aOs);
    const MDesObserver* obs = desObserver();
    if (obs && (aLevel & Ifu::EDM_Recursive)) {
        aIdt += 4;
        Ifu::offset(aIdt, aOs); aOs << ">> " << obs->Uid() << endl;
        obs->MDesObserver_doDump(aLevel, aIdt, aOs);
    }
}

/*
void Des::pauseDes()
{
    mPaused = true;
}

void Des::resumeDes()
{
    mPaused = false;
    setActivated();
}

bool Des::isPaused() const
{
    return mPaused;
}


MNode* Des::getMag()
{
    return getNode(Des::KControlledUri);
}

*/


bool Des::bindDesCtx(MIface* aCtx)
{
    bool res = false;
    bool found = false;
    if (MDesCtxCsm* csm = aCtx->lIf(csm)) {
        if (MVert* csmv = csm->lIf(csmv)) {
            for (auto pitr = ownerCp()->pairsBegin(); pitr != ownerCp()->pairsEnd(); pitr++) {
                auto owdCp = *pitr;
                MDesCtxSpl* spl = owdCp->provided()->lIf(spl);
                if (spl && spl->getId() == csm->getId()) {
                    MVert* splv = spl->lIf(csmv);
                    if (splv) {
                        res = MVert::connect(csmv, splv);
                        if (!res) {
                            LOGN(EErr, "Failed connecting csm to spl " + spl->getId());
                        }
                    } else {
                        LOGN(EErr, "No Vert iface in DES supplier " + spl->getId());
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                // Propagate to owner
                MDesCtxBinder* owrdcb = owner() ? owner()->lIf(owrdcb) : nullptr;
                if (owrdcb) {
                    res = owrdcb->bindDesCtx(csm);
                }
            }
        } else {
            LOGN(EErr, "No Vert iface in DES consumer");
        }
    } else if (MDesCtxSpl* spl = aCtx->lIf(spl)) {
        if (MVert* splv = spl->lIf(splv)) {
            for (auto pitr = ownerCp()->pairsBegin(); pitr != ownerCp()->pairsEnd(); pitr++) {
                auto owdCp = *pitr;
                MDesCtxCsm* owdcsm = owdCp->provided()->lIf(owdcsm);
                if (spl && owdcsm && owdcsm->getId() == spl->getId()) {
                    MVert* csmv = owdcsm->lIf(csmv);
                    if (csmv) {
                        res = MVert::connect(csmv, splv);
                        if (!res) {
                            LOGN(EErr, "Failed connecting spl to csm " + spl->getId());
                            break;
                        }
                    } else {
                        LOGN(EErr, "No Vert iface in DES consumer " + csm->getId());
                        break;
                    }
                    found = true;
                }
            }
            if (!found) {
                // Propagate to owned
                for (auto pitr = ownerCp()->pairsBegin(); pitr != ownerCp()->pairsEnd(); pitr++) {
                    auto owdCp = *pitr;
                    MDesCtxBinder* owdbnd = owdCp->provided()->lIf(owdbnd);
                    if (owdbnd) {
                        owdbnd->bindDesCtx(aCtx);
                    }
                }
            }

        } else {
            LOGN(EErr, "No Vert iface in DES supplier");
        }
    }
    return res;
}


//// DesLauncher


const string KCont_TopDesUri = "Path";
const string KCont_Output = "OutpCount";
const string KUri_Counter = "Counter";


DesLauncher::DesLauncher(const string &aType, const string& aName, MEnv* aEnv): Des(aType, aName, aEnv), mStop(false)
        {
        }


bool DesLauncher::Run(int aCount, int aIdleCount)
{
    bool res = true;
    int cnt = 0;
    int idlecnt = 0;
    LOGN(EInfo, "START");
    PFL_DUR_STAT_START(PEvents::EDurStat_LaunchRun);
    while (!mStop && (aCount == 0 || cnt < aCount) && (aIdleCount == 0 || idlecnt < aIdleCount)) {
        mCounter++;
        if (!mActive->empty()) {
            //updateCounter(cnt); // TODO Is it needed?
            LOGN(EDbg, ">>> Update [" + to_string(mCounter) + "], count: " + to_string(countOfActive()) +
                    ", dur: " + PFL_DUR_VALUE(PEvents::EDur_LaunchActive));
            PFL_DUR_START(PEvents::EDur_LaunchActive);
            PFL_DUR_STAT_START(PEvents::EDurStat_LaunchUpdate);
            update();
            PFL_DUR_STAT_REC(PEvents::EDurStat_LaunchUpdate);
            PFL_DUR_STAT_START(PEvents::EDurStat_LaunchConfirm);
            if (isLogLevel(EDbg)) {
                LOGN(EDbg, ">>> Confirm [" + to_string(mCounter) + "]");
            }
            //outputCounter(cnt); // TODO Is it needed?
            confirm();
            PFL_DUR_STAT_REC(PEvents::EDurStat_LaunchConfirm);
            cnt++;
            idlecnt = 0;
            PFL_DUR_REC(PEvents::EDur_LaunchActive);
        } else {
            if (idlecnt == 0) {
                LOGN(EInfo, "IDLE");
            }
            //LOGN(EInfo, "Idle [" + to_string(idlecnt) + "]");
            OnIdle();
            idlecnt++;
        }
    }
    PFL_DUR_STAT_REC(PEvents::EDurStat_LaunchRun);
    LOGN(EInfo, "END " + PFL_DUR_STAT_FIELD(PEvents::EDurStat_LaunchRun, PIndFId::EStat_SUM));
    return res;
}

bool DesLauncher::Stop()
{
    mStop = true;
    return true;
}

void DesLauncher::OnIdle()
{
    this_thread::sleep_for(std::chrono::milliseconds(100));
    //mStop = true;
}

MIface* DesLauncher::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMLauncher));
    else res = Des::MOwned_getLif(aId);
    return res;
}

MIface* DesLauncher::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMLauncher));
    else res = Des::MNode_getLif(aId);
    return res;
}

void DesLauncher::updateCounter(int aCnt)
{
    MNode* ctrn = getNode(KUri_Counter);
    if (ctrn) {
        MContentOwner* cnto = ctrn ? ctrn->lIf(cnto) : nullptr;
        if (cnto) {
            cnto->setContent("", string("SS " + to_string(aCnt)));
        }
    }
}

/*
void DesLauncher::outputCounter(int aCnt)
{
    if (getCont(KCont_Output)) {
        cout << "Count : " << aCnt << endl;
    }
}
*/


#if 0
// Seems not used anymore

/// Input access point base

void DesIapb::Construct(CpStateInp* aInp)
{
    aInp->bind(this);
    mInp = aInp;
}
#endif



/// Active subsystem of DES

static const string K_SsInitUri = "Init";
static const string K_SsUri = "Subsys";

DesAs::DesAs(const string &aType, const string& aName, MEnv* aEnv): DesLauncher(aType, aName, aEnv),
    mRunning(false)
{
}

bool DesAs::Run(int aCount, int aIdleCount)
{
    bool res = true;
    int cnt = 0;
    int idlecnt = 0;
    do {
        /*
        MNode* ss = getNode(K_SsUri);
        if (ss == nullptr) {
            LOGN(EErr, "No subsystem [" + K_SsUri + "] found");
            break;
        }
        MNode* ssinit = ss->getNode(K_SsInitUri);
        */
        MNode* ssinit = getNode(K_SsInitUri);
        if (!ssinit) {
            LOGN(EErr, "Couldn't find Init state");
            res = false; break;
        }
        MContentOwner* cntInit = ssinit->lIf(cntInit);
        if (!cntInit) {
            LOGN(EErr, "Couldn't find Init state content");
            res = false; break;
        }
        cntInit->setContent("", "SB true");
        if (!mActive->empty()) {
            LOGN(EInfo, ">>> Init update");
            Des::update();
            if (!mUpdated->empty()) {
                LOGN(EInfo, ">>> Init confirm");
                Des::confirm();
            }
        }
        cntInit->setContent("", "SB false");
        // Run subsystem
        while (!mStop && (aCount == 0 || cnt < aCount) && !mActive->empty()) {
            //updateCounter(cnt);
            LOGN(EInfo, ">>> Update [" + to_string(cnt) + "]");
            Des::update();
            if (!mUpdated->empty()) {
                LOGN(EInfo, ">>> Confirm [" + to_string(cnt) + "]");
                //outputCounter(cnt);
                Des::confirm();
            }
            cnt++;
        }
    } while (false);
    return res;
}

void DesAs::update()
{
    PFL_DUR_STAT_START(PEvents::EDurStat_DesAsUpd);
    mRunning = true;
    bool res = Run(0, 1);
    mRunning = false;
    if (!res) {
        LOGN(EErr, "Failed run");
    }
    PFL_DUR_STAT_REC(PEvents::EDurStat_DesAsUpd);
}

// Running on confirm phase, ref solution ds_desas_nio_ric
void DesAs::confirm()
{
}

void DesAs::setActivated()
{
    if (!mRunning) {
        DesLauncher::setActivated();
    }
}



/// Active subsystem of DES ver.2, ds_desas_nio_do

const GUri DesAs2::K_OutpUri = "Output";
const GUri DesAs2::K_SsysUri = "Subsys";
const GUri DesAs2::K_SsysInitUri = "Init";


DesAs2::DesAs2(const string &aType, const string& aName, MEnv* aEnv): DesLauncher(aType, aName, aEnv),
    mRunning(false)
{
}

bool DesAs2::Run(int aCount, int aIdleCount)
{
    bool res = true;
    int cnt = 0;
    int idlecnt = 0;
    do {
        MNode* ss = getNode(K_SsysUri);
        if (mSubsys == nullptr) {
            mSubsys = ss->lIf(mSubsys);
        }
        if (mSubsys == nullptr) {
            LOGN(EErr, "No subsystem [" + K_SsysUri.toString() + "] found");
            break;
        }
        MNode* ssinit = ss->getNode(K_SsysInitUri);
        if (!ssinit) {
            LOGN(EErr, "Couldn't find Init state");
            res = false; break;
        }
        MContentOwner* cntInit = ssinit->lIf(cntInit);
        if (!cntInit) {
            LOGN(EErr, "Couldn't find Init state content");
            res = false; break;
        }
        cntInit->setContent("", "SB true");
        if (!mActive->empty()) {
            LOGN(EInfo, ">>> Init update");
            mSubsys->update();
            LOGN(EInfo, ">>> Init confirm");
            mSubsys->confirm();
        }
        cntInit->setContent("", "SB false");
        // Run subsystem
        while (!mStop && (aCount == 0 || cnt < aCount) && mSubsys->isActive()) {
            //updateCounter(cnt);
            LOGN(EInfo, ">>> Subs Update [" + to_string(cnt) + "]");
            mSubsys->update();
            LOGN(EInfo, ">>> Subs Confirm [" + to_string(cnt) + "]");
            //outputCounter(cnt);
            mSubsys->confirm();
            cnt++;
        }
    } while (false);
    return res;
}

void DesAs2::update()
{
    PFL_DUR_STAT_START(PEvents::EDurStat_DesAsUpd);
    mRunning = true;
    bool res = Run(0, 1);
    mRunning = false;
    if (!res) {
        LOGN(EErr, "Failed run");
    }
    DesLauncher::update();
    PFL_DUR_STAT_REC(PEvents::EDurStat_DesAsUpd);
}

void DesAs2::confirm()
{
    LOGN(EInfo, "Confirm");
    DesLauncher::confirm();

    /*
       MNode* outp = getNode(K_OutpUri);
       MDesSyncable* outpds = outp->lIf(outpds);
       if (!outpds) {
       LOGN(EErr, "No outputs [" + K_OutpUri.toString() + "] found");
       } else {
       outpds->confirm();
       } 
       */
}

void DesAs2::onActivated(MDesSyncable* aComp)
{
    if (mRunning) {
        // When subs running ignore activation from subs but enable activation from output
        if (aComp != mSubsys) {
            setActivated();
            if (aComp) {
#ifdef DES_RGS_VERIFY
                for (auto comp : *mActive) {
                    assert(aComp != comp);
                }
#endif
                mActive->push_back(aComp);
            }
        }
    } else {
        DesLauncher::onActivated(aComp);
    }
}

void DesAs2::setActivated()
{
    if (!mRunning) {
        DesLauncher::setActivated();
    }
}


#if 0






// Embedded elements support

/// Embedded Input buffered



/// Embedded Output state

void DesEOstb::NotifyInpsUpdated()
{
    MNode* cp = mHost->getNode(mCpUri);
    MUnit* cpu = cp ? cp->lIf(cpu) : nullptr;
    auto ifaces = cpu->getIfs<MDesInpObserver>();
    if (ifaces) for (auto ifc : *ifaces) {
        MDesInpObserver* ifco = reinterpret_cast<MDesInpObserver*>(ifc);
        if (ifco) {
            ifco->onInpUpdated();
        }
    }
}

//// DES affecting Parameter

void DesEParb::updatePar(const MContent* aCont)
{
}

#endif


/// DES context supplier

DesCtxSpl::DesCtxSpl(const string &aType, const string& aName, MEnv* aEnv): Verte(aType, aName, aEnv),
    mSplCp(this)
{}

MIface* DesCtxSpl::MNode_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesCtxSplPtr));
    else res = Verte::MNode_getLif(aId);
    return res;
}

MIface* DesCtxSpl::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDesCtxSplPtr));
    else res = Verte::MOwned_getLif(aId);
    return res;
}

MIface* DesCtxSpl::MDesCtxSpl_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDesCtxSplPtr));
    else res = Verte::MVert_getLif(aId);
    return res;
}

MIface* DesCtxSpl::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDesCtxSplPtr));
    else res = Verte::MVert_getLif(aId);
    return res;
}

void DesCtxSpl::onOwnedAttached(MOwned* aOwned)
{
    MVert* owdv = aOwned->lIf(owdv);
    if (owdv) {
        MVert* pair = getPair(0); 
        MDesCtxCsm* csm = pair ? pair->lIf(csm) : nullptr; 
        if (csm) {
            csm->bindCtx(aOwned->ownedId(), owdv);
        }
    }
}

void DesCtxSpl::onOwnedDetached(MOwned* aOwned)
{
}

bool DesCtxSpl::bindCtx(const string& aCtxId, MVert* aCtx)
{
    bool res = false;
    // TODO using extenders as context elements, consider redesign to pins, ref ds_vbcr_omb 
    MNode* ctsn =  getComp(aCtxId);
    MVert* ctsv = ctsn ? ctsn->lIf(ctsv) : nullptr;
    MVert* ctse = ctsv ? ctsv->getExtd() : nullptr;
    if (ctse) {
        MVert* ctce = aCtx ? aCtx->getExtd() : nullptr;
        if (ctce) {
            res = MVert::connect(ctce, ctse);
        }
    }
    return res;
}


/// DES context consumer


const string KCnt_ID = "Id"; // Consumer Id

DesCtxCsm::DesCtxCsm(const string &aType, const string& aName, MEnv* aEnv): Verte(aType, aName, aEnv),
    mCsmCp(this), mDesSyncCp(this)
{}

MIface* DesCtxCsm::MNode_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesCtxCsm));
    else if (res = checkLif2(aId, mMDesSyncable));
    else res = Verte::MNode_getLif(aId);
    return res;
}

MIface* DesCtxCsm::MOwned_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesCtxCsm));
    else if (res = checkLif2(aId, mMDesSyncable));
    else res = Verte::MOwned_getLif(aId);
    return res;
}

MIface* DesCtxCsm::MDesCtxCsm_getLif(TIdHash aId)
{
    MIface* res = NULL;
    if (res = checkLif2(aId, mMDesCtxCsm));
    else res = Verte::MVert_getLif(aId);
    return res;
}

void DesCtxCsm::onOwnedAttached(MOwned* aOwned)
{
    MVert* owdv = aOwned->lIf(owdv);
    if (owdv) {
        MVert* pair = getPair(0); 
        MDesCtxSpl* spl = pair ? pair->lIf(spl) : nullptr; 
        if (spl) {
            spl->bindCtx(aOwned->ownedId(), owdv);
        }
    }
}

void DesCtxCsm::onOwnedDetached(MOwned* aOwned)
{
}

bool DesCtxCsm::bindCtx(const string& aCtxId, MVert* aCtx)
{
    bool res = false;
    // TODO using extenders as context elements, consider redesign to pins, ref ds_vbcr_omb 
    MNode* ctsn =  getComp(aCtxId);
    MVert* ctsv = ctsn ? ctsn->lIf(ctsv) : nullptr;
    MVert* ctse = ctsv ? ctsv->getExtd() : nullptr;
    if (ctse) {
        MVert* ctce = aCtx ? aCtx->getExtd() : nullptr;
        if (ctce) {
            res = MVert::connect(ctce, ctse);
        }
    }
    return res;
}

bool DesCtxCsm::bindAllCtx()
{
    bool res = false;
    MVert* pair = getPair(0);
    MDesCtxSpl* spl = pair ? pair->lIf(spl) : nullptr;
    if (spl) {
        for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd(); it++) {
            auto owdCp = *it;
            MOwned* compod = owdCp->provided();
            MVert* compv = compod ? compod->lIf(compv) : nullptr;
            if (compv) {
                MVert* extd = compv->getExtd();
                if (extd) {
                    res = spl->bindCtx(compod->ownedId(), compv);
                    if (!res) break;
                }
            }
        }
    } else {
        LOGN(EErr, "Binding all ctx elements, cannot get supplier");
    }
    return res;
}

void DesCtxCsm::onConnected(MVert* aPair)
{
    if (mName == "DrpCtx") {
        LOGN(EDbg, "onConnected");
    }
    bindAllCtx();
}

void DesCtxCsm::onDisconnected()
{
}

void DesCtxCsm::confirm()
{
    if (!mBound) {
        MDesCtxCsm* csm = MDesCtxCsm::lIf(csm);
        MDesCtxBinder* owrdcb = owner() ? owner()->lIf(owrdcb) : nullptr;
        if (owrdcb) {
            bool res = owrdcb->bindDesCtx(csm);
            if (!res) {
                LOGN(EErr, "Failed binding context supplier " + csm->Uid());
            } else {
                LOGN(EDbg, "Context consumer has been bound");
            }
        }
        mBound = true;
    }
}

void DesCtxCsm::setActivated()
{
    MDesObserver* obs = mDesSyncCp.mPair ? mDesSyncCp.mPair->provided() : nullptr;
    if (obs) {
        obs->onActivated(this);
    }
}









#if 0
// DES Input demultiplexor

        static const string K_Cp_Inp = "Inp";
        static const string K_Cp_Outp = "Outp";

DesInpDemux::DesInpDemux(const string &aType, const string& aName, MEnv* aEnv): Des(aType, aName, aEnv)
{
    AddInput(K_Cp_Inp);
    AddInput("Done");
    AddOutput(K_Cp_Outp);
}

void DesInpDemux::AddInput(const string& aName)
{
    MNode* cp = Provider()->createNode(CpStateInp::Type(), aName, mEnv);
    assert(cp);
    bool res = attachOwned(cp);
    assert(res);
}

void DesInpDemux::AddOutput(const string& aName)
{
    MNode* cp = Provider()->createNode(CpStateOutp::Type(), aName, mEnv);
    assert(cp);
    bool res = attachOwned(cp);
    assert(res);
}

int DesInpDemux::getIfcCount()
{
    int res = 0;
    MNode* inp = getNode(K_Cp_Inp);
    MUnit* inpu = inp ? inp->lIf(inpu) : nullptr;
    auto ifaces = inpu->getIfs<MDVarGet>();
    if (ifaces && !ifaces->empty()) {
        res = ifaces->size();
    }
    return res;
}

void DesInpDemux::resolveIfc(const string& aName, MIfReq::TIfReqCp* aReq)
{
    if (aName == MDVarGet::Type()) {
        MNode* outp = getNode(K_Cp_Outp);
        MUnit* outpu = outp ? outp->lIf(outpu) : nullptr;
        MIfProvOwner* outppo = outpu ? outpu->lIf(outppo) : nullptr;
        if (outppo && aReq->provided()->isRequestor(outppo)) {
            // Request from output
            MNode* inp = getNode(K_Cp_Inp);
            MUnit* inpu = inp ? inp->lIf(inpu) : nullptr;
            auto ifaces = inpu->getIfs<MDVarGet>();
            if (!ifaces || ifaces->empty() || ifaces->size() <= mIdx) {
                Logger()->Write(EErr, this, "Ifaces idx overflow");
            } else {
                auto* ifc = reinterpret_cast<MDVarGet*>(ifaces->at(mIdx));
                addIfpLeaf(ifc, aReq);
            }
        }
    } else if (aName == MDesObserver::Type()) {
        MNode* inp = getNode(K_Cp_Inp);
        MUnit* inpu = inp ? inp->lIf(inpu) : nullptr;
        MIfProvOwner* inppo = inpu ? inpu->lIf(inppo) : nullptr;
        if (inppo && aReq->provided()->isRequestor(inppo)) {
            // Request from input, redirect to output
            MNode* outp = getNode(K_Cp_Outp);
            MUnit* outpu = outp ? outp->lIf(outpu) : nullptr;
            outpu->resolveIface(aName, aReq);
        }
    } else {
        Des::resolveIfc(aName, aReq);
    }
}


void DesInpDemux::confirm()
{
    Des::confirm();
    int ifcnt = getIfcCount();
    if (mIdx < (ifcnt - 1)) {
        mIdx++;
    }

}

#endif
