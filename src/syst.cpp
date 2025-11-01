#include "syst.h"


/// CpSystExplorable

MIface* CpSystExplorable::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSystExploring));
    else res = TBase::MOwned_getLif(aId);
    return res;
}

MIface* CpSystExplorable::MSystExplorable_getLif(TIdHash aId)
{
    MIface* res = checkLif2(aId, mMSystExplorable);
    return res;
}

MIface* CpSystExplorable::MSystExploring_getLif(TIdHash aId)
{
    MIface* res = checkLif2(aId, mMSystExploring);
    return res;
}

void CpSystExplorable::onConnected(MVert* aPair)
{
    notifyMagChanged();
}

void CpSystExplorable::onDisconnected()
{
}

void CpSystExplorable::onBound()
{
}

void CpSystExplorable::onUnbound()
{
}

MNode* CpSystExplorable::getMag()
{
    MNode* res = nullptr;
    if (mExploringCp.pcount() == 1) {
	auto expbl = mExploringCp.pairAt(0);
	assert(expbl);
	res = expbl->provided()->getMag();
    }
    return res;
}

void CpSystExplorable::notifyMagChanged()
{
    // Notify pairs
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        auto pair = *it;
        MSystExploring* pairExpn = pair ? pair->lIft<MSystExploring>() : nullptr;
        if (pairExpn) {
            pairExpn->onMagChanged();
        }
    }

}



/// CpSystExploring


void CpSystExploring::onConnected(MVert* aPair)
{
    if (mProvidedPx) {
        mProvidedPx->onMagChanged();
    }
}

void CpSystExploring::onDisconnected()
{
}

void CpSystExploring::onBound()
{
}

void CpSystExploring::onUnbound()
{
}

 
// ExtdSystExplorable

string ExtdSystExplorable::KIntName = "Int";

ExtdSystExplorable::ExtdSystExplorable(const string &aType, const string& aName, MEnv* aEnv): 
    ConnPoint<MSystExplorable, MSystExploring>(aType, aName, aEnv)
{
}
void ExtdSystExplorable::Construct()
{
    mInt = new CpSystExploring(string(CpSystExploring::idStr()), KIntName, mEnv);
    assert(mInt);
    bool res = attachOwned(mInt);
    assert(res);
    res = mInt->bind(MNode::lIft<MVert>());
    assert(res);
}

MNode* ExtdSystExplorable::getMag()
{
    // Redirect to int point
    auto pair = (mInt->mPairs.begin() != mInt->mPairs.end()) ? *(mInt->mPairs.begin()) : nullptr;
    MSystExplorable* pairExpb = pair ? pair->lIft<MSystExplorable>() : nullptr;
    return pairExpb ? pairExpb->getMag() : nullptr;
}

void ExtdSystExplorable::onMagChanged()
{
    // Redirect to pairs
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        auto pair = *it;
        MSystExploring* pairExpn = pair ? pair->lIft<MSystExploring>() : nullptr;
        if (pairExpn) {
            pairExpn->onMagChanged();
        }
    }
}


// ExtdSystExploring

string ExtdSystExploring::KIntName = "Int";

ExtdSystExploring::ExtdSystExploring(const string &aType, const string& aName, MEnv* aEnv): 
    CpSystExploring(aType, aName, aEnv)
{
}
void ExtdSystExploring::Construct()
{
    mInt = new CpSystExplorable(string(CpSystExplorable::idStr()), KIntName, mEnv);
    assert(mInt);
    bool res = attachOwned(mInt);
    assert(res);
    res = mInt->bind(MNode::lIft<MVert>());
    assert(res);
}

MNode* ExtdSystExploring::getMag()
{
    MNode* res = nullptr;
    // Redirect to pairs
    if (mPairs.size() == 1) {
        auto pair = *mPairs.begin();
        MSystExplorable* pairExpb = pair ? pair->lIft<MSystExplorable>() : nullptr;
        if (pairExpb) {
            res = pairExpb->getMag();
        }
    } else {
        // TODO handle
    }
    return res;
}

