#ifndef __FAP5_VERT_H
#define __FAP5_VERT_H

#include "mvert.h"
#include "node.h"
#include "elem.h"

class Vert : public Node, public MVert
{
    public:
	using TPairs = vector<MVert*>;
    public:
	inline static constexpr std::string_view idStr() { return "Vert"sv;}
        static vector<GUri> getParentsUri();
        Vert(const string &aType, const string &aName, MEnv* aEnv): Node(aType, aName, aEnv) {}
        virtual ~Vert();
        // From MNode
        MIface *MNode_getLif(TIdHash aId) override;
	GUri parentUri() const override { return string(idStr());}
        vector<GUri> parentsUris() const override { return getParentsUri(); }
        // From MOwned
	void deleteOwned() override;
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
	void vertGetUri(GUri& aUri, const MNode* aBase = nullptr) const override;
	bool isBound(const MVert* aBound) const override { return false; }
        bool bind(MVert* aPair) override { return false; }
        bool unbind(MVert* aPair) override { return false; }
	void onBound(MVert* aPair) override {}
	void onUnbinding(MVert* aPair) override {}
	void onUnbound(MVert* aPair) override {}
    protected:
        // Local
	virtual void onConnected(MVert* aPair);
	virtual void onDisconnecting(MVert* aPair) {}
	virtual void onDisconnected();
        void disconnect();
    public:
        TPairs mPairs;
    private:
	MVert* mMVert = nullptr;
};


class Verte : public Elem, public MVert
{
    public:
	using TPairs = vector<MVert*>;
    public:
	inline static constexpr std::string_view idStr() { return "Verte"sv;}
        static vector<GUri> getParentsUri();
        Verte(const string &aType, const string &aName, MEnv* aEnv): Elem(aType, aName, aEnv) {}
        virtual ~Verte();
        // From MNode
        MIface *MNode_getLif(TIdHash aId) override;
	GUri parentUri() const override { return string(idStr());}
        vector<GUri> parentsUris() const override { return getParentsUri(); }
        // From MOwned
	void deleteOwned() override;
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
	void vertGetUri(GUri& aUri, const MNode* aBase = nullptr) const override;
	bool isBound(const MVert* aBound) const override { return false; }
        bool bind(MVert* aPair) override { return false; }
        bool unbind(MVert* aPair) override { return false; }
	void onBound(MVert* aPair) override {}
	void onUnbinding(MVert* aPair) override {}
	void onUnbound(MVert* aPair) override {}
    protected:
        // Local
	virtual void onConnected(MVert* aPair);
	virtual void onDisconnecting(MVert* aPair) {}
	virtual void onDisconnected();
        void disconnect();
    public:
        TPairs mPairs;
    private:
	MVert* mMVert = nullptr;
};

#endif
