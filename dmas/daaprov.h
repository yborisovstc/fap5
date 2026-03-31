#ifndef __FAP5_DAAPROV_H
#define __FAP5_DAAPROV_H

#include "../src/prov.h"

// Provider for Distributed model agents
//
class DaaProv: public Provider
{
    public:
	DaaProv(const string &aName, MEnv* aEnv);
	// From MProvider
	MIface* createIfi(const string& aType, const string& aName, MEnv* aEnv) override;
};


#endif