void ExtdSystExploring::onMagChanged()
{
    // Redirect to int point
    auto pair = (mInt->mPairs.begin() != mInt->mPairs.end()) ? *(mInt->mPairs.begin()) : nullptr;
    MSystExploring* pairExpn = pair ? pair->lIft<MSystExploring>() : nullptr;
    if (pairExpn) {
        pairExpn->onMagChanged();
    }
}


// PinSystExplorable 

PinSystExplorable::PinSystExplorable(const string &aType, const string& aName, MEnv* aEnv): 
    ConnPoint<MSystExplorable, MSystExploring>(aType, aName, aEnv)
{
}

void PinSystExplorable::onMagChanged()
{
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        auto pair = *it;
        MSystExploring* pairExpn = pair ? pair->lIft<MSystExploring>() : nullptr;
        if (pairExpn) {
            pairExpn->onMagChanged();
        }
    }

}

// PinSystExploring 

PinSystExploring::PinSystExploring(const string &aType, const string& aName, MEnv* aEnv): 
    CpSystExploring(aType, aName, aEnv)
{
}

MNode* PinSystExploring::getMag()
{
    MNode* res = nullptr;
    if (mPairs.size() == 1) {
        auto* pair = mPairs.at(0); 
        MSystExplorable* pairExpb = pair ? pair->lIft<MSystExplorable>() : nullptr;
        if (pairExpb) {
            res = pairExpb->getMag();
        }
    }
    return res;
}



// System

Syst::Syst(const string &aType, const string &aName, MEnv* aEnv): Elem(aType, aName, aEnv),
    mAgtCp(this), mExplorableCp(this)
{
}

Syst::~Syst()
{
}

MIface* Syst::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSyst));
    else res = Elem::MNode_getLif(aId);
    return res;
}

MIface* Syst::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSyst));
    else if (res = TBase::MOwned_getLif(aId));
    else res = MSyst_getLif(aId);
    return res;
}

MIface* Syst::MSyst_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSyst));
    else {
        // Redirect to agents. Keep policy "no same iface in agents"
        for (auto it = mAgtCp.pairsBegin(); it != mAgtCp.pairsEnd(); it++) {
            auto* agt = (*it)->provided();
            MIface* ares = agt->MAgent_getLif(aId);
            if (ares && res) {
                LOGN(EErr, "Duplicated iface [" + ares->Uid() + "] in syst agent " + agt->Uid());
                break;
            } else {
                res = ares;
            }
        }
    }
    return res;
}


MIface* Syst::MAhost_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMNode));
    else res = Elem::MNode_getLif(aId);
    return res;
}

MIface* Syst::MSystExplorable_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMNode));
    return res;
}

MNode* Syst::getMag()
{
    return MNode::lIft<MNode>();
}

void Syst::onOwnedAttached(MOwned* aOwned)
{
    Elem::onOwnedAttached(aOwned);
    auto expl = aOwned->lIft<MSystExploring>();
    if (expl && expl->getCp()) {
	// Connect owned as exploring/controlling
	bool res = mExplorableCp.connect(expl->getCp());
	assert(res);
    }
    auto agent = aOwned->lIft<MAgent>();
    if (agent) {
	bool res = mAgtCp.connect(agent->getMAgentCp());
	assert(res);
    }
}

