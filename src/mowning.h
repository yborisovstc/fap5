#ifndef __FAP5_MOWNING_H
#define __FAP5_MOWNING_H

#include "miface.h"
#include "guri.h"
#include "chromo.h"
#include "nconn.h"

class MOwned;
class MutCtx;

class MParent;
class MNode;


/** @brief Native net owner interface
 * */
class MOwner : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MOwner"sv;}
	inline static constexpr TIdHash idHash() { return 0x5ef6ce210778d73c;}

    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	virtual string Uid() const override { return MOwner_Uid();}
	virtual string MOwner_Uid() const = 0;
	virtual MIface* getLif(TIdHash aId) { return MOwner_getLif(aId);}
	virtual MIface* MOwner_getLif(TIdHash aId) = 0;
	// Local
	virtual bool attachOwned(MOwned* aOwned) = 0;
	/** @brief Gets node by URI
	 * @param aReq  requesting node
	 * */
	//virtual MNode* ownerGetNode(const GUri& aUri, const MNode* aReq) const = 0;
	virtual MOwned* getOwned(const GUri& aUri, const MOwned* aReq) const = 0;
	virtual MOwned* getOwned(const string& aId) const = 0;
	virtual void ownerGetUri(GUri& aUri, const MOwner* aBase = nullptr) const = 0;
	virtual void onOwnedMutated(const MOwned* aOwned, const ChromoNode& aMut, const MutCtx& aCtx) = 0;
	virtual void onOwnedAttached(MOwned* aOwned) = 0;
	virtual void onOwnedDetached(MOwned* aOwned) = 0;
	/** @brief Gets the array of modules, the nearest the first */
	//virtual void getModules(vector<MNode*>& aModules) = 0;
	/** @brief Provides parent node by given URI
	 * the variation of getNode with using parent access rule
	 * */
	// TODO to return MParent?
	virtual MNode* getParent(const GUri& aUri) = 0;
	virtual bool isOwned(const MOwned* mOwned) const = 0;
};


/** @brief Native net owned interface
 * */
class MOwned : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MOwned"sv;}
	inline static constexpr TIdHash idHash() { return 0x20c735e6cf1457bd;}
    public:
	using TOwnedCp = MNcp<MOwned, MOwner>;
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	virtual string Uid() const override { return MOwned_Uid();}
	virtual string MOwned_Uid() const = 0;
	virtual MIface* getLif(TIdHash aTid) override { return MOwned_getLif(aTid);}
	virtual MIface* MOwned_getLif(TIdHash aTid) = 0;
	// Local
	virtual string ownedId() const = 0;
	virtual void deleteOwned() = 0;
	virtual void onOwnerAttached() = 0;
	virtual void onOwnerDetached() = 0;
	virtual MOwner* asOwner() = 0;
	virtual const MOwner* asOwner() const = 0;
	virtual TOwnedCp* ownedCp() = 0;
	virtual const TOwnedCp* ownedCp() const = 0;
};



#endif //  __FAP3_MOWNING_H
