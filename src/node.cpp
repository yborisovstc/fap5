
#include <list>
#include "node.h"
#include "prof_ids.h"

static const string KContentLogLevel = "LogLevel";
static const string KContentAbout = "About";

static const vector<string> KContentIds = { KContentAbout, KContentLogLevel };

static const map<string, int> KLogLevels = {
    { "Err", EErr},
    { "Warn", EWarn},
    { "Info", EInfo},
    { "Dbg", EDbg},
    { "Verbose", EVerbose},
    { "All", EAll}
};

static const map<int, string> KLogLevelsStr = {
    { EErr, "Err"},
    { EWarn, "Warn"},
    { EInfo, "Info"},
    { EDbg, "Dbg"},
    { EVerbose, "Verbose"},
    { EAll, "All"}
};

Node::Node(const string &aType, const string &aName, MEnv* aEnv): mName(aName.empty() ? aType : aName), mEnv(aEnv), mOcp(this), mOwsNode(this, this),
    mLogLevel(EInfo), mExplorable(false), mControllable(false)
{
} 

Node::~Node()
{
    // Removing the owneds
    auto bg = ownerCp()->pairsBegin();
    while (bg != ownerCp()->pairsEnd()) {
        auto* owdCp = *bg;
        ownerCp()->disconnect(owdCp);
        owdCp->provided()->deleteOwned();
        bg = ownerCp()->pairsBegin();
    }
    // TODO Use raw connpoint NTnip<MOwned, MOwner> here, get access to pairs container etc.
    /*
       auto pairs = mOwsNode.mCnode.pairsCtr();
       auto bg = pairs.begin();
       while (bg != pairs.end()) {
       auto* owdCp = bg->second;
       ownerCp()->disconnect(owdCp);
       owdCp->provided()->deleteOwned();
       bg = pairs.begin();
       }
       */

    // Disconnect all observers
    mOcp.disconnectAll();
}

void Node::MNode_doDump(int aLevel, int aIdt, ostream& aOs) const
{
    if (aLevel & Ifu::EDM_Base) {
	Ifu::offset(aIdt, aOs); aOs << "Name: " << mName << endl;
    }
    if (aLevel & Ifu::EDM_Comps) {
	for (int i = 0; i < ownerCp()->pcount(); i++) {
	    if (ownerCp()->pairAt(i)) {
		const MOwned* comp = ownerCp()->pairAt(i)->provided();
		const MNode* compn = comp->lIf(compn);
		if (compn) {
		    Ifu::offset(aIdt, aOs);
		    aOs << "- "  << compn->name() << endl;
		    if (aLevel & Ifu::EDM_Recursive) {
			compn->doDump(Ifu::EDM_Comps | Ifu::EDM_Recursive, aIdt + Ifu::KDumpIndent, aOs);
		    }
		} else {
		    aOs << "=== ERROR on getting component [" << i << "] MNode" << endl;
		}
	    } else {
		aOs << "=== ERROR on getting comp [" << i << "]" << endl;
	    }
	}
    }
}

MIface* Node::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMOwned));
    else if (res = checkLif2(aId, mMOwner));
    else if (res = checkLif2(aId, mMContentOwner));
    else if (res = checkLif2(aId, mMObservable));
    else if (res = checkLif2(aId, mMParent));
    else if (res = checkLif2(aId, mMChild));
    else if (res = checkLif2(aId, mMNode));
    return res;
}

MNode* Node::addComp(const string_view& aId, const string& aName)
{
    MNode* cp = Provider()->createNode(string(aId), aName, mEnv);
    assert(cp);
    bool res = attachOwned(cp->MNode::lIft<MOwned>());
    assert(res);
    return cp;
}

const MNode* Node::getComp(const string& aId) const
{
    auto compOwnedCp = ownerCp()->pairAt(aId);
    const MOwned* ores = compOwnedCp ? compOwnedCp->provided() : nullptr;
    const MNode* res = ores ? ores->lIf(res) : nullptr;
    return res;
}

