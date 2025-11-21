#ifndef __FAP5_DES_H
#define __FAP5_DES_H

#include "mlog.h"
#include "mdata.h"
#include "mdes.h"
#include "mlauncher.h"
//#include "mdadp.h"

#include "nconn.h"
#include "syst.h"
#include "rdatauri.h"
#include "connpoint.h"


/** @brief Simple connection point - state input
 * Needs to be bound to state to attach state as Provided proxy
 * Specific of simple connpoint is that it is bound to single binder (actually onwer)
 * so connpont can just redirect provided iface resolution request instead of implementing 
 * iface locally. This is a bit faster than implementing iface. However implementing iface
 * option remains open here.
 * */
class CpStateInp: public ConnPoint, public MDVarGet
{
    public:
        using TBase = ConnPoint;
    public:
	inline static constexpr std::string_view idStr() { return "CpStateInp"sv;}
        CpStateInp(const string &aType, const string& aName, MEnv* aEnv);
        virtual ~CpStateInp() {}
        // From MVert
        MIface *MVert_getLif(TIdHash aId) override;
        // From MDVarGet
	string MDVarGet_Uid() const override {return getUid<MDVarGet>();}
	string VarGetIfid() const override;
	MIface* DoGetDObj(const char *aName) override {return nullptr;}
	const DtBase* VDtGet(const string& aType) override;
	void VDtGet(const string& aType, MDVarGet::TData& aData) override;
        // From ConnPoint
        MNpc* bP() override { return &mBcp;}
    protected:
        // From ConnPoint
	void onConnected(MVert* aPair) override;
	void onDisconnected() override;
        // From Vert
	void onBound(MVert* aPair) override;
	void onUnbinding(MVert* aPair) override {}
	void onUnbound(MVert* aPair) override;
    protected:
        NpcOnp mBcp;
};

class CpStateOutp: public ConnPoint, public MDesInpObserver
{
    public:
	inline static constexpr std::string_view idStr() { return "CpStateOutp"sv;}
        CpStateOutp(const string &aType, const string& aName, MEnv* aEnv);
        virtual ~CpStateOutp() {}
        // From MVert
	MIface* MVert_getLif(TIdHash aId) override;
	// From MDesInpObserver
	string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>();}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	void onInpUpdated() override;
        // From ConnPoint
        MNpc* bP() override { return &mBcp;}
    protected:
        NpcOnp mBcp;
};

#if 0

/** @brief State input CP that implement required iface and proxied provided
 *  Can be used as pin in combined CPs (Socket etc.)
 *  Supports multiple binders
 * */
class CpStateInpPin: public ConnPoint<MDesInpObserver, MDVarGet>, public MDesInpObserver, public MDVarGet
{
    public:
        using TBase = ConnPoint<MDesInpObserver, MDVarGet>;
        using TBcp = NCpOmnp<TRequired, TProvided>;
    public:
	inline static constexpr std::string_view idStr() { return "CpStateInpPin"sv;}
        CpStateInpPin(const string &aType, const string& aName, MEnv* aEnv);
        virtual ~CpStateInpPin();
	// From MDesInpObserver
	string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>();}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	void onInpUpdated() override;
        // From MDVarGet
	string MDVarGet_Uid() const override {return getUid<MDVarGet>();}
	string VarGetIfid() const override;
	MIface* DoGetDObj(const char *aName) override {return nullptr;}
	const DtBase* VDtGet(const string& aType) override {
            const DtBase* res = nullptr;
            auto* pair = (mPairs.size() == 1) ? mPairs.at(0) : nullptr; 
            MDVarGet* pairDget = pair ? pair->lIft<MDVarGet>() : nullptr;
            res = pairDget ? pairDget->VDtGet(aType) : nullptr;
            return res;
        }
    protected:
        // From ConnPoint
	void onConnected(MVert* aPair) override;
	void onDisconnected() override;
    protected:
        TBcp mBcp;
};


/** @brief State output CP that implement required iface
 *  Can be used as pin in combined CPs (Socket etc.)
 * */
class CpStateOutpPin: public CpStateOutp, public MDVarGet, public MDesInpObserver
{
    public:
	inline static constexpr std::string_view idStr() { return "CpStateOutpPin"sv;}
        CpStateOutpPin(const string &aType, const string& aName, MEnv* aEnv):
            CpStateOutp(aType, aName, aEnv) {}
        virtual ~CpStateOutpPin();
        // From MDVarGet
	string MDVarGet_Uid() const override {return getUid<MDVarGet>();}
	string VarGetIfid() const override;
	MIface* DoGetDObj(const char *aName) override {return nullptr;}
	const DtBase* VDtGet(const string& aType) override;
        /*
	const DtBase* VDtGet(const string& aType) override {
            return mProvidedPx ? mProvidedPx->VDtGet(aType) : nullptr;
        }
        */
	// From MDesInpObserver
	string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>();}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	void onInpUpdated() override {
            for (auto pair : mPairs) {
                auto inpObs = pair->lIft<MDesInpObserver>();
                if (inpObs) inpObs->onInpUpdated();
            }
        }
    protected:
        // From ConnPoint
	void onBound() override {
            onInpUpdated();
        }
	void onUnbound() override {
            onInpUpdated();
        }
};

#endif


/** @brief CpStateInp direct (specialized) extender (extd as inp)
 * It is faster than generic CpStateInp extender
 * */
