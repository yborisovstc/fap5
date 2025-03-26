#ifndef __FAP5_MECONT_H
#define __FAP5_MECONT_H

#include "miface.h"

/** @brief Iface of DCE content owner
 * */
class MContentOwner: public MIface
{
    public:
	inline static std::string_view idStr() { return "MContentOwner"sv;}
	inline static TIdHash idHash() { return 0x1e4507d19fdbe;}
	// From MIface
	TIdHash id() const override { return idHash();}
	virtual string Uid() const override { return MContentOwner_Uid();}
	virtual string MContentOwner_Uid() const = 0;
	virtual MIface* getLif(TIdHash aId) override { return MContentOwner_getLif(aId);}
	virtual MIface* MContentOwner_getLif(TIdHash aId) = 0;
	virtual void doDump(int aLevel, int aIdt, ostream& aOs) const override { MContentOwner_doDump(aLevel, aIdt, aOs);}
	virtual void MContentOwner_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual int contCount() const = 0;
	virtual bool getContentId(int aIdx, string& aRes) const = 0;
	virtual bool getContent(const string& aId, string& aRes) const = 0;
	virtual bool setContent(const string& aId, const string& aData) = 0;
	// TODO is it needed?
	virtual void onContentChanged(const string& aId) = 0;
};

#endif
