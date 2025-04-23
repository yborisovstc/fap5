
#ifndef __FAP5_MOBSERV_H
#define __FAP5_MOBSERV_H

#include "nconn.h"

class MObservable;
class MContent;

/** @brief Observer interface
 * */
class MObserver: public MIface
{
    public:
	inline static std::string_view idStr() { return "MObserver"sv;}
	inline static TIdHash idHash() { return 0x6162659973315528;}
    public:
	using TCp = MNcp<MObserver, MObservable>;
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MObserver_Uid();}
	virtual string MObserver_Uid() const = 0;
	MIface* getLif(TIdHash aId) override { return MObserver_getLif(aId);}
	virtual MIface* MObserver_getLif(TIdHash aId) = 0;
	// Local
	virtual TCp* observerCp() = 0;
	virtual void onObsOwnerAttached(MObservable* aObl) = 0;
	virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) = 0;
	virtual void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) = 0;
	virtual void onObsContentChanged(MObservable* aObl, const string& aId) = 0;
	virtual void onObsChanged(MObservable* aObl) = 0;
	virtual void onObsEvent(MObservable* aObl, const MEvent* aEvent) {}
};

/** @brief Observable interface
 * */
class MObservable: public MIface
{
    public:
	inline static std::string_view idStr() { return "MObservable"sv;}
	inline static TIdHash idHash() { return 0x1f3be8525c6d0537;}
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MObservable_Uid();}
	virtual string MObservable_Uid() const = 0;
	MIface* getLif(TIdHash aId) { return MObservable_getLif(aId);}
	virtual MIface* MObservable_getLif(TIdHash aId) = 0;
	void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MObservable_doDump(aLevel, aIdt, std::cout);}
	virtual void MObservable_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual bool addObserver(MObserver::TCp* aObs) = 0;
	virtual bool rmObserver(MObserver::TCp* aObs) = 0;
	virtual bool addObserver(MObserver* aObs, TIdHash aEventId) = 0;
	virtual bool rmObserver(MObserver* aObs, TIdHash aEventId) = 0;
};


#endif
