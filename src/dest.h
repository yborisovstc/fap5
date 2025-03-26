#ifndef __FAP5_DEST_H
#define __FAP5_DEST_H

#include "mdata.h"
#include "mdes.h"
#include "msyst.h"
#include "func.h"
#include "des.h"
#include "rmutdata.h"


/** @brief Transition function base 
 * */ 
class TrBase: public CpStateOutp, public MDVarGet, public MDesInpObserver
{
    public:
	static const char* Type() { return "TrBase";}
	//static vector<GUri> getParentsUri();
	TrBase(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From Node
	MIface* MNode_getLif(TIdHash aId) override;
	MIface* MOwner_getLif(TIdHash aId) override;
	//vector<GUri> parentsUri() const override { return getParentsUri(); }
	// From MVert
	MIface *MVert_getLif(TIdHash aId) override;
	// From MDesInpObserver
	string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>();}
	void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	void onInpUpdated() override;
	// From MDVarGet
	string MDVarGet_Uid() const override { return getUid<MDVarGet>();}
	MIface* DoGetDObj(const char *aName) override { return nullptr;}
	const DtBase* VDtGet(const string& aType) override;
    protected:
	// Local
	/** @brief Gets input data
	 * */
	template <class T> inline const T* GetInpData(CpStateInp* aInp, const T* aData);
	virtual const DtBase* doVDtGet(const string& aType) { return nullptr;}
	virtual CpStateInp* GetFinp(int aId) { return nullptr; }
	int InpIcCount(const CpStateInp* aInp);
	int InpIcCount(int aInpId);
        MDVarGet* InpIc(const CpStateInp* aInp, int aIcId = 0);
        MDVarGet* InpIc(int aInpId, int aIcId = 0);
    protected:
	CpStateInp* AddInput(const string& aName);
	bool mCInv;              //!< Sign of data cache invalidated
	const DtBase* mResp = nullptr;
	MDVarGet* mMDVarGet = nullptr;
	MVert* mMVert = nullptr;
	MDesInpObserver* mMDesInpObserver = nullptr;
};

template <class T> inline const T* TrBase::GetInpData(CpStateInp* aInp, const T* aData)
{
    const T* data = nullptr;

    MDVarGet* get =  aInp->mPairs.empty() ? nullptr : aInp->mPairs.at(0)->lIft<MDVarGet>();
    data = get ? get->DtGet(data) : nullptr;
    if (get && !data) { // There already is the log in GetInps
        LOGN(EDbg, "Cannot get input [" + aInp->name() + "] data");
    }
    return data;
}

// Agent base of Var transition function
class TrVar: public TrBase, public Func::Host
{
    public:
	static const char* Type() { return "TrVar";};
	TrVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From MDVarGet
	virtual string MDVarGet_Uid() const override { return getUid<MDVarGet>();}
	virtual void MDVarGet_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	virtual string VarGetIfid() const override;
	// From Func::Host
	string GetInpUri(int aId) const override;
	inline void log(int aCtg, const string& aMsg) override { LOGN(aCtg, aMsg); }
	bool hostIsLogLevel(int aLevel) const override { return isLogLevel(aLevel);}
	string getHostUri() const override { return getUriS(nullptr);}
	int GetInpIcCount(int aInpId) override;
        MDVarGet* GetInpIc(int aInpId, int aIcId = 0) override;
    protected:
	// Local
	virtual void Init(const string& aIfaceName);
	/** @brief Gets value of MDVarGet MDtGet input */
	template<typename T> bool DtGetInp(T& aData, const string& aInpName);
	/** @brief Gets value of MDVarGet MDtGet Sdata<T> input */
	template<typename T> bool DtGetSdataInp(T& aData, const string& aInpName);
	MDVarGet* GetInp(int aInpId);
	virtual const DtBase* doVDtGet(const string& aType) override;
	virtual int GetInpCpsCount() const {return 0;}
    protected:
	Func* mFunc;
};

/** @brief Transition "Addition of Var data, negative inp, multi-connecting inputs"
 * Note: inputs validity isn't detected proreply, ref iss_014
 * 
 * */
class TrAddVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrAddVar"sv;}
	TrAddVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From TrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
	int GetInpCpsCount() const override {return 2;}
    protected:
	const static string K_InpInp;
	const static string K_InpInpN;
	CpStateInp* mInp;
	CpStateInp*  mInpN;
};