void Syst::mutConnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    bool res = false;
    string sp = aMut.Attr(ENa_P);
    string sq = aMut.Attr(ENa_Q);
    MNode* pn = getNode(sp, aCtx.mNs);
    MNode* qn = nullptr;
    if (sq.empty()) {
	if (aMut.Count() == 1) {
	    // Q-dependency
	    ChromoNode qdep = *aMut.Begin();
	    mutate(qdep, aUpdOnly, aCtx);
	    if (qdep.Type() == ENt_Node) {
		sq = qdep.Attr(ENa_Id);
	    } else if (qdep.Type() == ENt_Seg) {
		if (qdep.AttrExists(ENa_Targ)) {
		    sq = qdep.Attr(ENa_Targ);
		} else if (qdep.AttrExists(ENa_NS)) {
		    sq = qdep.Attr(ENa_NS);
		}
	    }
	    qn = getNode(sq, aCtx.mNs);
	}
    } else {
	qn = getNode(sq, aCtx.mNs);
    }
    if (pn && qn) {
	MVert* pv = pn->lIf(pv);
	MVert* qv = qn->lIf(pv);
	if (pv && qv) {
	    // Vertex to Vertex
	    if (!pv->isConnected(qv) && !qv->isConnected(pv)) {
		res = MVert::connect(pv, qv);
		if (!res) {
		    LOGN(EErr, "Failed connecting [" + sp + "] to [" + sq + "]");
		    res = MVert::connect(pv, qv);
		} else {
		    mEdges.push_back(TEdge(pv, qv));
		}
	    } else {
		LOGN(EErr, "Connecting [" + sp + "] to [" + sq + "] - already connected");
	    }
	} else {
            /*
	    MLink* pl = pn->lIf(pl);
	    if (pl) {
		// Link, one-way
		res = pl->connect(qn);	
		if (!res) {
		    LOGN(EErr, "Failed link [" + sq + "] to [" + sp + "]");
		}
	    } else {
		LOGN(EErr, "Connecting [" + sp + "] to [" + sq + "] -  [" + (pv ? sq : sp) + "] isn't connectable");
	    }
            */
	}
    } else {
	LOGN(EErr, "Connecting [" + sp + "] to [" + sq + "] - cannot find [" + (pn ? sq : sp) + "]");
    }
}

void Syst::mutDisconnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    bool res = false;
    string sp = aMut.Attr(ENa_P);
    string sq = aMut.Attr(ENa_Q);
    MNode* pn = getNode(sp, aCtx.mNs);
    MNode* qn = getNode(sq, aCtx.mNs);
    if (pn && qn) {
	MVert* pv = pn->lIf(pv);
	MVert* qv = qn->lIf(pv);
	if (pv && qv) {
	    if (pv->isConnected(qv)) {
		// Vertex to Vertex
		res = MVert::disconnect(pv, qv);
		if (!res) {
		    LOGN(EErr, "Failed disconnecting [" + sp + "] from [" + sq + "]");
		}
	    } else {
		LOGN(EWarn, "Disconnecting [" + sp + "] and [" + sq + "]: aren't connected");
	    }
	} else {
            /*
	    MLink* pl = pn->lIf(pl);
	    if (pl) {
		// Link, one-way
		res = pl->disconnect(qn);	
		if (!res) {
		    LOGN(EErr, "Failed disconnecting [" + sq + "] to [" + sp + "]");
		}
	    } else {
		LOGN(EErr, "Disconnecting [" + sp + "] from [" + sq + "] -  [" + (pv ? sq : sp) + "] isn't connectable");
	    }
            */
	}
    } else {
	LOGN(EErr, "Disconnecting [" + sp + "] from [" + sq + "] - cannot find [" + (pn ? sq : sp) + "]");
    }
    if (!aUpdOnly) {
	//notifyNodeMutated(aMut, aCtx);
    }
}




AgtBase::AgtBase(const string &aType, const string& aName, MEnv* aEnv): Node(aType, aName, aEnv), mAgtCp(this)
{
}

AgtBase::~AgtBase()
{
}


MNode* AgtBase::ahostNode()
{
    auto* pair = *mAgtCp.pairsBegin();
    MAhost* ahost = pair ? pair->provided() : nullptr;
    MNode* hostn = ahost ? ahost->lIf(hostn) : nullptr;
    return hostn;
}

MIface* AgtBase::MAgent_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMAgentPtr));
    return res;
}
