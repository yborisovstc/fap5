#ifndef __FAP5_MLINK_H
#define __FAP5_MLINK_H


#include "miface.h"
#include "ifu.h"

using namespace std;


class MNode;

/** @brief One-way one-to-one relation interface
 *
 * */
class MLink: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MLink"sv;}
	inline static constexpr TIdHash idHash() { return 0x4f0a000556332ad5;}
    public:
	// From MIface
	string Uid() const override { return MLink_Uid();}
	MIface* getLif(const char *aType) { return MLink_getLif(aType);}
	void doDump(int aLevel, int aIdt = 0) const { return MLink_doDump(aLevel, aIdt, std::cout);}
	virtual string MLink_Uid() const = 0;
	virtual MIface* MLink_getLif(const char *aType) = 0;
	virtual void MLink_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual bool connect(MNode* aPair) = 0;
	virtual bool disconnect(MNode* aPair) = 0;
	virtual MNode* pair() = 0;
};






#endif
