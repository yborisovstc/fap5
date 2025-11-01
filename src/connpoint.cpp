
#include "connpoint.h"


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
                const MVert* pinv = pin ? pin->lIf(pinv) : nullptr;
                if (pinv) {
                    MVert* ppinv = const_cast<MSocket*>(pairSocket)->GetPin(GetPinId(i));
                    if (ppinv) {
                        res = ppinv->isCompatible(pinv, ext) && pinv->isCompatible(ppinv, ext);
                    } else {
                        res = false;
                    }
                }
            }
        } else {
            res = false;
        }
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
    MVert* res = nullptr;
    MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    res = owd ? owd->lIft<MVert>() : nullptr;
    return res;
}

MVert* Socket::GetPin(const string& aId)
{
    MNode* pinn = getNode(aId);
    return pinn ? pinn->lIft<MVert>() : nullptr;
}

string Socket::GetPinId(int aInd) const
{
    const MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    return owd->ownedId();
}

void Socket::bindPins(MSocket* aPair, bool aConnect, bool aUndo)
{
    for (int i = 0; i < PinsCount(); i++) {
        auto pin = GetPin(i);
        // Allow GetPin return null to not specialized PinsCount
        if (pin == nullptr) continue;
        auto pairPin = aPair->GetPin(GetPinId(i));
        if (pin->isCompatible(pairPin)/* && pairPin->isCompatible(pin)*/) {
            if (aConnect) {
                if (aUndo) {
                    bool res = pin->disconnect(pairPin);
                    if (!res) {
                        LOGN(EErr, "Failed disconnecting pin [" + GetPinId(i) + "]");
                    }
                } else {
                    bool res = pin->connect(pairPin);
                    if (!res) {
                        LOGN(EErr, "Failed connecting pin [" + GetPinId(i) + "]");
                    }
                }
            } else {
                if (aUndo) {
                    bool res = pin->unbind(pairPin)/* && pairPin->unbind()*/;
                    if (!res) {
                        LOGN(EErr, "Failed unbinding pins [" + GetPinId(i) + "]");
                    }
                } else {
                    bool res = pin->bind(pairPin)/* && pairPin->bind(pin)*/;
                    if (!res) {
                        LOGN(EErr, "Failed binding pins [" + GetPinId(i) + "]");
                    }
                }
            }
        } else {
            LOGN(EErr, "Pin [" + GetPinId(i) + "] is incompatible");
        }
    }
}

void Socket::onConnected(MVert* aPair)
{
    MVert* ecp = aPair->getExtd(); 
    // Checking if the pair is Extender
    if (ecp) {
        // Extended socket, connect pins
        MSocket* esock = ecp->lIf(esock);
        bindPins(esock, true);
    } else {
        // Direct socket, bind pins
        MSocket* sock = aPair->lIf(sock);
        bindPins(sock, false);
    }
}

void Socket::onDisconnecting(MVert* aPair)
{
    MVert* ecp = aPair->getExtd(); 
    // Checking if the pair is Extender
    if (ecp) {
        // Extended socket, connect pins
        MSocket* esock = ecp->lIf(esock);
        bindPins(esock, true, true);
    } else {
        // Direct socket, bind pins
        MSocket* sock = aPair->lIf(sock);
        bindPins(sock, false, true);
    }
}

void Socket::onDisconnected()
{
}


// SocketExtd 

const string SocketExtd::KUriInt = "Int";  /*!< Internal connpoint */

SocketExtd::SocketExtd(const string &aType, const string& aName, MEnv* aEnv): Socket(aType, aName, aEnv)
{
}

void SocketExtd::onOwnedAttached(MOwned* aOwned)
{
    Socket::onOwnedAttached(aOwned);
    MSocket* owds = aOwned->lIf(owds);
    if (owds && aOwned->ownedId() == KUriInt) {
        // Internal CP. Connect pins
        bindPins(owds, true);
    }
}
 
void SocketExtd::onOwnedDetached(MOwned* aOwned)
{
    Socket::onOwnedDetached(aOwned);
}

MVert* SocketExtd::GetPin(int aInd)
{
    MVert* res = nullptr;
    MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    if (owd->ownedId() != KUriInt) {
        res = owd ? owd->lIft<MVert>() : nullptr;
    }
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


