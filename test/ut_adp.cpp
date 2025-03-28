#include <stdlib.h>

#include "ut_base.h"
#include "des.h"
#include "env.h"
#include "prov.h"

//#include <cppunit/extensions/HelperMacros.h>


/** @brief This test of vertex/system layers
*/
class Ut_adp : public Ut_fixture
{
    CPPUNIT_TEST_SUITE(Ut_adp);

    //CPPUNIT_TEST(test_MnodeAdp_1);
    //CPPUNIT_TEST(test_MnodeAdp_2);
    //CPPUNIT_TEST(test_MnodeAdp_3);
    //CPPUNIT_TEST(test_AdpDcs_1);
    CPPUNIT_TEST(test_Dadp_1);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    private:
    string getStateDstr(const string& aUri);
    private:
    /*
    void test_MnodeAdp_1();
    void test_MnodeAdp_2();
    void test_MnodeAdp_3();
    void test_AdpDcs_1();
    */
    void test_Dadp_1();
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_adp );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_adp, "Ut_adp");


string Ut_adp::getStateDstr(const string& aUri)
{
    MNode* st = mEnv->Root()->getNode(aUri);
    MDVarGet* stg = st ? st->lIf(stg) : nullptr;
    const DtBase* data = stg ? stg->VDtGet(string()) : nullptr;
    return data ? data->ToString(true) : string();
}

void Ut_adp::setUp()
{
}

void Ut_adp::tearDown()
{
    //    delete mEnv;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}


#if 0
/** @brief MNode DES adapter test
 * */
void Ut_adp::test_MnodeAdp_1()
{
    printf("\n === Test of MNode DES adapter w/o specifying MAG owner\n");

    const string specn("ut_adp_node_1");
    string ext = "chs";
    string spec = specn + string(".") + "chs";
    string log = specn + "_" + ext + ".log";
    mEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", mEnv != 0);
    mEnv->ImpsMgr()->ResetImportsPaths();
    mEnv->ImpsMgr()->AddImportsPaths("../modules");
    mEnv->constructSystem();
    MNode* root = mEnv->Root();
    MElem* eroot = root ? root->lIf(eroot) : nullptr;
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root && eroot);

    // Verify target agent
    MNode* tag = root->getNode("test.Controller.Adapter.Targets.Target");
    CPPUNIT_ASSERT_MESSAGE("Fail to get target agent", tag);
    // Verify adapter
    MNode* adp = root->getNode("test.Controller.Adapter");
    CPPUNIT_ASSERT_MESSAGE("Fail to get adapter", adp);

    bool res = mEnv->RunSystem(7);

    // Verify comps count
    MNode* cmpCount = root->getNode("test.Controller.Adapter.CompsCount");
    CPPUNIT_ASSERT_MESSAGE("Fail to get adapters CompsCount", cmpCount);
    MUnit* cmpCountu = cmpCount->lIf(cmpCountu);
    MDVarGet* cmpCountVget = cmpCountu->getSif(cmpCountVget);
    CPPUNIT_ASSERT_MESSAGE("Fail to get CompsCount VarGet iface", cmpCountVget);
    MDtGet<Sdata<int>>* cmpCountGsi = cmpCountVget->GetDObj(cmpCountGsi);
    CPPUNIT_ASSERT_MESSAGE("Fail to get cmpCountGsi ", cmpCountGsi);
    Sdata<int> cmpCountSi = 0;
    cmpCountGsi->DtGet(cmpCountSi);
    int cc = cmpCountSi.mData;
    CPPUNIT_ASSERT_MESSAGE("Incorrect CompsCount value", cc == 4);
    /*
    // Verify comp UID
    MNode* cmpUid = root->GetNode("./test/Controller/Adapter/CompUid");
    CPPUNIT_ASSERT_MESSAGE("Fail to get adapters CompUid", cmpUid != NULL);
    MDVarGet* cmpUidVget = cmpUid->GetSIfit(cmpUidVget);
    CPPUNIT_ASSERT_MESSAGE("Fail to get CompUid VarGet iface", cmpUidVget != NULL);
    MDtGet<Sdata<string>>* cmpUidGsi = cmpUidVget->GetDObj(cmpUidGsi);
    CPPUNIT_ASSERT_MESSAGE("Fail to get cmpUidGsi ", cmpUidGsi != NULL);
    Sdata<string> cmpUidSi;
    cmpUidGsi->DtGet(cmpUidSi);
    MNode* tagCmp = tag->GetComp(cnt);
    if (tagCmp) {
    CPPUNIT_ASSERT_MESSAGE("Incorrect CompUid", cmpUidSi.mData == tagCmp->Uid());
    }
    */

    delete mEnv;
}

/** @brief MNode DES adapter test
 * */
