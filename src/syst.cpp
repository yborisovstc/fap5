#include "syst.h"



// System

Syst::Syst(const string &aType, const string &aName, MEnv* aEnv): Elem(aType, aName, aEnv)
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
