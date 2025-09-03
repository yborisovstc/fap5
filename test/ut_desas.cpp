#include <stdlib.h>

#include "ut_base.h"

#include "des.h"
#include "env.h"
#include "prov.h"
#include "rdatauri.h"

#include <cppunit/extensions/HelperMacros.h>


/** @brief This test of DES active subsystem
*/
class Ut_desas : public Ut_fixture
{
    CPPUNIT_TEST_SUITE(Ut_desas);
    //CPPUNIT_TEST(test_Desas_1);
    //CPPUNIT_TEST(test_Desas_2);
    CPPUNIT_TEST(test_Desas_3);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    private:
    void test_Desas_1();
    void test_Desas_2();
    void test_Desas_3();
    string getStateDstr(const string& aUri);
};


string Ut_desas::getStateDstr(const string& aUri)
{
    MNode* st = mEnv->Root()->getNode(aUri);
    MDVarGet* stg = st ? st->lIf(stg) : nullptr;
    const DtBase* data = stg ? stg->VDtGet(string()) : nullptr;
    return data ? data->ToString(true) : string();
}

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_desas );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_desas, "Ut_desas");

void Ut_desas::setUp()
{
}

void Ut_desas::tearDown()
{
    //    delete mEnv;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}


/** @brief DESAS test - counter
 * */
void Ut_desas::test_Desas_1()
{
    cout << endl << "=== Test of DES active subs: simple subs" << endl;
    MNode* root = constructSystem("ut_desas_1");

    bool res = mEnv->RunSystem(1, 0);
    CPPUNIT_ASSERT_MESSAGE("St1 failed", getStateDstr("Launcher.Ds1.As1.Res") == "SI 5");
    res = mEnv->RunSystem(1, 0);
    CPPUNIT_ASSERT_MESSAGE("St1 failed", getStateDstr("Launcher.Ds1.As1.Res") == "SI 9");
    res = mEnv->RunSystem(1, 0);
    CPPUNIT_ASSERT_MESSAGE("St1 failed", getStateDstr("Launcher.Ds1.As1.Res") == "SI 13");

    delete mEnv;
}

/** @brief DESAS test - list
 * */
void Ut_desas::test_Desas_2()
{
    printf("\n === Test of DES active subs: list\n");
    MNode* root = constructSystem("ut_desas_2");

    bool res = mEnv->RunSystem(12, 2);
    CPPUNIT_ASSERT_MESSAGE("AsRes failed", getStateDstr("Launcher.ToListStart.AsRes") == "URI .MyRoot.Launcher.List.Start");

    delete mEnv;
}

/** @brief DESAS test - getting list position
 * */
void Ut_desas::test_Desas_3()
{
    printf("\n === Test of DES active subs: getting list node\n");
    MNode* root = constructSystem("ut_desas_3");

    bool res = mEnv->RunSystem(2, 0);
    CPPUNIT_ASSERT_MESSAGE("AsRes failed", getStateDstr("Launcher.ListPos.OutpNode") == "URI Start");
    res = mEnv->RunSystem(1, 0);
    CPPUNIT_ASSERT_MESSAGE("AsRes failed", getStateDstr("Launcher.ListPos.OutpNode") == "URI .MyRoot.Launcher.List.E1");
    res = mEnv->RunSystem(1, 0);
    CPPUNIT_ASSERT_MESSAGE("AsRes failed", getStateDstr("Launcher.ListPos.OutpNode") == "URI .MyRoot.Launcher.List.E2");
    res = mEnv->RunSystem(1, 0);
    CPPUNIT_ASSERT_MESSAGE("AsRes failed", getStateDstr("Launcher.ListPos.OutpNode") == "URI .MyRoot.Launcher.List.E3");

    delete mEnv;
}




