#ifndef __FAP5_DESSDO_H
#define __FAP5_DESSDO_H

#include "dest.h"
#include "mobserv.h"
#include "nconn.h"
//#include "mlink.h"

/** @brief SDO base
 * */
class SdoBase : public CpStateOutp, public MDVarGet, public MObserver, public MDesInpObserver, public MSystExploring
{
    public:
	using TObserverCp = NCpOmnp<MObserver, MObservable>;
    protected:
	class InpBase {
	    public:
		InpBase(SdoBase* aHost, const string& aName);
	    public:
		SdoBase* mHost;
		const string mName;
	};
	template <typename T> class Inp : public InpBase {
	    public:
		using Stype = Sdata<T>;
	    public:
		Inp(SdoBase* aHost, const string& aName): InpBase(aHost, aName) {}
		bool getData(T& aData) {
		    bool res = false;
		    MNode* inp = mHost->getNode(mName);
		    if (inp) {
			res = GetSData(inp, aData);
		    } else {
			LOGNN(mHost, EDbg, "Cannot get input [" + mName + "]");
		    }
		    return res;
		}
	};
	template <typename T> class Inpg : public InpBase {
	    public:
		using Stype = T;
	    public:
		Inpg(SdoBase* aHost, const string& aName): InpBase(aHost, aName) {}
		bool getData(T& aData) {
		    bool res = false;
		    MNode* inp = mHost->getNode(mName);
		    if (inp) {
			res = GetGData(inp, aData);
		    } else {
			LOGNN(mHost, EDbg, "Cannot get input [" + mName + "]");
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
		    mHost->onEagOwnedAttached(aOwned);
		}
		void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override {
		    mHost->onEagOwnedDetached(aOwned);
		}
		void onObsContentChanged(MObservable* aObl, const string& aId) override {
		    mHost->onEagContentChanged(aId);
		}
		void onObsChanged(MObservable* aObl) override {
		    mHost->onEagChanged();
		}
	    public:
		TObserverCp mOcp;               /*!< Observer connpoint */
	    private:
		SdoBase* mHost;
	};

    public:
	static const char* Type() { return "SdoBase";};
	SdoBase(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
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
	// From MDesInpObserver
	string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>();}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	void onInpUpdated() override;
	// From MSystExploring
	string MSystExploring_Uid() const override  {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override { return nullptr;}
	void onMagChanged() override;
	MSystExploring::TCp* getCp() override;
    protected:
	void UpdateMag();
	void NotifyInpsUpdated();
	MSystExplorable* getExplorable();
	// Local
	virtual void onEagOwnedAttached(MOwned* aOwned) {}
	virtual void onEagOwnedDetached(MOwned* aOwned) {}
	virtual void onEagContentChanged(const string& aId) {}
	virtual void onEagChanged() {}
    protected:
	TObserverCp mObrCp;               /*!< Observer connpoint */
	EagObs mEagObs;  /*!< Explored agent observer */
	MNode* mSue; /*!< System under exploring */
	bool mCInv;              //!< Sign of data cache invalidated
	NCpOnp<MSystExploring, MSystExplorable> mExploringCp;
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

#if 0

/** @brief SDO "URI of observed node"
 * */
class SdoUri : public Sdog<DGuri>
{
    public:
	static const char* Type() { return "SdoUri";};
	SdoUri(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};
#endif


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


#if 0
/** @brief SDO "Comp Owner"
 * */
class SdoCompOwner : public Sdog<DGuri>
{
    public:
	static const char* Type() { return "SdoCompOwner";};
	SdoCompOwner(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	Inpg<DGuri> mInpCompUri;  //<! Comp URI
};

/** @brief SDO "Comp Comp"
 * */
class SdoCompComp : public Sdog<DGuri>
{
    public:
	static const char* Type() { return "SdoCompComp";};
	SdoCompComp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	Inpg<DGuri> mInpCompUri;  //<! Comp URI
	Inpg<DGuri> mInpCompCompUri;  //<! Comp comp URI
};




/** @brief SDO "Component exists"
 * */
class SdoComp : public Sdog<Sdata<bool>>
{
    public:
	static const char* Type() { return "SdoComp";};
	SdoComp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	Inp<string> mInpName;
};

#endif

/** @brief SDO "Component count"
 * */
class SdoCompsCount : public Sdog<Sdata<int>>
{
    public:
	inline static constexpr std::string_view idStr() { return "SdoCompsCount"sv;}
	SdoCompsCount(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onEagOwnedAttached(MOwned* aOwned) override;
	virtual void onEagOwnedDetached(MOwned* aOwned) override;
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
	virtual void onEagOwnedAttached(MOwned* aOwned) override;
	virtual void onEagOwnedDetached(MOwned* aOwned) override;
};

#if 0
/** @brief SDO "Component count"
 * */
class SdoCompsUri : public Sdog<Vector<DGuri>>
{
    public:
	static const char* Type() { return "SdoCompsUri";};
	SdoCompsUri(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onEagOwnedAttached(MOwned* aOwned) override;
	virtual void onEagOwnedDetached(MOwned* aOwned) override;
};




/** @brief SDO "Vertexes are connected"
 * */
class SdoConn : public Sdog<Sdata<bool>>
{
    public:
	static const char* Type() { return "SdoConn";};
	SdoConn(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onObsChanged(MObservable* aObl) override;
    protected:
	Inp<string> mInpVp;
	Inp<string> mInpVq;
	MNode* mVpUe;        //<! Vertex under exploring 
	MNode* mVqUe;        //<! Vertex under exploring 
};

/** @brief SDO "Vertex pairs count"
 * */
class SdoPairsCount : public Sdog<Sdata<int>>
{
    public:
	static const char* Type() { return "SdoPairsCount";};
	SdoPairsCount(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onObsChanged(MObservable* aObl) override;
	virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
    protected:
	void observingVertUeExst();
    protected:
	Inp<string> mInpVert;  //<! Vertex URI
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
	static const char* Type() { return "SdoPair";};
	SdoPair(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onObsChanged(MObservable* aObl) override;
    protected:
	Inp<string> mInpTarg;  //<! Target Vertex URI
	MNode* mVertUe;        //<! Vertex under exploring 
};

/** @brief SDO "Single pair of targets comp"
 * Invalid if the vertex is not connected
 * It is usefull to explore subsystems connpoints pair
 * */
class SdoTcPair : public Sdog<DGuri>
{
    public:
	static const char* Type() { return "SdoTcPair";};
	SdoTcPair(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	Inpg<DGuri> mInpTarg;  //<! Target URI
	Inpg<DGuri> mInpTargComp;  //<! Target comp vertex URI relative to target
};


/** @brief SDO "Pairs"
 * */
class SdoPairs : public Sdog<Vector<DGuri>>
{
    public:
	static const char* Type() { return "SdoPairs";};
	SdoPairs(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};

/** @brief SDO "Target Pairs"
 * */
class SdoTPairs : public Sdog<Vector<DGuri>>
{
    public:
	static const char* Type() { return "SdoTPairs";};
	SdoTPairs(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
    protected:
	Inpg<DGuri> mInpTarg;  //<! Target URI
};

/** @brief SDO "Edges"
 * */
class SdoEdges : public Sdog<Vector<Pair<DGuri>>>
{
    public:
	static const char* Type() { return "SdoEdges";};
	SdoEdges(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
};

/** @brief SDO "DES is idle"
 * */
class SdoDesIdle : public Sdog<Sdata<bool>>
{
    public:
	static const char* Type() { return "SdoDesIdle";};
	SdoDesIdle(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual void onEagChanged() override;
};

#endif



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