MNode* Node::getNode(const GUri& aUri)
{
    MNode* res = nullptr;
    if (aUri.isValid()) {
	if (aUri.size() == 0 || aUri.size() == 1 && aUri.at(0) == GUri::K_Self) {
	    res = this;
	} else {
            MOwned* owd = getOwned(aUri, this);
            res = owd ? owd->lIf(res) : nullptr;
	}
    } else {
	LOGN(EErr, "Invalid URI [" + aUri.toString() + "]");
    }
    return res;
}

MNode* Node::getNode(const GUri& aUri, const TNs& aNs)
{
    MNode *res = nullptr;
    MNode *resns = nullptr;
    res = getNode(aUri);
    for (auto nsit = aNs.rbegin(); nsit != aNs.rend() && !resns; nsit++) {
	auto ns = *nsit;
	if (ns == this) continue;
	resns = ns->getNode(aUri);
    }
    // Resolving name in current context/native first
    if (!res) {
	// Then in namespaces
	// Applied namespaces priority approach, ref I_NRC
	res = resns;
    } else if (resns && res != this) {
	LOGN(EErr, "Same node [" + aUri.toString() + "] is resolved both in current ctx and namespace");
    }
    return res;
}

// TODO condider design of provided agents namespaces
// Optimization ds_desopt_aip
void Node::getUri(GUri& aUri, const MNode* aBase) const
{
    // Note that it's disabled for native agents to be in native tree, ref ds_iss_021
    aUri.clear();
    if (aBase != this) {
	const MOwner* owner = this->owner();
	if (owner) {
            const MOwner* base = MNode::lIf(base);
	    owner->ownerGetUri(aUri, base);
	} else {
	    if (mEnv && mEnv->provider() && mEnv->provider()->isProvided(this)) {
		// Provided
	    } else {
		// Root
		aUri.appendElem(string());
	    }
	}
	aUri.appendElem(mName);
    }
}

void Node::setCtx(MOwner* aContext)
{
    assert(!mContext && aContext || !aContext);
    mContext = aContext;
}


