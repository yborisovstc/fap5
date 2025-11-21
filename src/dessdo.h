#ifndef __FAP5_DESSDO_H
#define __FAP5_DESSDO_H

#include "dest.h"
#include "mobserv.h"
#include "nconn.h"
//#include "mlink.h"

/** @brief SDO base
 * */
class SdoBase : public CpStateOutp, public MDVarGet, public MObserver, public MSystExploring
{
    public:
	using TObserverCp = NCpOmnp<MObserver, MObservable>;
    protected:
	class InpBase {
	    public:
		InpBase(SdoBase* aHost, const string& aName);
		void Construct();
	    public:
		SdoBase* mHost;
		const string mName;
	};
	template <typename T> class Inpg : public InpBase {
	    public:
		using Stype = T;
	    public:
		Inpg(SdoBase* aHost, const string& aName): InpBase(aHost, aName) {}
		bool getData(T& aData) {
		    bool res = false;
		    MNode* inp = mHost->getNode(mName);
		    MVert* inpv = inp ? inp->lIf(inpv) : nullptr;
		    MDVarGet* vget = (inpv && inpv->pairsCount() == 1) ? inpv->getPair(0)->lIf(vget) : nullptr;
		    if (vget) {
			const T* data = vget->DtGet(data);
			if (data) {
			    aData = *data;
			    res = data->IsValid();
			}
		    }
		    return res;
		}
	};
	/** @brief Explored agent observer
	 * */
	class EagObs : public MObserver {
	    public:
		EagObs(SdoBase* aHost): mHost(aHost), mOcp(this) {}
		virtual ~EagObs() { }
		// From MObserver
		string MObserver_Uid() const override {return mHost->getUidC<MObserver>("EagObs");}
		MIface* MObserver_getLif(TIdHash aId) override { return nullptr;}
		void onObsOwnerAttached(MObservable* aObl) override {}
		void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override {
		    //mHost->onEagOwnedAttached(aOwned);
		}
		void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override {
		    //mHost->onEagOwnedDetached(aOwned);
		}
		void onObsContentChanged(MObservable* aObl, const string& aId) override {
		    //mHost->onEagContentChanged(aId);
		}
		void onObsChanged(MObservable* aObl) override {
		    mHost->onEagChanged();
		}
		void onObsEvent(MObservable* aObl, const MEvent* aEvent) override {
		    //if (aEvent->id() == MNodeEventOwnedAttached::idHash()) {
		    if (aEvent->mId == TNodeEventOwnedAttached::idHash) {
			auto event = reinterpret_cast<const TNodeEventOwnedAttached*>(aEvent);
			mHost->onEagOwnedAttached(event->mOwned);
		    } else if (aEvent->mId == TNodeEventOwnedDetached::idHash) {
			auto event = reinterpret_cast<const TNodeEventOwnedDetached*>(aEvent);
			mHost->onEagOwnedDetached(event->mOwned);
		    }
		}
		TCp* observerCp() override { return &mOcp;}
	    public:
		TObserverCp mOcp;               /*!< Observer connpoint */
	    private:
		SdoBase* mHost;
	};

