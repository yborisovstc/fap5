
#include "connpoint.h"

//// ConnPoint

vector<GUri> ConnPoint::getParentsUri()
{
    auto p = Vert::getParentsUri();
    p.insert(p.begin(), string(idStr()));
    return p;
}


MIface* ConnPoint::MNode_getLif(TIdHash aId)
{
    // TODO to reuse MOwned resolver?
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLif2(aId, mMConnPoint));
    else res = Node::MNode_getLif(aId);
    return res;
}

MIface* ConnPoint::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLif2(aId, mMConnPoint));
    else res = Node::MOwned_getLif(aId);
    return res;
}

MIface* ConnPoint::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMConnPoint));
    else res = Vert::MVert_getLif(aId);
    return res;
}

bool ConnPoint::isCompatible(const MVert* aPair, bool aExt) const
{
    bool res = false;
    const MConnPoint* pcp = aPair->lIft<const MConnPoint>();
    if (pcp) {
        auto sRid = idRequired();
        auto sPid = idProvided();
        auto pRid = pcp->idRequired();
        auto pPid = pcp->idProvided();
        res = aExt ? (pRid == sRid && pPid == sPid) : (pRid == sPid && pPid == sRid);
    }
    return res;
}

MVert* ConnPoint::getExtd()
{
    return nullptr;
}

bool ConnPoint::bind(MNpc* aPair)
{
    bool res = aPair->connect(bP());
    return res;
}

bool ConnPoint::unbind(MNpc* aPair)
{
    return aPair->disconnect(bP());
}

bool ConnPoint::isBound(const MNpc* aPair) const
{
    return aPair->isConnected(const_cast<ConnPoint*>(this)->bP());
}

bool ConnPoint::isBound(const MVert* aPair) const
{
    MVert* pairv = const_cast<MVert*>(aPair);
    auto* pairCp = const_cast<MConnPoint*>(pairv->lIft<MConnPoint>());
    return pairCp ? pairCp->bP()->isConnected(const_cast<ConnPoint*>(this)->bP()) : false;
}

bool ConnPoint::bind(MVert* aPair)
{
    bool res = false;
    auto* pairCp = aPair->lIft<MConnPoint>();
    if (pairCp) {
        res = bP()->connect(pairCp->bP());
        if (res) {
            onBound(aPair);
            aPair->onBound(this);
        }
    }
    return res;
}

bool ConnPoint::unbind(MVert* aPair)
{
    bool res = false;
    auto* pairCp = aPair->lIft<MConnPoint>();
    if (pairCp) {
        res = bP()->disconnect(pairCp->bP());
        if (res) {
            onUnbound(aPair);
            aPair->onUnbound(this);
        }
    }
    return res;
}


#if 0
//// ExtdInt

ExtdInt::ExtdInt(const string &aType, const string& aName, MEnv* aEnv): Vert(aType, aName, aEnv),
    mEbp(this)
{}

MIface* ExtdInt::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLifn(aId, mEbp.mPair ? mEbp.mPair->provided() : nullptr));
    else res = Vert::MVert_getLif(aId);
    return res;
}

bool ExtdInt::isCompatible(const MVert* aPair, bool aExt) const
{
    bool res = false;
    MVert* host = mEbp.mPair ? mEbp.mPair->provided() : nullptr;
    res = host ? host->isCompatible(aPair, !aExt) : false;
    return res;
}

MVert* ExtdInt::GetPin(const string& aId)
{
    auto* hosts = hostAsSocket();
    return hosts ? hosts->GetPin(aId, true) : nullptr;
}

MVert* ExtdInt::GetPin(const string& aId, bool aExtd)
{
    assert(false);
    return nullptr;
}

MSocket* ExtdInt::hostAsSocket()
{
    MVert* host = mEbp.mPair ? mEbp.mPair->provided() : nullptr;
    return host ? host->lIft<MSocket>() : nullptr;
}
#endif




//// Socket

vector<GUri> Socket::getParentsUri()
{
    auto p = Verte::getParentsUri();
    p.insert(p.begin(), string(idStr()));
    return p;
}

