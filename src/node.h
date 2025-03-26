#ifndef __FAP5_NODE_H
#define __FAP5_NODE_H

#include "mnode.h"
#include "mobserv.h"
#include "mowning.h"
#include "minh.h"
#include "menv.h"
#include "mecont.h"
#include "ifu.h"
#include "prof.h"


class Node : public MNode, public MObservable, public MOwner, public MOwned, public MParent, public MChild, public MContentOwner
{
    public:
	inline static std::string_view idStr() { return "Node"sv;}
	inline static TIdHash idHash() { return 0x183c6ea2097bf7bf;}

    protected:
	class NOwningNode : public NTnip<MOwned, MOwner> {
	    public:
		NOwningNode(MOwned* aProvPx, MOwner* aReqPx): NTnip<MOwned, MOwner>(aProvPx, aReqPx) {}
		// Note: we need to implement virt destructor here because clean-up method disconnectAll()
		// is virtual and uses other virt method. It virt destructor is not impl here the lower
		// level virt destr will be called and the that context virt methods (often stubs) will be called
		virtual ~NOwningNode() { disconnectAll();}
		virtual bool getId(string& aId) const override { aId = provided()->ownedId(); return true;}
		virtual typename TCnode::TPair* cnodeBinded() override { return nullptr;} // To disable navigating to top
	};

    protected:
	using TOwtNode = NOwningNode;                    /*!< Ownership tree node */ 
	using TObsCp = NCpOmnp<MObservable, MObserver>;  /*!< Observable connpoint */
	using TContents = map<string, string>;

    public:
	Node(const string &aType, const string &aName, MEnv* aEnv);
	virtual ~Node();
	// From MNode
	void Construct() override {}
	MIface* MNode_getLif(TIdHash aTid) override;
	string MNode_Uid() const override { return getUid<MNode>();}
	void MNode_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	const string& name() const override { return mName;}
	const MNode* getNode(const GUri& aUri) const override { return const_cast<const MNode*>(const_cast<Node*>(this)->getNode(aUri));}
	MNode* getNode(const GUri& aUri) override;
	MNode* getNodeS(const char* aUri) override { return getNode(aUri);}
	MNode* getNode(const GUri& aUri, const TNs& aNs) override;
	void getUri(GUri& aUri, const MNode* aBase = nullptr) const override;
	GUri parentUri() const override { return string(idStr());}
	void setCtx(MOwner* aContext) override;
	void mutate(const ChromoNode& aMut, bool aChange /*EFalse*/, const MutCtx& aCtx, bool aTreatAsChromo = false, bool aLocal = false) override;
	vector<GUri> parentsUris() const override { return vector(1, parentUri()); }
	TOwnerCp* ownerCp() override { return mOwsNode.binded();}
	const TOwnerCp* ownerCp() const override { return const_cast<const TOwnerCp*>(const_cast<Node*>(this)->mOwsNode.binded());}
	// From Owned
	string MOwned_Uid() const {return getUid<MOwned>();}
	MIface* MOwned_getLif(TIdHash aId) override;
	string ownedId() const override { return name();}
	void deleteOwned() override { delete this;}
	void onOwnerAttached() override;
	void onOwnerDetached() override {}
	MOwner* asOwner() override { MOwner* res = MOwned::lIf(res); return res; }
	const MOwner* asOwner() const override { const MOwner* res = MOwned::lIf(res); return res; }
	TOwnedCp* ownedCp() override { return &mOwsNode;}
	const TOwnedCp* ownedCp() const override { return &mOwsNode;}
	// From MOwner
	string MOwner_Uid() const {return getUid<MOwner>();}
	MIface* MOwner_getLif(TIdHash aId) override;
	void ownerGetUri(GUri& aUri, const MOwner* aBase = nullptr) const override;
	bool attachOwned(MOwned* aOwned) override;
	//MNode* ownerGetNode(const GUri& aUri, const MNode* aReq) const override;
	MOwned* getOwned(const GUri& aUri, const MOwned* aReq) const override;
	MOwned* getOwned(const string& aId) const override;
	void onOwnedMutated(const MOwned* aOwned, const ChromoNode& aMut, const MutCtx& aCtx) override;
	void onOwnedAttached(MOwned* aOwned) override;
	void onOwnedDetached(MOwned* aOwned) override;
	//void getModules(vector<MNode*>& aModules) override;
	bool isOwned(const MOwned* mOwned) const override;
	MNode* getParent(const GUri& aUri);
	// From MObservable
	string MObservable_Uid() const override { return getUid<MObservable>();}
	MIface* MObservable_getLif(TIdHash aId) override {return nullptr;}
	void MObservable_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	bool addObserver(MObserver::TCp* aObs) override;
	bool rmObserver(MObserver::TCp* aObs) override;
	// From MParent
	string MParent_Uid() const override {return getUid<MParent>();}
	MIface* MParent_getLif(TIdHash aId) override { return nullptr;}
	void MParent_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	void onChildDeleting(MChild* aChild) override {}
	bool onChildRenaming(MChild* aChild, const string& aNewName) override { return false;}
	MNode* createHeir(const string& aName) override;
	bool attachChild(MChild* aChild) override { return false;}
	void parentGetUri(GUri& aUri) const override;
        vector<GUri> parentsUri() const override { GUri uri; parentGetUri(uri); return vector(1, uri);}
	// From MChild
	string MChild_Uid() const override {return getUid<MChild>();}
	MIface* MChild_getLif(TIdHash aId) override { return nullptr;}
	void MChild_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	MParent* parent() override;
	const MParent* parent() const override;
	void onParentDeleting(MParent* aParent) override {}
	TCp* cP() override {return nullptr;}
	// From MContentOwner
	virtual string MContentOwner_Uid() const override { return getUid<MContentOwner>();}
	virtual MIface* MContentOwner_getLif(TIdHash aId) override { return nullptr;}
	virtual void MContentOwner_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	int contCount() const override;
	bool getContentId(int aIdx, string& aRes) const override;
	bool getContent(const string& aId, string& aRes) const override;
	bool setContent(const string& aId, const string& aData) override;
	void onContentChanged(const string& aId) override;

