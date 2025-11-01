
#ifndef __FAP3_MMNTP_H
#define __FAP3_MMNTP_H

#include "miface.h"

// TODO do we really need this iface? Actually system exploration feature
// supports getting mounted system root as proper managed agents (MAG).
/** @brief Model mount point interface
 * */
class MMntp : public MIface
{

    public:
	inline static constexpr std::string_view idStr() { return "MMntp"sv;}
	inline static constexpr TIdHash idHash() { return 0x307011d9f2032675;}
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	virtual string Uid() const override { return MMntp_Uid();}
	virtual string MMntp_Uid() const = 0;
	virtual MIface* getLif(TIdHash aId) { return MMntp_getLif(aId);}
	virtual MIface* MMntp_getLif(TIdHash aId) = 0;
	// Local
	virtual MNode* root() const = 0;
};



#endif