Socket::Socket(const string &aType, const string& aName, MEnv* aEnv): Verte(aType, aName, aEnv) {}


MIface* Socket::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLif2(aId, mMSocket));
    else res = Verte::MNode_getLif(aId);
    return res;
}

MIface* Socket::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert)); // To allow embedding to SocketExtd
    else if (res = checkLif2(aId, mMSocket));
    else res = Verte::MOwned_getLif(aId);
    return res;
}


MIface* Socket::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSocket));
    else res = Verte::MVert_getLif(aId);
    return res;
}

bool Socket::isCompatible(const MVert* aPair, bool aExt) const
{
    Socket* self = const_cast<Socket*>(this);
    bool res = true;
    const MSocket* pairSocket = aPair ? aPair->lIf(pairSocket) : nullptr;
    if (pairSocket) {
        for (auto it = self->ownerCp()->pairsBegin(); it != self->ownerCp()->pairsEnd(); it++) {
            MOwned* comp = (*it)->provided();
            MVert* pin = self->checkPin(comp);
            if (pin) {
                MVert* pairPin = const_cast<MSocket*>(pairSocket)->GetPin(comp->ownedId());
                if (pairPin) {
                    res = pairPin->isCompatible(pin) && pin->isCompatible(pairPin);
                } else {
                    res = false;
                }
            }
            if (!res) break;
        }
    } else {
        res = false;
    }
    return res;
}

MVert* Socket::getExtd()
{
    MVert* res = nullptr;
    return res;
}

MVert::TDir Socket::getDir() const
{
    TDir res = ERegular;
    return res;
}

/*
int Socket::PinsCount() const
{
    return ownerCp()->pcount();
}

MVert* Socket::GetPin(int aInd)
{
    assert(false);
    MVert* res = nullptr;
    MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    res = owd ? owd->lIft<MVert>() : nullptr;
    return res;
}
*/

MVert* Socket::GetPin(const string& aId)
{
    MVert* res = nullptr;
    auto owdCp = ownerCp()->pairAt(aId);
    MOwned* owd = owdCp ? owdCp->provided() : nullptr;
    return checkPin(owd);
}

/*
string Socket::GetPinId(int aInd) const
{
    assert(false);
    const MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    return owd->ownedId();
}
*/

MVert* Socket::checkPin(MOwned* aOwned)
{
    MVert* res = nullptr;
    MVert* owdv = aOwned->lIft<MVert>();
    /* ds_sock_mnpsd_sus
    MSocket* owds = aOwned->lIft<MSocket>();
    if (owds) {
        res = owdv;
    } else {
        res = owdv ? owdv->getExtd() : nullptr;
    }
    */
    res = owdv ? owdv->getExtd() : nullptr;
    return res;
}

void Socket::bindPins(MSocket* aPair, bool aConnect, bool aUndo)
{
    for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd(); it++) {
        MOwned* comp = (*it)->provided();
        MVert* pin = checkPin(comp);
        string pinId = comp->ownedId();
        if (pin == nullptr) continue;
        MSocket* pinS = pin->lIft<MSocket>();
        auto pairPin = aPair->GetPin(pinId);
        MSocket* pairPinS = pairPin->lIft<MSocket>();
        if (pin && pairPin) {
            if (pin->isCompatible(pairPin)/* && pairPin->isCompatible(pin)*/) {
                if (aConnect) {
                    if (aUndo) {
                        bool res = pin->disconnect(pairPin);
                        if (!res) {
                            LOGN(EErr, "Failed disconnecting pin [" + pinId + "]");
                        }
                    } else {
                        bool res = pin->connect(pairPin);
                        if (!res) {
                            LOGN(EErr, "Failed connecting pin [" + pinId + "]");
                        }
                    }
                } else {
                    if (aUndo) {
                        if (pin->isBound(pairPin)) {
                            bool res = pin->unbind(pairPin);
                            if (!res) {
                                LOGN(EErr, "Failed unbinding pins [" + pinId + "]");
                            }
                        }
                    } else {
                        if (!pin->isBound(pairPin)) {
                            bool res = pin->bind(pairPin);
                            if (!res) {
                                LOGN(EErr, "Failed binding pins [" + pinId + "]");
                            }
                        }
                    }
                }
            } else {
                LOGN(EErr, string((aConnect ? "Connecting" : "Binding")) + ". Pin [" + pinId + "] is incompatible");
                pin->isCompatible(pairPin);
            } 
        } else {
            LOGN(EErr, "Pin [" + pinId + "] isn't valid");
        }
    }
}

