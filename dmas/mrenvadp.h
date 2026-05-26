#ifndef _FAP5_MRENVADP_H_
#define _FAP5_MRENVADP_H_

#include <string>
#include "../src/miface.h"

using namespace std;

/* @brief Remote Environment adapter
 * Acts as a bridge to remote environment
 * owned part on remote environment
 * */
class MRenvAdp : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MRenvAdp"sv;}
	inline static constexpr TIdHash idHash() { return 0xbcc495df8e631178;}
    public:
	// From MIface 
	TIdHash id() const override { return idHash();}
	string ids() const override { return string(idStr());}
	virtual string MRenvAdp_Uid() const = 0;
	string Uid() const override { return MRenvAdp_Uid();}
	MIface *getLif(TIdHash aId) override { return MRenvAdp_getLif(aId);}
	virtual MIface *MRenvAdp_getLif(TIdHash aId) = 0;
        // Local
        // TBD
};



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
	string ids() const override { return string(idStr());}
	virtual string MOwdRenvAdp_Uid() const = 0;
	string Uid() const override { return MOwdRenvAdp_Uid();}
	MIface *getLif(TIdHash aId) override { return MOwdRenvAdp_getLif(aId);}
	virtual MIface *MOwdRenvAdp_getLif(TIdHash aId) = 0;
        // Local
        virtual bool createRenv(const string& aRmtSrvUid, const string& aPrmUid, const ChromoNode& aChromo) = 0;
};

/* @brief Owning Remote Environment adapter
 * Acts as a bridge from owned part of system to 
 * owning part on remote environment
 * */
class MOwgRenvAdp : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MOwgRenvAdp"sv;}
	inline static constexpr TIdHash idHash() { return 0x6868df43d8873846;}
    public:
	// From MIface 
	TIdHash id() const override { return idHash();}
	string ids() const override { return string(idStr());}
	virtual string MOwgRenvAdp_Uid() const = 0;
	string Uid() const override { return MOwgRenvAdp_Uid();}
	MIface *getLif(TIdHash aId) override { return MOwgRenvAdp_getLif(aId);}
	virtual MIface *MOwgRenvAdp_getLif(TIdHash aId) = 0;
        // Local
        virtual bool bindRenv(const string& aPSid, const string& aPEid, const string& aPUid) = 0;
};


#endif
