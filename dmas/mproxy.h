#ifndef _MPROXY_H_
#define _MPROXY_H_

#include <string>
#include "../src/miface.h"

using namespace std;


/*
 * Proxy interface
 */
class MProxy : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MProxy"sv;}
	inline static constexpr TIdHash idHash() { return 0xc9162aa096356c6a;}
    public:
	// From MIface 
	TIdHash id() const override { return idHash();}
	virtual bool setContext(const string& aContext) const = 0;
	virtual const string& GetContext() const = 0;
	virtual string MProxy_Uid() const = 0;
	string Uid() const override { return MProxy_Uid();}
};

/*
 * Proxy manager interface
 */
class MProxyMgr 
{
    public:
	virtual MProxy* CreateProxy(const string& aId, const string& aContext) = 0;
	virtual bool Request(const string& aContext, const string& aReq, string& aResp) = 0;
	// Object Id
	virtual string Oid() const = 0;
	virtual void OnProxyDeleting(const MProxy* aProxy) = 0;
};
#endif
