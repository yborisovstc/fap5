
#include "provdef.h"
/*
#include "vert.h"
#include "prop.h"
#include "syst.h"
#include "incaps.h"
#include "data.h"
#include "func.h"
#include "des.h"
#include "module.h"
#include "imports.h"
#include "mnt.h"
*/
#include "node.h"
#include "chromo2.h"
#include "elem.h"
#include "import.h"
#include "syst.h"
#include "des.h"
#include "dest.h"
#include "desconst.h"
#include "desadp.h"
#include "dessdo.h"
#include "dessdc.h"
#include "desio.h"
/*
#include "content.h"
#include "dessp.h"
#include "mnt.h"
#include "link.h"
*/

// TODO [YB] To import from build variable
const string KModulesPath = "/usr/share/grayb/modules/";

/** @brief Chromo arguments */
const string KChromRarg_Xml = "xml";
const string KChromRarg_Chs = "chs";

/** Native agents factory registry */
const ProvDef::TFReg ProvDef::mReg ( {
        Item<Node>(), Item<Elem>(), Item<Vert>(), Item<Verte>(), Item<Import>(), Item<Syst>(), Item<State>(), Item<BState>(), Item<Des>(),
        Item<DesLauncher>(), Item<Const>(),
        /* Transitions */
        Item<TrAddVar>(), Item<TrAdd2Var>(), Item<TrSizeVar>(), Item<TrSwitchBool>(), Item<TrSwitchBool2>(), Item<TrMplVar>(),
        Item<TrDivVar>(), Item<TrMinVar>(), Item<TrMaxVar>(), Item<TrAndVar>(), Item<TrOrVar>(), Item<TrNegVar>(), 
        Item<TrToUriVar>(), Item<TrAtVar>(), Item<TrAtgVar>(), Item<TrApndVar>(),
        Item<TrFindByP>(), Item<TrTuple>(), Item<TrTupleSel>(), 
        Item<TrCmpVar>(), Item<TrTostrVar>(),
        Item<TrInpSel>(), Item<TrInpCnt>(),
        Item<TrChr>(), Item<TrChrc>(), 
        Item<TrMutNode>(), Item<TrMutConn>(), Item<TrMutDisconn>(), Item<TrMutCont>(), 
        Item<TrIsValid>(), Item<TrIsInvalid>(), Item<TrSvldVar>(), 
        Item<TrTailVar>(), Item<TrHeadVar>(), Item<TrHeadtnVar>(), Item<TrTailnVar>(),
        Item<TrHash>(), Item<TrPair>(),
        /* Connpoints */
        Item<CpStateInp>(), Item<CpStateInpPin>(), Item<CpStateOutp>(), Item<CpStateOutpPin>(), Item<ExtdStateInp>(), Item<ExtdStateOutp>(),
        Item<Socket>(), Item<CpSystExplorable>(), Item<CpSystExploring>(),
        /* DES constants */
        Item<SI_0>(), Item<SI_1>(), Item<SB_False>(), Item<SB_True>(),
        /* DES active subs */
        Item<DesAs>(), Item<DesAs2>(),
	/* DES adapter (system exploring) */
	Item<DAdp>(),
        /* SDOs */
	Item<SdoName>(), Item<SdoUri>(), Item<SdoCompsCount>(), Item<SdoCompsNames>(), Item<SdoParent>(), Item<SdoParents>(),
	Item<SdoCompOwner>(), Item<SdoCompsUri>(), Item<SdoCompComp>(), Item<SdoComp>(), 
	Item<SdoConn>(), Item<SdoPairsCount>(), Item<SdoPair>(), Item<SdoTcPair>(), Item<SdoPairs>(), 
        Item<SdoTPairs>(), Item<SdoEdges>(), Item<SdoDesIdle>(), 
        /* SDCs */
	Item<ASdcComp>(), Item<ASdcCompT>(), Item<ASdcRm>(), Item<ASdcConn>(), Item<ASdcDisconn>(), 
	Item<ASdcInsert2>(), Item<ASdcExtract>(), 
	/* DES IO */
	Item<Sout>(), 
	/* SDCs */
        Item<ASdcMut>(), 

#if 0
        Item<Link>(),
        Item<Extd>(), Item<Extde>(), Item<TrSub2Var>(),
        Item<ADes>(),
        /* Des adapters */
        Item<AMntp>(),
	/* SDCs */
        Item<ASdcInsertN>(),
        Item<ASdcPause>(), Item<ASdcResume>(), Item<ASdcConnT>(), 
        /* DES context */
        Item<DesCtxSpl>(), Item<DesCtxCsm>(), 
        /* DES service point */
        Item<DesSpe>(), Item<DesSp>(), Item<DesSpt>(), Item<ADesSpc>(), 
#endif
});

/** Data factory registry */
const ProvDef::TDtFReg ProvDef::mDtReg ( {
	DItem<DGuri>(), 
	DItem<Sdata<int>>(), DItem<Sdata<bool>>(), DItem<Sdata<string>>(),
	DItem<Pair<string>>(), DItem<Pair<Sdata<int>>>(), DItem<Pair<Sdata<string>>>(), DItem<Pair<DGuri>>(),
	DItem<Vector<string>>(), DItem<Vector<DGuri>>(), DItem<Vector<Pair<DGuri>>>(), DItem<Vector<Pair<string>>>(), 
	DItem<NTuple>(), 
	DItem<DChr2>(), DItem<DMut>()
	});




ProvDef::ProvDef(const string& aName, MEnv* aEnv): ProvBase(aName, aEnv)
{
}

ProvDef::~ProvDef()
{
}

void ProvDef::getNodesInfo(vector<string>& aInfo)
{
    for (auto elem : mReg) {
	aInfo.push_back(elem.first);
    }
}

const string& ProvDef::modulesPath() const
{
    return KModulesPath;
}

MChromo* ProvDef::createChromo(const string& aRargs)
{
    MChromo* res = NULL;
    if (aRargs == KChromRarg_Chs) {
	res = new Chromo2();
    } else if (aRargs.empty()) {
	// Default chromo type
	res = new Chromo2();
    }
    return res;
}
