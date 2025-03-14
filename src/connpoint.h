#ifndef __FAP5_CONNPOINT_H
#define __FAP5_CONNPOINT_H

#include "mvert.h"
#include "node.h"

template <class Provided, class Required>
class ConnPoint : public Node, public MVert
{
    public:
        using TProvided = Provided;
        using TRequired = Required;
        using TPair = MVert;
	using TPairs = vector<TPair*>;
    public:
        void setProvided(TProvided* aProvided) {
            assert(mProvided == nullptr);
            mProvided = aProvided;
        }
    public:
        //ConnPoint(const string &aType, const string &aName, MEnv* aEnv);
        ConnPoint(const string &aType, const string &aName, MEnv* aEnv): Node(aType, aName, aEnv) {}
        virtual ~ConnPoint() {}
        // From MNode
        MIface *MNode_getLif(TIdHash aId) override;
        // From MVert
        string MVert_Uid() const override { return getUid<MVert>();}
        MIface *MVert_getLif(TIdHash aId) override;
        void MVert_doDump(int aLevel, int aIdt, ostream& aOs) const override;
        bool isCompatible(MVert* aPair, bool aExt = false) const override;
        MVert* getExtd() override;
        TDir getDir() const override;
        int pairsCount() const override;
        MVert* getPair(int aInd) const override;
	bool connect(MVert* aPair) override;
	bool disconnect(MVert* aPair) override;
	bool isConnected(const MVert* aPair) const override;
    protected:
        // Local
	virtual void onConnected() {}
	virtual void onDisconnected() {}
    public:
        TPairs mPairs;
    protected:
        TProvided* mProvided = nullptr;
    private:
	MVert* mMVert = nullptr;
};

template <class Provided, class Required>
MIface* ConnPoint<Provided, Required>::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLif2(aId, mProvided));
    else res = Node::MNode_getLif(aId);
    return res;
}

template <class Provided, class Required>
MIface* ConnPoint<Provided, Required>::MVert_getLif(TIdHash aId)
{
    MIface* res = checkLif2(aId, mProvided);
    return res;
}

template <class Provided, class Required>
void ConnPoint<Provided, Required>::MVert_doDump(int aLevel, int aIdt, ostream& aOs) const
{
}

template <class Provided, class Required>
bool ConnPoint<Provided, Required>::isCompatible(MVert* aPair, bool aExt) const
{
    TRequired* pRif = aPair->lIf(pRif);
    return (pRif != nullptr);
}

template <class Provided, class Required>
MVert* ConnPoint<Provided, Required>::getExtd()
{
    return nullptr;
}

template <class Provided, class Required>
MVert::TDir ConnPoint<Provided, Required>::getDir() const
{
    return ERegular;
}

template <class Provided, class Required>
int ConnPoint<Provided, Required>::pairsCount() const
{
    return mPairs.size();
}

template <class Provided, class Required>
MVert* ConnPoint<Provided, Required>::getPair(int aInd) const
{
    return nullptr;
}

template <class Provided, class Required>
bool ConnPoint<Provided, Required>::isConnected(const MVert* aPair) const
{
    bool res = false;
    for (auto pair : mPairs) {
        if (pair == aPair) {
            res = true; break;
        }
    }
    return res;
}

template <class Provided, class Required>
bool ConnPoint<Provided, Required>::connect(MVert* aPair)
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

    template <class Provided, class Required>
bool ConnPoint<Provided, Required>::disconnect(MVert* aPair)
{
    bool res = false;
    assert(aPair && isConnected(aPair));
    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
        if (*it == aPair) {
            mPairs.erase(it);
            onDisconnected();
            res = true;
            break;
        }
    }
    return res;
}


#endif