/** @brief Transition "Addition of Var data, single connection inputs"
 * */
class TrAdd2Var: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrAdd2Var"sv;}
	TrAdd2Var(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	//virtual string parentName() const override { return Type(); }
	// From TrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
	int GetInpCpsCount() const override {return 2;}
    protected:
	const static string K_InpInp;
	const static string K_InpInp2;
	CpStateInp* mInp = nullptr;
	CpStateInp* mInp2 = nullptr;
};

#if 0

/** @brief Transition "Subtraction of Var data, single connection inputs"
 * */
class TrSub2Var: public TrVar
{
    public:
	static const char* Type() { return "TrSub2Var";};
	TrSub2Var(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From TrVar
	virtual void Init(const string& aIfaceName) override;
	virtual FInp* GetFinp(int aId) override;
	virtual int GetInpCpsCount() const override {return 2;}
    protected:
	const static string K_InpInp;
	const static string K_InpInp2;
	FInp mInp;
	FInp mInp2;
};


#endif


/** @brief Transition "Multiplication of Var data"
 * */
class TrMplVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrMplVar"sv;}
	TrMplVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From TrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
	int GetInpCpsCount() const override {return 2;}
    protected:
	const static string K_InpInp;
	CpStateInp* mInp = nullptr;
};

/** @brief Transition "Division of Var data"
 * */
class TrDivVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrDivVar"sv;}
	TrDivVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From TrVar
	void Init(const string& aIfaceName) override;
	CpStateInp * GetFinp(int aId) override;
	int GetInpCpsCount() const override {return 2;}
    protected:
	const static string K_InpInp;
	const static string K_InpInp2;
	CpStateInp* mInp = nullptr;
	CpStateInp* mInp2 = nullptr;
};



/** @brief Agent function "Min of Var data"
 * */
class TrMinVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrMinVar"sv;}
	TrMinVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From ATrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
    protected:
	const static string K_InpInp;
	CpStateInp* mInp = nullptr;
};


/** @brief Agent function "Max of Var data"
 * */
class TrMaxVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrMaxVar"sv;}
	TrMaxVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From ATrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
    protected:
	const static string K_InpInp;
	CpStateInp* mInp = nullptr;
};


/** @brief Transition "Compare"
 * */
class TrCmpVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrCmpVar"sv;}
	TrCmpVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	FCmpBase::TFType GetFType();
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
	int GetInpCpsCount() const override {return 2;}
    protected:
	const static string K_InpInp;
	const static string K_InpInp2;
	CpStateInp* mInp = nullptr;
	CpStateInp* mInp2 = nullptr;
};

#if 0
/** @brief Transition "Is equal"
 * */
// TODO Not completed. To complete
class TrEqVar: public TrVar
{
    public:
	static const char* Type() { return "TrEqVar";};
	TrEqVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual void Init(const string& aIfaceName) override;
	virtual FInp* GetFinp(int aId) override;
	virtual int GetInpCpsCount() const override {return 2;}
    protected:
	const static string K_InpInp;
	const static string K_InpInp2;
	FInp mInp;
	FInp mInp2;
};

#endif

/** @brief Agent function "Switcher"
 * */
