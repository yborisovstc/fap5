#ifndef __FAP5_MSYST_H
#define __FAP5_MSYST_H

#include <vector>

#include "miface.h"

#include "mvert.h"


/** @brief Edge spec
 * */
using TEdge = pair<MVert*, MVert*>;
using TEdges = std::vector<TEdge>;

/** @brief System as connected subsystems
 * */
class MSyst: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MSyst"sv;}
	inline static constexpr TIdHash idHash() { return 0x3558aa937bbb2f;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MSyst_Uid();}
	virtual string MSyst_Uid() const = 0;
	// Local
	/** @brief Get connections
	 * */
	virtual const TEdges& connections() const = 0;
};

class MAhost;

/** @brief System embedded managing agent
 * System delegates agent handling the requests
 * */
class MAgent: public MIface
{
    public:
	using TCp = MNcp<MAgent, MAhost>;
    public:
	inline static constexpr std::string_view idStr() { return "MAgent"sv;}
	inline static constexpr TIdHash idHash() { return 0x253392665466b0a2;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MAgent_Uid();}
	MIface* getLif(TIdHash aId) override { return MAgent_getLif(aId);}
	// Local
	virtual string MAgent_Uid() const = 0;
	virtual MIface* MAgent_getLif(TIdHash aId) = 0;
};


/** @brief System host (manageable) of embedded managing agent
 * System delegates agent handling the requests thru this iface
 * */
class MAhost: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MAhost"sv;}
	inline static constexpr TIdHash idHash() { return 0x5daf5c9fd0239bfb;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MAhost_Uid();}
	MIface* getLif(TIdHash aId) override { return MAhost_getLif(aId);}
	// Local
	virtual string MAhost_Uid() const = 0;
	virtual MIface* MAhost_getLif(TIdHash aId) = 0;
};

class MNode;

/** @brief System managed iface
 * */
class MSystExplorable: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MSystExplorable"sv;}
	inline static constexpr TIdHash idHash() { return 0x20a2680ef32e47b1;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MSystExplorable_Uid();}
	MIface* getLif(TIdHash aId) override { return MSystExplorable_getLif(aId);}
	// Local
	virtual string MSystExplorable_Uid() const = 0;
	virtual MIface* MSystExplorable_getLif(TIdHash aId) = 0;
	virtual MNode* getMag() = 0;
};

/** @brief System manageding iface
 * */
class MSystExploring: public MIface
{
    public: 
	using TCp = MNcp<MSystExploring, MSystExplorable>;
    public:
	inline static constexpr std::string_view idStr() { return "MSystExploring"sv;}
	inline static constexpr TIdHash idHash() { return 0x5be939ac33d3b333;}
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MSystExploring_Uid();}
	MIface* getLif(TIdHash aId) override { return MSystExploring_getLif(aId);}
	// Local
	virtual string MSystExploring_Uid() const = 0;
	virtual MIface* MSystExploring_getLif(TIdHash aId) = 0;
	virtual void onMagChanged() = 0;
	virtual TCp* getCp() = 0;
};









#endif
