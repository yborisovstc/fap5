
#ifndef __FAP5_MNT_H
#define __FAP5_MNT_H

#include "syst.h"
#include "env.h"
#include "mmntp.h"

// TODO With using Explorable capability of system
// MMntp iface is not required anymore. To remove.
/** @brief Agent of mounting point
 * */
class AMntp : public Syst, public MMntp
{
    public:
	inline static std::string_view idStr() { return "AMntp"sv;}
	AMntp(const string &aType, const string &aName, MEnv* aEnv);
	virtual ~AMntp();
	// From MMntp
	virtual string MMntp_Uid() const override {return getUid<MMntp>();}
	virtual MIface* MMntp_getLif(TIdHash aId) override {return nullptr;}
	virtual MNode* root() const override;
	// From Node
	virtual MIface* MNode_getLif(TIdHash aId) override;
	// From Node.MContentOwner
	int contCount() const override;
	bool getContentId(int aIdx, string& aRes) const override;
	bool getContent(const string& aId, string& aRes) const override;
	bool setContent(const string& aId, const string& aData) override;
	void onContentChanged(const string& aId) override;
	// From MSystExplorable
	MNode* getMag() override;
    protected:
	bool CreateModel(const string& aSpecPath);
    private:
	Env* mMdlEnv;
	MMntp* mMMntpPtr = nullptr;
        string mCntEnvVar; 
};



#endif