class TrSwitchBool: public TrBase
{
    public:
	inline static constexpr std::string_view idStr() { return "TrSwitchBool"sv;}
	TrSwitchBool(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	const DtBase* doVDtGet(const string& aType) override;
	string VarGetIfid() const override;
    protected:
	MDVarGet* GetInp();
    protected:
	CpStateInp* mInp1 = nullptr;
	CpStateInp* mInp2 = nullptr;
	CpStateInp* mSel = nullptr;
	const static string K_InpInp1, K_InpInp2, K_InpSel;
};


/** @brief Agent function "Switcher" ver.2
 * Updated for solving desinp observing from non-selected inp, ref ds_dsps
 * Doens't improve the performance, mitigated by trans cache (ds_desopt_tdc) so
 * TrSwitchBool can be used instead.
 * */
class TrSwitchBool2: public TrBase
{
    public:
	class IobsPx : public MDesInpObserver {
	    public:
		IobsPx(TrSwitchBool2* aHost): mHost(aHost) {}
		virtual string MDesInpObserver_Uid() const {return mHost->getUid<MDesInpObserver>() + "Px";}
		virtual void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
		virtual void onInpUpdated() override { mHost->notifyInpsUpdated(this); }
		TrSwitchBool2* mHost;
	};
    friend class IobsPx;
    public:
	inline static constexpr std::string_view idStr() { return "TrSwitchBool2"sv;}
	TrSwitchBool2(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	virtual const DtBase* doVDtGet(const string& aType) override;
	virtual string VarGetIfid() const override;
    protected:
	MDVarGet* GetInp();
	void notifyInpsUpdated(const IobsPx* aPx);
    protected:
	CpStateInp* mInp1 = nullptr;
	CpStateInp* mInp2 = nullptr;
	CpStateInp* mSel = nullptr;
	const static string K_InpInp1, K_InpInp2, K_InpSel;
	bool mSelV = false;
	bool mSelected = false;
	IobsPx mPx1, mPx2;
};


class TrBool: public TrBase
{
    public:
	using TData = Sdata<bool>;
    public:
	TrBool(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From MDVarGet
	virtual string VarGetIfid() const override { return TData::TypeSig();}
	// From TrBase
	CpStateInp* GetFinp(int aId) override { return (aId == EInp) ? mInp : nullptr; }
    protected:
	CpStateInp* mInp = nullptr;
	const static string K_InpInp;
	TData mRes;
};



/** @brief Agent function "Boolena AND of Var data"
 * */
class TrAndVar: public TrBool
{
    public:
	inline static constexpr std::string_view idStr() { return "TrAndVar"sv;}
	TrAndVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): TrBool(aType, aName, aEnv) {}
	const DtBase* doVDtGet(const string& aType) override;
};

/** @brief Agent function "Boolena OR of Var data"
 * */
class TrOrVar: public TrBool
{
    public:
	inline static constexpr std::string_view idStr() { return "TrOrVar"sv;}
	TrOrVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): TrBool(aType, aName, aEnv) {}
	const DtBase* doVDtGet(const string& aType) override;
};


/** @brief Agent function "Boolean negation of Var data"
 * */
class TrNegVar: public TrBool
{
    public:
	inline static constexpr std::string_view idStr() { return "TrNegVar"sv;}
	TrNegVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): TrBool(aType, aName, aEnv) {}
	virtual const DtBase* doVDtGet(const string& aType) override;
};

/** @brief Transition agent "Convert to URI"
 * */
class TrToUriVar: public TrBase
{
    public:
	using TRes = DGuri;
	using TInp = Sdata<string>;
    public:
	inline static constexpr std::string_view idStr() { return "TrToUriVar"sv;}
	TrToUriVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	virtual string VarGetIfid() const override { return TRes::TypeSig();}
	virtual const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInp = nullptr;
	const static string K_InpInp;
	TRes mRes;
};


/** @brief Transition agent "Append"
 * */
class TrApndVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrApndVar"sv;}
	TrApndVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From ATrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
    protected:
	const static string K_InpInp1, K_InpInp2;
	CpStateInp* mInp1 = nullptr;
	CpStateInp* mInp2 = nullptr;
};

/** @brief Transition agent "Select valid"
 * */
class TrSvldVar: public TrBase
{
    public:
	inline static constexpr std::string_view idStr() { return "TrSvldVar"sv;}
	TrSvldVar(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From MDVarGet
	virtual string VarGetIfid() const override;
	virtual const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInp1 = nullptr;
	CpStateInp* mInp2 = nullptr;
	static const string K_InpInp1, K_InpInp2;
};


/** @brief Transition agent "Tail"
 * */
class TrTailVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrTailVar"sv;}
	TrTailVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From ATrVar
	virtual void Init(const string& aIfaceName) override;
	virtual CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpHead = nullptr;
	static const string K_InpInp, K_InpHead;
};

/** @brief Transition agent "Head"
 * */
class TrHeadVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrHeadVar"sv;}
	TrHeadVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From ATrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpTail = nullptr;
	static const string K_InpInp, K_InpTail;
};

/** @brief Transition agent "Head by tail length"
 * */
class TrHeadtnVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrHeadtnVar"sv;}
	TrHeadtnVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From ATrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpTailn = nullptr;
	static const string K_InpInp, K_InpTailn;
};


/** @brief Transition agent "Tail as num of elems"
 * */
class TrTailnVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrTailnVar"sv;}
	TrTailnVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From ATrVar
	void Init(const string& aIfaceName) override;
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpNum = nullptr;
	static const string K_InpInp, K_InpNum;
};


/** @brief Transition "Getting container size"
 * */
class TrSizeVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrSizeVar"sv;}
	TrSizeVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Init(const string& aIfaceName) override;
	int GetInpCpsCount() const override {return 1;}
	string VarGetIfid() const override;
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	static const string K_InpInp;
};


/** @brief Agent function "Getting component, wrapping it by Sdata"
 * */
class TrAtVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrAtVar"sv;}
	TrAtVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	void Init(const string& aIfaceName) override;
	int GetInpCpsCount() const override {return 2;}
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpIndex = nullptr;
	static const string K_InpInp, K_InpIndex;
};


/** @brief Agent function "Getting component, generic, no wrapping by Sdata"
 * */
class TrAtgVar: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrAtgVar"sv;}
	TrAtgVar(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	void Init(const string& aIfaceName) override;
	int GetInpCpsCount() const override {return 2;}
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpIndex = nullptr;
	static const string K_InpInp, K_InpIndex;
};


/** @brief Transition "Find in Vert<Pair<T>> by first element of pair"
 * */
class TrFindByP: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrFindByP"sv;}
	TrFindByP(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	void Init(const string& aIfaceName) override;
	int GetInpCpsCount() const override {return 2;}
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpSample = nullptr;
	static const string K_InpInp, K_InpSample;
};




/** @brief Transition agent "Tuple composer"
 * */
