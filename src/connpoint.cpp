
#include "connpoint.h"

//// ConnPoint

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


//// Socket

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


#if 0
bool Socket::isCompatible(const MVert* aPair, bool aExt) const
{
    Socket* self = const_cast<Socket*>(this);
    bool res = true;
    // Going thru connectable components and check their compatibility
    bool ext = aExt;
    const MVert *cp = aPair;
    MVert* ecp = const_cast<MVert*>(aPair)->getExtd(); 
    // Checking if the pair is Extender
    if (ecp) {
	ext = !ext;
	cp = ecp;
    }
    if (cp) {
	// TODO MNode shall not be resolved thru MVert. This is MVert resolution issue
        const MSocket* pairSocket = cp->lIf(pairSocket);
        if (pairSocket) {
            /*
            for (auto it = ownerCp()->pairsCBegin(); it != ownerCp()->pairsCEnd() && res; it++) {
                auto pin = (*it)->provided();
                const MVert* pinv = pin ? pin->lIf(pinv) : nullptr;
                if (pinv) {
                    MVert* ppinv = const_cast<MSocket*>(pairSocket)->GetPin(pin->ownedId());
                    if (ppinv) {
                        res = ppinv->isCompatible(pinv, ext) && pinv->isCompatible(ppinv, ext);
                    } else {
                        res = false;
                    }
                }
            }
            */
            for (int i = 0; i < PinsCount(); i++) {
                auto pin = self->GetPin(i);
                // In this implementation pin can be null
                if (pin) {
                    MVert* pairPin = const_cast<MSocket*>(pairSocket)->GetPin(GetPinId(i));
                    auto* pinEx = pin->getExtd();
                    auto* pairPinEx = pairPin->getExtd();
                    auto* pinP = pinEx ? pinEx : pin;
                    auto* pairPinP = pairPinEx ? pairPinEx : pairPin;
                    if (pinP && pairPinP) {
                        res = pairPinP->isCompatible(pinP, ext) && pinP->isCompatible(pairPinP, ext);
                    } else {
                        res = false;
                    }
                }
                if (!res) break;
            }
        } else {
            res = false;
        }
    }
    return res;
}
#endif


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
                if (pin && pairPin) {
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
    int res = 0;
    for (auto it = ownerCp()->pairsCBegin(); it != ownerCp()->pairsCEnd(); it++) {
	const MOwned* comp = (*it)->provided();
	const MVert* compv = comp->lIf(compv);
	if (compv) res++;
    }
    return res;
}

MVert* Socket::GetPin(int aInd)
{
    MVert* res = nullptr;
    int idx = -1;
    for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd(); it++) {
	MOwned* comp = (*it)->provided();
	MVert* compv = comp->lIf(compv);
	if (compv) idx++;
        if (idx == aInd) {
            res = compv; break;
        }
    }
    return res;
}

MVert* Socket::GetPin(const string& aId)
{
    MNode* pinn = getNode(aId);
    return pinn->lIft<MVert>();
}

string Socket::GetPinId(int aInd) const
{
    string res;
    int idx = -1;
    for (auto it = ownerCp()->pairsBegin(); it != ownerCp()->pairsEnd(); it++) {
	MOwned* comp = (*it)->provided();
	MVert* compv = comp->lIf(compv);
	if (compv) idx++;
        if (idx == aInd) {
            res = comp->ownedId(); break;
        }
    }
    return res;
}
*/
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

MVert* Socket::GetPin(const string& aId)
{
    MVert* res = nullptr;
    auto owdCp = ownerCp()->pairAt(aId);
    MOwned* owd = owdCp ? owdCp->provided() : nullptr;
    return checkPin(owd);
}

string Socket::GetPinId(int aInd) const
{
    assert(false);
    const MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    return owd->ownedId();
}