class ExtdStateInp : public CpStateInp
{
    public:
        using TBase = CpStateInp;
    public:
	inline static constexpr std::string_view idStr() { return "ExtdStateInp"sv;}
        ExtdStateInp(const string &aType, const string& aName, MEnv* aEnv);
        virtual ~ExtdStateInp() {}
	void Construct() override;
        // From MVert
        MVert* getExtd() override { return mInt;}
    public:
        CpStateOutp* mInt = nullptr;
    protected:
        static string KIntName;

};

// TODO consider using this instead of specialized extender for CpStateInp
#if 0
/** @brief CpStateInp extender basing on generic extender
 * */
class ExtdStateInp : public Extd
{
    public:
	inline static constexpr std::string_view idStr() { return "ExtdStateInp"sv;}
	ExtdStateInp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        void Construct() override;
        MIface* MVert_getLif(TIdHash aId) override;
    public:
        CpStateOutp* mInt = nullptr;
    protected:
	MDesInpObserver* mMDesInpObserver = nullptr;
	MDVarGet* mMDVarGet = nullptr;
};
#endif

/** @brief CpStateOutp direct (specialized) extender (extd as outp)
 * */
class ExtdStateOutp : public CpStateOutp
{
    public:
        using TBase = CpStateOutp;
    public:
	inline static constexpr std::string_view idStr() { return "ExtdStateOutp"sv;}
        ExtdStateOutp(const string &aType, const string& aName, MEnv* aEnv);
        virtual ~ExtdStateOutp() {}
	void Construct() override;
        // From MVert
        MVert* getExtd() override { return mInt;}
    public:
        CpStateInp* mInt = nullptr;
    protected:
        static string KIntName;

};


#if 0
/** @brief DES State input with data latched on update phase
 * Follows DFI approach but inverse, ref ds_dfi
 * Can be use in inverse DFI DES that change state on confirm phase
 * Needs to be bound to state to attach state as Provided proxy
 * */
class CpStateInpLu: public ConnPoint<MDesInpObserver, MDVarGet>, public MDesSyncable
{
    public:
	inline static constexpr std::string_view idStr() { return "CpStateInpLu"sv;}
        CpStateInpLu(const string &aType, const string& aName, MEnv* aEnv):
            ConnPoint<MDesInpObserver, MDVarGet> (aType, aName, aEnv) {}
        virtual ~CpStateInp() {}
    protected:
        // From ConnPoint
	void onConnected() override;
	void onDisconnected() override;
	void onBound() override;
	void onUnbound() override;
};
#endif


#if 0
/** @brief CpStateInp direct extender (extd as inp)
 * */
class ExtdStateMnodeInp : public Extd
{
    public:
	static const char* Type() { return "ExtdStateMnodeInp";};
	ExtdStateMnodeInp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
};

/** @brief CpStateOutp direct extender (extd as outp)
 * */
class ExtdStateMnodeOutp : public Extd
{
    public:
	static const char* Type() { return "ExtdStateMnodeOutp";};
	ExtdStateMnodeOutp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
};

/** @brief CpStateOutp direct extender (extd as outp) with ifaces impl, ref ds_irm_wprc_blc
 * !! Supports only single MDVarGet input
 * */
