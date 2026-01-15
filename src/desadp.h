#ifndef __FAP5_DESADP_H
#define __FAP5_DESADP_H

#include "des.h"


/** @brief DAdp ver.2, redesigned to use "standard" comps
 * got rid of specialized inp DesEIbb and use std inputs and states instead
 * Whe weak point here is that we need "binding" solution between DAdp and states
 * (e.g MagUriHandler)
 * */
class DAdp: public Des, public MSystExploring
{
    public:
	// TODO Do we need this personal binder? Maybe better to bind directly to DAdp
	class MagUriHandler : public CpStateInp, public MDesInpObserver {
	    public:
	    MagUriHandler(const string& aName, MEnv* aEnv, DAdp* aHost): CpStateInp(string(), aName, aEnv), mHost(aHost) {};
            MIface *MVert_getLif(TIdHash aId) override;
	    // From MDesInpObserver
	    string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>();}
	    void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	    void onInpUpdated() override;
	    DAdp* mHost;
            MDesInpObserver* mMDesInpObserver = nullptr;
	};
	friend MagUriHandler;
    public:
	inline static constexpr std::string_view idStr() { return "DAdp"sv;}
	DAdp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual ~DAdp();
    public:
	// From MNode
	MIface* MNode_getLif(TIdHash aId) override;
	// From MSystExploring
	string MSystExploring_Uid() const override {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override; 
	void onMagChanged() override;
	MNpc* getCp() override { return nullptr;}
	// From MSystExplorable
	MNode* getMag() override;
    protected:
	void Construct() override;
	void notifyMagChanged();
	MNode* getMagBase();
	void updateMag();
    protected:
	NpcOnp mCpExpl;
	MNode* mMagBase = nullptr;
	MNode* mMag = nullptr;
	MNode* mStMagUri = nullptr;
	MNode* mHdlr = nullptr;
    protected:
	MSystExploring* mMSystExploring = nullptr;
    protected:
	static const string KCpExplName;
};


#endif