    public:
	static const char* Type() { return "SdoBase";};
	SdoBase(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        void Construct() override;
    public:
	// From MNode
	MIface* MNode_getLif(TIdHash aId) override;
	// From MOwned
        MIface *MOwned_getLif(TIdHash aId) override;
	// From MDVarGet
	string MDVarGet_Uid() const override { return getUid<MDVarGet>();}
	void MDVarGet_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	// From Node.MOwned
	void onOwnerAttached() override;
	// From MObserver
	string MObserver_Uid() const {return getUid<MObserver>();}
	MIface* MObserver_getLif(TIdHash aId) override {return nullptr;}
	void onObsOwnerAttached(MObservable* aObl) override {}
	void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
	void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override;
	void onObsContentChanged(MObservable* aObl, const string& aId) override;
	void onObsChanged(MObservable* aObl) override;
	void onObsEvent(MObservable* aObl, const MEvent* aEvent) override;
	MObserver::TCp* observerCp() override { return &mObrCp;}
	// From MDesInpObserver
	string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>();}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	void onInpUpdated() override;
	// From MSystExploring
	string MSystExploring_Uid() const override  {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override { return nullptr;}
	void onMagChanged() override;
	MNpc* getCp() override;
    protected:
	void UpdateMag();
	void NotifyInpsUpdated();
	MSystExplorable* getExplorable();
	// Local
	virtual void onEagOwnedAttached(const MOwned* aOwned) {}
	virtual void onEagOwnedDetached(const MOwned* aOwned) {}
	//virtual void onEagContentChanged(const string& aId) {}
	virtual void onEagChanged() {}
    protected:
	TObserverCp mObrCp;               /*!< Observer connpoint */
	EagObs mEagObs;  /*!< Explored agent observer */
	MNode* mSue; /*!< System under exploring */
	bool mCInv;              //!< Sign of data cache invalidated
	NpcOnp mExploringCp;
        NpcOmnp mInpsBp;  /*!< Inputs binding point */
	NpcOnp mOutpBp;   /*!< Output binding point */
	MDVarGet* mMDVarGet = nullptr;
	MObserver* mMObserver = nullptr;
	MSystExploring* mMSystExploring = nullptr;
	MDesInpObserver* mMDesInpObserver = nullptr;
};

/** @brief SDO providing generic data
 * */
template <typename T> class Sdog : public SdoBase
{
    public:
	using Stype = T;
    public:
	Sdog(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): SdoBase(aType, aName, aEnv) {}
	// From MDVarGet
	virtual const DtBase* VDtGet(const string& aType) override { return &mRes;}
	virtual MIface* DoGetDObj(const char *aName) override {return nullptr;}
    	virtual string VarGetIfid() const override {return T::TypeSig();}
    protected:
	T mRes;
};

/** @brief SDO "Name"
 * */
class SdoName : public Sdog<Sdata<string>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoName"sv;}
	SdoName(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};


/** @brief SDO "URI of observed node"
 * */
class SdoUri : public Sdog<DGuri>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoUri"sv;}
	SdoUri(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};


/** @brief SDO "Parent"
 * */
class SdoParent : public Sdog<Sdata<string>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoParent"sv;}
	SdoParent(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};

/** @brief SDO "Parents heirarchy"
 * */
class SdoParents : public Sdog<Vector<DGuri>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoParents"sv;}
	SdoParents(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};


/** @brief SDO "Comp Owner"
 * */
class SdoCompOwner : public Sdog<DGuri>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoCompOwner"sv;}
	SdoCompOwner(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	void Construct() override;
    protected:
	Inpg<DGuri> mInpCompUri;  //<! Comp URI
};

/** @brief SDO "Comp Comp"
 * */
class SdoCompComp : public Sdog<DGuri>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoCompComp"sv;}
	SdoCompComp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	void Construct() override;
    protected:
	Inpg<DGuri> mInpCompUri;  //<! Comp URI
	Inpg<DGuri> mInpCompCompUri;  //<! Comp comp URI
};


/** @brief SDO "Component exists"
 * */
class SdoComp : public Sdog<Sdata<bool>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoComp"sv;}
	SdoComp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	void Construct() override;
    protected:
	Inpg<DGuri> mInpName;
};


/** @brief SDO "Component count"
 * */
class SdoCompsCount : public Sdog<Sdata<int>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoCompsCount"sv;}
	SdoCompsCount(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onEagOwnedAttached(const MOwned* aOwned) override;
	virtual void onEagOwnedDetached(const MOwned* aOwned) override;
};


/** @brief SDO "Components names"
 * NOTE: Gives Vector<string> instead of Vector<Sdata<string>>
 * this approach is obsolete and needs to be replaced
 * */
class SdoCompsNames : public Sdog<Vector<string>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoCompsNames"sv;}
	SdoCompsNames(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onEagOwnedAttached(const MOwned* aOwned) override;
	virtual void onEagOwnedDetached(const MOwned* aOwned) override;
};

/** @brief SDO "Components URIs"
 * */
class SdoCompsUri : public Sdog<Vector<DGuri>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoCompsUri"sv;}
	SdoCompsUri(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onEagOwnedAttached(const MOwned* aOwned) override;
	virtual void onEagOwnedDetached(const MOwned* aOwned) override;
};




/** @brief SDO "Vertexes are connected"
 * Note that SdoConn doesn't monitor of vertexes creation
 * (like it NodeCreationObserver does). So SdoConn cannot be used for getting  "future" connection
 * status.
 * */
