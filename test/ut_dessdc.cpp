#include <stdlib.h>

#include "des.h"
#include "env.h"
#include "prov.h"
#include "rdatauri.h"

#include "ut_base.h"


/** @brief This test of vertex/system layers
*/
class Ut_sdc : public Ut_fixture
{
    CPPUNIT_TEST_SUITE(Ut_sdc);

    //CPPUNIT_TEST(test_Sdc_1);
    //CPPUNIT_TEST(test_Sdc_2);
    //CPPUNIT_TEST(test_Sdc_2_1);
    //CPPUNIT_TEST(test_Sdc_3);
    //CPPUNIT_TEST(test_Sdc_4);
    //CPPUNIT_TEST(test_Sdc_5);
    //CPPUNIT_TEST(test_Sdo_1);
    CPPUNIT_TEST(test_Sdo_2);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    private:
    string getStateDstr(const string& aUri);
    private:
    void test_Sdc_1();
    void test_Sdc_2();
    void test_Sdc_2_1();
    void test_Sdc_3();
    void test_Sdc_4();
    void test_Sdc_5();
    void test_Sdo_1();
    void test_Sdo_2();
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_sdc );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_sdc, "Ut_sdc");

string Ut_sdc::getStateDstr(const string& aUri)
{
    MNode* st = mEnv->Root()->getNode(aUri);
    MDVarGet* stg = st ? st->lIf(stg) : nullptr;
    const DtBase* data = stg ? stg->VDtGet(string()) : nullptr;
    return data ? data->ToString(true) : string();
}

void Ut_sdc::setUp()
{
}

void Ut_sdc::tearDown()
{
    //    delete mEnv;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}


/** @brief MNode DES adapter test
 * */
void Ut_sdc::test_Sdc_1()
{
    printf("\n === Test of SDC base controllers\n");
    const string specn("ut_sdc_1");
    MNode* root = constructSystem(specn);

    bool res = mEnv->RunSystem(7, 2);

    // Verify
    MNode* comp = root->getNode("Launcher.Comp2");
    CPPUNIT_ASSERT_MESSAGE("Comp hasn't been created", comp);

    CPPUNIT_ASSERT_MESSAGE("CompsCount failed", getStateDstr("Launcher.Dc_Conn_Dbg") == "SB true");

    delete mEnv;
}


/** @brief MNode DES adapter test
 * */
void Ut_sdc::test_Sdc_2()
{
    printf("\n === Test of SDC base controllers - list insertion/extraction\n");

    const string specn("ut_sdc_2");
    MNode* root = constructSystem(specn);

    bool res = mEnv->RunSystem(7, 2);

    // Verify the comp created
    MNode* lnode = root->getNode("Launcher.List.Node_1");
    CPPUNIT_ASSERT_MESSAGE("List node hasn't been created", lnode);

    delete mEnv;
}

/** @brief SDC test - insertion the number of list items
 * */
void Ut_sdc::test_Sdc_2_1()
{
    printf("\n === Test of SDC: list insertion of number of items\n");
    MNode* root = constructSystem("ut_sdc_2_1");

    bool res = mEnv->RunSystem(12, 2);

    // Verify the connection
    // TODO This verification doesn't work because of SdoConn doesn't support
    // observing of MAG updates, to fix
    CPPUNIT_ASSERT_MESSAGE("Node_1 connected incorrectly", getStateDstr("Launcher.List.Is_conn_ok_Dbg") == "SB true");

    delete mEnv;
}



/** @brief MNode SDC test - create and remove node in cycle
 * */
void Ut_sdc::test_Sdc_3()
{
    printf("\n === Test of SDC: create and remove in cycle\n");

    MNode* root = constructSystem("ut_sdc_3");
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root);

    bool res = mEnv->RunSystem(7, 2);

    // Verify the comp created
    MNode* comp = root->getNode("Launcher.Comp");
    CPPUNIT_ASSERT_MESSAGE("Comp hasn't been created", comp);

    delete mEnv;
}

/** @brief SDC test - connect/disconnect
 * */
void Ut_sdc::test_Sdc_4()
{
    printf("\n === Test of SDC: connect-disconnect, inp notif via socket\n");
    MNode* root = constructSystem("ut_sdc_4");

    bool res = mEnv->RunSystem(12, 2);
    CPPUNIT_ASSERT_MESSAGE("SResult_Dbg-1 failed", getStateDstr("Launcher.SResult_Dbg") == "SI 1");

    res = mEnv->RunSystem(1, 2);
    CPPUNIT_ASSERT_MESSAGE("SResult_Dbg-2 failed", getStateDstr("Launcher.SResult_Dbg") == "SI 2");

    delete mEnv;
}

