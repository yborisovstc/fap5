#ifndef __FAP5_MIFACE_H
#define __FAP5_MIFACE_H

#include <string>
#include <iostream>

using namespace std;

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
	/** @brief Getting id unique in the scope of the env */
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

#endif
