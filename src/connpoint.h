#ifndef __FAP5_CONNPOINT_H
#define __FAP5_CONNPOINT_H

#include "mconnpoint.h"
#include "vert.h"
#include "node.h"
#include "elem.h"

/** @brief Connection point in native treee.
 * In contrastof native connpoints (nconn.h) it is native tree node
 * so can be managed via model mutation.
 * Connpoint proxies provided iface, set via binding operation
 * */
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
        MIface *MOwned_getLif(TIdHash aId) override;
        // From MVert
        MIface *MVert_getLif(TIdHash aId) override;
        bool isCompatible(const MVert* aPair, bool aExt = false) const override;
        MVert* getExtd() override;
        TDir getDir() const override { return ERegular;}
        bool bind(MIface* aBound) override {
            auto* bound = aBound->lIft<TProvided>();
	    assert(mProvidedPx == nullptr);
            bool res = bound ? mProvidedPx = bound, true : false;
            if (res) onBound();
            return res;
        }
        bool unbind(MIface* aBound) override {
	    auto* bound = aBound->lIft<TProvided>();
	    assert(mProvidedPx == bound);
            bool res = mProvidedPx ? mProvidedPx = nullptr, true : false;
            if (res) onUnbound();
            return res;
        }
	bool isBound(const MIface* aBound) const override {
	    const auto* bound = const_cast<MIface*>(aBound)->lIft<TProvided>();
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
    // TODO to reuse MOwned resolver?
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLifn(aId, mProvidedPx));    // Keep binded as priority
    else if (res = checkLif2(aId, mProvided));
    else res = Node::MNode_getLif(aId);
    return res;
}

    template <class Provided, class Required>
MIface* ConnPoint<Provided, Required>::MOwned_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLif2(aId, mMVert));
    else if (res = checkLifn(aId, mProvidedPx));    // Keep binded as priority
    else if (res = checkLif2(aId, mProvided));
    else res = Node::MOwned_getLif(aId);
    return res;
}

    template <class Provided, class Required>
MIface* ConnPoint<Provided, Required>::MVert_getLif(TIdHash aId)
{
    MIface* res = nullptr;
    if (res = checkLifn(aId, mProvidedPx));    // Keep binded as priority
    else if (res = checkLif2(aId, mProvided));
    else if (res = checkLif2(aId, mRequired)); // Needs for CPs binding
    else res = Vert::MVert_getLif(aId);
    return res;
}

template <class Provided, class Required>
bool ConnPoint<Provided, Required>::isCompatible(const MVert* aPair, bool aExt) const
{
    const TRequired* pRif = aPair->lIf(pRif);
    const TProvided* pPif = aPair->lIf(pPif);
    return aExt ? (pPif != nullptr) : (pRif != nullptr);
}

    template <class Provided, class Required>
MVert* ConnPoint<Provided, Required>::getExtd()
{
    return nullptr;
}




/** @brief Socket, monolitic.
 * Note that socket pins shall be bindable, e.g. CpStateInpPin (not CpStateInp)
*/
class Socket: public Verte, public MSocket
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

/** @brief Generic extender of CP, ref ds_vbcr_extd_ep
 * It is "universal" but less effective that specialized extender
 * Extender doesnt act as iface implementor, i.e. doesn't propagate iface calls
 * So Internal CP has to be iface implementor itslef (i.e. be CpPin)
 * */
class Extd: public Vert
{
    public:
        inline static constexpr std::string_view idStr() { return "Extd"sv;}
	Extd(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        // From MOwner
        MIface *MOwner_getLif(TIdHash aId) override;
	// From MVert
	bool isCompatible(const MVert* aPair, bool aExt = false) const override;
	MVert* getExtd() override;
	TDir getDir() const override;
    public:
	static const string KUriInt;  /*!< Internal connpoint Uri */
    protected:
        MVert* mMVert = nullptr;
};





#endif