void Node::mutate(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx, bool aTreatAsChromo, bool aLocal)
{
    bool res = true;
    TNs root_ns = aCtx.mNs;
    updateNs(root_ns, aMut);
    bool targ_nil = false;

    ChromoNode rno = aMut;
    //LOGN(EInfo, "Mut ");
    Logger()->SetContextMutId(rno.LineId());
    // Get target node by analysis of mut-target and mut-node, ref ds_chr2_cctx_tn_umt
    PFL_DUR_STAT_START(PEvents::EDurStat_MutNtf);
    notifyNodeMutated(aMut, aCtx);
    PFL_DUR_STAT_REC(PEvents::EDurStat_MutNtf);
    MNode* targ = this;
    bool exs_targ = rno.AttrExists(ENa_Targ);
    bool exs_mnode = rno.AttrExists(ENa_MutNode);
    string starg, smnode;
    if (!aTreatAsChromo && exs_targ && !aLocal) {
	starg = rno.Attr(ENa_Targ);
	if (starg == Ifu::K_SpName_Ns) {
	    if (root_ns.size()) {
		targ = root_ns.back();
	    } else {
		Logger()->Write(EErr, this, "No namespace defined");
	    }
	} else if (starg == Ifu::K_SpName_Nil) {
	    targ_nil = true;
	} else {
	    targ = targ->getNode(starg, root_ns);
	}
	if (!targ) {
	    Logger()->Write(EErr, this, "Cannot find target node [%s]", starg.c_str());
	    res = false;
	}
    }
    //LOGN(EInfo, "Mut 2");
    if (res) {
	if (targ != this || targ_nil) {
	    // Targeted mutation
	    if (!targ_nil) {
		// Redirect the mut to target: no run-time to keep the mut in internal nodes
		// Propagate till target owning comp if run-time to keep hidden all muts from parent
		MutCtx mctx(aUpdOnly ? targ : aCtx.mNode, root_ns);
		targ->mutate(rno, aUpdOnly, mctx, aTreatAsChromo, true);
	    }
	} else  {
	    // Local mutation
	    MutCtx mctx(aCtx.mNode, root_ns);
	    TNodeType rnotype = rno.Type();
	    if (rnotype == ENt_Seg || aTreatAsChromo) {
		mutSegment(rno, aUpdOnly, mctx);
	    } else if (rnotype == ENt_Node) {
		MNode* mres = mutAddElem(rno, aUpdOnly, mctx);
		if (mres) {
		    if (rno.Count()) {
			TNs root_ns;
			assert(!(exs_targ && rno.AttrExists(ENa_NS)));
			if (exs_targ) {
			    targ = mres;
			} else if (rno.AttrExists(ENa_NS) && rno.Attr(ENa_NS).empty()) {
			    if (rno.Attr(ENa_NS).empty()) {
				root_ns.push_back(mres);
				targ = this;
			    } else {
			    }
			} else {
			    targ = mres;
			}
			MutCtx mctx(this, root_ns);
			if (targ) {
			    targ->mutate(rno, aUpdOnly, mctx, true);
			}
		    }
		}
	    } else if (rnotype == ENt_Change) {
		//ChangeAttr(rno, aRunTime, aCheckSafety, aTrialMode, mctx);
	    } else if (rnotype == ENt_Cont) {
		PFL_DUR_STAT_START(PEvents::EDurStat_MutCont);
		mutContent(rno, aUpdOnly, mctx);
		PFL_DUR_STAT_REC(PEvents::EDurStat_MutCont);
	    } else if (rnotype == ENt_Move) {
		//MoveNode(rno, aRunTime, aTrialMode, mctx);
	    } else if (rnotype == ENt_Import) {
		mutImport(rno, aUpdOnly, mctx);
	    } else if (rnotype == ENt_Rm) {
		//PFL_DUR_STAT_START(PEvents::EDurStat_MutRm);
		mutRemove(rno, aUpdOnly, mctx);
		//PFL_DUR_STAT_REC(PEvents::EDurStat_MutRm);
	    } else if (rnotype == ENt_Conn) {
		PFL_DUR_STAT_START(PEvents::EDurStat_MutConn);
		mutConnect(rno, aUpdOnly, mctx);
		PFL_DUR_STAT_REC(PEvents::EDurStat_MutConn);
	    } else if (rnotype == ENt_Disconn) {
		//PFL_DUR_STAT_START(PEvents::EDurStat_MutDsc);
		mutDisconnect(rno, aUpdOnly, mctx);
		//PFL_DUR_STAT_REC(PEvents::EDurStat_MutDsc);
	    } else if (rnotype == ENt_Note) {
		// Comment, just accept
	    } else {
		Logger()->Write(EErr, this, "Unknown mutation [%d]", rnotype);
	    }
	    Logger()->SetContextMutId();
	}
    }
}



MIface* Node::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMNode));
    else if (res = checkLif2(aId, mMOwner));
    return res;
}

MIface* Node::MOwner_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    // TODO Vulnerabilty, consider to configure the access
    if (res = checkLif2(aId, mMObservable));
    // TODO to introduce specific iface for explorable, ref ds_dcs_aes_acp
    else if (res = mExplorable ? checkLif2(aId, mMNode) : nullptr);
    else if (res = mControllable ? checkLif2(aId, mMNode) : nullptr);
    else if (res = checkLif2(aId, mMContentOwner));
    return res;
}

// Optimization ds_desopt_aip
void Node::ownerGetUri(GUri& aUri, const MOwner* aBase) const
{
    aUri.clear();
    if (aBase != this) {
	const MOwner* owner = this->owner();
	if (owner) {
	    owner->ownerGetUri(aUri, aBase);
	} else {
	    // Root
	    aUri.appendElem(string());
	}
	aUri.appendElem(mName);
    }
}


