#ifndef __FAP5_ELEM_H
#define __FAP5_ELEM_H

#include "melem.h"
#include "minh.h"
#include "chromo.h"
#include "node.h"

/** @brief Mutable base agent 
 * */
class Elem: public Node, public MElem
{
    public:
	using TInhTreeNode = NTnnp<MChild, MParent>; 
    public:
	inline static std::string_view idStr() { return "Elem"sv;}
        static vector<GUri> getParentsUri();
	Elem(const string &aType, const string &aName, MEnv* aEnv);
	virtual ~Elem();
	// From MNode.MIface
	MIface* MNode_getLif(TIdHash aId) override;
	GUri parentUri() const override;
	// From MElem.MIface
	string MElem_Uid() const override { return getUid<MElem>();}
	MIface* MElem_getLif(TIdHash aId) override;
	void MElem_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	// From MNode
	void mutate(const ChromoNode& aMut, bool aChange /*EFalse*/, const MutCtx& aCtx, bool aTreatAsChromo = false, bool aLocal = false) override;
	// From MElem
	MChromo& Chromos() override { return *mChromo;}
	MChild* asChild() override;
	MParent* asParent() override;
	// From MParent
	MIface* MParent_getLif(TIdHash aId) override;
	void MParent_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	void onChildDeleting(MChild* aChild) override;
	bool onChildRenaming(MChild* aChild, const string& aNewName) override;
	MNode* createHeir(const string& aName) override;
	bool attachChild(MChild* aChild) override;
	// From MChild
	MIface* MChild_getLif(TIdHash aId) override;
	void MChild_doDump(int aLevel, int aIdt, ostream& aOs) const override;
	MParent* parent() override;
	const MParent* parent() const override;
	void onParentDeleting(MParent* aParent) override;
	MChild::TCp* cP() override;
    protected:
	void notifyParentMutated(const TMut& aMut);
	void setCrAttr(const string& aEType, const string& aName);
	// From Node
	MNode* mutAddElem(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx) override;
	void onOwnedMutated(const MOwned* aOwned, const ChromoNode& aMut, const MutCtx& aCtx) override;
	// Local
	// TODO Should it be the method of MParent as attachChild is?
	bool detachChild(MChild* aChild);
    protected:
	MChromo* mChromo;     /*!< Chromo */
	TInhTreeNode mInode;  /*!< Inheritance tree node */
	MElem* mMElem = nullptr;
	MParent* mMParent = nullptr;
};

#endif

