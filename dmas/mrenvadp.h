#ifndef _FAP5_MRENVADP_H_
#define _FAP5_MRENVADP_H_

#include <string>
#include "../src/miface.h"

using namespace std;


/* @brief Owned Remote Environment adapter
 * Acts as a bridge from owning part of system to 
 * owned part on remote environment
 * */
class MOwdRenvAdp : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MOwdRenvAdp"sv;}
	inline static constexpr TIdHash idHash() { return 0xc6b46716a5f28aa0;}
    public:
	// From MIface 
	TIdHash id() const override { return idHash();}
	virtual string MOwdRenvAdp_Uid() const = 0;
	string Uid() const override { return MOwdRenvAdp_Uid();}
	MIface *getLif(TIdHash aId) override { return MOwdRenvAdp_getLif(aId);}
	virtual MIface *MOwdRenvAdp_getLif(TIdHash aId) = 0;
        // Local
        virtual bool createRenv(const string& aRmtSrvUid, const string& aPrmUid, const ChromoNode& aChromo) = 0;
};

#endif
