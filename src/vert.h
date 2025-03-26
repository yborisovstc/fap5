#ifndef __FAP5_VERT_H
#define __FAP5_VERT_H

#include "mvert.h"
#include "node.h"

class Vert : public Node, public MVert
{
    public:
	using TPairs = vector<MVert*>;
    public:
	inline static constexpr std::string_view idStr() { return "Vert"sv;}
        Vert(const string &aType, const string &aName, MEnv* aEnv): Node(aType, aName, aEnv) {}
        // From MNode
        MIface *MNode_getLif(TIdHash aId) override;
        // From MVert
        string MVert_Uid() const override { return getUid<MVert>();}
        MIface *MVert_getLif(TIdHash aId) override;
        void MVert_doDump(int aLevel, int aIdt, ostream& aOs) const override;
        bool isCompatible(const MVert* aPair, bool aExt = false) const override;
        MVert* getExtd() override;
        TDir getDir() const override;
        int pairsCount() const override;
        MVert* getPair(int aInd) const override;
	bool connect(MVert* aPair) override;
	bool disconnect(MVert* aPair) override;
	bool isConnected(const MVert* aPair) const override;
        bool bind(MIface* aBound) override { return false; }
        bool unbind(MIface* aBound) override { return false; }
	bool isBound(const MIface* aBound) const override { return false;}
    protected:
        // Local
	virtual void onConnected() {}
	virtual void onDisconnecting(MVert* aPair) {}
	virtual void onDisconnected() {}
	virtual void onBound() {}
	virtual void onUnbinding() {}
	virtual void onUnbound() {}
    public:
        TPairs mPairs;
    private:
	MVert* mMVert = nullptr;
};



#endif