void Socket::onConnected(MVert* aPair)
{
    Verte::onConnected(aPair);
    MSocket* sock = aPair->lIf(sock);
    bindPins(sock, true);
}

void Socket::onDisconnecting(MVert* aPair)
{
    Verte::onDisconnecting(aPair);
    MSocket* sock = aPair->lIf(sock);
    // TODO Analyze why -sock- is null when disconnect from node destructor
    if (sock) {
        bindPins(sock, true, true);
    }
}

void Socket::onDisconnected()
{
    Verte::onDisconnected();
}

bool Socket::isBound(const MVert* aPair) const
{
    bool res = true;
    auto* self = const_cast<Socket*>(this);
    for (auto it = self->ownerCp()->pairsBegin(); it != self->ownerCp()->pairsEnd(); it++) {
        MOwned* comp = (*it)->provided();
        MVert* pin = self->checkPin(comp);
        string pinId = comp->ownedId();
        auto* pairSc = aPair->lIft<const MSocket>();
        auto* pairS = const_cast<MSocket*>(pairSc);
        MVert* pairPin = pairS->GetPin(pinId);
        res = pin->isBound(pairPin);
        if (!res) break;
    }
    return res;
}

bool Socket::bind(MVert* aPair)
{
    // Solution#1, ref ds_sock_ses_s1
    bool res = false;
    auto* pairS = aPair->lIft<MSocket>();
    if (pairS) {
        bindPins(pairS, false, false);
        res = true;
    }
    return res;
}

bool Socket::unbind(MVert* aPair)
{
    bool res = false;
    auto* pairS = aPair->lIft<MSocket>();
    if (pairS) {
        bindPins(pairS, false, true);
        res = true;
    }
    return res;
}





// SocketExtd 

const string SocketExtd::KUriInt = "Int";  /*!< Internal connpoint */

vector<GUri> SocketExtd::getParentsUri()
{
    auto p = Socket::getParentsUri();
    p.insert(p.begin(), string(idStr()));
    return p;
}


SocketExtd::SocketExtd(const string &aType, const string& aName, MEnv* aEnv): Socket(aType, aName, aEnv)
{
}

void SocketExtd::onOwnedAttached(MOwned* aOwned)
{
    Socket::onOwnedAttached(aOwned);
    MSocket* owds = aOwned->lIf(owds);
    if (owds && aOwned->ownedId() == KUriInt) {
        // Internal CP. Connect pins
        bindPins(owds, false, false);
    }
}
 
void SocketExtd::onOwnedDetached(MOwned* aOwned)
{
    Socket::onOwnedDetached(aOwned);
}

MVert* SocketExtd::getExtd()
{
    MVert* res = nullptr;
    MNode* extn = getComp(KUriInt);
    res = extn ? extn->lIf(res) : nullptr;
    return res;
}

MVert* SocketExtd::GetPin(const string& aId)
{
    MVert* res = nullptr;
    MNode* pinn = getNode(aId);
    res = pinn ? pinn->lIft<MVert>() : nullptr;
    return res;
}


MVert* SocketExtd::checkPin(MOwned* aOwned)
{
    return (aOwned->ownedId() != KUriInt) ? aOwned->lIft<MVert>() : nullptr;
}


// SocketExtdInt


SocketExtdInt::SocketExtdInt(const string &aType, const string& aName, MEnv* aEnv): Socket(aType, aName, aEnv)
{
}

void SocketExtdInt::onOwnedAttached(MOwned* aOwned)
{
    Socket::onOwnedAttached(aOwned);
}
 
void SocketExtdInt::onOwnedDetached(MOwned* aOwned)
{
    Socket::onOwnedDetached(aOwned);
}

