#ifndef __FAP5_MIFACE_H
#define __FAP5_MIFACE_H

#include <string>
#include <iostream>

using namespace std;

// TODO consider adding Uri() method or improve UID, ds_miuri
/** @brief Interface of interface
 * Any interface has to include type static char* idStr() const
 * and static const  TIdHash idHash() const
 * */
class MIface
{
    public:
	using TIdHash = std::size_t;
    public:
	virtual TIdHash id() const = 0;
	/** @brief Getting id unique in the scope of the env, mostly for debug puprose */
	virtual string Uid() const = 0;
	/** @brief Gets local interface of type aType */
	virtual MIface* getLif(TIdHash aTid) { return nullptr;}
	template <class T> T* lIf(T* aInst) {return aInst = reinterpret_cast<T*>(getLif(T::idHash()));}
	template <class T> const T* lIf(T* aInst) const { MIface* self = const_cast<MIface*>(this); return aInst = reinterpret_cast<T*>(self->getLif(T::idHash()));}
	template <class T> T* lIft() {return reinterpret_cast<T*>(getLif(T::idHash()));}
	/** @brief outputs dump
	 * @param aInt  indentation level
	 * */
	inline void dump(int aLevel, int aIdt = 0) const { doDump(aLevel, aIdt, std::cout);}
	virtual void doDump(int aLevel, int aIdt, std::ostream& aOs) const {}
};

#if 0
class MEvent : public MIface
{
    public:
	string Uid() const override { return string();}
	/** @brief Gets local interface of type aType */
	MIface* getLif(TIdHash aTid) override { return nullptr;}
	void doDump(int aLevel, int aIdt, std::ostream& aOs) const override {}
};
#endif

struct MEvent
{
    //TEvent(MIface::TIdHash aId): mId(aId) {}
    MEvent(MIface::TIdHash aId): mId(aId) {}
    MIface::TIdHash mId;
};


#endif