    protected:
	//void addComp(const string& aType, const string& aName);
	const MNode* getComp(const string& aId) const;
	MNode* getComp(const string& aId) { return const_cast<MNode*>(const_cast<const Node*>(this)->getComp(aId));}
	template<class T> inline MIface* checkLif2(TIdHash aId, T*& aPtr) {
	    return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
	}
	template<class T> inline MIface* checkLifn(TIdHash aId, T*& aPtr) {
	    return (aId == T::idHash()) ? aPtr : nullptr;
	}
	template<class T> string getUid() const {return getUriS() + Ifu::KUidSep + string(T::idStr());}
	// TODO YB to avoid using it:w
	template<class T> string getUidC(const string& aCname) const {return getUriS() + Ifu::KUidSepIc + aCname + Ifu::KUidSep + string(T::idStr());}
	MOwner* owner();
	const MOwner* owner() const;
	inline MLogRec* Logger() const {return mEnv ? mEnv->Logger(): nullptr; }
	inline MProvider* Provider() const {return mEnv ? mEnv->provider(): nullptr; }
	inline bool isLogLevel(int aLevel) const { return aLevel <= mLogLevel;}
	void updateNs(TNs& aNs, const ChromoNode& aCnode);
	// Mutations
	virtual MNode* mutAddElem(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx);
	virtual void mutSegment(const ChromoNode& aMut, bool aChange /*EFalse*/, const MutCtx& aCtx);
	virtual void mutRemove(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx);
	virtual void mutContent(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx);
	virtual void mutConnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx);
	virtual void mutDisconnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx);
	virtual void mutImport(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx);
	void notifyNodeMutated(const ChromoNode& aMut, const MutCtx& aCtx);
    protected:
	MEnv* mEnv = nullptr;
	string mName;
	MOwner* mContext = nullptr;
	TObsCp mOcp;                      /*!< Observable CP */
	TOwtNode mOwsNode;                /*!< Ownership node */
	TContents mContents;
	int mLogLevel;                /*!< Logging level */
	bool mExplorable;                /*!< Exploring is enabled, ref ds_dcs_aes_acp */
	bool mControllable;             /*!< Control is enabled, ref ds_dcs_aes_acp */
	string mAbout;                  /*!< Content About */
	MNode* mMNode = nullptr;
	MContentOwner* mMContentOwner = nullptr;
	MOwner* mMOwner = nullptr;
	MOwned* mMOwned = nullptr;
	MObservable* mMObservable = nullptr;
	MParent* mMParent = nullptr;
	MChild* mMChild = nullptr;

};


#define LOG_LEVEL(aLevel) (Logger()->MeetsLevel(aLevel) && isLogLevel(aLevel))

#define LOGN(aLevel, aContent) \
    if (Logger()->MeetsLevel(aLevel) && isLogLevel(aLevel)) {\
	TLog rec(aLevel, this, aContent);\
	Logger()->Write(rec);\
    }\

#define LOGNN(aNode, aLevel, aContent) \
    if (aNode->Logger()->MeetsLevel(aLevel) && aNode->isLogLevel(aLevel)) {\
	TLog rec(aLevel, aNode, aContent);\
	aNode->Logger()->Write(rec);\
    }\

// Profiler's routines
#define PFLE() (mEnv ? mEnv->profiler() : nullptr)
#define PFL_SAVE() PROF_SAVE(PFLE())
#define PFL_DUR_STAT_START(ID) PROF_DUR_START(PFLE(), PROF_DUR_STAT, ID)
#define PFL_DUR_STAT_REC(ID) PROF_DUR_REC(PFLE(), PROF_DUR_STAT, ID)
#define PFL_DUR_STAT_FIELD(ID, FID) PROF_FIELD(PFLE(), PROF_DUR_STAT, ID, FID)
#define PFL_DUR_START(ID) PROF_DUR_START(PFLE(), PROF_DUR, ID)
#define PFL_DUR_REC(ID) PROF_DUR_REC(PFLE(), PROF_DUR, ID)
#define PFL_DUR_VALUE(ID) PROF_FIELD(PFLE(), PROF_DUR, ID, PIndFId::EInd_VAL)
#define PFL_DUR_STAT_CNT(ID) PROF_FIELD(PFLE(), PROF_DUR_STAT, ID, PIndFId::EStat_CNT)



#endif
