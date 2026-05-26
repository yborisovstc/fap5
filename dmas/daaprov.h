#ifndef __FAP5_DAAPROV_H
#define __FAP5_DAAPROV_H

#include "../src/prov.h"

// Provider for Distributed model agents
//
class DaaProv: public ProvBase
{
    public:
	DaaProv(const string &aName, MEnv* aEnv);
	// From MProvider
	//MIface* createIfi(const string& aType, const string& aName, MEnv* aEnv) override;
        // From ProvBase
	const TFReg& FReg() const override { return mReg;}
	const TIfiFReg& FIfiReg() const override { return mIfiReg;}
	const TIfiFRegH& FIfiRegH() const override { return mIfiRegH;}
	const TDtFReg& FDtReg() const override { return mDtReg;}
	void setChromoRslArgs(const string& aRargs) override {}
	void getChromoRslArgs(string& aRargs) override {} 
    protected:
	static const TFReg mReg;
	static const TIfiFReg mIfiReg;
	static const TIfiFRegH mIfiRegH;
	static const TDtFReg mDtReg;
};


#endif