MVert* SocketExtdInt::checkPin(MOwned* aOwned)
{
    return aOwned->lIft<MVert>();
}

MVert* SocketExtdInt::GetPin(const string& aId)
{
    MVert* res = nullptr;
    MNode* pinn = getNode(aId);
    res = pinn ? pinn->lIft<MVert>() : nullptr;
    return res;
}



//// Socket2

const string Socket2::KUriInt = "Int";  /*!< Internal connpoint */

vector<GUri> Socket2::getParentsUri()
{
    auto p = Verte::getParentsUri();
    p.insert(p.begin(), string(idStr()));
    return p;
}

Socket2::Socket2(const string &aType, const string& aName, MEnv* aEnv): Verte(aType, aName, aEnv), mEbp(this)
{}

MIface* Socket2::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLif2(aId, mMSocket));
    else res = Verte::MNode_getLif(aId);
    return res;
}

MIface* Socket2::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert)); 
    else if (res = checkLif2(aId, mMSocket));
    else res = Verte::MOwned_getLif(aId);
    return res;
}


MIface* Socket2::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSocket));
    else res = Verte::MVert_getLif(aId);
    return res;
}

bool Socket2::isCompatible(const MVert* aPair, bool aExt) const
{
    Socket2* self = const_cast<Socket2*>(this);
    bool res = true;
    const MSocket* pairSocket = aPair ? aPair->lIf(pairSocket) : nullptr;
    if (pairSocket) {
        for (auto it = self->ownerCp()->pairsBegin(); it != self->ownerCp()->pairsEnd(); it++) {
            MOwned* comp = (*it)->provided();
            auto pinId = comp->ownedId();
            if (pinId == KUriInt) continue;
            MVert* pin = self->GetPin(pinId);
            if (pin) {
                MVert* pairPin = const_cast<MSocket*>(pairSocket)->GetPin(comp->ownedId());
                if (pairPin) {
                    res = pairPin->isCompatible(pin) && pin->isCompatible(pairPin);
                } else {
                    res = false;
                }
            }
            if (!res) break;
        }
    } else {
        res = false;
    }
    return res;
}

MVert* Socket2::getExtd()
{
    //assert(false);
    //return nullptr;
    MVert* res = mEbp.mPair ? mEbp.mPair->provided() : nullptr ;
    return res;
}

MVert::TDir Socket2::getDir() const
{
    TDir res = ERegular;
    return res;
}

MVert* Socket2::GetPin(const string& aId)
{
    MVert* res = nullptr;
    auto owdCp = ownerCp()->pairAt(aId);
    MOwned* owd = owdCp ? owdCp->provided() : nullptr;
    return owd ? owd->lIft<MVert>() : nullptr;
}

bool Socket2::bindPins(MSocket* aPair, bool aConnect, bool aUndo)
{
    bool res = true;
    for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd() && res; it++) {
        MOwned* comp = (*it)->provided();
        string pinId = comp->ownedId();
        if (pinId == KUriInt) continue;
        MVert* pin = GetPin(pinId);
        if (pin == nullptr) continue;
        MSocket* pinS = pin->lIft<MSocket>();
        if (pin) {
            auto pairPin = aPair->GetPin(pinId);
            MSocket* pairPinS = pairPin->lIft<MSocket>();
            if (pairPin) {
                if (pin->isCompatible(pairPin)/* && pairPin->isCompatible(pin)*/) {
                    if (aConnect) {
                        if (aUndo) {
                            bool res = pin->disconnect(pairPin);
                            if (!res) {
                                LOGN(EErr, "Failed disconnecting pin [" + pinId + "]");
                            }
                        } else {
                            bool res = pin->connect(pairPin);
                            if (!res) {
                                LOGN(EErr, "Failed connecting pin [" + pinId + "]");
                            }
                        }
                    } else {
                        if (aUndo) {
                            if (pin->isBound(pairPin)) {
                                bool res = pin->unbind(pairPin);
                                if (!res) {
                                    LOGN(EErr, "Failed unbinding pins [" + pinId + "]");
                                }
                            }
                        } else {
                            if (!pin->isBound(pairPin)) {
                                bool res = pin->bind(pairPin);
                                if (!res) {
                                    LOGN(EErr, "Failed binding pins [" + pinId + "]");
                                    res = pin->bind(pairPin);
                                }
                            }
                        }
                    }
                } else {
                    LOGN(EErr, string((aConnect ? "Connecting" : "Binding")) + ". Pin [" + pinId + "] is incompatible");
                    pin->isCompatible(pairPin);
                } 
            } else {
                LOGN(EErr, "Pin [" + pinId + "] isn't valid");
            }
        }
    }
    return res;
}

