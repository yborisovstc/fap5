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






#endif
