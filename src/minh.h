
#ifndef __FAP5_MINH_H
#define __FAP5_MINH_H

#include "miface.h"
#include "nconn.h"


class MChild;
class MNode;

/** @brief Native net Parent interface
 * */
class MParent : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MParent"sv;}
	inline static constexpr TIdHash idHash() { return 0x8f2ba00fd4cbd280;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MParent_Uid();}
	virtual string MParent_Uid() const = 0;
	MIface* getLif(TIdHash aId) override { return MParent_getLif(aId);}
	virtual MIface* MParent_getLif(TIdHash aId) = 0;
	void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MParent_doDump(aLevel, aIdt, std::cout);}
	virtual void MParent_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual void onChildDeleting(MChild* aChild) = 0;
	virtual bool onChildRenaming(MChild* aChild, const string& aNewName) = 0;
	/** @brief Create heir as parent */
	virtual MNode* createHeir(const string& aName) = 0;
	virtual bool attachChild(MChild* aChild) = 0;
	/** @brief Get URI (absolutie) as parent
         * This method is intended mostly for navigation thru system
         * TODO consider alternative way: to get MNode via Lif, or maybe introduce new "light" MNode
         * iface to avoid vulnerability with getting MNode
         * */
	virtual void parentGetUri(GUri& aUri) const = 0;
        /** @brief Gets native parents URI chain, binding to MNode::parentsUris() * */
        virtual vector<GUri> parentsUri() const = 0;
};


/** @brief Native net Child interface
 * */
class MChild : public MIface
{
    public:
	using TCp =  MNcp<MChild, MParent>;
    public:
	inline static constexpr std::string_view idStr() { return "MChild"sv;}
	inline static constexpr TIdHash idHash() { return 0xa9ff92bbd5b98def;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MChild_Uid();}
	virtual string MChild_Uid() const = 0;
	MIface* getLif(TIdHash aId) override { return MChild_getLif(aId);}
	virtual MIface* MChild_getLif(TIdHash aId) = 0;
	void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MChild_doDump(aLevel, aIdt, std::cout);}
	virtual void MChild_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual MParent* parent() = 0;
	virtual const MParent* parent() const = 0;
	virtual void onParentDeleting(MParent* aParent) = 0;
        /** @brief Get child connection point
         * TODO this opens access to the whole inheritance treee but not to childs only:
         * having connpoint it is possible to get parents cp etc. Consider redesign
         * */
	virtual TCp* cP() = 0;
};



#endif