void Socket2::onConnected(MVert* aPair)
{
    Verte::onConnected(aPair);
    MSocket* sock = aPair->lIf(sock);
    bindPins(sock, true, false);
}

void Socket2::onDisconnecting(MVert* aPair)
{
    Verte::onDisconnecting(aPair);
    MSocket* sock = aPair->lIf(sock);
    // TODO Analyze why -sock- is null when disconnect from node destructor
    if (sock) {
        bindPins(sock, true, true);
    }
}

void Socket2::onDisconnected()
{
    Verte::onDisconnected();
}

bool Socket2::isBound(const MVert* aPair) const
{
    return (mEbp.mPair != nullptr);
}

bool Socket2::connect(MVert* aPair)
{
    bool res = true;
    // Disable connecting "pure" socket
    if (const_cast<MVert*>(aPair)->getExtd()) {
        res = Verte::connect(aPair);
    } else {
        LOGN(EErr, "Connecting -pure- socket is not allowed.");
        res = false;
    }
    return res;
}

bool Socket2::bind(MVert* aPair)
{
    auto* pairs = aPair->lIft<MSocket>();
    bool res = bindPins(pairs, false, false);
    if (res) {
        res = mEbp.connect(pairs->bP());
    }
    return res;
}

bool Socket2::unbind(MVert* aPair)
{
    assert(false);
    return false;
}

void Socket2::onOwnedAttached(MOwned* aOwned)
{
    Verte::onOwnedAttached(aOwned);
    auto* owdv = aOwned->lIft<MVert>();
    if (owdv && aOwned->ownedId() == KUriInt) {
        bind(owdv);
    }
}
 



//// Socket3

const string Socket3::KContBPeer = "BPeer";

vector<GUri> Socket3::getParentsUri()
{
    auto p = Verte::getParentsUri();
    p.insert(p.begin(), string(idStr()));
    return p;
}

Socket3::Socket3(const string &aType, const string& aName, MEnv* aEnv): Verte(aType, aName, aEnv), mEbp(this)
{}

MIface* Socket3::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLif2(aId, mMSocket));
    else res = Verte::MNode_getLif(aId);
    return res;
}

MIface* Socket3::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert)); 
    else if (res = checkLif2(aId, mMSocket));
    else res = Verte::MOwned_getLif(aId);
    return res;
}


MIface* Socket3::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSocket));
    else if (res = checkLif2(aId, mMContentOwner));
    else res = Verte::MVert_getLif(aId);
    return res;
}

bool Socket3::isCompatible(const MVert* aPair, bool aExt) const
{
    Socket3* self = const_cast<Socket3*>(this);
    bool res = true;
    const MSocket* pairSocket = aPair ? aPair->lIf(pairSocket) : nullptr;
    if (pairSocket) {
        for (auto it = self->ownerCp()->pairsBegin(); it != self->ownerCp()->pairsEnd(); it++) {
            MOwned* comp = (*it)->provided();
            auto pinId = comp->ownedId();
            MVert* pin = self->GetPin(pinId);
            if (pin) {
                MVert* pairPin = const_cast<MSocket*>(pairSocket)->GetPin(comp->ownedId());
                if (pairPin) {
                    res = pairPin->isCompatible(pin) && pin->isCompatible(pairPin);
                } else {
                    res = false;
                }
            }
            if (!res) break;
        }
    } else {
        res = false;
    }
    return res;
}

MVert* Socket3::getExtd()
{
    return nullptr;
}

