#ifndef __FAP5_MWORKERS_H
#define __FAP5_MWORKERS_H

#include "miface.h"

class MJobOwner;


/** @brief Thread job
 * */
class MJob
{
    public:
	//inline static std::string_view idStr() { return "MJob"sv;}
	//inline static TIdHash idHash() { return 0x1a550117247d00;}
        virtual std::string id() const = 0;
        virtual const MJobOwner* owner() const = 0;
        virtual void perform() = 0;
        virtual void complete() = 0;
};

/** @brief Thread job owner
 * */
class MJobOwner : MIface
{
    public:
	inline static std::string_view idStr() { return "MJobOwner"sv;}
	inline static TIdHash idHash() { return 0x649693ffb9834c1b;}
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MJobOwner_Uid();}
	virtual string MJobOwner_Uid() const = 0;
	MIface* getLif(TIdHash aId) override { return MJobOwner_getLif(aId);}
	virtual MIface* MJobOwner_getLif(TIdHash aId) = 0;
	// Local
        virtual void onJobCompleted(const MJob* aJob) = 0;
};


/** @brief Thread pool interface
 * */
class MThreadPool
{
    public:
        virtual bool takeJob(MJob* aJob) = 0;
};

#endif


