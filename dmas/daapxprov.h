#ifndef __FAP5_DAAPXPROV_H__
#define __FAP5_DAAPXPROV_H__

#include "menv.h"
#include "mipxprov.h"

/*
 * Base agent for DAA proxies provider, ref. ds_pp_da
 */
class DaaPxProv: public MIpxProv
{
    public:
	DaaPxProv(const string& aName, MEnv* aEnv);
	virtual ~DaaPxProv();
    public:
	// From MIpxProv
	string MIpxProv_Uid() const override { return string();}
	MIface* MIpxProv_getLif(TIdHash aTid) override;
	MProxy* createProxy(const string& aId, MProxyMgr* aMgr, const string& aContext) const override;
	MProxy* createProxy(TIdHash aIfaceId, MProxyMgr* aMgr, const string& aContext) const override;
    protected:
        template<class T> inline MIface* checkLif(TIdHash aId, T*& aPtr) {
            return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
        }
    protected:
        string mName;
        MEnv* mEnv = nullptr;
        MIpxProv* mMIpxProv = nullptr;
};

#endif