MVert::TDir Socket3::getDir() const
{
    TDir res = ERegular;
    return res;
}

MVert* Socket3::GetPin(const string& aId)
{
    MVert* res = nullptr;
    auto owdCp = ownerCp()->pairAt(aId);
    MOwned* owd = owdCp ? owdCp->provided() : nullptr;
    return owd ? owd->lIft<MVert>() : nullptr;
}

bool Socket3::bindPin(MSocket* aPair, const string& aPinId)
{
    bool res = false;
    MVert* pin = GetPin(aPinId);
    if (pin) {
        auto* pairPin = aPair->GetPin(aPinId);
        if (pin->isCompatible(pairPin) && pairPin->isCompatible(pin)) {
            if (!pin->isBound(pairPin)) {
                bool res = pin->bind(pairPin);
                if (!res) {
                    LOGN(EErr, "Failed binding pins [" + aPinId + "]");
                    res = pin->bind(pairPin);
                }
            }
        } else {
            LOGN(EErr, "Binding pins [" + aPinId + "] are incompatible");
        }
    } else {
        LOGN(EErr, "Failed getting pin [" + aPinId + "]");
    }
    return res;
}

bool Socket3::connectPin(MSocket* aPair, const string& aPinId)
{
    bool res = false;
    MVert* pin = GetPin(aPinId);
    if (pin) {
        auto* pairPin = aPair->GetPin(aPinId);
        if (pin->isCompatible(pairPin) && pairPin->isCompatible(pin)) {
            if (!pin->isConnected(pairPin)) {
                res = pin->connect(pairPin);
                if (!res) {
                    LOGN(EErr, "Failed connecting pins [" + aPinId + "]");
                    res = pin->connect(pairPin);
                }
            } else {
                LOGN(EErr, "Connecting pins [" + aPinId + "]: already connected");
            }
        } else {
            LOGN(EErr, "Connecting pins [" + aPinId + "] are incompatible");
        }
    } else {
        LOGN(EErr, "Failed getting pin [" + aPinId + "]");
    }
    return res;
}

bool Socket3::bindPins(MSocket* aPair)
{
    bool res = true;
    for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd() && res; it++) {
        MOwned* comp = (*it)->provided();
        string pinId = comp->ownedId();
        res = bindPin(aPair, pinId);
    }
    return res;
}

bool Socket3::connectPins(MSocket* aPair)
{
    bool res = true;
    for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd() && res; it++) {
        MOwned* comp = (*it)->provided();
        string pinId = comp->ownedId();
        if (!GetPin(pinId)) continue;
        res = connectPin(aPair, pinId);
    }
    return res;
}

void Socket3::onConnected(MVert* aPair)
{
    Verte::onConnected(aPair);
    MSocket* sock = aPair->lIf(sock);
    bool res = connectPins(sock);
    if (!res) {
        LOGN(EErr, "Failed connected pins, pair [" + aPair->Uid() + "]");
    }
}

void Socket3::onDisconnecting(MVert* aPair)
{
    Verte::onDisconnecting(aPair);
    MSocket* sock = aPair->lIf(sock);
    // TODO Analyze why -sock- is null when disconnect from node destructor
    if (sock) {
//        bindPins(sock, true, true);
    }
}

void Socket3::onDisconnected()
{
    Verte::onDisconnected();
}

bool Socket3::isBound(const MVert* aPair) const
{
    return (mEbp.mPair != nullptr);
}

bool Socket3::connect(MVert* aPair)
{
    bool res = true;
    // Disable connecting "pure" socket
    /*
    if (const_cast<MVert*>(aPair)->getExtd()) {
        res = Verte::connect(aPair);
    } else {
        LOGN(EErr, "Connecting -pure- socket is not allowed.");
        res = false;
    }
    */
    res = Verte::connect(aPair);
    return res;
}

bool Socket3::bind(MVert* aPair)
{
    auto* pairs = aPair->lIft<MSocket>();
    bool res = bindPins(pairs);
    if (res) {
        res = mEbp.connect(pairs->bP());
        if (res) {
            aPair->onBound(this);
            onBound(aPair);
        }
    }
    return res;
}

