#ifndef __FAP5_MCONNPOINT_H
#define __FAP5_MCONNPOINT_H

#include "miface.h"
#include "npconn.h"
#include "nconn.h"

class MVert;

/** @brief Connection point interface
 * */
class MConnPoint : public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MConnPoint"sv;}
	inline static constexpr TIdHash idHash() { return 0x4ab927ef8407add3;}
    public:
	// From MIface
	TIdHash id() const override { return idHash(); }
	string Uid() const override { return MConnPoint_Uid();}
	virtual string MConnPoint_Uid() const = 0;
	// Local
        virtual TIdHash idProvided() const = 0;
        virtual TIdHash idRequired() const = 0;
        virtual MNpc* bP() = 0;
        // Binding
        virtual bool bind(MNpc* aPair) = 0;
        virtual bool unbind(MNpc* aPair) = 0;
	virtual bool isBound(const MNpc* aPair) const = 0;
};

/** @brief Socket interface
 * */
class MSocket: public MIface
{
    public:
        using TBp = MNcp<MVert, MVert>;
    public:
	inline static constexpr std::string_view idStr() { return "MSocket"sv;}
	inline static constexpr TIdHash idHash() { return 0xef54a3eb58d53b3c;}
    public:
	// From MIface
	TIdHash id() const override { return idHash(); }
	string Uid() const override { return MSocket_Uid();}
	virtual string MSocket_Uid() const = 0;
	// Local
	//virtual int PinsCount() const = 0;
	//virtual MVert* GetPin(int aInd) = 0;
	//virtual string GetPinId(int aInd) const = 0;
	virtual MVert* GetPin(const string& aId) = 0;
	//virtual MVert* GetPin(const string& aId, bool aExtd) { return nullptr;}
        virtual TBp* bP() { return nullptr;} // TODO make abstract
};

/** @brief Extender internal point interface
 * */
class MExtdInt: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MExtdInt"sv;}
	inline static constexpr TIdHash idHash() { return 0x2cf390006a434928;}
    public:
        using TBp = MNcp<MVert, MVert>;
    public:
	// From MIface
	TIdHash id() const override { return idHash(); }
	string Uid() const override { return MExtdInt_Uid();}
	virtual string MExtdInt_Uid() const = 0;
        // Local
        virtual TBp* bP() = 0;
};


#endif
