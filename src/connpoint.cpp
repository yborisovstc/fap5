
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


MIface* Socket::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMSocket));
    else res = Verte::MVert_getLif(aId);
    return res;
}


bool Socket::isCompatible(const MVert* aPair, bool aExt) const
{
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
        const MSocket* pairSocket = aPair->lIf(pairSocket);
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

string Socket::GetPinId(int aInd)
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
    MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    return owd ? owd->lIft<MVert>() : nullptr;
}

MVert* Socket::GetPin(const string& aId)
{
    MNode* pinn = getNode(aId);
    return pinn ? pinn->lIft<MVert>() : nullptr;
}

string Socket::GetPinId(int aInd)
{
    MOwned* owd = ownerCp()->pairAt(aInd)->provided();
    return owd->ownedId();
}

void Socket::bindPins(bool aUnbind)
{
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        auto pair = *it;
        auto pairSocket = pair->lIft<MSocket>();
        // Bind pins mutually
        for (int i = 0; i < PinsCount(); i++) {
            auto pin = GetPin(i);
            auto pairPin = pairSocket->GetPin(GetPinId(i));
            if (pin->isCompatible(pairPin)/* && pairPin->isCompatible(pin)*/) {
                if (aUnbind) {
                    bool res = pin->unbind(pairPin)/* && pairPin->unbind()*/;
                    if (!res) {
                        LOGN(EErr, "Filed unbinding pins [" + GetPinId(i) + "]");
                    }
                } else {
                    bool res = pin->bind(pairPin)/* && pairPin->bind(pin)*/;
                    if (!res) {
                        LOGN(EErr, "Filed binding pins [" + GetPinId(i) + "]");
                    }
                }
            } else {
                LOGN(EErr, "Pins [" + GetPinId(i) + "] are incompatible");
            }
        }
    }
}


void Socket::onConnected()
{
    bindPins();
}

void Socket::onDisconnecting(MVert* aPair)
{
    bindPins(true);
}

void Socket::onDisconnected()
{
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