bool Socket3::unbind(MVert* aPair)
{
    assert(false);
    return false;
}

void Socket3::onOwnedAttached(MOwned* aOwned)
{
    bool res = false;
    Verte::onOwnedAttached(aOwned);
    auto* owdv = aOwned->lIft<MVert>();
    /*
    if (owdv && aOwned->ownedId() == KUriInt) {
        bind(owdv);
    }
    */
    auto* bPeer = mEbp.mPair ? mEbp.mPair->provided() : nullptr;
    if (bPeer) {
        auto* bPeers = bPeer->lIft<MSocket>();
        if (bPeers) {
            res = bPeers->createBindPeer(owdv, aOwned->ownedId());
            if (!res) {
                LOGN(EErr, "Failed creating pin peer in extd int node: " + aOwned->ownedId());
            }
        }
    }
}

int Socket3::contCount() const
{
    return Verte::contCount() + 1;
}

bool Socket3::getContentId(int aIdx, string& aRes) const
{
    bool res = true;
    if (aIdx < contCount()) {
        if (aIdx < Verte::contCount()) {
            Verte::getContentId(aIdx, aRes);
        } else {
            aRes = KContBPeer;
        }
    } else {
        res = false;
    }
    return res;
}

bool Socket3::getContent(const string& aId, string& aRes) const
{
    bool res = true;
    if (aId == KContBPeer) {
        aRes = mCntBPeer;
    } else {
	res = Verte::getContent(aId, aRes);
    }
    return res;
}

bool Socket3::setContent(const string& aId, const string& aData)
{
    bool res = true;
    if (aId == KContBPeer) {
        mCntBPeer = aData;
	res = true;
    } else {
	res = Verte::setContent(aId, aData);
    }
    return res;
}
 
bool Socket3::createBindPeer(MVert* aPeer, const string& aName)
{
    bool res = false;
    string bPeerPname;
    auto* peercp = aPeer->lIft<MConnPoint>();
    if (peercp) {
        // Connpoint
        bPeerPname = peercp->bpeerPname();
    } else {
        auto peers = aPeer->lIft<MSocket>();
        if (peers) {
            // Socket with pins
            /*
            auto peerco = aPeer->lIft<MContentOwner>();
            if (peerco) {
                bool cres = peerco->getContent(KContBPeer, bPeerPname);
                if (!cres || bPeerPname.empty()) {
                    // Empty socket
                    bPeerPname = Socket3ExtdInt::idStr();
                }
            }
            */
            bPeerPname = Socket3ExtdInt::idStr();
        }
    }
    if (!bPeerPname.empty()) {
        auto* pinn = addComp(bPeerPname, aName);
        auto* pinv = pinn->lIft<MVert>();
        if (pinv) {
            res = pinv->bind(aPeer);
        } else {
            LOGN(EErr, "Failed getting pin/pair MVert: " + aName);
        }
    } else {
        LOGN(EErr, "Couldn't acquire binding peer parent name for pin: " + aName);
    }
    return res;
}


void Socket3::onBound(MVert* aPair)
{
    auto* pairs = aPair->lIft<MSocket>();
    if (pairs) {
        createBindPinsPeers(aPair);
    }
}

bool Socket3::createBindPinsPeers(MVert* aPair)
{
    bool res = true;
    auto* pairs = aPair->lIft<MSocket>();
    if (pairs) {
        for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd() && res; it++) {
            MOwned* comp = (*it)->provided();
            string pinId = comp->ownedId();
            if (!GetPin(pinId)) continue;
            auto* compv = comp->lIft<MVert>();
            res = pairs->createBindPeer(compv, pinId);
            if (!res) {
                LOGN(EErr, "Failed creating pin peer in pair: " + pinId);
            }
        }
    } else {
        LOGN(EErr, "Failed creating pin peer in pair, pair isn't socket: " + aPair->Uid());
    }
    return res;
}





//// Socket3Extd

const string Socket3Extd::KUriInt = "Int";  /*!< Internal connpoint */

