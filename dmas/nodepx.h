#ifndef __FAP5_NODEPX_H__
#define __FAP5_NODEPX_H__

#include "daaproxy.h"
#include "mnode.h"

class NodePx : public DaaProxy, public MNode {
    public:
        NodePx(MEnv* aEnv, MProxyMgr* aMgr, const string& aContext);
        virtual ~NodePx();
    public:
        // From MProxy
        virtual string MProxy_Uid() const { return string();}
        // From MNode
        string MNode_Uid() const {return string();} // TODO TBD
        MIface* MNode_getLif(TIdHash aId) override;
	const string& name() const override;
	GUri parentUri() const override;
	vector<GUri> parentsUris() const override;
	MOwned* getOwned(const string& aId) const override;
	const MNode* getNode(const GUri& aUri) const override;
	MNode* getNode(const GUri& aUri) override;
	MNode* getNodeS(const char* aUri) override;
	MNode* getNode(const GUri& aUri, const TNs& aNs) override;
	void getUri(GUri& aUri, const MNode* aBase = nullptr) const override;
	string getUriS(MNode* aBase = nullptr) const { GUri uri; getUri(uri, aBase); return uri.toString();}
	void setCtx(MOwner* aContext) override;
	void mutate(const ChromoNode& aMut, bool aChange /*EFalse*/, const MutCtx& aCtx, bool aTreatAsChromo = false, bool aLocal = false) override;
	bool attachOwned(MNode* aOwned) override;
	TOwnerCp* ownerCp() override;
	const TOwnerCp* ownerCp() const override;
    protected:
        template<class T> inline MIface* checkLif(TIdHash aId, T*& aPtr) {
            return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
        }
    protected:
        MNode* mMNode = nullptr;
        string mName;
};

#endif