/** @brief SDC test - connecting after target vertexes get created. Ref ISS_018
 * */
void Ut_sdc::test_Sdc_5()
{
    printf("\n === Test of SDC: connecting after target vertexes get created. Ref ISS_018\n");
    MNode* root = constructSystem("ut_sdc_5");

    bool res = mEnv->RunSystem(12, 2);

    MNode* v1 = root->getNode("Launcher.Syst1.Comp1");
    CPPUNIT_ASSERT_MESSAGE("Comp1 hasn't been created", v1 != nullptr);
    MNode* v2 = root->getNode("Launcher.Syst2.Comp1");
    CPPUNIT_ASSERT_MESSAGE("Comp1 hasn't been created", v2 != nullptr);
    MVert* v1v = v1->lIf(v1v);
    MVert* v2v = v2->lIf(v2v);
    CPPUNIT_ASSERT_MESSAGE("Comp1 and Comp2 aren't connected", v2v->isConnected(v1v));

    delete mEnv;
}


/** @brief MNode SDO test - existence of component
 * */
void Ut_sdc::test_Sdo_1()
{
    printf("\n === Test of SDOs: comps, connections, etc.\n");
    MNode* root = constructSystem("ut_sdo_1");

    bool res = mEnv->RunSystem(5, 2);

    // Verify V1 observing
    MNode* cdbg = root->getNode("Launcher.Dbg_DcoComp");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.Dbg_DcoComp", cdbg);
    bool dres = false;
    GetSData(cdbg, dres);
    CPPUNIT_ASSERT_MESSAGE("Wrong DCO result", dres);
    // Verify V1~V2 observing
    MNode* conndbg = root->getNode("Launcher.Dbg_DcoConn");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.Dbg_DcoConn", conndbg);
    dres = false;
    GetSData(conndbg, dres);
    CPPUNIT_ASSERT_MESSAGE("Wrong DCO V1~V2 result", dres);
    // Verify V1~V3 observing
    conndbg = root->getNode("Launcher.Dbg_DcoConn2");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.Dbg_DcoConn2", conndbg);
    dres = true;
    GetSData(conndbg, dres);
    CPPUNIT_ASSERT_MESSAGE("Wrong DCO V1~V3 result", !dres);
    // Verify V1 pairs count
    conndbg = root->getNode("Launcher.Dbg_DcoPairsCount");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.Dbg_DcoPairsCount", conndbg);
    int dresi = -1;
    GetSData(conndbg, dresi);
    CPPUNIT_ASSERT_MESSAGE("Wrong DCO V1 pairs count result", dresi == 2);
    // Verify V2 pair
    conndbg = root->getNode("Launcher.Dbg_DcoPair");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.Dbg_DcoPair", conndbg);
    DGuri dresu;
    GetGData(conndbg, dresu);
    CPPUNIT_ASSERT_MESSAGE("Wrong Dbg_DcoPair URI result", dresu.mData == ".Root.Launcher.V1");
    // Verify V4_1 owner
    conndbg = root->getNode("Launcher.Dbg_DcoCompOwner");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.Dbg_DcoCompOwner", conndbg);
    dresu;
    GetGData(conndbg, dresu);
    CPPUNIT_ASSERT_MESSAGE("Wrong Dbg_DcoCompOwner URI result", dresu.mData == "V4");
    // Verify V4 comp V4_1
    conndbg = root->getNode("Launcher.Dbg_DcoCompComp");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.Dbg_DcoCompComp", conndbg);
    dresu;
    GetGData(conndbg, dresu);
    CPPUNIT_ASSERT_MESSAGE("Wrong Dbg_DcoCompComp URI result", dresu.mData == ".Root.Launcher.V4.V4_1");

    delete mEnv;
}

void Ut_sdc::test_Sdo_2()
{
    printf("\n === Test of SDO #2: edges, etc.\n");
    MNode* root = constructSystem("ut_sdo_2");

    bool res = mEnv->RunSystem(5, 2);

    // Verify
    CPPUNIT_ASSERT_MESSAGE("Dbg_DcoEdges failed", getStateDstr("Launcher.Dbg_DcoEdges") == "VPDU (PDU (URI .Root.Launcher.V1,URI .Root.Launcher.V2),PDU (URI .Root.Launcher.V1,URI .Root.Launcher.V4),PDU (URI .Root.Launcher.V5,URI .Root.Launcher.V4),PDU (URI .Root.Launcher.V5,URI .Root.Launcher.V3),PDU (URI .Root.Launcher.V4.V4_1,URI .Root.Launcher.V6),PDU (URI .Root.Launcher.Dbg_DcoEdges.Inp,URI .Root.Launcher.DcoComp))");
    bool dres = false;

    delete mEnv;
}