class SdoConn : public Sdog<Sdata<bool>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoConn"sv;}
	SdoConn(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	//virtual void onObsChanged(MObservable* aObl) override;
	void onObsEvent(MObservable* aObl, const MEvent* aEvent) override;
    protected:
	void Construct() override;
    protected:
	Inpg<DGuri> mInpVp;
	Inpg<DGuri> mInpVq;
	MNode* mVpUe;        //<! Vertex under exploring 
	MNode* mVqUe;        //<! Vertex under exploring 
};

/** @brief SDO "Vertex pairs count"
 * */
class SdoPairsCount : public Sdog<Sdata<int>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoPairsCount"sv;}
	SdoPairsCount(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onObsChanged(MObservable* aObl) override;
	virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
	void onObsEvent(MObservable* aObl, const MEvent* aEvent) override;
    protected:
	void Construct() override;
	void observingVertUeExst();
    protected:
	Inpg<DGuri> mInpVert;  //<! Vertex URI
	MNode* mVertUe;        //<! Vertex under exploring 
	MNode* mVertUeOwr = nullptr;     //<! Vertex under exploring owner
	int mVertUeOwrLevel = -1;
};

/** @brief SDO "Vertex pair URI"
 * Invalid if the vertex is not connected
 * */
class SdoPair : public Sdog<DGuri>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoPair"sv;}
	SdoPair(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onObsChanged(MObservable* aObl) override;
    protected:
	void Construct() override;
    protected:
	Inpg<DGuri> mInpTarg;  //<! Target Vertex URI
	MNode* mVertUe;        //<! Vertex under exploring 
};

/** @brief SDO "Single pair of targets comp"
 * Invalid if the vertex is not connected
 * It is usefull to explore subsystems connpoints pair
 * */
class SdoTcPair : public Sdog<DGuri>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoTcPair"sv;}
	SdoTcPair(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	void Construct() override;
    protected:
	Inpg<DGuri> mInpTarg;  //<! Target URI
	Inpg<DGuri> mInpTargComp;  //<! Target comp vertex URI relative to target
};


/** @brief SDO "Pairs"
 * */
// TODO This SDO isn't usable now sinse Vert is not Explorable/Controllable now
class SdoPairs : public Sdog<Vector<DGuri>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoPairs"sv;}
	SdoPairs(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};

/** @brief SDO "Target Pairs"
 * */
class SdoTPairs : public Sdog<Vector<DGuri>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoTPairs"sv;}
	SdoTPairs(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	void Construct() override;
    protected:
	Inpg<DGuri> mInpTarg;  //<! Target URI
};

/** @brief SDO "Edges"
 * */
class SdoEdges : public Sdog<Vector<Pair<DGuri>>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoEdges"sv;}
	SdoEdges(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};

/** @brief SDO "DES is idle"
 * */
class SdoDesIdle : public Sdog<Sdata<bool>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoDesIdle"sv;}
	SdoDesIdle(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onEagChanged() override;
};



#if 0 // NOT COMPLETED
/** @brief Inputs update indicator, ref ds_dcs_iui_sla
 * This is not SDO exactly but nevertheless acts as SDO observing the interior of system
 * The output is boolean indication of inputs update happened
 * */
class SdoUpdateInd: public CpStateOutp, public MDesSyncable, public MDesInpObserver, public MDVarGet, public MDtGet<Sdata<bool>>
{
    public:
	static const char* Type() { return "SdoUpdateInd";};
	SdoUpdateInd(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    public:
	// From MNode
	virtual MIface* MNode_getLif(const char *aType) override;
	// From MDVarGet
	virtual string MDVarGet_Uid() const override { return getUid<MDVarGet>();}
	virtual void MDVarGet_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	// From MDesSyncable
	virtual string MDesSyncable_Uid() const override {return getUid<MDesSyncable>();}
	virtual void MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual MIface* MDesSyncable_getLif(const char *aType) override { return nullptr; }
	virtual void update() override;
	virtual void confirm() override;
	virtual void setUpdated() override;
	virtual void setActivated() override;
	// From MDesInpObserver
	virtual string MDesInpObserver_Uid() const {return getUid<MDesInpObserver>();}
	virtual void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual void onInpUpdated() override;
    protected:
	Sdata<bool> mData;
};
#endif


#endif  // __FAP3_DESSDO_H
