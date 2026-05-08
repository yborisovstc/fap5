
#ifndef __FAP5_OWDRENVADP_H__
#define __FAP5_OWDRENVADP_H__

#include <memory>

#include "../src/mowning.h"

#include "mproxy.h"
#include "daaproxy.h"
#include "mrenvadp.h"


/* @brief Adapter to remote environment, base class for specialized
 * adapters to remote env
 * */
class RenvAdp : public MRenvAdp, public MProxyMgrOwner
{
    public:
	inline static constexpr std::string_view idStr() { return "RenvAdp"sv;}

    public:
        /*
        class ProxyMgrOwner : public MProxyMgrOwner {
            public:
                ProxyMgrOwner(OwdRenvAdp& aHost): mHost(aHost) {}
                // From MProxyMgrOwner
                string getOid() const override;
            protected:
                OwdRenvAdp& mHost;
        };
        */
    public:
        RenvAdp(const string& aName, MEnv* aEnv);
        virtual ~RenvAdp();
    public:
        // From MRenvAdp
	string MRenvAdp_Uid() const override { return mName;}
	MIface* MRenvAdp_getLif(TIdHash aId) override;
        // From MProxyMgrOwner
        string getOid() const override;
    protected:
        bool connect(const string& aSrvUri);
    protected:
        template<class T> inline MIface* checkLif(TIdHash aId, T*& aPtr) {
            return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr; }
    protected:
        string mName;
        MEnv* mEnv;
        RenvClient mRenvClient;
	DaaPxMgr mPxMgr;
        string mRmtSrvUri;
        string mContext;
        //ProxyMgrOwner mPxMgrOwner;
        MRenvAdp* mMRenvAdp = nullptr;
};



/* @brief Adapter to owned part remote environment
 * */
class OwdRenvAdp : public RenvAdp, public MOwdRenvAdp
{
    public:
	inline static constexpr std::string_view idStr() { return "OwdRenvAdp"sv;}

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
        /*
        class ProxyMgrOwner : public MProxyMgrOwner {
            public:
                ProxyMgrOwner(OwdRenvAdp& aHost): mHost(aHost) {}
                // From MProxyMgrOwner
                string getOid() const override;
            protected:
                OwdRenvAdp& mHost;
        };
        */
        class OwnedPpx : public DaaProxy, public MOwned {
            public:
                OwnedPpx(OwdRenvAdp* aHost, const string& aContext);
                virtual ~OwnedPpx();
            public:
                // From MProxy
                virtual string MProxy_Uid() const { return string();}
                // From Owned
                string MOwned_Uid() const {return string();} // TODO TBD
                MIface* MOwned_getLif(TIdHash aId) override;
                string ownedId() const override;
                void deleteOwned() override { }
                void onOwnerAttached() override { }
                void onOwnerDetached() override { }
                MOwner* asOwner() override { MOwner* res = MOwned::lIf(res); return res; }
                const MOwner* asOwner() const override { const MOwner* res = MOwned::lIf(res); return res; }
                TOwnedCp* ownedCp() override { return &mOwsCp;}
                const TOwnedCp* ownedCp() const override { return &mOwsCp;}
            protected:
                template<class T> inline MIface* checkLif(TIdHash aId, T*& aPtr) {
                    return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
                }
            protected:
                OwdRenvAdp* mHost;
                OwdCp mOwsCp;                /*!< Ownership CP */
                MOwned* mMOwned = nullptr;
        };
    public:
        OwdRenvAdp(const string& aName, MEnv* aEnv);
        virtual ~OwdRenvAdp();
    public:
        // From MOwdRenvAdp
	string MOwdRenvAdp_Uid() const override { return mName;}
	MIface* MOwdRenvAdp_getLif(TIdHash aId) override;
        bool createRenv(const string& aRmtSrvUid, const string& aPrmUid, const ChromoNode& aChromo) override;
        // For Owned
        string MOwned_Uid() const {return string();} // TODO TBD
        string ownedId() const { return mName;}
        void deleteOwned() { delete this;}
        void onOwnerAttached() ;
        void onOwnerDetached() {}
        MOwner* asOwner() { return nullptr; }
        const MOwner* asOwner() const { return nullptr; }
    protected:
        MProxy* mOwdPpx = nullptr;
};

/* @brief Adapter to owning part remote environment
 * */
class OwgRenvAdp : public RenvAdp, public MOwgRenvAdp
{
    public:
	inline static constexpr std::string_view idStr() { return "OwgRenvAdp"sv;}

    public:
	class OwrCp : public NCpOnp<MOwner, MOwned> {
	    public:
		OwrCp(MOwner* aProvPx): NCpOnp<MOwner, MOwned>(aProvPx) {}
		// Note: we need to implement virt destructor here because clean-up method disconnectAll()
		// is virtual and uses other virt method. It virt destructor is not impl here the lower
		// level virt destr will be called and the that context virt methods (often stubs) will be called
		virtual ~OwrCp() { disconnectAll();}
		//virtual bool getId(string& aId) const override { aId = provided()->ownedId(); return true;}
	};
    public:
        class OwnerPpx : public DaaProxy, public MOwner {
            public:
                OwnerPpx(OwgRenvAdp* aHost, const string& aContext);
                virtual ~OwnerPpx();
            public:
                // From MProxy
                virtual string MProxy_Uid() const { return string();}
                // From Owner
                string MOwner_Uid() const {return string();} // TODO TBD
                MIface* MOwner_getLif(TIdHash aId) override;
                MOwned* getOwned(const GUri& aUri, const MOwned* aReq) const override;
                void ownerGetUri(GUri& aUri, const MOwner* aBase = nullptr) const override;
                void onOwnedMutated(const MOwned* aOwned, const ChromoNode& aMut, const MutCtx& aCtx) override;
                void onOwnedAttached(MOwned* aOwned) override;
                void onOwnedDetached(MOwned* aOwned) override;
                MNode* getParent(const GUri& aUri) override;
                bool isOwned(const MOwned* mOwned) const override;
                bool  owrAttachOwned(MOwned* aOwned) override;
            protected:
                OwgRenvAdp* mHost;
                OwrCp mOwsCp;                /*!< Ownership CP */
        };
    public:
        OwgRenvAdp(const string& aName, MEnv* aEnv);
        virtual ~OwgRenvAdp();
    public:
        // From MOwgRenvAdp
        string MOwgRenvAdp_Uid() const override { return mName;}
        MIface* MOwgRenvAdp_getLif(TIdHash aId) override;
        bool bindRenv(const string& aPSid, const string& aPEid, const string& aPUid) override;
    protected:
        MProxy* mOwrPpx = nullptr;
};



#endif
