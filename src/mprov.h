
#ifndef __FAP5_MPROV_H
#define __FAP5_MPROV_H


#include <string>
#include <vector>

#include "miface.h"

class MEnv;
class MNode;
class MChromo;


using namespace std;

class DtBase;

/** @brief Interface of native agents provicer
 *
 * */
class MProvider: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MProvider"sv;}
	inline static constexpr TIdHash idHash() { return 0xb1cbbc1ec7173a5c;}
	TIdHash id() const override { return idHash();}
	virtual ~MProvider() {};
	// From MIface
	string Uid() const override { return MProvider_Uid();}
	virtual string MProvider_Uid() const = 0;
	MIface* getLif(TIdHash aId) override { return MProvider_getLif(aId);}
	virtual MIface* MProvider_getLif(TIdHash aId) = 0;
	void doDump(int aLevel, int aIdt, std::ostream& aOs) const override { return MProvider_doDump(aLevel, aIdt, aOs);}
	virtual void MProvider_doDump(int aLevel, int aIdt, ostream& aOs) const = 0;
	// Local
	virtual const string& providerName() const = 0;
	virtual MNode* createNode(const string& aType, const string& aName, MEnv* aEnv) = 0;
	virtual MNode* provGetNode(const string& aUri) = 0;
	virtual bool isProvided(const MNode* aElem) const = 0;
	/** @brief Sets chromo resolution arguments
	 * Just one argument, chromo spec file extension is used
	 * */
	virtual void setChromoRslArgs(const string& aRargs) = 0;
	virtual void getChromoRslArgs(string& aRargs) = 0;
	virtual MChromo* createChromo(const string& aRargs = string()) = 0;
	virtual void getNodesInfo(vector<string>& aInfo) = 0;
	virtual const string& modulesPath() const = 0;
	virtual void setEnv(MEnv* aEnv) = 0;
	/** @brief Creates data instnce by the type info
	 * */
	virtual DtBase* createData(const string& aType) {return nullptr;}
};



#endif