bool Node::attachOwned(MOwned* aOwned)
{
    bool res = ownerCp()->connect(aOwned->ownedCp());
    if (res) {
	aOwned->onOwnerAttached();
	onOwnedAttached(aOwned);
    } else {
	LOGN(EErr, "Attaching owned: already exists [" + aOwned->ownedId() + "]");
    }
    return res;
}

MOwner* Node::owner()
{
    return (mOwsNode.pcount() > 0) ? (*mOwsNode.pairsBegin())->provided() : nullptr;
}

const MOwner* Node::owner() const
{
    auto onode = const_cast<TOwtNode*>(&mOwsNode);
    return (mOwsNode.pcount() > 0) ? (*onode->pairsBegin())->provided() : nullptr;
}

MOwned* Node::getOwned(const GUri& aUri, const MOwned* aReq) const
{
    MOwned* res = nullptr;
    Node* self = const_cast<Node*>(this);
    if (aUri.isValid()) {
	if (aUri.size() == 0 || aUri.size() == 1 && aUri.at(0) == GUri::K_Self) {
	    res = self;
	} else {
	    if (aUri.isAbsolute()) {
		if (owner()) {
		    res = owner()->getOwned(aUri, this);
                } else {
                    // Root
                    if (aUri.size() > 2) {
                        res = self->getOwned(aUri.at(2));
                        bool foundReq = res == aReq;
                        for (int i = 3; i < aUri.size() && res; i++) {
                            res = res->asOwner()->getOwned(aUri.at(i));
                            foundReq |= (res == aReq);
                        }
                        res = foundReq ? res : nullptr;
                    }
                }
            } else {
                res = getOwned(aUri.at(0));
                for (int i = 1; i < aUri.size() && res; i++) {
                    res = res->asOwner()->getOwned(aUri.at(i));
                }
            }
        }
    } else {
	LOGN(EErr, "Invalid URI [" + aUri.toString() + "]");
    }
    return res;
}

MOwned* Node::getOwned(const string& aId) const
{
    auto compOwnedCp = ownerCp()->pairAt(aId);
    const MOwned* ores = compOwnedCp ? compOwnedCp->provided() : nullptr;
    return const_cast<MOwned*>(ores);
}

void Node::onOwnedMutated(const MOwned* aOwned, const ChromoNode& aMut, const MutCtx& aCtx)
{
    // Node is not inheritable, so nothing to do.
    if (owner() && aCtx.mNode != this) {
	// Propagate to owner
	owner()->onOwnedMutated(aOwned, aMut, aCtx);
    }
}


bool Node::addObserver(MObserver::TCp* aObs)
{
    bool res = mOcp.connect(aObs);
    return res;
}

bool Node::rmObserver(MObserver::TCp* aObs)
{
    bool res = mOcp.disconnect(aObs);
    return res;
}

MNode* Node::createHeir(const string& aName)
{
    MNode* uheir = nullptr;
    if (Provider()->isProvided(this)) {
	uheir = Provider()->createNode(name(), aName, mEnv);
    } else {
	LOGN(EInfo, "The parent of [" + aName + "] is not of provided");
    }
    return uheir;
}


void Node::parentGetUri(GUri& aUri) const
{
    getUri(aUri);
}


int Node::contCount() const 
{
    return KContentIds.size();
}

bool Node::getContentId(int aIdx, string& aRes) const
{
    bool res = false;
    if (aIdx < contCount()) {
        aRes = KContentIds.at(aIdx);
        res = true;
    }
    return res;
}

bool Node::getContent(const string& aId, string& aRes) const
{
    bool res = false;
    if (aId == KContentLogLevel) {
        auto it = KLogLevelsStr.find(mLogLevel);
        if (it != KLogLevelsStr.end()) {
            aRes = it->second;
            res = true;
        }
    } else if (aId == KContentAbout) {
        aRes = mAbout;
    } else {
        LOGN(EErr, "Content [" + aId + "] isn't supported");
    }
    return res;
}

