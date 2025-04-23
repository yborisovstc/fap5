#ifndef __FAP5_MNODE_H
#define __FAP5_MNODE_H

#include "miface.h"
#include "mowning.h"
#include "nconn.h"
#include "guri.h"


class MNode;

/** @brief Name spaces
 * */
class TNs: public vector<MNode*>
{
    public:
	TNs() {}
	TNs(MNode* aElem) { push_back(aElem);}
};

/** @brief Mutation context
 * */
class MutCtx
{
    public:
	MutCtx(const MutCtx& aSrc): mNode(aSrc.mNode), mNs(aSrc.mNs) {}
	MutCtx(const MNode* aNode, const TNs& aNs): mNode(aNode), mNs(aNs) {}
	MutCtx(const MNode* aNode): mNode(aNode) {}
	MutCtx(): mNode(nullptr) {}
    public:
	const MNode* mNode; //!< Node
	TNs mNs;     //!< Name spaces
};


struct TNodeEventOwnerAttached : public MEvent
{
    static std::string_view idStr() { return "TNodeEventOwnerAttached"sv;}
    static constexpr MIface::TIdHash idHash = 0x1454003d3331b595;
    TNodeEventOwnerAttached(): MEvent(idHash) {}
};

// TODO Do we really need aOwned as ptr here, consider event with Guri instead
struct TNodeEventOwnedAttached : public MEvent
{
    static std::string_view idStr() { return "TNodeEventOwnedAttached"sv;}
    static constexpr MIface::TIdHash idHash = 0xb0ed3284325af49;
    TNodeEventOwnedAttached(const MOwned* aOwned): MEvent(idHash), mOwned(aOwned) {}
    const MOwned* mOwned;
};

struct TNodeEventOwnedDetached : public MEvent
{
    static std::string_view idStr() { return "TNodeEventOwnedDetached"sv;}
    static constexpr MIface::TIdHash idHash = 0x2559a814ded159b;
    TNodeEventOwnedDetached(const MOwned* aOwned): MEvent(idHash), mOwned(aOwned) {}
    const MOwned* mOwned;
};

struct TNodeEventContentChanged : public MEvent
{
    static std::string_view idStr() { return "TNodeEventContentChanged"sv;}
    static constexpr MIface::TIdHash idHash = 0xf3f1cbc5ffc9b3de;
    TNodeEventContentChanged(const string& aId): MEvent(idHash), mId(aId) {}
    string mId;
};

/** @brief Any changes other than above
 * */
struct TNodeEventChanged : public MEvent
{
    static std::string_view idStr() { return "TNodeEventChanged"sv;}
    static constexpr MIface::TIdHash idHash = 0x4f51bd9654227072;
    TNodeEventChanged(): MEvent(idHash) {}
};


/** @brief Native heirarchy node interface. It is iface-aggregator, i.e represent
 * convenient access to set of the base ifaces (owner-owned etc.)
 * */
class MNode: public MIface
{
     public:
	using TOwnerCp = MNcp<MOwner, MOwned>;
	//using TOwnedCp = MNcp<MOwned, MOwner>;

    public:
	// From MIface
	inline static constexpr std::string_view idStr() { return "MNode"sv;}
	inline static constexpr TIdHash idHash() { return 0xf0a96e4185c535;}

    public:
	virtual ~MNode() {}
	/** Secondary constructor */
	virtual void Construct() = 0;
	TIdHash id() const override { return idHash();}
	virtual string Uid() const override { return MNode_Uid();}
	virtual string MNode_Uid() const = 0;
	virtual MIface* getLif(TIdHash aTid) override { return MNode_getLif(aTid);}
	virtual MIface* MNode_getLif(TIdHash aTid) = 0;
	virtual void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MNode_doDump(aLevel, aIdt, aOs);}
	virtual void MNode_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual const string& name() const = 0;
        /** @brief Gets name of parent. It's intended mostly for SDO **/
	virtual GUri parentUri() const = 0;
        /** @brief Gets the chain of native parents **/
	virtual vector<GUri> parentsUris() const = 0;
	virtual MOwned* getOwned(const string& aId) const = 0;
	virtual const MNode* getNode(const GUri& aUri) const = 0;
	virtual MNode* getNode(const GUri& aUri) = 0;
	virtual MNode* getNodeS(const char* aUri) = 0;
	virtual MNode* getNode(const GUri& aUri, const TNs& aNs) = 0;
	virtual void getUri(GUri& aUri, const MNode* aBase = nullptr) const = 0;
	string getUriS(MNode* aBase = nullptr) const { GUri uri; getUri(uri, aBase); return uri.toString();}
	virtual void setCtx(MOwner* aContext) = 0;
	/** @brief Apply mutation
	 * @param aChange  to not save the mut in chromo but do change only
	 * @param aCtx  mutation context
	 * @param aTreatAsChromo the indication that the only segment needs to be handled
	 * @param  aLocal  the sign of local mut. This means that target attr needs to be ignored
	 * */
	virtual void mutate(const ChromoNode& aMut, bool aChange /*EFalse*/, const MutCtx& aCtx, bool aTreatAsChromo = false, bool aLocal = false) = 0;
	//virtual MNode* createHeir(const string& aName) = 0;
	//virtual bool attachHeir(MNode* aHeir) = 0;
	virtual bool attachOwned(MNode* aOwned) = 0;
	virtual TOwnerCp* ownerCp() = 0;
	virtual const TOwnerCp* ownerCp() const = 0;
	// virtual TOwnedCp* ownedCp() = 0; //  Moved to MOwned
	// virtual const TOwnedCp* ownedCp() const = 0; //  Moved to MOwned
	//virtual const MContentOwner* cntOw() const = 0;
	//virtual MContentOwner* cntOw() = 0;
   
};


#endif
