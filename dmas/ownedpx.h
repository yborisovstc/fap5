#ifndef __FAP5_OWNEDPX_H__
#define __FAP5_OWNEDPX_H__

#include "daaproxy.h"
#include "mowning.h"

class OwnedPx : public DaaProxy, public MOwned {
    public:
	class OwdCp : public NCpOnp<MOwned, MOwner> {
	    public:
		OwdCp(MOwned* aProvPx): NCpOnp<MOwned, MOwner>(aProvPx) {}
		// Note: we need to implement virt destructor here because clean-up method disconnectAll()
		// is virtual and uses other virt method. It virt destructor is not impl here the lower
		// level virt destr will be called and the that context virt methods (often stubs) will be called
		virtual ~OwdCp() { disconnectAll();}
		virtual bool getId(string& aId) const override { aId = provided()->ownedId(); return true;}
	};
    public:
        OwnedPx(MEnv* aEnv, MProxyMgr* aMgr, const string& aContext);
        OwnedPx(MEnv* aEnv, const string& aContext);
        virtual ~OwnedPx();
    public:
        // From MProxy
        MIface* MProxy_getLif(TIdHash aId) override;
        virtual string MProxy_Uid() const override { return string();}
        // From MOwned
        string ownedId() const override;
        MIface* MOwned_getLif(TIdHash aId) override;
	string MOwned_Uid() const override;
	void deleteOwned() override;
	void onOwnerAttached() override;
	void onOwnerDetached() override;
        MOwner* asOwner() override { MOwner* res = MOwned::lIf(res); return res; }
        const MOwner* asOwner() const override { const MOwner* res = MOwned::lIf(res); return res; }
        TOwnedCp* ownedCp() override { return &mOwsCp;}
        const TOwnedCp* ownedCp() const override { return &mOwsCp;}
    protected:
        MOwned* mMOwned = nullptr;
        OwdCp mOwsCp;                /*!< Ownership CP */
};
 

#endif
