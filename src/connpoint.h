#ifndef __FAP5_CONNPOINT_H
#define __FAP5_CONNPOINT_H

#include "mconnpoint.h"
#include "vert.h"
#include "node.h"
#include "elem.h"

template <class Provided, class Required>
class ConnPoint : public Vert
{
    public:
        using TProvided = Provided;
        using TRequired = Required;
    public:
        /*
           void setProvided(TProvided* aProvided) {
           assert(mProvided == nullptr);
           mProvided = aProvided;
           }
           */
    public:
        //ConnPoint(const string &aType, const string &aName, MEnv* aEnv);
        ConnPoint(const string &aType, const string &aName, MEnv* aEnv): Vert(aType, aName, aEnv) {}
        virtual ~ConnPoint() {}
        // From MNode
        MIface *MNode_getLif(TIdHash aId) override;
        MIface *MOwned_getLif(TIdHash aId) override {
            MIface* res = nullptr;
            if (res = checkLif2(aId, mMVert));
            else res = Vert::MOwned_getLif(aId);
            return res;
        }
        // From MVert
        MIface *MVert_getLif(TIdHash aId) override;
        bool isCompatible(const MVert* aPair, bool aExt = false) const override;
        MVert* getExtd() override;
        TDir getDir() const override { return ERegular;}
        bool bind(MVert* aBound) override {
            auto* bound = aBound->lIft<TProvided>();
	    assert(mProvidedPx == nullptr);
            bool res = bound ? mProvidedPx = bound, true : false;
            if (res) onBound();
            return res;
        }
        bool unbind(MVert* aBound) override {
	    auto* bound = aBound->lIft<TProvided>();
	    assert(mProvidedPx == bound);
            bool res = mProvidedPx ? mProvidedPx = nullptr, true : false;
            if (res) onUnbound();
            return res;
        }
	bool isBound(const MVert* aBound) const override {
	    const auto* bound = const_cast<MVert*>(aBound)->lIft<TProvided>();
	    return bound && (mProvidedPx == bound);
	}
    protected:
        TProvided* mProvidedPx = nullptr;
    private:
        MVert* mMVert = nullptr;
        TRequired* mRequired = nullptr;
        TProvided* mProvided = nullptr;
};

    template <class Provided, class Required>
MIface* ConnPoint<Provided, Required>::MNode_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLifn(aId, mProvidedPx));    // Keep binded as priority
    else if (res = checkLif2(aId, mProvided));
    else res = Node::MNode_getLif(aId);
    return res;
}

    template <class Provided, class Required>
MIface* ConnPoint<Provided, Required>::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLifn(aId, mProvidedPx));    // Keep binded as priority
    else if (res = checkLif2(aId, mProvided));
    else if (res = checkLif2(aId, mRequired)); // Needs for CPs binding
    return res;
}

template <class Provided, class Required>
bool ConnPoint<Provided, Required>::isCompatible(const MVert* aPair, bool aExt) const
{
    const TRequired* pRif = aPair->lIf(pRif);
    return (pRif != nullptr);
}

    template <class Provided, class Required>
MVert* ConnPoint<Provided, Required>::getExtd()
{
    return nullptr;
}




/** @brief Socket, monolitic.
*/
class Socket: public Vert, public MSocket
{
    public:
        inline static constexpr std::string_view idStr() { return "Socket"sv;}
        Socket(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        // From MNode
        MIface* MNode_getLif(TIdHash aId) override;
        //string parentName() const override { return Type(); }
        // From MVert
        MIface* MVert_getLif(TIdHash aId) override;
        bool isCompatible(const MVert* aPair, bool aExt = false) const override;
        MVert* getExtd() override;
        TDir getDir() const override;
        // From MSocket
        string MSocket_Uid() const override  { return getUid<MSocket>();}
        int PinsCount() const override;
        MVert* GetPin(int aInd) override;
        MVert* GetPin(const string& aId) override;
        string GetPinId(int aInd) override;
    protected:
        // From Vert
        void onConnected() override;
        void onDisconnecting(MVert* aPair) override;
        void onDisconnected() override;
    protected:
        void bindPins(bool aUnbind = false);
    protected:
        MVert* mMVert = nullptr;
        MSocket* mMSocket = nullptr;
};




#endif
