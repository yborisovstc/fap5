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
    CPPUNIT_TEST(test_Dadp_1);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    private:
    string getStateDstr(const string& aUri);
    private:
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
    CPPUNIT_ASSERT_MESSAGE("Phase1: CompExists failed", getStateDstr("test.Controller.CompExists") == "SB false");
    CPPUNIT_ASSERT_MESSAGE("Phase1: CompExists failed", getStateDstr("test.Controller.ConnExists") == "SB true");
    CPPUNIT_ASSERT_MESSAGE("Phase1: PairsCount failed", getStateDstr("test.Controller.PairsCount") == "SI 1");
    CPPUNIT_ASSERT_MESSAGE("Phase1: DesIsIdle failed", getStateDstr("test.Controller.DesIsIdle") == "SB false");
    CPPUNIT_ASSERT_MESSAGE("Phase1: VPair failed", getStateDstr("test.Controller.VPair") == "URI .testroot.test.Controller.Targets.Target5.Ext1.Int");
    CPPUNIT_ASSERT_MESSAGE("Phase1: TcPair failed", getStateDstr("test.Controller.TcPair") == "URI .testroot.test.Controller.Targets.Target5.St1");
    CPPUNIT_ASSERT_MESSAGE("Phase1: MagPairs failed", getStateDstr("test.Controller.MagPairs") == "VDU <ERR>");
    CPPUNIT_ASSERT_MESSAGE("Phase1: TPairs failed", getStateDstr("test.Controller.TPairs") == "VDU (URI .testroot.test.Controller.Targets.Target5.Ext1.Int)");
    CPPUNIT_ASSERT_MESSAGE("Phase1: MagEdges failed", getStateDstr("test.Controller.MagEdges") ==
	    "VPDU (PDU (URI .testroot.test.Controller.Targets.Target5.Ext1.Int,URI .testroot.test.Controller.Targets.Target5.St1))");


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
    CPPUNIT_ASSERT_MESSAGE("Phase2: VPair failed", getStateDstr("test.Controller.VPair") == "URI <ERR>");
    CPPUNIT_ASSERT_MESSAGE("Phase2: TcPair failed", getStateDstr("test.Controller.TcPair") == "URI <ERR>");
    CPPUNIT_ASSERT_MESSAGE("Phase2: MagPairs failed", getStateDstr("test.Controller.MagPairs") == "VDU <ERR>");
    CPPUNIT_ASSERT_MESSAGE("Phase2: TPairs failed", getStateDstr("test.Controller.TPairs") == "VDU <ERR>");
    CPPUNIT_ASSERT_MESSAGE("Phase1: MagEdges failed", getStateDstr("test.Controller.MagEdges") == "VPDU <ERR>");

    // Phase 3 - new comp added
    printf("\n ==== Phase#3\n");
    res = mEnv->RunSystem(3);
    CPPUNIT_ASSERT_MESSAGE("Phase3: AddCompOutp_Dbg failed", getStateDstr("test.Controller.AddCompOutp_Dbg") == "SB true");
    CPPUNIT_ASSERT_MESSAGE("Phase3: CompsCount failed", getStateDstr("test.Controller.CompCount") == "SI 5");
    CPPUNIT_ASSERT_MESSAGE("Phase3: CompsNames failed", getStateDstr("test.Controller.CompNames") == "VS (Cmp3_0,Cmp3_1,Cmp3_2,Cmp3_3,New_node)");
    CPPUNIT_ASSERT_MESSAGE("Phase2: CompsUri failed", getStateDstr("test.Controller.CompsUri") == "VDU (URI Cmp3_0,URI Cmp3_1,URI Cmp3_2,URI Cmp3_3,URI New_node)");

    delete mEnv;
}