vector<GUri> Socket3Extd::getParentsUri()
{
    auto p = Socket3::getParentsUri();
    p.insert(p.begin(), string(idStr()));
    return p;
}

Socket3Extd::Socket3Extd(const string &aType, const string& aName, MEnv* aEnv): Socket3(aType, aName, aEnv)
{}

void Socket3Extd::Construct()
{
    /*
    auto* mInt = addComp(Socket3::idStr(), KUriInt);
    assert(mInt);
    auto intv = mInt->lIft<MVert>();
    bool res = intv->bind(this);
    assert(res);
    */

    // Create Int
    mInt = new Socket3ExtdInt(string(Socket3ExtdInt::idStr()), KUriInt, mEnv);
    assert(mInt);
    bool res = attachOwned(mInt);
    assert(res);
    res = mInt->bind(this);
    assert(res);

}

MVert::TDir Socket3Extd::getDir() const
{
    TDir res = ERegular;
    return res;
}


void Socket3Extd::onOwnedAttached(MOwned* aOwned)
{
    if (aOwned->ownedId() != KUriInt) {
        Socket3::onOwnedAttached(aOwned);
#if 0
        auto* owdv = aOwned->lIft<MVert>();
        if (owdv) {
            if (mInt) {
                mInt->createBindPeer(owdv, aOwned->ownedId());
            } else {
                LOGN(EErr, "Missing [Int]");
            }
            /*
               auto* owdn = aOwned->lIft<MNode>();
               assert(owdn);
               auto* owdcp = owdn->lIft<MConnPoint>();
               auto ipName = owdcp->parentOfBpeer();
               auto* bpeer = addComp(ipName, aOwned->ownedId());
               assert(bpeer);
               */
        } else {
            LOGN(EErr, "Adding non-connpoint pin into Socket extender: " + aOwned->ownedId());
        }
#endif
    }
}

bool Socket3Extd::bindPins(MSocket* aPair)
{
    bool res = true;
    for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd() && res; it++) {
        MOwned* comp = (*it)->provided();
        string pinId = comp->ownedId();
        if (pinId == KUriInt) continue;
        res = bindPin(aPair, pinId);
    }
    return res;
}

MVert* Socket3Extd::GetPin(const string& aId)
{
    MVert* res = nullptr;
    if (aId != KUriInt) {
        auto owdCp = ownerCp()->pairAt(aId);
        MOwned* owd = owdCp ? owdCp->provided() : nullptr;
        res = owd ? owd->lIft<MVert>() : nullptr;
    }
    return res;
}

MVert* Socket3Extd::getExtd()
{
    return mInt;
}




//// Socket3ExtdInt


vector<GUri> Socket3ExtdInt::getParentsUri()
{
    auto p = Socket3::getParentsUri();
    p.insert(p.begin(), string(idStr()));
    return p;
}

Socket3ExtdInt::Socket3ExtdInt(const string &aType, const string& aName, MEnv* aEnv): Socket3(aType, aName, aEnv)
{}


void Socket3ExtdInt::onOwnedAttached(MOwned* aOwned)
{
    Verte::onOwnedAttached(aOwned);
}

void Socket3ExtdInt::onBound(MVert* aPair)
{
    Verte::onBound(aPair);
}









// Extd


const string Extd::KUriInt = "Int";  /*!< Internal connpoint */



Extd::Extd(const string &aType, const string& aName, MEnv* aEnv): Vert(aType, aName, aEnv)
{
}

MIface* Extd::MOwner_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else res = Vert::MOwner_getLif(aId);
    return res;
}


bool Extd::isCompatible(const MVert* aPair, bool aExt) const
{
    bool res = false;
    auto self = const_cast<Extd*>(this);
    MVert* intcp = self->getExtd();
    if (intcp) {
        res = intcp->isCompatible(aPair, !aExt);
    }
    return res;
}

MVert* Extd::getExtd()
{
    MVert* res = nullptr;
    MNode* extn = getComp(KUriInt);
    res = extn ? extn->lIf(res) : nullptr;
    return res;
}

MVert::TDir Extd::getDir() const
{
    TDir res = ERegular;
    return res;
}


