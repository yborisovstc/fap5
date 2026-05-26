#ifndef _DAAPROXY_H_
#define _DAAPROXY_H_

//#include "mproxy.h"
#include "menv.h"
#include "mlog.h"
#include <map>
#include "../client/bclient.h"
#include "../src/ifu.h"

#include "mproxy.h"
#include "mipxprov.h"

/* @brief Remote environment client. Provides set of clients session to same remote environment
 * This allows to avoid interaction deadlock in case of cyclyc requests, ref ds_pa_msra
 */
class RenvClient
{
    public:
        using TClients = vector<BaseClient*>;
    public:
        RenvClient();
        ~RenvClient();
        void SetRmtSID(const string& aSID);
        void Connect(const string& aHostUri);
        void Disconnect();
        bool Request(const string& aRequest, string& aResponse);
        bool Request(const string& aReqId, const string& aReqArgs, string& aResponse);
        bool IsConnected() const;
    protected:
        BaseClient* GetClient();
    protected:
        TClients mClients;
        // Session ID of remote environment
        string mRmtSID;
        string mHostUri;
};

class MProxyMgrOwner {
    public:
        virtual string getOid() const = 0;
};


class DaaProxy;

/* @brief Proxies manager
 * Owns all proxies via MProxy iface. Offers MProxyMgr for proxies
 * to interact to remote environment
 */
class DaaPxMgr: public MProxyMgr
{
    public:
	typedef map<string, MProxy*> TPxs;
    public:
	DaaPxMgr(MEnv* aEnv, MProxyMgrOwner* aOwner, RenvClient& aRenvClient);
	virtual ~DaaPxMgr();
    public:
	// From MProxyMgr
	MProxy* CreateProxy(const string& aId, const string& aContext) override;
	MProxy* CreateProxy(MIface::TIdHash aId, const string& aContext) override;
	bool Request(const string& aContext, const string& aReq, string& aResp) override;
	string Oid() const override;
	void OnProxyDeleting(const MProxy* aProxy) override;
	void RegisterProxy(MProxy* aProxy) override;
    protected:
	void RegProxy(MProxy* aProxy);
	void UnregProxy(const MProxy* aProxy);
	bool IsCached(const string& aContext) const;
	MProxy* GetProxy(const string& aContext) const;
	inline MLogRec* Logger() const;
    protected:
	MEnv* mEnv;
	MProxyMgrOwner* mOwner;
        MIpxProv* mIpxProv = nullptr;
	TPxs mProxies;
	RenvClient& mRenvClient;
};

inline MLogRec* DaaPxMgr::Logger() const {return mEnv ? mEnv->Logger(): NULL; }


/*
 * Proxy base
 */
class DaaProxy : public MProxy
{
    public:
	DaaProxy(MEnv* aEnv, MProxyMgr* aMgr, const string& aContext);
	DaaProxy(MEnv* aEnv, const string& aContext);
	virtual ~DaaProxy();
	// From MProxy
	bool setContext(const string& aContext) override;
	virtual const string& GetContext() const;
        string MProxy_Uid() const override {return string();} // TODO TBD
        MIface* MProxy_getLif(TIdHash aId) override;
	MIface* getLif(const string& aId) override;
    protected:
	inline MProvider* Provider() const;
	inline MLogRec* Logger() const;
	bool Request(const string& aReq, string& aResp);
	MIface* NewProxyRequest(const string& aCallSpec, const string& aPxType);
	MIface* GetProxy(const string& aSpec, const string& aId) const;
	MIface* GetProxy(const string& aSpec, TIdHash aIfaceId) const;
	const MIface* NewProxyRequest(const string& aCallSpec, const string& aPxType) const {
            auto self = const_cast<DaaProxy*>(this);
            return self->NewProxyRequest(aCallSpec, aPxType);
        }
	template<typename TRet> TRet Rpc(const string& aName) const;
	template<typename TRet, typename TArg1> TRet Rpc(const string& aName, TArg1 aArg) const;
	template<typename TRet, typename TArg1, typename TArg2> TRet Rpc(const string& aName, TArg1 aArg1, TArg2 aArg2) const;
	template<typename TRet, typename TArg1, typename TArg2, typename TArg3> TRet Rpc(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const;
	template<typename TRet> TRet* RpcPx(const string& aName) const;
	template<typename TRet> const TRet* RpcPxC(const string& aName) const;
	template<typename TRet, typename TArg1> TRet* RpcPx(const string& aName, TArg1 aArg) const;
	template<typename TRet, typename TArg1> const TRet* RpcPxC(const string& aName, TArg1 aArg) const;
	template<typename TRet, typename TArg1, typename TArg2> TRet* RpcPx(const string& aName, TArg1 aArg1, TArg2 aArg2) const;
	template<typename TRet, typename TArg1, typename TArg2, typename TArg3> TRet* RpcPx(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const;
	template<typename TRet, typename TArg1, typename TArg2, typename TArg3, typename TArg4>
	    TRet* RpcPx(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3, TArg4 aArg4) const;
	inline void Rpcv(const string& aName) const;
	template<typename TArg1> void Rpcv(const string& aName, TArg1 aArg) const;
	template<typename TArg1, typename TArg2> void Rpcv(const string& aName, TArg1 aArg1, TArg2 aArg2) const;
	template<typename TArg1, typename TArg2, typename TArg3> void Rpcv(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const;
	template<typename TArg1, typename TArg2, typename TArg3, typename TArg4> void Rpcv(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3, TArg4 aArg4) const;
	MIface* RpcPxN(const string& aName, const string& aIfType) const;
	MIface* RpcPxNh(const string& aName, TIdHash aIfType) const;
	template<typename TArg1> MIface* RpcPxN(const string& aName, const string& aIfType, TArg1 aArg1) const;
	template<typename TArg1, typename TArg2> MIface* RpcPxN(const string& aName, const string& aIfType, TArg1 aArg1, TArg2 aArg2) const;
	template<typename TArg1, typename TArg2, typename TArg3> MIface* RpcPxN(const string& aName, const string& aIfType, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const;
    protected:
        template<class T> inline MIface* checkLif(TIdHash aId, T*& aPtr) {
            return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
        }
        template<class T> inline MIface* checkLif(const string& aId, T*& aPtr) {
            return (aId == T::idStr()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
        }
    protected:
	MEnv* mEnv;
	MProxyMgr* mMgr = nullptr;
        MProxy* mMProxy = nullptr;
	string mContext;
};

inline MProvider* DaaProxy::Provider() const {return mEnv ? mEnv->provider(): NULL; }

inline MLogRec* DaaProxy::Logger() const {return mEnv ? mEnv->Logger(): NULL; }

template<typename TRet> TRet DaaProxy::Rpc(const string& aName) const
{
    string resp;
    TRet res;
    bool rr = mMgr->Request(mContext, Ifu::PackMethod(aName), resp);
    if (rr) res = Ifu::Unpack(resp, res);
    return res;
}

template<typename TRet, typename TArg1> TRet DaaProxy::Rpc(const string& aName, TArg1 aArg) const
{
    string resp;
    TRet res;
    bool rr = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg), resp);
    if (rr) res = Ifu::Unpack(resp, res);
    return res;
}

template<typename TRet, typename TArg1, typename TArg2> TRet DaaProxy::Rpc(const string& aName, TArg1 aArg1, TArg2 aArg2) const
{
    string resp;
    TRet res;
    bool rr = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2), resp);
    if (rr) res = Ifu::Unpack(resp, res);
    return res;
}

template<typename TRet, typename TArg1, typename TArg2, typename TArg3> TRet DaaProxy::Rpc(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const
{
    string resp;
    TRet res;
    bool rr = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2, aArg3), resp);
    if (rr) res = Ifu::Unpack(resp, res);
    return res;
}


