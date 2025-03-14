#ifndef __FAP5_MELEM_H
#define __FAP5_MELEM_H


#include "miface.h"
#include "guri.h"
#include "menv.h"

class MChild;
class MParent;

/** @brief Interface of mutable agent
 *
 * */
class MElem: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MElem"sv;}
	inline static constexpr TIdHash idHash() { return 0xd0cb09cd212045e1;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MElem_Uid();}
	virtual string MElem_Uid() const = 0;
	MIface* getLif(TIdHash aId) override { return MElem_getLif(aId);}
	virtual MIface* MElem_getLif(TIdHash aId) = 0;
	void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MElem_doDump(aLevel, aIdt, aOs);}
	virtual void MElem_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual MChromo& Chromos() = 0;
	virtual MChild* asChild() = 0;
	virtual MParent* asParent() = 0;
};


#endif
