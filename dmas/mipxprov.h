#ifndef __FAP5_MIPXPROV_H_
#define __FAP5_MIPXPROV_H_

#include "mproxy.h"

/** @brief Proxy provider interface. Is used for providing interfaces proxies
 * for distributed async agents (DAA) implementation
 */
class MIpxProv: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MIpxProv"sv;}
	inline static constexpr TIdHash idHash() { return 0x4e3d46ccad7f0a6a;}
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	string ids() const override { return string(idStr());}
	virtual string Uid() const override { return MIpxProv_Uid();}
	virtual string MIpxProv_Uid() const = 0;
	virtual MIface* getLif(TIdHash aTid) override { return MIpxProv_getLif(aTid);}
	virtual MIface* MIpxProv_getLif(TIdHash aTid) = 0;
        // Local
	virtual MProxy* createProxy(const string& aIfaceId, MProxyMgr* aMgr, const string& aContext) const = 0;
	virtual MProxy* createProxy(TIdHash aIfaceId, MProxyMgr* aMgr, const string& aContext) const = 0;
};

#endif