class ExtdStateOutpI : public ExtdStateOutp, public MDVarGet, protected MDesInpObserver
{
    public:
	static const char* Type() { return "ExtdStateOutpI";};
	ExtdStateOutpI(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual MIface* MNode_getLif(const char *aType) override;
	// From MDesInpObserver
	virtual string MDesInpObserver_Uid() const {return getUid<MDesInpObserver>();}
	virtual void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual void onInpUpdated() override;
	// From MDVarGet
	virtual string MDVarGet_Uid() const override { return getUid<MDVarGet>();}
	virtual MIface* DoGetDObj(const char *aName) override { return nullptr;}
	virtual const DtBase* VDtGet(const string& aType) override;
	virtual string VarGetIfid() const override;
	virtual void resolveIfc(const string& aName, MIfReq::TIfReqCp* aReq) override;
    protected:
	MDesInpObserver* mMDesInpObserverPtr = nullptr;
	MDVarGet* mMDVarGetPtr = nullptr;
};



/** @brief Connection point - output of state provided MNode iface
 * Just ConnPointu with pre-configured prepared/required
 * */
class CpStateMnodeOutp: public CpState
{
    public:
	static const char* Type() { return "CpStateMnodeOutp";};
	CpStateMnodeOutp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
};

#endif


// TODO Use BState as a base
/** @brief State, ver. 2, non-inhritable, monolitic, direct data, switching updated-confirmed
 * */
class State: public ConnPoint, public MDesSyncable, public MDesInpObserver, public MDVarGet, public MDVarSet
{
    public:
        using TBase = ConnPoint;
        using TDesSyncCp = NCpOnp<MDesSyncable, MDesObserver>;  /*!< DES syncable connpoint */
    public:
	inline static constexpr std::string_view idStr() { return "State"sv;}
	State(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual ~State();
	// From Node.MIface
	MIface* MNode_getLif(TIdHash aId) override;
	// From Node
	void Construct() override;
	MIface* MOwner_getLif(TIdHash aId) override;
	GUri parentUri() const override { return string(idStr());}
	// From MVert
	MIface* MVert_getLif(TIdHash aId) override;
	virtual TDir getDir() const override { return EOut;}
	// From MDesSyncable
	virtual string MDesSyncable_Uid() const override {return getUid<MDesSyncable>();}
	virtual void MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual MIface* MDesSyncable_getLif(TIdHash aId) override { return nullptr; }
	virtual void update() override;
	virtual void confirm() override;
	virtual void setUpdated() override;
	virtual void setActivated() override;
	virtual bool isActive() const override { return false;}
	virtual int countOfActive(bool aLocal = false) const override { return 0;}
        TDesSyncableCp* desSyncableCp() override { return &mDesSyncCp;}
	// From MDesInpObserver
	virtual string MDesInpObserver_Uid() const {return getUid<MDesInpObserver>();}
	virtual void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual void onInpUpdated() override;
	// From Node.MContentOwner
	int contCount() const override { return Node::contCount() + 1;}
	bool getContentId(int aIdx, string& aRes) const override;
	bool getContent(const string& aId, string& aRes) const override;
	bool setContent(const string& aId, const string& aData) override;
	void onContentChanged(const string& aId) override {}
	// From MDVarGet
	string MDVarGet_Uid() const override {return getUid<MDVarGet>();}
	string VarGetIfid() const override;
	MIface* DoGetDObj(const char *aName) override {return nullptr;}
	DtBase* VDtGet(const string& aType) override;
	void VDtGet(const string& aType, MDVarGet::TData& aData) override;
	// From MDVarSet
	virtual string MDVarSet_Uid() const override {return getUid<MDVarSet>();}
	virtual string VarGetSIfid();
	virtual const bool VDtSet(const DtBase& aData) override;
        // From MConnPoint
        MNpc* bP() override { return &mInpBp;}
    public:
	static const string KCont_Value;
    protected:
	MDVarGet* GetInp();
	bool updateWithContValue(const string& aCnt);
        void notifyInpsUpdated();
    protected:
	// From MNode
	MIface* MOwned_getLif(TIdHash aId) override;
	// Local
	DtBase* CreateData(const string& aSrc);
        inline MDesObserver* desObserver() { return mDesSyncCp.mPair ? mDesSyncCp.mPair->provided() : nullptr;}
    protected:
        TDesSyncCp mDesSyncCp; //<! DES Syncable connpoint
	DtBase* mPdata;   //<! Preparing (updating) phase data
	DtBase* mCdata;   //<! Confirming phase data
        CpStateInp* mInp = nullptr;
	bool mUpdNotified;  //<! Sign of that State notified observers on Update
	bool mActNotified;  //<! Sign of that State notified observers on Activation
	bool mInpValid;
	bool mStDead;     //<! Sign of State destructed, needs to avoid callbacks initialted by bases */
	NpcOnp mInpBp;
	MDesSyncable* mMDesSyncable = nullptr;
	MDesInpObserver* mMDesInpObserver = nullptr;
	MDVarGet* mMDVarGet = nullptr;
	MDVarSet* mMDVarSet = nullptr;
	static const string KInpName;
};


/** @brief Constant data
 * */
class Const: public ConnPoint, public MDVarGet
{
    public:
        using TBase = ConnPoint;
    public:
	inline static constexpr std::string_view idStr() { return "Const"sv;}
	Const(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual ~Const();
	// From Node.MIface
	MIface* MNode_getLif(TIdHash aId) override;
	// From Node
	MIface* MOwner_getLif(TIdHash aId) override;
	GUri parentUri() const override { return string(idStr());}
	// From MVert
	virtual TDir getDir() const override { return EOut;}
	MIface* MVert_getLif(TIdHash aId) override;
	// From Node.MContentOwner
	int contCount() const override { return Node::contCount() + 1;}
	bool getContentId(int aIdx, string& aRes) const override;
	bool getContent(const string& aId, string& aRes) const override;
	bool setContent(const string& aId, const string& aData) override;
	void onContentChanged(const string& aId) override {}
	// From MDVarGet
	virtual string MDVarGet_Uid() const override {return getUid<MDVarGet>();}
	virtual string VarGetIfid() const override;
	virtual MIface* DoGetDObj(const char *aName) override {return nullptr;}
	virtual DtBase* VDtGet(const string& aType) override;
        // From MConnPoint
        MNpc* bP() override { return nullptr;}
    public:
	static const string KCont_Value;
    protected:
	MDVarGet* GetInp();
	bool updateWithContValue(const string& aCnt);
        void notifyInpsUpdated();
    protected:
	// From MNode
	MIface* MOwned_getLif(TIdHash aId) override;
	// Local
	DtBase* CreateData(const string& aSrc);
    protected:
	DtBase* mData;
        CpStateInp* mInp = nullptr;
	bool mInpValid;
	bool mStDead;     //<! Sign of State destructed, needs to avoid callbacks initialted by bases */
	MDVarGet* mMDVarGet = nullptr;
};



/** @brief DES system
* */
class Des: public Syst, public MDesSyncable, public MDesObserver/*, public MDesAdapter, public MDesManageable*/,
    public MDesCtxBinder
{
    public:
        //using TScblReg = list<MDesSyncable*>;
        using TScblReg = vector<MDesSyncable*>;
        using TDesSyncCp = NCpOnp<MDesSyncable, MDesObserver>;  /*!< DES syncable connpoint */
        using TDesObsrCp = NCpOmnp<MDesObserver, MDesSyncable>;  /*!< DES observer connpoint */
    public:
	inline static constexpr std::string_view idStr() { return "Des"sv;}
	Des(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From Node.MIface
	virtual MIface* MNode_getLif(TIdHash aId) override;
	// From Node
	MIface* MOwner_getLif(TIdHash aId) override;
	void onOwnedAttached(MOwned* aOwned) override;
	void onOwnedDetached(MOwned* aOwned) override;
	MIface* MOwned_getLif(TIdHash aId);
	// From MDesSyncable
	string MDesSyncable_Uid() const override {return getUid<MDesSyncable>();}
	void MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	MIface* MDesSyncable_getLif(TIdHash aId) override { return nullptr; }
	void update() override;
	void confirm() override;
	void setUpdated() override;
	void setActivated() override;
	bool isActive() const override;
	int countOfActive(bool aLocal = false) const override;
        TDesSyncableCp* desSyncableCp() override { return &mDesSyncCp;}
	// From MDesObserver
	string MDesObserver_Uid() const override {return getUid<MDesObserver>();}
	void MDesObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	void onActivated(MDesSyncable* aComp) override;
	void onUpdated(MDesSyncable* aComp) override;
	// From MDesCtxBinder
	string MDesCtxBinder_Uid() const override {return getUid<MDesCtxBinder>();}
        bool bindDesCtx(MIface* aCtx) override;
        /*
	// From MDesManageable
	virtual string MDesManageable_Uid() const override {return getUid<MDesManageable>();}
	virtual void pauseDes() override;
	virtual void resumeDes() override;
	virtual bool isPaused() const;
        // From MDesAdapter
        virtual string MDesAdapter_Uid() const override { return getUid<MDesAdapter>(); }
        virtual void MDesAdapter_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
        virtual MNode* getMag() override;
        */
    protected:
	static void RmSyncable(TScblReg& aReg, MDesSyncable* aScbl);
        inline MDesObserver* desObserver() { return mDesSyncCp.mPair ? mDesSyncCp.mPair->provided() : nullptr;}
        inline const MDesObserver* desObserver() const { return mDesSyncCp.mPair ? mDesSyncCp.mPair->provided() : nullptr;}
    public:
	static const GUri KControlledUri;
    protected:
        TDesSyncCp mDesSyncCp; //<! DES Syncable connpoint
        TDesObsrCp mDesObsrCp; //<! DES Observer connpoint
	TScblReg mSP;     /*!< Active compoments */
	TScblReg mSQ;    /*!< Updated compoments */
	TScblReg* mActive = &mSP;
	TScblReg* mUpdated = &mSQ;
	bool mUpdNotified;  //<! Sign of that State notified observers on Update
	bool mActNotified;  //<! Sign of that State notified observers on Activation
	bool mUpd = false;
	bool mPaused;       //<! Status of pause of DES evolving
	bool mIsActive = true; //<! Status of being active. Tmp solution of ds_desopt_au
	MDesSyncable* mMDesSyncable = nullptr;
	MDesObserver* mMDesObserver = nullptr;
	//MDesAdapter* mMDesAdapter = nullptr;
	//MDesManageable* mMDesManageable = nullptr;
	MDesCtxBinder* mMDesCtxBinder = nullptr;
};

/** @brief Launcher of DES
 * Runs DES syncable owned
 * */
class DesLauncher: public Des, public MLauncher
{
    public:
	inline static constexpr std::string_view idStr() { return "DesLauncher"sv;}
	DesLauncher(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From Node.MIface
	virtual MIface* MNode_getLif(TIdHash aId) override;
	// From MOwned
	virtual MIface* MOwned_getLif(TIdHash aId) override;
	// From MLauncher
	virtual string MLauncher_Uid() const override {return getUid<MLauncher>();}
	virtual bool Run(int aCount = 0, int aIdleCount = 0) override;
	virtual bool Stop() override;
	virtual int GetCounter() const override { return mCounter; }
	// Local
	virtual void OnIdle();
    protected:
	void updateCounter(int aCnt);
	void outputCounter(int aCnt);
    protected:
	int mCounter = 0;
	bool mStop;
	MLauncher* mMLauncher = nullptr;
};

/** @brief Active subsystem of DES
 * Runs on master DES confirm, ds_desas_nio_ric
 * !! Blocked atm because design is wrong, ref ds_desas_nio_ric_swr
 * */
class DesAs: public DesLauncher
{
    public:
	inline static constexpr std::string_view idStr() { return "DesAs"sv;}
	DesAs(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From MLauncher
	virtual bool Run(int aCount = 0, int aIdleCount = 0) override;
	// From MDesSyncable
	virtual void update() override;
	virtual void confirm() override;
	virtual void setUpdated() override {}
	virtual void setActivated() override;
    protected:
	bool mRunning;
};


/** @brief Active subsystem of DES, ver. 2, ref ds_desas_nio_do
 * Runs on master DES update, buffers outputs in "Output" subnode
 * Not working atm.
 * */
class DesAs2: public DesLauncher
{
    public:
	inline static constexpr std::string_view idStr() { return "DesAs2"sv;}
	DesAs2(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From MLauncher
	virtual bool Run(int aCount = 0, int aIdleCount = 0) override;
	// From MDesObserver
	virtual void onActivated(MDesSyncable* aComp) override;
	// From MDesSyncable
	virtual void update() override;
	virtual void confirm() override;
	virtual void setUpdated() override {}
	virtual void setActivated() override;
    protected:
	bool mRunning;
        MDesSyncable* mSubsys = nullptr;
	static const GUri K_OutpUri;
	static const GUri K_SsysUri;
	static const GUri K_SsysInitUri;
};

/** @brief Binded State 
 * It doesnt include input but gets bounded with input by owner
 * So with inputs it expose simmetric dipole structure instead of asymmetric one in case of state
 * This allows simplify "custom" systems (like DAdp) having pair CpStateInp-DState
 * instead of ExtdStateInp-State. I.e. the system exposes input binded to state as it's own.
 * */
class BState: public ConnPoint, public MDesSyncable, public MDesInpObserver, public MDVarGet, public MDVarSet
{
    public:
        using TBase = ConnPoint;
        using TDesSyncCp = NCpOnp<MDesSyncable, MDesObserver>;  /*!< DES syncable connpoint */
    public:
	inline static constexpr std::string_view idStr() { return "BState"sv;}
	BState(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual ~BState();
	// From Node.MIface
	MIface* MNode_getLif(TIdHash aId) override;
	// From Node
	MIface* MOwner_getLif(TIdHash aId) override;
	GUri parentUri() const override { return string(idStr());}
	// From MVert
	TDir getDir() const override { return EOut;}
	MIface* MVert_getLif(TIdHash aId) override;
        // From MConnPoint
        //bool bind(MNpc* aPair) override;
	// From MDesSyncable
	virtual string MDesSyncable_Uid() const override {return getUid<MDesSyncable>();}
	virtual void MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual MIface* MDesSyncable_getLif(TIdHash aId) override { return nullptr; }
	virtual void update() override;
	virtual void confirm() override;
	virtual void setUpdated() override;
	virtual void setActivated() override;
	virtual bool isActive() const override { return false;}
	virtual int countOfActive(bool aLocal = false) const override { return 0;}
        TDesSyncableCp* desSyncableCp() override { return &mDesSyncCp;}
	// From MDesInpObserver
	virtual string MDesInpObserver_Uid() const {return getUid<MDesInpObserver>();}
	virtual void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual void onInpUpdated() override;
	// From Node.MContentOwner
	int contCount() const override { return Node::contCount() + 1;}
	bool getContentId(int aIdx, string& aRes) const override;
	bool getContent(const string& aId, string& aRes) const override;
	bool setContent(const string& aId, const string& aData) override;
	void onContentChanged(const string& aId) override {}
	// From MDVarGet
	virtual string MDVarGet_Uid() const override {return getUid<MDVarGet>();}
	virtual string VarGetIfid() const override;
	virtual MIface* DoGetDObj(const char *aName) override {return nullptr;}
	virtual DtBase* VDtGet(const string& aType) override;
	// From MDVarSet
	virtual string MDVarSet_Uid() const override {return getUid<MDVarSet>();}
	virtual string VarGetSIfid();
	virtual const bool VDtSet(const DtBase& aData) override;
        // From MConnPoint
        MNpc* bP() override { return &mInpBp;}
    public:
	static const string KCont_Value;
    protected:
	MDVarGet* GetInp();
	bool updateWithContValue(const string& aCnt);
        void notifyInpsUpdated();
    protected:
	// From MNode
	MIface* MOwned_getLif(TIdHash aId) override;
	// Local
	DtBase* CreateData(const string& aSrc);
        inline MDesObserver* desObserver() { return mDesSyncCp.mPair ? mDesSyncCp.mPair->provided() : nullptr;}
    protected:
        TDesSyncCp mDesSyncCp; //<! DES Syncable connpoint
	DtBase* mPdata;   //<! Preparing (updating) phase data
	DtBase* mCdata;   //<! Confirming phase data
	bool mUpdNotified;  //<! Sign of that State notified observers on Update
	bool mActNotified;  //<! Sign of that State notified observers on Activation
	bool mInpValid;
	bool mStDead;     //<! Sign of State destructed, needs to avoid callbacks initialted by bases */
        NpcOnp mInpBp; //<! Input binding point
	MDesSyncable* mMDesSyncable = nullptr;
	MDesInpObserver* mMDesInpObserver = nullptr;
	MDVarGet* mMDVarGet = nullptr;
	MDVarSet* mMDVarSet = nullptr;
};






// Helpers

template <typename T> bool GetSData(MNode* aDvget, T& aData)
{
    bool res = false;
    MDVarGet* vget = aDvget ? aDvget->lIf(vget) : nullptr;
    if (vget) {
	const Sdata<T>* data = vget->DtGet(data);
	if (data) {
	    aData = data->mData;
	    res = data->IsValid();
	}
    }
    return res;
}

// TODO get rid of returned data
template <typename T> bool GetGData(MNode* aDvget, T& aData)
{
    bool res = false;
    MDVarGet* vget = aDvget ? aDvget->lIf(vget) : nullptr;
    if (vget) {
	const T* data = vget->DtGet(data);
	if (data) {
	    aData = *data;
	    res = data->IsValid();
	}
    }
    return res;
}


///// Embedded DES elements support

class DesEIbb;
class DesEOstb;
class DesEParb;

/** @brief Local iface of the host of DES embedded elements
 * */
class IDesEmbHost
{
    public:
	virtual void registerIb(DesEIbb* aIap) = 0;
	virtual void registerOst(DesEOstb* aOst) = 0;
	virtual void registerPar(DesEParb* aPar) { assert(false);}
	virtual bool meetsLogLev(int aLev) const = 0;
	virtual void logEmb(int aCtg, const TLog& aRec) =0;
};

/** @brief Double buffering Input base, ref ds_dee_ssi
 * This is lightweight solution for embedding states into some DES syncable 
 * Another option of embedding the state would be separate simple input access point and pseudo state
 * but this option is more complicated.
 * Note: this embedding approach "erodes" strong DES design, so needs to be used carefully
 *
 * @parem Th  type of host
 * */
// TODO seems not used anymore. Remove?
class DesEIbb: public MDesInpObserver, public MDesSyncable
{
    public:
	DesEIbb(MNode* aHost, const string& aInpUri, const string& aCpType = string(CpStateInp::idStr())):
	    mHost(aHost), mUri(aInpUri), mCpType(aCpType), mUpdated(false), mActivated(true),
	    mChanged(false), mValid(false) { eHost()->registerIb(this);}
	string getUri() const { return mUri;}
	// From MDesInpObserver
	void onInpUpdated() override { setActivated();}
	string MDesInpObserver_Uid() const override { return string(MDesInpObserver::idStr());}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual MIface* MDesInpObserver_getLif(TIdHash aId) {
	    return checkLif2(aId, mMDesInpObserver);
	}
	// From MDesSyncable
	string MDesSyncable_Uid() const override {return string(MDesSyncable::idStr());} 
	void MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	MIface* MDesSyncable_getLif(TIdHash aId) override { return nullptr; }
	void update() override { mChanged = false;}
	void setUpdated() override { mUpdated = true; sHost()->setUpdated();}
	void setActivated() override { mActivated = true; auto shost = sHost(); if (shost) shost->setActivated();}
	bool isActive() const override { return false;}
	int countOfActive(bool aLocal = false) const override { return 1;}
        TDesSyncableCp* desSyncableCp() override { return nullptr;}
    protected:
	template <typename S> string toStr(const S& aData) { return to_string(aData); }
	string toStr(const string& aData) { return aData; }
	string toStr(MNode*& aData) { return (aData ? aData->Uid() : "nil"); }
	string toStr(const DtBase& aData) { return aData.ToString(true); }
	string toStr(const DGuri& aData) { return aData.ToString(true); }
	MDesSyncable* sHost() { MDesSyncable* ss = mHost->lIf(ss); return ss;}
	IDesEmbHost* eHost() { return dynamic_cast<IDesEmbHost*>(mHost);}
	template<class T> inline MIface* checkLif2(TIdHash aId, T*& aPtr) {
	    return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
	}
    public:
	MNode* mHost;  /*!< Host */
	string mUri;  /*!< Input URI */
	string mCpType; /*!< Type of input connpoint */
	bool mActivated; /*!< Indication of data is active (to be updated) */
	bool mUpdated; /*!< Indication of data is updated */
	bool mChanged; /*!< Indication of data is changed */
	bool mValid;  /*!< Indication of data validity */
	MDesInpObserver* mMDesInpObserver = nullptr;
};

#define LOGEMB(aLevel, rec) \
    if (this->eHost()->meetsLogLev(aLevel)) {\
	this->eHost()->logEmb(aLevel, rec);\
    }\


/** @brief Input buffered with typed data
 * @param  T  data type 
 * */
template <typename T> class DesEIbt: public DesEIbb
{
    public:
	DesEIbt(MNode* aHost, const string& aInpUri, const string& aCpType = string(CpStateInp::idStr())): DesEIbb(aHost, aInpUri, aCpType) {}
	// From MDesSyncable
	virtual void confirm() override;
	// Local
	T& data() {return mCdt;}
    public:
	T mUdt, mCdt;  /*!< Updated and confirmed data */
};

template <typename T> void DesEIbt<T>::confirm()
{
    if (mUdt != mCdt) {
	//eHost()->logEmb(TLogRecCtg::EDbg, TLog(mHost) + "[" + mUri + "] Updated: [" + toStr(mCdt) + "] -> [" + toStr(mUdt) + "]");
	LOGEMB(TLogRecCtg::EDbg, (TLog(mHost) + "[" + mUri + "] Updated: [" + toStr(mCdt) + "] -> [" + toStr(mUdt) + "]"));
	mCdt = mUdt; mChanged = true;
    } else mChanged = false;
    mUpdated = false;
}

/** @brief Input buffered base data DtBase
 * @param  T  data type
 * */
// TODO to migrate to base data buffered input
template <typename T> class DesEIbd: public DesEIbt<T>
{
    public:
	using TP = DesEIbb;
	DesEIbd(MNode* aHost, const string& aInpUri): DesEIbt<T>(aHost, aInpUri) {}
	// From MDesSyncable
	virtual void update() override;
};

template <typename T> void DesEIbd<T>::update()
{
    DesEIbb::update();
    MNode* inp = TP::mHost->getNode(TP::mUri);
    if (inp) TP::mValid = GetGData(inp, this->mUdt);
    if (!TP::mValid) {
	//this->eHost()->logEmb(TLogRecCtg::EDbg, TLog(TP::mHost) + "Cannot get input [" + this->mUri + "]");
	LOGEMB(TLogRecCtg::EDbg, (TLog(TP::mHost) + "Cannot get input [" + this->mUri + "]"));
    } else { this->mActivated = false; this->setUpdated(); }
}


/** @brief Input buffered Sdata based
 * @param  T  data type 
 * */
// TODO to migrate to base data buffered input 
template <typename T> class DesEIbs: public DesEIbt<T>
{
    public:
	using TP = DesEIbb;
	DesEIbs(MNode* aHost, const string& aInpUri): DesEIbt<T>(aHost, aInpUri) {}
	// From MDesSyncable
	virtual void update() override;
};

template <typename T> void DesEIbs<T>::update()
{
    DesEIbb::update();
    MNode* inp = TP::mHost->getNode(TP::mUri);
    if (inp) TP::mValid = GetSData(inp, this->mUdt);
    if (!TP::mValid) {
	//this->eHost()->logEmb(TLogRecCtg::EDbg, TLog(TP::mHost) + "Cannot get input [" + this->mUri + "]");
	LOGEMB(TLogRecCtg::EDbg, (TLog(TP::mHost) + "Cannot get input [" + this->mUri + "]"));
    } else { this->mActivated = false; this->setUpdated(); }
}


/** @brief Output state - embedded pseudo-state "connnected" to host output
 * */
class DesEOstb: public MDVarGet {
    public:
	DesEOstb(MNode* aHost, const string& aCpUri, const string& aCpType = string(CpStateOutp::idStr())):
	    mHost(aHost), mCpUri(aCpUri), mCpType(aCpType) { eHost()->registerOst(this);}
	string getCpUri() const { return mCpUri;}
	// From MDVarGet
	virtual string MDVarGet_Uid() const override {return string(MDVarGet::idStr());}
    public:
	void NotifyInpsUpdated();
    protected:
	MDesSyncable* sHost() { MDesSyncable* ss = mHost->lIf(ss); return ss;}
	IDesEmbHost* eHost() { return dynamic_cast<IDesEmbHost*>(mHost);}
    public:
	MNode* mHost;
	string mCpUri;  /*!< Output URI */
	string mCpType; /*!< Type of input connpoint */
};


/** @brief Output state with Sdata
 * */
template <typename T> class DesEOsts: public DesEOstb {
    public:
	using Tdata = Sdata<T>;
	DesEOsts(MNode* aHost, const string& aCpUri): DesEOstb(aHost, aCpUri) {}
	// From MDVarGet
	virtual string VarGetIfid() const override {return Tdata::TypeSig();}
	virtual MIface* DoGetDObj(const char *aName) override { return nullptr;}
	virtual const DtBase* VDtGet(const string& aType) override { return &mData;}
	// Local
	void updateData(const T& aData);
    public:
	Tdata mData;
};

// TODO to use Tdata as arg
template <typename T>
void DesEOsts<T>::updateData(const T& aData)
{
    if (aData != mData.mData || !mData.IsValid()) {
	Tdata newData(aData);
	//eHost()->logEmb(TLogRecCtg::EDbg, TLog(mHost) + "[" + mCpUri + "] Updated: [" + mData.ToString() + "] -> [" + newData.ToString() + "]");
	LOGEMB(TLogRecCtg::EDbg, (TLog(mHost) + "[" + mCpUri + "] Updated: [" + mData.ToString() + "] -> [" + newData.ToString() + "]"));
	mData.mData = aData; mData.mValid = true;
	NotifyInpsUpdated();
    }
}

/** @brief Output state with generic data
 * */
template <typename T> class DesEOst: public DesEOstb {
    public:
	using Tdata = T;
	DesEOst(MNode* aHost, const string& aCpUri): DesEOstb(aHost, aCpUri) {}
	// From MDVarGet
	virtual string VarGetIfid() const override {return Tdata::TypeSig();}
	virtual MIface* DoGetDObj(const char *aName) override { return nullptr;}
	virtual const DtBase* VDtGet(const string& aType) override { return &mData;}
	// Local
	void updateData(const T& aData);
	void updateInvalid();
    public:
	Tdata mData;
};

template <typename T>
void DesEOst<T>::updateData(const T& aData)
{
    if (aData != mData) {
	//eHost()->logEmb(TLogRecCtg::EDbg, TLog(mHost) + "[" + mCpUri + "] Updated: [" + mData.ToString() + "] -> [" + aData.ToString() + "]");
	LOGEMB(TLogRecCtg::EDbg, (TLog(mHost) + "[" + mCpUri + "] Updated: [" + mData.ToString() + "] -> [" + aData.ToString() + "]"));
	mData = aData;
	NotifyInpsUpdated();
    }
}

template <typename T>
void DesEOst<T>::updateInvalid()
{
    if (mData.IsValid()) {
	T prevData = mData;
	mData.mValid = false;
	//eHost()->logEmb(TLogRecCtg::EDbg, TLog(mHost) + "[" + mCpUri + "] Updated: [" + prevData.ToString() + "] -> [" + mData.ToString() + "]");
	LOGEMB(TLogRecCtg::EDbg, (TLog(mHost) + "[" + mCpUri + "] Updated: [" + prevData.ToString() + "] -> [" + mData.ToString() + "]"));
	NotifyInpsUpdated();
    }
}

#if 0
// Seems not used anymore

/** @brief Input access point base
 * */
class DesIapb : public MDesInpObserver
{
    public:
        DesIapb(const string& aName): mName(aName) {}
        void Construct(CpStateInp* aInp);
        virtual bool updateData() = 0;
	// From MDesInpObserver
	string MDesInpObserver_Uid() const override {return mName + Ifu::KUidSep + string(MDesInpObserver::idStr());}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
    public:
        string mName;    /*!< Iap name */
        //bool mUpdated = false;
        Vert* mInp = nullptr;
};

/** @brief Input access point operating with DtBase
 * @param  T  data type 
 * */
template <class T>
class DesIap: public DesIapb {
    public:
        DesIap(const string& aName): DesIapb(aName) {}
        // Local
        virtual bool updateData() override;
	void onInpUpdated() override {
            auto vget = mInp->mPairs.empty() ? nullptr : mInp->mPairs.at(0)->lIft<MDVarGet>();
            if (vget) {
                const DtBase* pdata = vget->VDtGet(T::TypeSig());
                if (pdata) {
                    //mUpdated = mData != *pdata;
                    mData = *pdata;
                }
            }
        }
    public:
        T mData;
};
#endif





#if 0

/** @brief DES affecting Parameter base (not completed)
 * */
class DesEParb
{
    public:
        DesEParb(MNode* aHost, const string& aUri): mHost(aHost), mUri(aUri) { eHost()->registerPar(this);}
    protected:
        IDesEmbHost* eHost() { return dynamic_cast<IDesEmbHost*>(mHost);}
	void updatePar(const MContent* aCont);
    protected:
	MNode* mHost;
	const string mUri;  //!< Paremeter's URI
};

#endif


/* @brief DES context supplier, ver 2 (socket based), ref ds_dctx_v2
 * */
class DesCtxSpl : public Verte, public MDesCtxSpl
{
    public:
	using TSplCp = NCpOmnp<MDesCtxSpl, MDesCtxCsm>;  /*!< Supplier connpoint */
    public:
	inline static constexpr std::string_view idStr() { return "DesCtxSpl"sv;}
	DesCtxSpl(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From Node
	MIface* MNode_getLif(TIdHash aId) override;
	MIface* MOwned_getLif(TIdHash aId) override;
	void onOwnedAttached(MOwned* aOwned) override;
	void onOwnedDetached(MOwned* aOwned) override;
        // From MVert
	MIface* MVert_getLif(TIdHash aId) override;
	// From MDesCtxSpl
	string MDesCtxSpl_Uid() const override {return getUid<MDesCtxSpl>();}
	void MDesCtxSpl_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	MIface* MDesCtxSpl_getLif(TIdHash aId) override;
	virtual string getId() const { return name();}
	bool bindCtx(const string& aCtxId, MVert* aCtx) override;
    protected:
	TSplCp mSplCp;  /*!< Ctx supplier CP */
	MDesCtxSpl* mMDesCtxSplPtr = nullptr;
};


/** @brief DES context consumer
 * */
class DesCtxCsm : public Verte, public MDesCtxCsm, public MDesSyncable
{
    public:
	using TCsmCp = NCpOnp<MDesCtxCsm, MDesCtxSpl>;
        using TDesSyncCp = NCpOnp<MDesSyncable, MDesObserver>;  /*!< DES syncable connpoint */
    public:
	inline static constexpr std::string_view idStr() { return "DesCtxCsm"sv;}
	DesCtxCsm(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From Node
	MIface* MNode_getLif(TIdHash aId) override;
	MIface* MOwned_getLif(TIdHash aId) override;
	void onOwnedAttached(MOwned* aOwned) override;
	void onOwnedDetached(MOwned* aOwned) override;
	// From MDesCtxCsm
	MIface* MDesCtxCsm_getLif(TIdHash aId) override;
	string MDesCtxCsm_Uid() const override {return getUid<MDesCtxCsm>();}
	void MDesCtxCsm_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual string getId() const { return name();}
	bool bindCtx(const string& aCtxId, MVert* aCtx) override;
        // From Vert
        void onConnected(MVert* aPair) override;
        void onDisconnected() override;
	// From MDesSyncable
	virtual string MDesSyncable_Uid() const override {return getUid<MDesSyncable>();}
	virtual void MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	virtual MIface* MDesSyncable_getLif(TIdHash aId) override { return nullptr; }
	virtual void update() override {}
	virtual void confirm() override;
	virtual void setUpdated() override {}
	virtual void setActivated() override;
	virtual bool isActive() const override { return false;}
	virtual int countOfActive(bool aLocal = false) const override { return 0;}
        TDesSyncableCp* desSyncableCp() override { return &mDesSyncCp;}
    protected:
        // Local
	bool bindAllCtx();
    protected:
        bool mBound = false;
	TCsmCp mCsmCp;  /*!< Consumer Cp */
        TDesSyncCp mDesSyncCp; //<! DES Syncable connpoint
	MDesCtxCsm* mMDesCtxCsm = nullptr;
	MDesSyncable* mMDesSyncable = nullptr;
};

#if 0

/** @brief DES Input demultiplexor, ref ds_des_idmux
 * */
class DesInpDemux : public Des
{
    public:
	static const char* Type() { return "DesInpDemux";};
	DesInpDemux(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From Unit.MIfProvOwner
	virtual void resolveIfc(const string& aName, MIfReq::TIfReqCp* aReq) override;
	// From MDesSyncable
	virtual void confirm() override;
    protected:
	void AddInput(const string& aName);
	void AddOutput(const string& aName);
	int getIfcCount();
    protected:
	int mIfcCount; //!< Ifaces count
	int mIdx; //!< Index of input iface
};
#endif

#endif
