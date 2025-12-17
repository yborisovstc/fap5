
#include "dessdo.h"


SdoBase::InpBase::InpBase(SdoBase* aHost, const string& aName): mHost(aHost), mName(aName)
{
}

void SdoBase::InpBase::Construct()
{
    // Add  input to the host
    MNode* cp = mHost->Provider()->createNode(string(CpStateInp::idStr()), mName, mHost->mEnv);
    assert(cp);
    bool res = mHost->attachOwned(cp);
    assert(res);
    auto cpv = cp->lIft<MConnPoint>();
    assert(cpv);
    res = cpv->bind(&mHost->mInpsBp);
    assert(res);
}

SdoBase::SdoBase(const string &aType, const string& aName, MEnv* aEnv): CpStateOutp(aType, aName, aEnv),
    mObrCp(this), mEagObs(this), mSue(nullptr), mCInv(true),
    mExploringCp(MSystExploring::idHash(), MSystExplorable::idHash(), dynamic_cast<MSystExploring*>(this)),
    mInpsBp(MDesInpObserver::idHash(), MDVarGet::idHash(), dynamic_cast<MDesInpObserver*>(this)),
    mOutpBp(MDVarGet::idHash(), MDesInpObserver::idHash(), dynamic_cast<MDVarGet*>(this))
{
}

void SdoBase::Construct()
{
    bool res = bind(&mOutpBp);
    assert(res);
}

MIface* SdoBase::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMDVarGet));
    else if (res = checkLif2(aId, mMDesInpObserver));
    else res = CpStateOutp::MNode_getLif(aId);
    return res;
}

MIface* SdoBase::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSystExploring));
    else res = CpStateOutp::MOwned_getLif(aId);
    return res;
}

void SdoBase::MDVarGet_doDump(int aLevel, int aIdt, ostream& aOs) const
{
}

void SdoBase::onOwnerAttached()
{
    bool res = false;
    MObservable* obl = owner()->lIf(obl);
    if (obl) {
	//res = obl->addObserver(&mObrCp);
	res = obl->addObserver(this, TNodeEventChanged::idHash);
    }
    if (!res || !obl) {
	LOGN(EErr, "Cannot attach to observer");
    } else {
	// Getting explorable
	// TODO to migrate to specific explorable iface
	UpdateMag();
    }
}

MSystExplorable* SdoBase::getExplorable()
{
    MSystExplorable* res = nullptr;
    if (mExploringCp.pcount() == 1) {
	auto pair = mExploringCp.pairAt(0);
	res = pair->prov<MSystExplorable>();
    }
    return res;
}

void SdoBase::UpdateMag()
{
    if (mName == "Name") {
        LOGN(EDbg, "UpdateMag");
    }
    // Attempt MDesAdapter iface first
    MSystExplorable* desa = getExplorable();
    MNode* mag = nullptr;
    if (desa) {
	mag = desa->getMag();
    }
    if (!mag) {
	//LOGN(EErr, "Cannot get owners explorable");
    } else {
	// Set explored system
	if (mag != mSue) {
	    mSue = mag;
	    NotifyInpsUpdated();
            // Set observing both local and agent obervable
            MObservable* obl = mSue->lIf(obl);
            if (obl) {
                //obl->addObserver(&mEagObs.mOcp);
                obl->addObserver(&mEagObs, TNodeEventOwnedAttached::idHash);
                obl->addObserver(&mEagObs, TNodeEventChanged::idHash);
            }
            LOGN(EInfo, "Explorable is attached [" + mSue->Uid() + "]");
            // We need to observe also agents observable but ifr cannot get it because of stopping on getting local iface
            // So, we need to use the trick: first get agents and then use ifr for getting observable
#if 0
            MUnit* sueu = mSue->lIf(sueu);
            if (sueu) {
                auto agents = sueu->getIfs<MAgent>();
                for (auto* agt: *agents) {
                    MUnit* agentu = reinterpret_cast<MAgent*>(agt)->lIf(agentu);
                    if (agentu) {
                        auto obls = agentu->getIfs<MObservable>();
                        for (auto* observable : *obls) {
                            reinterpret_cast<MObservable*>(observable)->addObserver(&mEagObs.mOcp);
                        }
                    }
                }
                LOGN(EInfo, "Explorable is attached [" + mSue->Uid() + "]");
            }
#endif
        }
    }
}