bool Node::setContent(const string& aId, const string& aData)
{
    bool res = false;
    if (aId == KContentLogLevel) {
        auto it = KLogLevels.find(aData);
        if (it != KLogLevels.end()) {
            mLogLevel = it->second;
            res = true;
        }
    } else if (aId == KContentAbout) {
        mAbout = aData;
        res = true;
    } else {
        LOGN(EErr, "Content [" + aId + "] isn't supported");
    }
    return res;
}

void Node::onContentChanged(const string& aId)
{
}

void Node::updateNs(TNs& aNs, const ChromoNode& aCnode)
{
    if (aCnode.AttrExists(ENa_NS)) {
	string ns = aCnode.Attr(ENa_NS);
	// Handle only NS for explicit Ctx, ref ds_cli_ddmc_ues_ics
	if (!ns.empty()) {
	    MNode* nsu = getNode(ns, aNs);
	    if (nsu == NULL) {
		LOGN(EErr, "Cannot find namespace [" + ns + "]");
	    } else {
		aNs.clear(); // Override namespace by explicitly stated one
		aNs.push_back(nsu);
	    }
	}
    }
}

void Node::notifyNodeMutated(const ChromoNode& aMut, const MutCtx& aCtx)
{
    auto owr = owner();
    MOwned* owd = MNode::lIf(owd);
    if (owr && aCtx.mNode != this) {
	owr->onOwnedMutated(owd, aMut, aCtx);
    }
}

MNode* Node::getParent(const GUri& aUri)
{
    MNode* res = getNode(aUri);
    if (!res) {
	MOwner* owr = mContext ? mContext : owner();
	if (owr) {
	    res = mContext ?  mContext->getParent(aUri) : nullptr;
	    if (!res) {
		res = owner() ?  owner()->getParent(aUri) : nullptr;
	    }
	} else{
	    MNode* mdl = getNode(mEnv->modulesUri());
	    res = mdl ? mdl->getNode(aUri) : nullptr;
	}
    }
    return res;
}


MNode* Node::mutAddElem(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    string sparent = aMut.Attr(ENa_Parent);
    string sname = aMut.Attr(ENa_Id);
    bool res = false;
    //Log(EDbg2, TLog(this, aMut) + "Adding element [" + sname + "]"); // To LOGN

    assert(!sname.empty());
    MNode* uelem = NULL;
    // Obtain parent first
    MNode *parentNode = NULL;
    // Check if the parent is specified
    if (!sparent.empty()) {
	// Check the parent scheme
	GUri prnturi(sparent);
        parentNode = getParent(prnturi);
        if (!parentNode && prnturi.size() == 1) {
            // Try native
            if (mEnv && mEnv->provider()) {
                parentNode = mEnv->provider()->provGetNode(prnturi.at(0));
            }
        }
        if (parentNode) {
            // Create heir from the parent
            MParent* parent = parentNode->lIf(parent);
            uelem = parent->createHeir(sname);
            if (uelem) {
                uelem->Construct();
                PFL_DUR_STAT_START(PEvents::EDurStat_MutAtt);
                MOwned* elemOwned = uelem->lIft<MOwned>();
                attachOwned(elemOwned);
                MChild* elemChild = uelem->lIft<MChild>();
                parent->attachChild(elemChild);
                PFL_DUR_STAT_REC(PEvents::EDurStat_MutAtt);
            } else {
                LOGN(EErr, "Adding node [" + sname + "] failed");
            }
        } else {
            Logger()->Write(EErr, this, "Creating [%s] - parent [%s] not found", sname.c_str(), sparent.c_str());
            parentNode = getParent(prnturi);
        }
    } else {
        LOGN(EErr, "Missing parent name");
    }
    return uelem;
}

