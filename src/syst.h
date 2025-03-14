#ifndef __FAP5_SYST_H
#define __FAP5_SYST_H

#include "msyst.h"

#include "elem.h"


/** @brief System, inheritable
 * Isn't connectable itlelf but can represent itself via connpoints
 * Can contain connectables and connect them
 * */
class Syst : public Elem, public MSyst
{
    public:
	inline static constexpr std::string_view idStr() { return "Syst"sv;}
	inline static constexpr TIdHash idHash() { return 0xac85b9e6fc11ee6e;}
	Syst(const string &aType, const string &aName, MEnv* aEnv);
	virtual ~Syst();
	// From Node
	virtual MIface* MNode_getLif(TIdHash aId) override;
	// From MSyst
	virtual string MSyst_Uid() const override {return getUid<MSyst>();};
	virtual const TEdges& connections() const { return mEdges; }
    protected:
	// From Node
	void mutConnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx) override;
	void mutDisconnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx) override;
    protected:
	TEdges mEdges;  /*!< Edges */
	MSyst* mMSyst = nullptr;
};

#endif