class TrTuple: public TrBase
{
    public:
	inline static constexpr std::string_view idStr() { return "TrTuple"sv;}
	TrTuple(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	// From MDVarGet
	virtual string VarGetIfid() const override;
	virtual const DtBase* doVDtGet(const string& aType) override;
	// From MOwner
	// Required to bind inputs added on model creation
	void onOwnedAttached(MOwned* aOwned) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	const static string K_InpInp;
	NTuple mRes;
};

/** @brief Transition agent "Tuple component selector"
 * */
class TrTupleSel: public TrBase
{
    public:
	inline static constexpr std::string_view idStr() { return "TrTupleSel"sv;}
	TrTupleSel(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	// From MDVarGet
	string VarGetIfid() const override;
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpComp = nullptr;
	const static string K_InpInp, K_InpComp;
};



/** @brief Transition agent "To string"
 * */
class TrTostrVar: public TrBase
{
    using TRes = Sdata<string>;
    public:
	inline static constexpr std::string_view idStr() { return "TrTostrVar"sv;}
	TrTostrVar(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	// From MDVarGet
	string VarGetIfid() const override { return TRes::TypeSig();}
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	TRes mRes;
	CpStateInp* mInpInp = nullptr;
	const static string K_InpInp;
};


/** @brief Transition "Input selector"
 * */
class TrInpSel: public TrBase
{
    public:
	inline static constexpr std::string_view idStr() { return "TrInpSel"sv;}
	TrInpSel(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	// From MDVarGet
	string VarGetIfid() const override;
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	MDVarGet* GetInp();
    protected:
	CpStateInp* mInpInp = nullptr;
	CpStateInp* mInpIdx = nullptr;
	const static string K_InpInp, K_InpIdx;
};

/** @brief Transition "Inputs counter"
 * */
class TrInpCnt: public TrBase
{
    public:
	using TRes = Sdata<int>;
	inline static constexpr std::string_view idStr() { return "TrInpCnt"sv;}
	TrInpCnt(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct();
	// From MDVarGet
	string VarGetIfid() const override;
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	TRes mRes;
	const static string K_InpInp;
};

/** @brief Agent function "Pair composer"
 * */
class TrPair: public TrVar
{
    public:
	inline static constexpr std::string_view idStr() { return "TrPair"sv;}
	TrPair(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	void Init(const string& aIfaceName) override;
	int GetInpCpsCount() const override {return 2;}
	CpStateInp* GetFinp(int aId) override;
    protected:
	CpStateInp* mInpFirst = nullptr;
	CpStateInp* mInpSecond = nullptr;
	const static string K_InpFirst, K_InpSecond;
};



/** @brief Agent functions "Mut composer" base
 * */
class TrMut: public TrBase
{
    public:
	static const char* Type() { return "TrMut";};
	TrMut(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual string GetInpUri(int aId) const {return string();}
	// From MDVarGet
	virtual string VarGetIfid() const override;
    protected:
	DMut mRes;  /*<! Cached result */
};


/** @brief Agent function "Mut Node composer"
 * */
class TrMutNode: public TrMut
{
    public:
	enum { EInpName, EInpParent };
    public:
	inline static constexpr std::string_view idStr() { return "TrMutNode"sv;}
	TrMutNode(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	virtual const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpParent = nullptr;
	CpStateInp* mInpName = nullptr;
	static const string K_InpParent, K_InpName;
};

/** @brief Agent function "Mut connect composer"
 * */
class TrMutConn: public TrMut
{
    public:
	using TInp = Sdata<string>;
	inline static constexpr std::string_view idStr() { return "TrMutConn"sv;}
	TrMutConn(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpCp1 = nullptr;
	CpStateInp* mInpCp2 = nullptr;
	static const string K_InpCp1, K_InpCp2;
};

/** @brief Agent function "Mut content composer"
 * */
class TrMutCont: public TrMut
{
    public:
	inline static constexpr std::string_view idStr() { return "TrMutCont"sv;}
	TrMutCont(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpName = nullptr;
	CpStateInp* mInpValue = nullptr;
	CpStateInp* mInpTarget = nullptr;
	static const string K_InpName, K_InpValue, K_InpTarget;
};


/** @brief Agent function "Mut disconnect composer"
 * */
class TrMutDisconn: public TrMut
{
    public:
	using TInp = Sdata<string>;
	inline static constexpr std::string_view idStr() { return "TrMutDisconn"sv;}
	TrMutDisconn(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpCp1 = nullptr;
	CpStateInp* mInpCp2 = nullptr;
	static const string K_InpCp1, K_InpCp2;
};



/** @brief Agent function "Chromo composer"
 * */
class TrChr: public TrBase
{
    public:
	inline static constexpr std::string_view idStr() { return "TrChr"sv;}
	TrChr(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From MDVarGet
	string VarGetIfid() const override;
	const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpBase = nullptr;
	CpStateInp* mInpMut = nullptr;
	DChr2 mRes;
	static const string K_InpBase, K_InpMut;
};


/** @brief Agent function "Chromo composer from chromo"
 * */
class TrChrc: public TrBase
{
    public:
	inline static constexpr std::string_view idStr() { return "TrChrc"sv;}
	TrChrc(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From MDVarGet
	virtual string VarGetIfid() const override;
	virtual const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	DChr2 mRes;
	static const string K_InpInp;
};


/** @brief Transition "Data is valid"
 * */
class TrIsValid: public TrBase
{
    public:
	using TRes = Sdata<bool>;
    public:
	inline static constexpr std::string_view idStr() { return "TrIsValid"sv;}
	TrIsValid(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From MDVarGet
	virtual string VarGetIfid() const override { return TRes::TypeSig(); }
	virtual const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	TRes mRes;
	static const string K_InpInp;
};

/** @brief Transition "Data is invalid"
 * */
class TrIsInvalid: public TrBase
{
    public:
	using TRes = Sdata<bool>;
    public:
	inline static constexpr std::string_view idStr() { return "TrIsInvalid"sv;}
	TrIsInvalid(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From MDVarGet
	virtual string VarGetIfid() const override { return TRes::TypeSig(); }
	virtual const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpInp = nullptr;
	TRes mRes;
	static const string K_InpInp;
};



#if 0
/** @brief Transition "Type enforcing"
 * !! NOT COMPLETED
 * */
class TrType: public TrBase
{
    public:
	static const char* Type() { return "TrType";};
	TrType(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual string VarGetIfid() const override;
    protected:
	static const string K_InpInp;
};
#endif


/** @brief Transition "Hash of data", ref ds_dcs_iui_tgh
 * */
class TrHash: public TrBase
{
    public:
	using TRes = Sdata<int>;
    public:
	inline static constexpr std::string_view idStr() { return "TrHash"sv;}
	TrHash(const string& aType, const string& aName = string(), MEnv* aEnv = NULL);
	void Construct() override;
	// From MDVarGet
	virtual string VarGetIfid() const override { return TRes::TypeSig();}
	virtual const DtBase* doVDtGet(const string& aType) override;
    protected:
	CpStateInp* mInpInp;
	TRes mRes;
	static const string K_InpInp;
};


#endif