template<typename TRet> TRet* DaaProxy::RpcPx(const string& aName) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName), resp);
    return (rres ? (TRet*) GetProxy(resp/*, TRet::idHash()*/, TRet::ids()) : NULL);
} 

template<typename TRet> const TRet* DaaProxy::RpcPxC(const string& aName) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName), resp);
    return (rres ? (const TRet*) GetProxy(resp, TRet::Type()) : NULL);
} 

template<typename TRet, typename TArg1> TRet* DaaProxy::RpcPx(const string& aName, TArg1 aArg) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg), resp);
    return (rres ? (TRet*) GetProxy(resp, string(TRet::idStr())) : NULL);
}

template<typename TRet, typename TArg1> const TRet* DaaProxy::RpcPxC(const string& aName, TArg1 aArg) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg), resp);
    return (rres ? (const TRet*) GetProxy(resp, TRet::Type()) : NULL);
}

template<typename TRet, typename TArg1, typename TArg2> TRet* DaaProxy::RpcPx(const string& aName, TArg1 aArg1, TArg2 aArg2) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2), resp);
    return (rres ? (TRet*) GetProxy(resp, TRet::Type()) : NULL);
}

template<typename TRet, typename TArg1, typename TArg2, typename TArg3> TRet* DaaProxy::RpcPx(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2, aArg3), resp);
    return (rres ? (TRet*) GetProxy(resp, TRet::Type()) : NULL);
}

template<typename TRet, typename TArg1, typename TArg2, typename TArg3, typename TArg4> TRet* DaaProxy::RpcPx(const string& aName, TArg1 aArg1, TArg2 aArg2,
	TArg3 aArg3, TArg4 aArg4) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2, aArg3, aArg4), resp);
    return (rres ? (TRet*) GetProxy(resp, TRet::Type()) : NULL);
}


void DaaProxy::Rpcv(const string& aName) const
{
    string resp;
    mMgr->Request(mContext, Ifu::PackMethod(aName), resp);
}

template<typename TArg1> void DaaProxy::Rpcv(const string& aName, TArg1 aArg) const
{
    string resp;
    mMgr->Request(mContext, Ifu::PackMethod(aName, aArg), resp);
}

template<typename TArg1, typename TArg2> void DaaProxy::Rpcv(const string& aName, TArg1 aArg1, TArg2 aArg2) const
{
    string resp;
    mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2), resp);
}

template<typename TArg1, typename TArg2, typename TArg3> void DaaProxy::Rpcv(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const
{
    string resp;
    mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2, aArg3), resp);
}

template<typename TArg1, typename TArg2, typename TArg3, typename TArg4> void DaaProxy::Rpcv(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3, TArg4 aArg4) const
{
    string resp;
    mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2, aArg3, aArg4), resp);
}
 
template<typename TArg1> MIface* DaaProxy::RpcPxN(const string& aName, const string& aIfType, TArg1 aArg1) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1), resp);
    return (rres ? GetProxy(resp, aIfType) : NULL);
}
 
template<typename TArg1, typename TArg2> MIface* DaaProxy::RpcPxN(const string& aName, const string& aIfType, TArg1 aArg1, TArg2 aArg2) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2), resp);
    return (rres ? GetProxy(resp, aIfType) : NULL);
}
 
template<typename TArg1, typename TArg2, typename TArg3> MIface* DaaProxy::RpcPxN(const string& aName, const string& aIfType, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3) const
{
    string resp;
    bool rres = mMgr->Request(mContext, Ifu::PackMethod(aName, aArg1, aArg2, aArg3), resp);
    return (rres ? GetProxy(resp, aIfType) : NULL);
}


#endif 
