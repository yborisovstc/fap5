#ifndef __FAP5_DESADP_H
#define __FAP5_DESADP_H

#include "des.h"


class DAdp: public Des, public MSystExploring, public IDesEmbHost
{
    public:
	inline static constexpr std::string_view idStr() { return "DAdp"sv;}
	DAdp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual ~DAdp();
    public:
	// From Node
	MIface* MNode_getLif(TIdHash aId) override;
	// From MDesSyncable
	void update() override;
	void confirm() override;
	// From IDesEmbHost
	void registerIb(DesEIbb* aIap) override;
	void registerOst(DesEOstb* aItem) override { assert(false);}
	bool meetsLogLev(int aLev) const override { return Logger()->MeetsLevel(aLev) && isLogLevel(aLev); }
	// TODO to avoid using it - decreases performance via TLog ctor
	void logEmb(int aCtg, const TLog& aRec) override { 
	    if (isLogLevel(aCtg)) {
		const_cast<TLog&>(aRec).SetCtg(aCtg); Logger()->Write(aRec);
	    }
	}
	// From MContentOwner
	int contCount() const override;
	bool getContentId(int aIdx, string& aRes) const override;
	bool getContent(const string& aId, string& aRes) const override;
	bool setContent(const string& aId, const string& aData) override;
	void onContentChanged(const string& aId) override;
	// From MSystExploring
	string MSystExploring_Uid() const override {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override; 
	void onMagChanged() override;
	MSystExploring::TCp* getCp() override { return nullptr;}
	// From MSystExplorable
	MNode* getMag() override;
    protected:
	void Construct() override;
	void notifyMagChanged();
	MNode* getMagBase();
	void updateMag();
    protected:
	CpSystExploring* mCpExpl = nullptr;
	MSystExploring* mMSystExploring = nullptr;
	// TODO not used atm!
	string mContMagUri;
	vector<DesEIbb*> mIbs; /*!< Inputs buffered registry */
	DesEIbd<DGuri> mIbMagUri;   //!< Buffered input "MagUri"
	MNode* mMagBase = nullptr;
	MNode* mMag = nullptr;
    protected:
	static const string KCpExplName;
};


#endif

