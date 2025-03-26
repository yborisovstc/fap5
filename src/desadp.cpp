
#include "desadp.h"


const string KCont_AgentUri = "AgentUri";

static const vector<string> KContentIds = { KCont_AgentUri };


const string DAdp::KCpExplName = "CpExploring";

DAdp::DAdp(const string &aType, const string& aName, MEnv* aEnv): Des(aType, aName, aEnv)
{
}

void DAdp::Construct()
{
    mCpExpl = new CpSystExploring(string(CpSystExploring::idStr()), KCpExplName, mEnv);
    assert(mCpExpl);
    bool res = attachOwned(mCpExpl);
    assert(res);
    mCpExpl->bind(MNode::lIft<MNode>());

}

DAdp::~DAdp()
{
    if (mCpExpl) {
	delete mCpExpl;
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
    if (mCpExpl && mCpExpl->pairsCount()) {
	auto* pairv = mCpExpl->mPairs.at(0);
	auto* explb = pairv->lIft<MSystExplorable>();
	res = explb ? explb->getMag() : nullptr;
    }
    return res;
}

void DAdp::updateMag()
{
    if (mMagBase) {
	GUri magUri(mContMagUri);
	if (magUri.isValid()) {
	    mMag = mMagBase->getNode(magUri);
	}
    }
}

void DAdp::onMagChanged()
{
    auto magBase = getMagBase();
    if (magBase != mMagBase) {
	mMagBase = magBase;
	updateMag();
	notifyMagChanged();
    }
}

void DAdp::notifyMagChanged()
{
    for (auto pair : mExplorableCp.mPairs) {
	pair->provided()->onMagChanged();
    }
}

int DAdp::contCount() const 
{
    return Des::contCount() + KContentIds.size();
}

bool DAdp::getContentId(int aIdx, string& aRes) const
{
    bool res = false;
    if (aIdx < contCount()) {
	if (aIdx < Des::contCount()) {
	    res = Des::getContentId(aIdx, aRes);
	} else {
	    aRes = KContentIds.at(aIdx - Des::contCount());
	    res = true;
	}
    }
    return res;
}


bool DAdp::getContent(const string& aId, string& aRes) const
{
    bool res = true;
    if (aId == KCont_AgentUri) {
	aRes = mContMagUri;
    } else {
	res = Des::getContent(aId, aRes);
    }
    return res;
}

bool DAdp::setContent(const string& aId, const string& aData)
{
    bool res = true;
    if (aId == KCont_AgentUri) {
	if (aData != mContMagUri) {
	    mContMagUri = aData;
	    onContentChanged(aId);
	}
	// TODO to notify update
    } else  {
	res = Des::setContent(aId, aData);
    }
    return res;
}

void DAdp::onContentChanged(const string& aId)
{
    if (aId == KCont_AgentUri) {
	updateMag();
	notifyMagChanged();
    }
}