void SdoBase::onObsEvent(MObservable* aObl, const MEvent* aEvent)
{
    if (aEvent->mId == TNodeEventChanged::idHash) {
	mCInv = true;
	UpdateMag();
    }
}

void SdoBase::NotifyInpsUpdated()
{
    mCInv = true;
    for (auto pair : mPairs) {
	auto* obs = pair->lIft<MDesInpObserver>();
	if (obs) {
            obs->onInpUpdated();
	}
    }
}

void SdoBase::onInpUpdated()
{
    NotifyInpsUpdated();
}

void SdoBase::onMagChanged()
{
    UpdateMag();
}

MNpc* SdoBase::getCp()
{
    return &mExploringCp;
}



///  SDO "Name"

SdoName::SdoName(const string &aType, const string& aName, MEnv* aEnv): Sdog<Sdata<string>>(aType, aName, aEnv) { }

const DtBase* SdoName::VDtGet(const string& aType)
{
    if (mCInv) {
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            mRes.mData = mSue->name();
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "Uri"

SdoUri::SdoUri(const string &aType, const string& aName, MEnv* aEnv): Sdog<DGuri>(aType, aName, aEnv) { }

const DtBase* SdoUri::VDtGet(const string& aType)
{
    if (mCInv) {
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            mSue->getUri(mRes.mData, nullptr);
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "Parent"

SdoParent::SdoParent(const string &aType, const string& aName, MEnv* aEnv): Sdog<Sdata<string>>(aType, aName, aEnv) { }

const DtBase* SdoParent::VDtGet(const string& aType)
{
    if (mCInv) {
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            mRes.mData = mSue->parentUri();
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}

///  SDO "Parents heirarchy"

SdoParents::SdoParents(const string &aType, const string& aName, MEnv* aEnv): Sdog<Vector<DGuri>>(aType, aName, aEnv) { }

const DtBase* SdoParents::VDtGet(const string& aType)
{
    if (mName == "Parents") {
        LOGN(EDbg, "VDtGet");
    }
    if (mCInv) {
        mRes.mValid = false;
        mRes.mData.clear();
        if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            MElem* suee = mSue->lIf(suee);
            if (suee) {
                MParent* prnt = nullptr;;
                auto* cn = *suee->asChild()->cP()->pairsBegin();
                // TODO this solution exploits the access to upper inheritance tree
                // that allowed thru MChild::cP() and creates vulnarability. Consider redesign. 
                while (cn) {
                    prnt = cn->provided();
                    GUri uri;
                    prnt->parentGetUri(uri);
                    mRes.mData.push_back(DGuri(uri));
                    cn = *cn->binded()->pairsBegin();
                }
                // Add native parents chain
                auto p = prnt->parentsUri();
                mRes.mData.insert(mRes.mData.end(), p.begin(), p.end());
            } else {
                // Explorable isn't elem - take just parent's name
                auto p = mSue->parentsUris();
                mRes.mData.insert(mRes.mData.end(), p.begin(), p.end());
            }
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}





///  SDO "Component exists"

SdoComp::SdoComp(const string &aType, const string& aName, MEnv* aEnv): Sdog<Sdata<bool>>(aType, aName, aEnv),
    mInpName(this, "Name")
{
}

void SdoComp::Construct()
{
    SdoBase::Construct();
    mInpName.Construct();
}

const DtBase* SdoComp::VDtGet(const string& aType)
{
    if (mCInv) {
        mRes.mValid = false;
        DGuri uri;
        bool res = mInpName.getData(uri);
        if (!res) {
            LOGN(EErr, "Failed getting input [" + mInpName.mName + "] data");
        } else if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            mRes.mData = mSue->getNode(uri.mData);
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "Components count"

SdoCompsCount::SdoCompsCount(const string &aType, const string& aName, MEnv* aEnv): Sdog<Sdata<int>>(aType, aName, aEnv) { }

const DtBase* SdoCompsCount::VDtGet(const string& aType)
{
    if (mCInv) {
        string name;
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            int count = mSue->ownerCp()->pcount();
            mRes.mData = count;
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}

void SdoCompsCount::onEagOwnedAttached(const MOwned* aOwned)
{
    //LOGN(EErr, "SdoCompsCount, Event Owned attached: " + aOwned->Uid());
    NotifyInpsUpdated();
}

void SdoCompsCount::onEagOwnedDetached(const MOwned* aOwned)
{
    LOGN(EErr, "SdoCompsCount, Event OwnedDetached: " + aOwned->Uid());
    NotifyInpsUpdated();
}


///  SDO "Components names"

SdoCompsNames::SdoCompsNames(const string &aType, const string& aName, MEnv* aEnv): Sdog<Vector<string>>(aType, aName, aEnv) { }

const DtBase* SdoCompsNames::VDtGet(const string& aType)
{
    if (mCInv) {
        string name;
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            Stype cnames;
            for (auto it = mSue->ownerCp()->pairsBegin(); it != mSue->ownerCp()->pairsEnd(); it++) {
                auto owdCp = *it;
                MNode* osn = owdCp->provided()->lIf(osn);
                cnames.mData.push_back(osn->name());
            }
            mRes.mData = cnames.mData;
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}

void SdoCompsNames::onEagOwnedAttached(const MOwned* aOwned)
{
    NotifyInpsUpdated();
}

void SdoCompsNames::onEagOwnedDetached(const MOwned* aOwned)
{
    NotifyInpsUpdated();
}

///  SDO "Components names, output of DGuri"

SdoCompsUri::SdoCompsUri(const string &aType, const string& aName, MEnv* aEnv): Sdog<Vector<DGuri>>(aType, aName, aEnv) { }

const DtBase* SdoCompsUri::VDtGet(const string& aType)
{
    if (mCInv) {
        string name;
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            Stype cnames;
            for (auto it = mSue->ownerCp()->pairsBegin(); it != mSue->ownerCp()->pairsEnd(); it++) {
                auto* owdCp = *it;
                MNode* osn = owdCp->provided()->lIf(osn);
                DGuri curi(osn->name());
                cnames.mData.push_back(curi);
            }
            mRes.mData = cnames.mData;
            mRes.mValid = true;
        }
        mCInv = false;
    }
    return &mRes;
}

void SdoCompsUri::onEagOwnedAttached(const MOwned* aOwned)
{
    NotifyInpsUpdated();
}

void SdoCompsUri::onEagOwnedDetached(const MOwned* aOwned)
{
    NotifyInpsUpdated();
}

///  SDO "Component owner"

SdoCompOwner::SdoCompOwner(const string &aType, const string& aName, MEnv* aEnv): Sdog<DGuri>(aType, aName, aEnv),
    mInpCompUri(this, "Comp")
{
}

void SdoCompOwner::Construct()
{
    SdoBase::Construct();
    mInpCompUri.Construct();
}

// TODO Do we need it at all? It just reduct giver Uri to get owner's Uri.
const DtBase* SdoCompOwner::VDtGet(const string& aType)
{
    if (mCInv) {
        DGuri curi;
        mRes.mValid = false;
        bool res = mInpCompUri.getData(curi);
        if (!res) {
            LOGN(EErr, "Failed getting input [" + mInpCompUri.mName + "] data");
        } else if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            MNode* comp = mSue->getNode(curi.mData);
            if (comp) {
                // TODO Comp URI reduction is used instead of comp MNode API. This is because
                // of MNode not allowing to access owner. To re-design.
                GUri ownerUri = curi.mData.head(curi.mData.size() - 1);
                MNode* owner = mSue->getNode(ownerUri);
                if (owner) {
                    mRes.mData = ownerUri;
                    mRes.mValid = true;
                } else {
                    LOGN(EErr, "Couldn't get component [" + curi.ToString(false) + "] owner");
                }
            } else {
                LOGN(EErr, "Couldn't get component [" + curi.ToString(false) + "]");
            }
        }
        mCInv = false;
    }
    return &mRes;
}

///  SDO "Component comp"

SdoCompComp::SdoCompComp(const string &aType, const string& aName, MEnv* aEnv): Sdog<DGuri>(aType, aName, aEnv),
    mInpCompUri(this, "Comp"), mInpCompCompUri(this, "CompComp")
{
}

void SdoCompComp::Construct()
{
    SdoBase::Construct();
    mInpCompUri.Construct();
    mInpCompCompUri.Construct();
}


const DtBase* SdoCompComp::VDtGet(const string& aType)
{
    // TODO Opt ds_desopt_tdc causes system failure. Needs to observe comp (curi). To fix.
    // TODO Do we need this. This can be replaced by uris append
    //if (mCInv) {
    if (true) {
        DGuri curi, ccuri;
        mRes.mValid = false;
        bool res = mInpCompUri.getData(curi);
        if (!res) {
            LOGN(EErr, "Failed getting input [" + mInpCompUri.mName + "] data");
        } else if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            res = mInpCompCompUri.getData(ccuri);
            if (!res) {
                LOGN(EErr, "Failed getting input [" + mInpCompCompUri.mName + "] data");
            } else {
                MNode* comp = mSue->getNode(curi.mData);
                if (comp) {
                    MNode* ccomp = comp->getNode(ccuri.mData);
                    if (ccomp) {
                        mRes.mData.clear();
                        ccomp->getUri(mRes.mData, mSue);
                        mRes.mValid = true;
                    } else {
                        LOGN(EErr, "Couldn't get component [" + mInpCompCompUri.mName + "] owner");
                    }
                } else {
                    LOGN(EErr, "Couldn't get component [" + mInpCompUri.mName + "]");
                }
            }
        }
        mCInv = false;
    }
    return &mRes;
}




///  SDO "Vertexes are connected"

SdoConn::SdoConn(const string &aType, const string& aName, MEnv* aEnv): Sdog<Sdata<bool>>(aType, aName, aEnv),
    mInpVp(this, "Vp"), mInpVq(this, "Vq"), mVpUe(nullptr), mVqUe(nullptr)
{
}

void SdoConn::Construct()
{
    SdoBase::Construct();
    mInpVp.Construct();
    mInpVq.Construct();
}


const DtBase* SdoConn::VDtGet(const string& aType)
{
    if (mCInv) {
        mRes.mValid = false;
        DGuri vps, vqs;
        bool res = mInpVp.getData(vps);
        bool resq = mInpVq.getData(vqs) ;
        if (!res || !resq) {
            LOGN(EErr, "Failed getting input [" + (res ? mInpVp.mName : mInpVq.mName) + "] data");
        } else if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            MNode* vpn = mSue->getNode(vps.mData);
            MNode* vqn = mSue->getNode(vqs.mData);
            if (vpn && vqn) {
                if (vpn != mVpUe) {
                    mVpUe = vpn;
                    MObservable* obl = mVpUe->lIf(obl);
                    if (obl) {
                        //res = obl->addObserver(&mObrCp);
                        res = obl->addObserver(this, TNodeEventChanged::idHash);
                    }
                    if (!res || !obl) {
                        LOGN(EErr, "Cannot attach VertUe to observer");
                    }
                }
                if (vqn != mVqUe) {
                    mVqUe = vqn;
                    MObservable* obl = mVqUe->lIf(obl);
                    if (obl) {
                        //res = obl->addObserver(&mObrCp);
                        res = obl->addObserver(this, TNodeEventChanged::idHash);
                    }
                    if (!res || !obl) {
                        LOGN(EErr, "Cannot attach VertUe to observer");
                    }
                }
                MVert* vpv = vpn->lIf(vpv);
                MVert* vqv = vqn->lIf(vqv);
                if (vpv && vqv) {
                    mRes.mData = vpv->isConnected(vqv);
                    mRes.mValid = true;
                }
            }
        }
        mCInv = false;
    }
    return &mRes;
}

void SdoConn::onObsEvent(MObservable* aObl, const MEvent* aEvent) {
    if (aEvent->mId == TNodeEventChanged::idHash) {
	MObservable* op = mVpUe ? mVpUe->lIf(op) : nullptr;
	MObservable* oq = mVqUe ? mVqUe->lIf(oq) : nullptr;
	if (aObl == op || aObl == oq) {
	    NotifyInpsUpdated();
	}
    }
    SdoBase::onObsEvent(aObl, aEvent);
}


///  SDO "Vertex pairs count"

SdoPairsCount::SdoPairsCount(const string &aType, const string& aName, MEnv* aEnv): Sdog<Sdata<int>>(aType, aName, aEnv),
    mInpVert(this, "Vp"), mVertUe(nullptr)
{
}

void SdoPairsCount::Construct()
{
    SdoBase::Construct();
    mInpVert.Construct();
}

const DtBase* SdoPairsCount::VDtGet(const string& aType)
{
    if (mName == "SdoPc") {
        LOGN(EDbg, "VDtGet");
    }
    if (mCInv) {
        mRes.mValid = false;
        DGuri verts;
        bool res = mInpVert.getData(verts);
        if (!res) {
            LOGN(EErr, "Failed getting input [" + mInpVert.mName + "] data");
        } else if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            MNode* vertn = mSue->getNode(verts.mData);
            if (vertn) {
                if (vertn != mVertUe) {
                    mVertUe = vertn;
                    // Start observing vertex under exploring
                    MObservable* obl = mVertUe->lIf(obl);
                    if (obl) {
                        //res = obl->addObserver(&mObrCp);
                        //res = obl->addObserver(this, TNodeEventOwnedAttached::idHash);
                        res = obl->addObserver(this, TNodeEventChanged::idHash);
                    }
                    if (!res || !obl) {
                        LOGN(EErr, "Cannot attach VertUe to observer");
                    }
                }
            } else {
                // ds_iss_15
                observingVertUeExst();
            }
            if (vertn) {
                MVert* vertv = vertn->lIf(vertv);
                if (vertv) {
                    mRes.mData = vertv->pairsCount();
                    mRes.mValid = true;
                }
            } else {
                LOGN(EDbg, "Explorable [" + verts.mData.toString() + "] is nil");
            }
            LOGN(EDbg, "Explorable [" + verts.mData.toString() + "], res: " + mRes.ToString(true));
        }
        mCInv = false;
    }
    return &mRes;
}

void SdoPairsCount::observingVertUeExst()
{
    // Vert under exploring doens't exists, observe it's existance
    // Observing it's existing owner
    DGuri verts;
    mInpVert.getData(verts);
    GUri sueUri(verts.mData);
    GUri vertUeOwrUri;
    mVertUeOwrLevel = sueUri.size() - 1;
    MNode* owner = nullptr;
    do {
        vertUeOwrUri = sueUri.head(mVertUeOwrLevel);
        owner = mSue->getNode(vertUeOwrUri);
        if (!owner) mVertUeOwrLevel--;
    } while (!owner);
    if (owner != mVertUeOwr) {
        LOGN(EDbg, "VertUE owner [" + vertUeOwrUri.toString() + "] to be observing, level: " + to_string(mVertUeOwrLevel));
        if (mVertUeOwr) {
            MObservable* obl = mVertUeOwr->lIf(obl);
	    //bool res = obl->rmObserver(&mObrCp);
	    bool res = obl->rmObserver(this, TNodeEventOwnedAttached::idHash);
            if (!res || !obl) {
                LOGN(EErr, "Failed deattaching VertUeOwr from observable");
            }
        }
        mVertUeOwr = owner;
        MObservable* obl = mVertUeOwr->lIf(obl);
        //bool res = obl ? obl->addObserver(&mObrCp) : false;
        bool res = obl ? obl->addObserver(this, TNodeEventOwnedAttached::idHash) : false;
        if (!res || !obl) {
            LOGN(EErr, "Cannot attach VertUeOwr to observer");
        } else {
            LOGN(EDbg, "VertUE owner [" + mVertUeOwr->getUriS(mSue) + "] observing, level: " + to_string(mVertUeOwrLevel));
        }
    }
}

void SdoPairsCount::onObsEvent(MObservable* aObl, const MEvent* aEvent)
{
    if (aEvent->mId == TNodeEventOwnedAttached::idHash) {
	auto event = reinterpret_cast<const TNodeEventOwnedAttached*>(aEvent);
	auto aOwned = event->mOwned;
	MObservable* obl = mVertUeOwr ? mVertUeOwr->lIf(obl) : nullptr;
	if (obl && aObl == obl) {
	    // VeruUE owner observable
	    LOGN(EDbg, "TNodeEventOwnedAttached, owned: " + aOwned->Uid());
	    DGuri verts;
	    mInpVert.getData(verts);
	    GUri sueUri(verts.mData);
	    GUri owdUri = sueUri.head(mVertUeOwrLevel + 1);
	    auto* vertUeOwrOwd = mSue->getNode(owdUri);
	    MOwned* vueOwd = vertUeOwrOwd ? vertUeOwrOwd->lIf(vueOwd) : nullptr;
	    if (vueOwd && aOwned == vueOwd) {
		LOGN(EDbg, "[" + mVertUeOwr->getUriS(mSue) + "] owned [" + vertUeOwrOwd->getUriS(mSue) + "] attached");
		// Checking if VertUe got attached
		MNode* vertUe = mSue->getNode(verts.mData);
		if (vertUe) {
		    // Yes, attached. Stop observing the attaching
		    LOGN(EDbg, "VertUe [" + vertUe->getUriS(mSue) + "] got attached");
		    NotifyInpsUpdated();
		} else {
		    // Not attached yet, proceed
		    observingVertUeExst();
		}
	    }
	}
    } else if (TNodeEventChanged::idHash) {
        MObservable* obl = mVertUe ? mVertUe->lIf(obl) : nullptr;
        if (obl && aObl == obl) {
            LOGN(EDbg, "VertUE changed");
            NotifyInpsUpdated();
        }
    }
    SdoBase::onObsEvent(aObl, aEvent);
}


///  SDO "Vertex pair URI"

SdoPair::SdoPair(const string &aType, const string& aName, MEnv* aEnv): Sdog<DGuri>(aType, aName, aEnv),
    mInpTarg(this, "Vp"), mVertUe(nullptr)
{
}

void SdoPair::Construct()
{
    SdoBase::Construct();
    mInpTarg.Construct();
}


const DtBase* SdoPair::VDtGet(const string& aType)
{
    if (mCInv) {
        DGuri verts;
        mRes.mValid = false;
        bool res = mInpTarg.getData(verts);
        if (!res) {
            LOGN(EErr, "Failed getting input [" + mInpTarg.mName + "] data");
        } else if (!mSue)  {
            LOGN(EWarn, "Owner is not explorable");
        } else {
            MNode* vertn = mSue->getNode(verts.mData);
            if (vertn && vertn != mVertUe) {
                mVertUe = vertn;
                // Start observing vertex under exploring
                MObservable* obl = mVertUe->lIf(obl);
                if (obl) {
                    res = obl->addObserver(&mObrCp);
                }
                if (!res || !obl) {
                    LOGN(EErr, "Cannot attach VertUe to observer");
                }
            }
            if (vertn) {
                MVert* vertv = vertn->lIf(vertv);
                if (vertv) {
                    if (vertv->pairsCount() == 1) {
                        MVert* pair = vertv->getPair(0);
                        if (pair) {
                            pair->vertGetUri(mRes.mData, mSue);
                            mRes.mValid = true;
                        }
                    }
                } else {
                    LOGN(EErr, "Target [" + verts.mData.toString() + "] is not vertex");
                }
            }
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "Single pair of targets comp"

SdoTcPair::SdoTcPair(const string &aType, const string& aName, MEnv* aEnv): Sdog<DGuri>(aType, aName, aEnv),
    mInpTarg(this, "Targ"), mInpTargComp(this, "TargComp")
{
}

void SdoTcPair::Construct()
{
    SdoBase::Construct();
    mInpTarg.Construct();
    mInpTargComp.Construct();
}


const DtBase* SdoTcPair::VDtGet(const string& aType)
{
    if (mCInv) {
        DGuri targUri;
        mRes.mValid = false;
        bool res = mInpTarg.getData(targUri);
        if (!res) {
            LOGN(EErr, "Failed getting input [" + mInpTarg.mName + "] data");
        } else {
            DGuri targCompUri;
            res = mInpTargComp.getData(targCompUri);
            if (!res) {
                LOGN(EErr, "Failed getting input [" + mInpTargComp.mName + "] data");
            } else if (!mSue)  {
                LOGN(EWarn, "Owner is not explorable");
            } else {
                targUri += targCompUri;
                MNode* vertn = mSue->getNode(targUri.mData);
                if (vertn) {
                    MVert* vertv = vertn->lIf(vertv);
                    if (vertv) {
                        if (vertv->pairsCount() == 1) {
                            MVert* pair = vertv->getPair(0);
                            if (pair) {
                                GUri uri;
                                pair->vertGetUri(uri, mSue);
                                mRes.mData = uri;
                                mRes.mValid = true;
                            }
                        }
                    } else {
                        LOGN(EErr, "Target [" + targUri.mData.toString() + "] is not vertex");
                    }
                }
            }
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "Pairs"

SdoPairs::SdoPairs(const string &aType, const string& aName, MEnv* aEnv): Sdog<Vector<DGuri>>(aType, aName, aEnv) { }

const DtBase* SdoPairs::VDtGet(const string& aType)
{
    if (mCInv) {
        if (!mSue)  {
            LOGN(EErr, "Owner is not explorable");
        } else {
            MVert* suev = mSue->lIf(suev);
            if (!suev) {
                LOGN(EErr, "Explorable isn't vertex");
            } else {
                mRes.mValid = true;
                mRes.mData.clear();
                for (int ind = 0; ind < suev->pairsCount(); ind++) {
                    MVert* pair = suev->getPair(ind);
                    if (!pair) {
                        LOGN(EErr, "Couldnt get URI for pair [" + pair->Uid() + "]");
                        mRes.mValid = false;
                        break;
                    } else {
                        mRes.mValid = true;
                        GUri puri;
                        pair->vertGetUri(puri, mSue);
                        DGuri purid(puri);
                        mRes.mData.push_back(purid);
                    }
                }
            }
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "Target pairs"

SdoTPairs::SdoTPairs(const string &aType, const string& aName, MEnv* aEnv): Sdog<Vector<DGuri>>(aType, aName, aEnv),
    mInpTarg(this, "Targ") {}

void SdoTPairs::Construct()
{
    SdoBase::Construct();
    mInpTarg.Construct();
}

const DtBase* SdoTPairs::VDtGet(const string& aType)
{
    if (mCInv) {
        DGuri turi;
        mRes.mValid = false;
        bool res = mInpTarg.getData(turi);
        if (!res) {
            LOGN(EErr, "Failed getting input [" + mInpTarg.mName + "] data");
        } else if (!mSue)  {
            LOGN(EErr, "Owner is not explorable");
        } else {
            MNode* targn = mSue->getNode(turi.mData);
            if (!targn) {
                LOGN(EErr, "Couldn't find target [" + mInpTarg.mName + "]");
            } else {
                MVert* targv = targn->lIf(targv);
                if (!targv) {
                    LOGN(EErr, "Target [" + mInpTarg.mName + "] isn't a vertex");
                } else {
                    mRes.mValid = true;
                    for (int ind = 0; ind < targv->pairsCount(); ind++) {
                        MVert* pair = targv->getPair(ind);
                        if (!pair) {
                            LOGN(EErr, "Couldnt get URI for pair [" + pair->Uid() + "]");
                            mRes.mValid = false;
                            break;
                        } else {
                            mRes.mValid = true;
                            GUri puri;
                            pair->vertGetUri(puri, mSue);
                            DGuri purid(puri);
                            mRes.mData.push_back(purid);
                        }
                    }
                }
            }
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "Edges"

SdoEdges::SdoEdges(const string &aType, const string& aName, MEnv* aEnv): Sdog<Vector<Pair<DGuri>>>(aType, aName, aEnv) { }

const DtBase* SdoEdges::VDtGet(const string& aType)
{
    if (mCInv) {
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EErr, "Owner is not explorable");
        } else {
            MSyst* sues = mSue->lIf(sues);
            if (!sues) {
                LOGN(EErr, "Explorable isn't system");
            } else {
                mRes.mValid = true;
                mRes.mData.clear();
                for (auto conn : sues->connections()) {
                    MVert* p = conn.first;
                    MVert* q = conn.second;
                    if (!p || !q) {
                        LOGN(EErr, "Couldnt get URI for vert [" + (p ? p->Uid(): q->Uid()) + "]");
                        mRes.mValid = false;
                        break;
                    } else {
                        mRes.mValid = true;
                        GUri puri;
                        p->vertGetUri(puri, mSue);
                        GUri quri;
                        q->vertGetUri(quri, mSue);
                        DGuri purid(puri);
                        DGuri qurid(quri);
                        Pair<DGuri> elem;
                        elem.mData.first = purid;
                        elem.mData.second = qurid;
                        elem.mValid = true;
                        mRes.mData.push_back(elem);
                    }
                }
            }
        }
        mCInv = false;
    }
    return &mRes;
}


///  SDO "DES is idle"

SdoDesIdle::SdoDesIdle(const string &aType, const string& aName, MEnv* aEnv): Sdog<Sdata<bool>>(aType, aName, aEnv) { }

const DtBase* SdoDesIdle::VDtGet(const string& aType)
{
    if (mCInv) {
        mRes.mValid = false;
        if (!mSue)  {
            LOGN(EErr, "Owner is not explorable");
        } else {
            MDesSyncable* sues = mSue->lIf(sues);
            if (!sues) {
                LOGN(EErr, "Explorable isn't DES syncable");
            } else {
                mRes.mData = !sues->isActive();
                mRes.mValid = true;
            }
        }
        mCInv = false;
    }
    return &mRes;
}

void SdoDesIdle::onEagChanged()
{
    NotifyInpsUpdated();
}



#if 0 // NOT COMPLETED

/// Inputs update indicator, ref ds_dcs_iui_sla

SdoUpdateInd::SdoUpdateInd(const string &aType, const string& aName, MEnv* aEnv): CpStateOutp(aType, aName, aEnv)
{
    mData.mData = false;
    mData.mValid = true;
}

MIface* SdoUpdateInd::MNode_getLif(const char *aType)
{
    MIface* res = nullptr;
    if (res = checkLif<MDVarGet>(aType));
    else res = CpStateOutp::MNode_getLif(aType);
    return res;
}

void SdoUpdateInd::MDVarGet_doDump(int aLevel, int aIdt, ostream& aOs) const
{
}
#endif
