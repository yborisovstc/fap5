#ifndef __FAP5_ENV_H
#define __FAP5_ENV_H

#include "menv.h"
#include "mnode.h"
#include "mlauncher.h"
#include "factr.h"
#include "prof.h"


class Logrec;
class Env;

/** @brief Import manager
 * */
class ImportsMgr: public MImportMgr
{
    public:
	ImportsMgr(Env& aHost);
	virtual ~ImportsMgr();
    public:
	// From MImportMgr
	virtual void GetModulesNames(vector<string>& aModules) const;
	virtual void ResetImportsPaths();
	virtual void AddImportsPaths(const string& aPaths);
	virtual string GetModulePath(const string& aModName) const;
	virtual bool Import(const string& aUri);
    private:
	void AddImportModulesInfo(const string& aPath);
	MNode* GetImportsContainer() const;
	void ImportToNode(MNode* aNode, const ChromoNode& aMut);
    private:
	Env& mHost;
	vector<string> mImportsPaths;
	map<string, string> mModsPaths;
	static const string KDefImportPath;
	static const string KImportsContainerUri;

	friend class Env;
};

template <int Dim> class DProf;

/** @brief Execution environment
 * */
class Env: public MEnv
{
    public:
	static const char* Type() { return "Env";};
	Env(const string& aSpecFile, const string& aLogFileName = string());
	//Env(const string& aSpec, const string& aLogFileName, bool aOpt);
	virtual ~Env();
	// From MEnv.MIface
	virtual string MEnv_Uid() const override { return Type();}
	virtual MIface* MEnv_getLif(const char *aType) override { return nullptr;}
	virtual void MEnv_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	// From MEnv
	virtual MProvider* provider() const override {return mProvider;}
	virtual void constructSystem() override;
	virtual MLogRec* Logger() override;
	inline virtual MProfiler *profiler() override { return mProf;}
	virtual MImportMgr* ImpsMgr() override { return mImpMgr;}
	virtual MNode* Root() const override;
	virtual MLauncher* Launcher() const { return mLauncher;}
	virtual bool RunSystem(int aCount = 0, int aIdleCount = 0) override;
	virtual bool StopSystem() override;
	bool addProvider(MProvider* aProv) override;
	void removeProvider(MProvider* aProv) override;
	virtual string modulesUri() const override;
	virtual bool setEVar(const string& aName, const string& aValue) override;
	virtual bool getEVar(const string& aName, string& aValue) const override;
    protected:
	MNode* mRoot;
	string mSpecFile;
	string mSpec;
	MChromo* mChromo;
	Factory* mProvider;
	MLogRec* mLogger;
	MLauncher* mLauncher = nullptr; /*!< Model's launcher */
	ImportsMgr* mImpMgr;
	map<string, string> mEVars;
	DProf<EPiid_NUM>* mProf;

};


#endif
