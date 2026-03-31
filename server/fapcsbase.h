#ifndef _FAPCSIONBASE_
#define _FAPSIONBASE_

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>

#include "../src/menv.h"
#include "../src/ifu.h"
#include "../src/melem.h"

using namespace std;


#if 0
/*
 * Agent observer
 */
class AgtObserver: public MAgentObserver
{
    public:
	AgtObserver();
	virtual ~AgtObserver();
    public:
	// From MCompsObserver
	virtual void OnCompDeleting(MElem& aComp, TBool aSoft = ETrue);
	virtual void OnCompAdding(MElem& aComp);
	virtual TBool OnCompChanged(MElem& aComp);
	virtual TBool OnContentChanged(MElem& aComp);
	virtual TBool OnCompRenamed(MElem& aComp, const string& aOldName);
	// From MIface	
	virtual MIface* Call(const string& aSpec, string& aRes);
	virtual string Mid() const;
};
#endif


class MEnvProvider: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MEnvProvider"sv;}
	inline static constexpr TIdHash idHash() { return 0x9f6c5e04f335e6b5;}
    public:
	// From MIface 
	TIdHash id() const override { return idHash();}
	virtual string MEnvProvider_Uid() const = 0;
	string Uid() const override { return MEnvProvider_Uid();}
	virtual void CreateEnv(const string& aChromo) = 0;
	virtual void AttachEnv(const string& aSessionId) = 0;
	virtual void GetId(string& aSessionId) = 0;
	virtual MEnv* GetEnv() = 0;
	//virtual void CreateAgtObserver() = 0;
    protected:
	struct EIfu: Ife { EIfu(); };
	static EIfu mIfu; // Interface methods utility
};

class CSessionBase : public MEnvProvider
{
    public:
	// Key of context map: Handle
	typedef string TCtxKey;
	// Context: key, pointer to iface
	typedef map<TCtxKey, MIface*> TCtx;
    public:
	static vector<CSessionBase*> sClients;
	static TCtx mSCtx; // Shared Context
	string mId;
	//Socket stuff
	int mSock;
	MEnv* mEnv;
	TCtx mCtx; // Context
	CSessionBase* mAttached;
	//AgtObserver* mAgtObs;
    public:
        CSessionBase();
        CSessionBase(int sock);
	virtual ~CSessionBase();
	// From MEnvProvider
        string MEnvProvider_Uid() const override { return string();}
	virtual void CreateEnv(const string& aChromo);
	virtual void AttachEnv(const string& aSessionId);
	virtual void GetId (string& aSessionId);
	virtual MEnv* GetEnv();
	//virtual void CreateAgtObserver();
	virtual MIface* Call(const string& aSpec, string& aRes);
	virtual string Uid() const;
	virtual string Mid() const;
        static void FindSessionById(const string& mId, CSessionBase *&c);
	// TODO [YB] Shouldn't we avoid passing aHandle via using aPtr::Uid() instead for registry key
	static void AddSContext(const string& aHandle, MIface* aPtr);
	static void RmSContext(MIface* aPtr);
    protected:
        virtual void HandleMessage(const string& aMsg);
        virtual void Send(string const& aMsg);
        virtual bool Run();
        void SetId(int id);
        void Send(string const& msg, const string& msg_args);
        static int FindSessionIndex(CSessionBase *c);
	void AddContext(const string& aHandle, MIface* aPtr);
	MIface* GetContext(const string& aHandle, bool aShared = false);
	// Debug
	void DumpCtx() const;
	static CSessionBase* GetSession(const string& aId);
};

#endif
