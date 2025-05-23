
#include "vert.h"



MIface* Vert::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else res = Node::MNode_getLif(aId);
    return res;
}

MIface* Vert::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    res = checkLif2(aId, mMVert);
    return res;
}

void Vert::MVert_doDump(int aLevel, int aIdt, ostream& aOs) const
{
}

bool Vert::isCompatible(const MVert* aPair, bool aExt) const
{
    return true;
}

MVert* Vert::getExtd()
{
    return nullptr;
}

MVert::TDir Vert::getDir() const
{
    return ERegular;
}

int Vert::pairsCount() const
{
    return mPairs.size();
}

MVert* Vert::getPair(int aInd) const
{
    MVert* res = nullptr;
    if (aInd < mPairs.size()) {
        res = mPairs.at(aInd);
    }
    return res;
}

bool Vert::isConnected(const MVert* aPair) const
{
    bool res = false;
    for (auto pair : mPairs) {
        if (pair == aPair) {
            res = true; break;
        }
    }
    return res;
}

bool Vert::connect(MVert* aPair)
{
    assert(aPair && !isConnected(aPair));
    bool res = isCompatible(aPair, false);
    if (res) {
        mPairs.push_back(aPair);
        onConnected();
        res = true;
    }
    return res;
}

bool Vert::disconnect(MVert* aPair)
{
    bool res = false;
    assert(aPair && isConnected(aPair));
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        if (*it == aPair) {
            onDisconnecting(aPair);
            mPairs.erase(it);
            onDisconnected();
            res = true;
            break;
        }
    }
    return res;
}

void Vert::vertGetUri(GUri& aUri, const MNode* aBase) const
{
    return getUri(aUri, aBase);
}

void Vert::onConnected()
{
    notifyChanged();
}

void Vert::onDisconnected()
{
    notifyChanged();
}


/// Verte


MIface* Verte::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else res = Node::MNode_getLif(aId);
    return res;
}

MIface* Verte::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    res = checkLif2(aId, mMVert);
    return res;
}

void Verte::MVert_doDump(int aLevel, int aIdt, ostream& aOs) const
{
}

bool Verte::isCompatible(const MVert* aPair, bool aExt) const
{
    return true;
}

MVert* Verte::getExtd()
{
    return nullptr;
}

MVert::TDir Verte::getDir() const
{
    return ERegular;
}

int Verte::pairsCount() const
{
    return mPairs.size();
}

MVert* Verte::getPair(int aInd) const
{
    MVert* res = nullptr;
    if (aInd < mPairs.size()) {
        res = mPairs.at(aInd);
    }
    return res;
}

bool Verte::isConnected(const MVert* aPair) const
{
    bool res = false;
    for (auto pair : mPairs) {
        if (pair == aPair) {
            res = true; break;
        }
    }
    return res;
}

bool Verte::connect(MVert* aPair)
{
    assert(aPair && !isConnected(aPair));
    bool res = isCompatible(aPair, false);
    if (res) {
        mPairs.push_back(aPair);
        onConnected();
        res = true;
    }
    return res;
}

bool Verte::disconnect(MVert* aPair)
{
    bool res = false;
    assert(aPair && isConnected(aPair));
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        if (*it == aPair) {
            onDisconnecting(aPair);
            mPairs.erase(it);
            onDisconnected();
            res = true;
            break;
        }
    }
    return res;
}

void Verte::vertGetUri(GUri& aUri, const MNode* aBase) const
{
    return getUri(aUri, aBase);
}

void Verte::onConnected()
{
    notifyChanged();
}

void Verte::onDisconnected()
{
    notifyChanged();
}