#if 0
// TODO return res?
void Socket::bindPins(MSocket* aPair, bool aConnect, bool aUndo)
{
    for (int i = 0; i < PinsCount(); i++) {
        auto pin = GetPin(i);
        if (pin == nullptr) continue;
        MSocket* pinS = pin->lIft<MSocket>();
        auto pairPin = aPair->GetPin(GetPinId(i));
        MSocket* pairPinS = pairPin->lIft<MSocket>();
        auto* pinEx = pin->getExtd();
        auto* pairPinEx = pairPin->getExtd();
        auto* pinP = pinEx ? pinEx : pin;
        auto* pairPinP = pairPinEx ? pairPinEx : pairPin;
        if (pinP && pairPinP) {
            if (pinP->isCompatible(pairPinP)/* && pairPin->isCompatible(pin)*/) {
                if (aConnect) {
                    if (pinEx || pairPinEx || pinS && pairPinS) {
                        if (aUndo) {
                            bool res = pinP->disconnect(pairPinP);
                            if (!res) {
                                LOGN(EErr, "Failed disconnecting pin [" + GetPinId(i) + "]");
                            }
                        } else {
                            bool res = pinP->connect(pairPinP);
                            if (!res) {
                                LOGN(EErr, "Failed connecting pin [" + GetPinId(i) + "]");
                            }
                        }
                    } else {
                        LOGN(EErr, "At least one connecting socket pin has to be extd [" + GetPinId(i) + "]");
                    }
                } else {
                    if (!pinEx && !pairPinEx) {
                        if (aUndo) {
                            if (pinP->isBound(pairPinP)) {
                                bool res = pinP->unbind(pairPinP);
                                if (!res) {
                                    LOGN(EErr, "Failed unbinding pins [" + GetPinId(i) + "]");
                                }
                            }
                        } else {
                            if (!pinP->isBound(pairPinP)) {
                                bool res = pinP->bind(pairPinP);
                                if (!res) {
                                    LOGN(EErr, "Failed binding pins [" + GetPinId(i) + "]");
                                }
                            }
                        }
                    } else {
                        LOGN(EErr, "Binded socket pin has to be not extd [" + GetPinId(i) + "]");
                    }
                }
            } else {
                LOGN(EErr, "Pin [" + GetPinId(i) + "] is incompatible");
                pinP->isCompatible(pairPinP);
            } 
        } else {
            LOGN(EErr, "Pin [" + GetPinId(i) + "] isn't valid");
        }
    }
}
#endif

MVert* Socket::checkPin(MOwned* aOwned)
{
    MVert* res = nullptr;
    MVert* owdv = aOwned->lIft<MVert>();
    MSocket* owds = aOwned->lIft<MSocket>();
    if (owds) {
        res = owdv;
    } else {
        res = owdv ? owdv->getExtd() : nullptr;
    }
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
                LOGN(EErr, "Pin [" + pinId + "] is incompatible");
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
    //MVert* ecp = aPair->getExtd(); 
    MVert* ecp = nullptr;  // We connect to extd as to socket, i.e. binding pins
    // Checking if the pair is Extender
    if (ecp) {
        // Extended socket, connect pins
        MSocket* esock = ecp->lIf(esock);
        bindPins(esock, true);
    } else {
        // Direct socket, connect pins
        MSocket* sock = aPair->lIf(sock);
        bindPins(sock, true);
    }
}

void Socket::onDisconnecting(MVert* aPair)
{
    Verte::onDisconnecting(aPair);
    MVert* ecp = aPair->getExtd(); 
    // Checking if the pair is Extender
    if (ecp) {
        // Extended socket, connect pins
        MSocket* esock = ecp->lIf(esock);
        bindPins(esock, true, true);
    } else {
        // Direct socket, disconnect pins
        MSocket* sock = aPair->lIf(sock);
        bindPins(sock, true, true);
    }
}

void Socket::onDisconnected()
{
    Verte::onDisconnected();
}

// TODO desing option w/o binding point, consider bp option also
/*
bool Socket::isBound(const MVert* aPair) const
{
    bool res = false;
    auto* self = const_cast<Socket*>(this);
    if (PinsCount()) {
        auto* pin = self->GetPin(self->GetPinId(0));
        auto* pairPinSc = aPair->lIft<const MSocket>();
        auto* pairPinS = const_cast<MSocket*>(pairPinSc);
        auto pairPin = pairPinS->GetPin(pairPinSc->GetPinId(0));
        res = pin->isBound(pairPin);
    }
    return res;
}*/


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

SocketExtd::SocketExtd(const string &aType, const string& aName, MEnv* aEnv): Socket(aType, aName, aEnv)
{
}

void SocketExtd::onOwnedAttached(MOwned* aOwned)
{
    if (mName == "S2_Pin3") {
        LOGN(EErr, "onOwnedAttached");
    }
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