void Ut_adp::test_MnodeAdp_2()
{
    printf("\n === Test of MNode DES adapter with specifying MAG owner\n");

    const string specn("ut_adp_node_2");
    string ext = "chs";
    string spec = specn + string(".") + "chs";
    string log = specn + "_" + ext + ".log";
    mEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", mEnv != 0);
    mEnv->ImpsMgr()->ResetImportsPaths();
    mEnv->ImpsMgr()->AddImportsPaths("../modules");
    mEnv->constructSystem();
    MNode* root = mEnv->Root();
    MElem* eroot = root ? root->lIf(eroot) : nullptr;
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root && eroot);

    // Verify target agent
    MNode* tag = root->getNode("test.Controller.Targets.Target");
    CPPUNIT_ASSERT_MESSAGE("Fail to get target agent", tag);
    // Verify adapter
    MNode* adp = root->getNode("test.Controller.Adapter");
    CPPUNIT_ASSERT_MESSAGE("Fail to get adapter", adp);

    bool res = mEnv->RunSystem(7);

    // Verify comps count
    MNode* cmpCount = root->getNode("test.Controller.Adapter.CompsCount");
    CPPUNIT_ASSERT_MESSAGE("Fail to get adapters CompsCount", cmpCount);
    MUnit* cmpCountu = cmpCount->lIf(cmpCountu);
    MDVarGet* cmpCountVget = cmpCountu->getSif(cmpCountVget);
    CPPUNIT_ASSERT_MESSAGE("Fail to get CompsCount VarGet iface", cmpCountVget);
    MDtGet<Sdata<int>>* cmpCountGsi = cmpCountVget->GetDObj(cmpCountGsi);
    CPPUNIT_ASSERT_MESSAGE("Fail to get cmpCountGsi ", cmpCountGsi);
    Sdata<int> cmpCountSi = 0;
    cmpCountGsi->DtGet(cmpCountSi);
    int cc = cmpCountSi.mData;
    CPPUNIT_ASSERT_MESSAGE("Incorrect CompsCount value", cc == 4);

    delete mEnv;
}

/** @brief MNode DES adapter test: MAG base via input
 * */
void Ut_adp::test_MnodeAdp_3()
{
    printf("\n === Test of MNode DES adapter with specifying MAG base via input\n");

    const string specn("ut_adp_node_3");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    mEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", mEnv != 0);
    mEnv->ImpsMgr()->ResetImportsPaths();
    mEnv->ImpsMgr()->AddImportsPaths("../modules");
    mEnv->constructSystem();
    MNode* root = mEnv->Root();
    MElem* eroot = root ? root->lIf(eroot) : nullptr;
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root && eroot);

    // Verify target agent
    MNode* tag = root->getNode("test.Controller.Targets.Target");
    CPPUNIT_ASSERT_MESSAGE("Fail to get target agent", tag);
    // Verify adapter
    MNode* adp = root->getNode("test.Controller.Adapter");
    CPPUNIT_ASSERT_MESSAGE("Fail to get adapter", adp);

    bool res = mEnv->RunSystem(9);

    // Verify comps count
    MNode* cmpCount = root->getNode("test.Controller.Adapter.CompsCount");
    CPPUNIT_ASSERT_MESSAGE("Fail to get adapters CompsCount", cmpCount);
    MUnit* cmpCountu = cmpCount->lIf(cmpCountu);
    MDVarGet* cmpCountVget = cmpCountu->getSif(cmpCountVget);
    CPPUNIT_ASSERT_MESSAGE("Fail to get CompsCount VarGet iface", cmpCountVget);
    MDtGet<Sdata<int>>* cmpCountGsi = cmpCountVget->GetDObj(cmpCountGsi);
    CPPUNIT_ASSERT_MESSAGE("Fail to get cmpCountGsi ", cmpCountGsi);
    Sdata<int> cmpCountSi = 0;
    cmpCountGsi->DtGet(cmpCountSi);
    int cc = cmpCountSi.mData;
    CPPUNIT_ASSERT_MESSAGE("Incorrect CompsCount value", cc == 5);

    delete mEnv;
}

/** @brief MNode DES adapter DCS test/example: ordering of agents
 * */
void Ut_adp::test_AdpDcs_1()
{
    printf("\n === DCS test/example: agents ordering \n");

    const string specn("ut_adp_dcs_1");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    mEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", mEnv != 0);
    mEnv->ImpsMgr()->ResetImportsPaths();
    mEnv->ImpsMgr()->AddImportsPaths("../modules");
    mEnv->constructSystem();
    MNode* root = mEnv->Root();
    MElem* eroot = root ? root->lIf(eroot) : nullptr;
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root && eroot);

    bool res = mEnv->RunSystem(9, 5);


    delete mEnv;
}
#endif

/** @brief MNode SDO test - existence of component
 * */
