
#ifndef __FAP5_DESIO_H
#define __FAP5_DESIO_H

#include "des.h"

/** @brief Output
 * */
class Sout : public State
{
    public:
	inline static constexpr std::string_view idStr() { return "Sout"sv;}
	Sout(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From State.MDesSyncable
	virtual void update() override;
	virtual void confirm() override;
};


#endif
