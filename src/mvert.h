#ifndef __FAP5_MVERT_H
#define __FAP5_MVERT_H

#include "nconn.h"
#include "miface.h"
#include "ifu.h"

using namespace std;


/** @brief Interface of connectable
 * Represents generic connection of system level
 * TODO seems MCIface inheritance isn't useful here, consider simplify design
 * */
class MVert: public MIface
{
    public:
	enum TDir { ERegular, EInp, EOut };
    public:
	inline static constexpr std::string_view idStr() { return "MVert"sv;}
	inline static constexpr TIdHash idHash() { return 0xedc4d46b41b2bbeb;}
	// From MIface 
	TIdHash id() const override { return idHash();}
	virtual string MVert_Uid() const = 0;
	string Uid() const override { return MVert_Uid();}
	MIface *getLif(TIdHash aId) override { return MVert_getLif(aId);}
	virtual MIface *MVert_getLif(TIdHash aId) = 0;
	void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MVert_doDump(aLevel, aIdt, std::cout);}
	virtual void MVert_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	/** @brief Indicates if connnectable is compatible */
	virtual bool isCompatible(const MVert* aPair, bool aExt = false) const = 0;
	/** @brief Gets extended part in case if checked is extender */
	virtual MVert* getExtd() = 0;
	/** @brief Indicates direction */
	virtual TDir getDir() const = 0;
	virtual int pairsCount() const = 0;
	virtual MVert* getPair(int aInd) const = 0;
	/** @brief Gets sign of vertex linked to given vertex
	 * Is shows not only direct linkage (pairing) but also the linkage via components
	 * */
	//virtual bool isLinked(const MVert* aPair, bool aDirect = false) const = 0;
        /** @brief Connects to pair, one-way, returns success indicator */
	virtual bool connect(MVert* aPair) = 0;
	/** @brief Disconnects to pair, one-way, returns success indicator */
	virtual bool disconnect(MVert* aPair) = 0;
	/** @brief Connects to pair, two-way, returns success indicator */
	static bool connect(MVert* aA, MVert* aB) {
            return (aA->isCompatible(aB) && aB->isCompatible(aA)) ? aA->connect(aB) && aB->connect(aA) : false;
        }
	/** @brief Disonnects pairs, two-way, returns success indicator */
	static bool disconnect(MVert* aSelf, MVert* aPair) { return aPair->disconnect(aSelf) ? aSelf->disconnect(aPair) : false;}
	/** @brief Indicates of connected to pair */
	virtual bool isConnected(const MVert* aPair) const = 0;
        /** @brief Bounds two verted for extending connection abilities
         * */
        // TODO maybe it makes sense to move this to separate iface, like MConnPoint
	virtual bool isBound(const MIface* aBound) const = 0;
        virtual bool bind(MIface* aBound) = 0;
        virtual bool unbind(MIface* aBound) = 0;
};

#endif
