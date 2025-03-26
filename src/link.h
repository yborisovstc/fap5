#ifndef __FAP5_LINK_H
#define __FAP5_LINK_H

#include "mlink.h"
#include "node.h"


/** @brief One-way relation 
 * */
class Link : public Node, public MLink
{
    public:
	inline static constexpr std::string_view idStr() { return "Link"sv;}
	Link(const string &aType, const string &aName, MEnv* aEnv);
	virtual ~Link();
	// From MNode.MIface
	virtual MIface* MNode_getLif(const char *aType) override;
	// From MLink.MIface
	string MLink_Uid() const override { return getUid<MLink>();}
	MIface* MLink_getLif(const char *aType) override;
	void MLink_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	bool connect(MNode* aPair) override;
	bool disconnect(MNode* aPair) override;
	MNode* pair() override;
    protected:
	MNode* mPair;
	MLink* mMLinkPtr = nullptr;
};

#endif