void Node::mutRemove(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    string snode, sname;
    sname = aMut.Attr(ENa_Id);
    if (ownerCp()->pairAt(sname)) {
        MOwned* owned = ownerCp()->pairAt(sname)->provided();
        onOwnedDetached(owned); // TODO to rename to onOwnedTobeDetached
        owned->onOwnerDetached();
        bool res = ownerCp()->disconnect(ownerCp()->pairAt(sname));
        if (res) {
            owned->deleteOwned();
            LOGN(EInfo, "Removed node [" + sname + "]");
	} else {
	    LOGN(EErr, "Failed detached owned [" + sname + "]");
	}
    } else {
	LOGN(EErr, "Failed removing [" + sname + "] - not found");
    }
}

void Node::mutContent(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    string snode = aMut.Attr(ENa_MutNode);
    string sdata = aMut.Attr(ENa_MutVal);
    bool res = setContent(snode, sdata);
}

void Node::mutConnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    string sp = aMut.Attr(ENa_P);
    string sq = aMut.Attr(ENa_Q);
    LOGN(EErr, "Connecting [" + sp + "] to [" + sq + "] - not supported");
}

void Node::mutDisconnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    string sp = aMut.Attr(ENa_P);
    string sq = aMut.Attr(ENa_Q);
    LOGN(EErr, "Disconnecting [" + sp + "] from [" + sq + "] - not supported");
}

void Node::mutImport(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    auto* impmgr = mEnv->ImpsMgr();
    assert(impmgr);
    string srcs = aMut.Attr(ENa_Id);
    bool res = impmgr->Import(srcs);
    if (!res) {
	LOGN(EErr, "Importing [" + srcs + "] failed");
    }
}

void Node::mutSegment(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    bool res = true;
    //!!TNs root_ns; // Don't propagate context from upper layer, ref ds_cli_sno_s3
    TNs root_ns = aCtx.mNs; 
    updateNs(root_ns, aMut);

    for (ChromoNode::Const_Iterator rit = aMut.Begin(); rit != aMut.End() && res; rit++)
    {
	ChromoNode rno = (*rit);
	MutCtx mctx(this, root_ns);
	try {
	    mutate(rno, aUpdOnly, mctx);
	} catch (std::exception e) {
	    Logger()->Write(EErr, this, "Unspecified error on mutation");
	}
    }
}

void Node::onOwnedAttached(MOwned* aOwned)
{
    // Notify the observers
    // Cache observers first to avoid iterating broked due to observers change
    list<MObserver*> cache;
    for (auto it = mOcp.pairsBegin(); it != mOcp.pairsEnd(); it++) {
	auto* pair = *it;
        cache.push_back(pair->provided());
    }
    for (auto obs : cache) {
        obs->onObsOwnedAttached(this, aOwned);
    }
}

void Node::onOwnedDetached(MOwned* aOwned)
{
    for (auto it = mOcp.pairsBegin(); it != mOcp.pairsEnd(); it++) {
        (*it)->provided()->onObsOwnedDetached(this, aOwned);
    }
}

void Node::onOwnerAttached()
{
    // Notify the observers
    // Cache observers first to avoid iterating broked due to observers change
    list<MObserver*> cache;
    for (auto it = mOcp.pairsBegin(); it != mOcp.pairsEnd(); it++) {
	auto* pair = *it;
        cache.push_back(pair->provided());
    }
    for (auto obs : cache) {
        obs->onObsOwnerAttached(this);
    }
}

bool Node::isOwned(const MOwned* aOwned) const
{
    bool res = false;
    res = ownerCp()->isConnected(const_cast<MOwned::TOwnedCp*>(aOwned->ownedCp()));
    if (!res) {
	// Not local owned, try the tree
	for (auto it = ownerCp()->pairsCBegin(); it != ownerCp()->pairsCEnd() && !res; it++) {
	    const MOwner* owr = (*it)->binded()->provided();
	    res = owr->isOwned(aOwned);
	}
    }
    return res;
}


// MChild


MParent* Node::parent()
{
    MNode* prntNode = Provider()->provGetNode(string(idStr()));
    return prntNode->lIft<MParent>();
}

const MParent* Node::parent() const
{
    MNode* prntNode = Provider()->provGetNode(string(idStr()));
    return prntNode->lIft<MParent>();
}