void Ut_adp::test_Dadp_1()
{
    printf("\n === Test of Adp: base\n");
    MNode* root = constructSystem("ut_adp_dadp_1");

    // Phase 1
    printf("\n ==== Phase#1\n");
    bool res = mEnv->RunSystem(2);
    CPPUNIT_ASSERT_MESSAGE("Phase1: ExUri failed", getStateDstr("test.Controller.ExUri") == "URI .testroot.test.Controller.Targets.Target5");
    CPPUNIT_ASSERT_MESSAGE("Phase1: Counter failed", getStateDstr("test.Controller.Counter") == "SI 2");
    //CPPUNIT_ASSERT_MESSAGE("Phase1: OutpMagUri_Dbg failed", getStateDstr("test.Controller.OutpMagUri_Dbg") == "SS 'Target2'");
    CPPUNIT_ASSERT_MESSAGE("Phase1: CompsCount failed", getStateDstr("test.Controller.CompCount") == "SI 2");
    CPPUNIT_ASSERT_MESSAGE("Phase1: CompsNames failed", getStateDstr("test.Controller.CompNames") == "VS (Ext1,St1)");
    CPPUNIT_ASSERT_MESSAGE("Phase1: Name failed", getStateDstr("test.Controller.Name_Dbg") == "SS 'Target5'");
    CPPUNIT_ASSERT_MESSAGE("Phase1: AddCompOutp_Dbg failed", getStateDstr("test.Controller.AddCompOutp_Dbg") == "SB false");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompExists failed", getStateDstr("test.Controller.CompExists") == "SB false");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompExists failed", getStateDstr("test.Controller.ConnExists") == "SB true");
    CPPUNIT_ASSERT_MESSAGE("Phase2: PairsCount failed", getStateDstr("test.Controller.PairsCount") == "SI 1");
    CPPUNIT_ASSERT_MESSAGE("Phase2: DesIsIdle failed", getStateDstr("test.Controller.DesIsIdle") == "SB false");


    // Phase 2
    printf("\n ==== Phase#2\n");
    res = mEnv->RunSystem(3);
    CPPUNIT_ASSERT_MESSAGE("Phase2: Counter failed", getStateDstr("test.Controller.Counter") == "SI 5");
    //CPPUNIT_ASSERT_MESSAGE("Phase2: OutpMagUri_Dbg failed", getStateDstr("test.Controller.OutpMagUri_Dbg") == "SS 'Target3'");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompsCount failed", getStateDstr("test.Controller.CompCount") == "SI 4");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompsNames failed", getStateDstr("test.Controller.CompNames") == "VS (Cmp3_0,Cmp3_1,Cmp3_2,Cmp3_3)");
    CPPUNIT_ASSERT_MESSAGE("Phase2: AddCompOutp_Dbg failed", getStateDstr("test.Controller.AddCompOutp_Dbg") == "SB false");
    //printf("test.Controller.CompsUri: %s\n", getStateDstr("test.Controller.CompsUri").c_str());
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompsUri failed", getStateDstr("test.Controller.CompsUri") == "VDU (URI Cmp3_0,URI Cmp3_1,URI Cmp3_2,URI Cmp3_3)");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompComp failed", getStateDstr("test.Controller.CompComp") == "URI .testroot.test.Controller.Targets.Target3.Cmp3_3.Cmp3_3_0");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompExists failed", getStateDstr("test.Controller.CompExists") == "SB true");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompExists failed", getStateDstr("test.Controller.ConnExists") == "SB <ERR>");
    CPPUNIT_ASSERT_MESSAGE("Phase2: PairsCount failed", getStateDstr("test.Controller.PairsCount") == "SI <ERR>");
    CPPUNIT_ASSERT_MESSAGE("Phase2: DesIsIdle failed", getStateDstr("test.Controller.DesIsIdle") == "SB <ERR>");

    // Phase 3 - new comp added
    printf("\n ==== Phase#3\n");
    res = mEnv->RunSystem(3);
    CPPUNIT_ASSERT_MESSAGE("Phase3: AddCompOutp_Dbg failed", getStateDstr("test.Controller.AddCompOutp_Dbg") == "SB true");
    CPPUNIT_ASSERT_MESSAGE("Phase3: CompsCount failed", getStateDstr("test.Controller.CompCount") == "SI 5");
    CPPUNIT_ASSERT_MESSAGE("Phase3: CompsNames failed", getStateDstr("test.Controller.CompNames") == "VS (Cmp3_0,Cmp3_1,Cmp3_2,Cmp3_3,New_node)");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompsUri failed", getStateDstr("test.Controller.CompsUri") == "VDU (URI Cmp3_0,URI Cmp3_1,URI Cmp3_2,URI Cmp3_3,URI New_node)");

    delete mEnv;
}

