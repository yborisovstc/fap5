
#include "desadp.h"



void DAdp::MagUriHandler::onInpUpdated()
{
    LOGN(EInfo, "MAG Uri changed");
    mHost->updateMag();
}

MIface* DAdp::MagUriHandler::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDesInpObserver));
    else res = CpStateInp::MVert_getLif(aId);
    return res;
}



const string DAdp::KCpExplName = "CpExploring";
const string K_DAdp_InpMagUri = "InpMagUri";
const string K_DAdp_StMagUri = "StMagUri";

DAdp::DAdp(const string &aType, const string& aName, MEnv* aEnv): Des(aType, aName, aEnv),
    mCpExpl(MSystExploring::idHash(), MSystExplorable::idHash(), dynamic_cast<MSystExploring*>(this))
{
}

void DAdp::Construct()
{
    // Add exploring cp
    auto ecp = addComp(CpSystExploring::idStr(), KCpExplName);
    assert(ecp);
    auto ecpcp = ecp->lIft<MConnPoint>();
    bool res = ecpcp->bind(&mCpExpl);
    assert(res);
    // Add MAG uri input
    auto inpMagUrin = addComp(CpStateInp::idStr(), K_DAdp_InpMagUri);
    auto inpMagUri = inpMagUrin ? inpMagUrin->lIft<MVert>() : nullptr;
    assert(inpMagUri);
    // Add MagUri state
    mStMagUri = addComp(BState::idStr(), K_DAdp_StMagUri);
    assert(mStMagUri);
    mStMagUri->lIft<MContentOwner>()->setContent("", "URI");
    mStMagUri->lIft<MContentOwner>()->setContent("LogLevel", "Dbg"); // YB!!
    auto* stMagUriCp = mStMagUri->lIft<MConnPoint>();
    auto* stMagUriv = mStMagUri->lIft<MVert>();
    res = stMagUriCp->bind(inpMagUri->lIft<MConnPoint>()->bP());
    assert(res);
    mHdlr = new MagUriHandler("MagUriHandler", mEnv, this);
    MVert::connect(stMagUriv, mHdlr->lIft<MVert>());
}

DAdp::~DAdp()
{
    if (mHdlr) {
	delete mHdlr;
    }
}

MIface* DAdp::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSystExploring));
    else res = Des::MNode_getLif(aId);
    return res;
}

MIface* DAdp::MSystExploring_getLif(TIdHash aId)
{
    MIface* res = checkLif2(aId, mMSystExploring);
    return res;
}

MNode* DAdp::getMag()
{
    return mMag;
}

MNode* DAdp::getMagBase()
{
    MNode* res = nullptr;
    if (mCpExpl.pcount()) {
	auto* pairv = mCpExpl.pairAt(0);
	auto* explb = pairv->prov<MSystExplorable>();
	res = explb ? explb->getMag() : nullptr;
    }
    return res;
}

void DAdp::updateMag()
{
    DGuri magUri;
    if (mMagBase) {
        GetGData(mStMagUri, magUri);
        //YB!! MNode* magn = magUri.IsValid() ? mMagBase->getNode(magUri.mData) : mMagBase;
        MNode* magn = magUri.IsValid() ? mMagBase->getNode(magUri.mData) : nullptr;
        if (magn && magn != mMag) {
            mMag = magn;
            notifyMagChanged();
            LOGN(EInfo, "Managed agent attached [" + mMag->Uid() + "]");
        }
    }
}

void DAdp::onMagChanged()
{
    if (mName == "MagAdp") {
        LOGN(EDbg, "onMagChanged");
    }
    auto magBase = getMagBase();
    if (magBase != mMagBase) {
        mMagBase = magBase;
        LOGN(EInfo, "Managed agent base changed [" + mMagBase->Uid() + "]");
        updateMag();
    }
}

void DAdp::notifyMagChanged()
{
    for (auto pair : mExplorableCp.mPairs) {
	pair->prov<MSystExploring>()->onMagChanged();
    }
}
