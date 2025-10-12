#include <stdlib.h>

#include "des.h"
#include "env.h"
#include "prov.h"
#include "rmutdata.h"
#include "ut_base.h"

#include <cppunit/extensions/HelperMacros.h>


/** @brief Test of DES contexts, ref ds_dctx
*/
class Ut_dctx : public Ut_fixture
{
    CPPUNIT_TEST_SUITE(Ut_dctx);
    //CPPUNIT_TEST(test_des_ctx_1);
    //CPPUNIT_TEST(test_des_ctx_2);
    CPPUNIT_TEST(test_des_ctx_3);
    //CPPUNIT_TEST(test_des_ctx_4);
    //CPPUNIT_TEST(test_des_ctx_5);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    private:
    string getStateDstr(const string& aUri);
    private:
    void test_des_ctx_1();
    void test_des_ctx_2();
    void test_des_ctx_3();
    void test_des_ctx_4();
    void test_des_ctx_5();
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_dctx );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_dctx, "Ut_dctx");

string Ut_dctx::getStateDstr(const string& aUri)
{
    MNode* st = mEnv->Root()->getNode(aUri);
    MDVarGet* stg = st ? st->lIf(stg) : nullptr;
    const DtBase* data = stg ? stg->VDtGet(string()) : nullptr;
    return data ? data->ToString(true) : string();
}

void Ut_dctx::setUp()
{
}

void Ut_dctx::tearDown()
{
    //    delete iEnv;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}


/** @brief Test of DES context #1
 * */
void Ut_dctx::test_des_ctx_1()
{
    cout << endl << "=== Test of DES context 1 ===" << endl;
    MNode* root = constructSystem("ut_des_ctx_1");
    bool res = mEnv->RunSystem(4, 2);
   
    delete mEnv;
}

/** @brief Test of DES context #2
 * */
void Ut_dctx::test_des_ctx_2()
{
    cout << endl << "=== Test of DES context 2 ===" << endl;
    MNode* root = constructSystem("ut_des_ctx_2");
    bool res = mEnv->RunSystem(4, 2);
    CPPUNIT_ASSERT_MESSAGE("CtxC2 failed", getStateDstr("Launcher.System1.System2.System3.CtxC2_Dbg") == "SI 32");
    delete mEnv;
}

/** @brief Test of DES context #3
 * */
void Ut_dctx::test_des_ctx_3()
{
    cout << endl << "=== Test of DES context 3 ===" << endl;
    MNode* root = constructSystem("ut_des_ctx_3");
    bool res = mEnv->RunSystem(4, 2);
   
    delete mEnv;
}

/** @brief Test of DES context #4 - upward data flow
 * */
void Ut_dctx::test_des_ctx_4()
{
    cout << endl << "=== Test of DES context 4 - upward data flow ===" << endl;
    MNode* root = constructSystem("ut_des_ctx_4");
    bool res = mEnv->RunSystem(4, 2);

    // Verify
    MNode* dbg = root->getNode("Launcher.System1.S1Add_Dbg");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.System1.S1Add_Dbg", dbg);
    int dres;
    GetSData(dbg, dres);
    CPPUNIT_ASSERT_MESSAGE("Wrong System1.S1Add result", dres == 9);

    delete mEnv;
}

/** @brief Test of DES context #5 - upward data flow + inputs serialization
 * */
void Ut_dctx::test_des_ctx_5()
{
    cout << endl << "=== Test of DES context #5 - upward data flow + inputs serialization ===" << endl;
    MNode* root = constructSystem("ut_des_ctx_5");
    bool res = mEnv->RunSystem(4, 2);

    // Verify
    MNode* dbg = root->getNode("Launcher.System1.S1Add_Dbg");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Launcher.System1.S1Add_Dbg", dbg);
    int dres;
    GetSData(dbg, dres);
    CPPUNIT_ASSERT_MESSAGE("Wrong System1.S1Add result", dres == 6);

    delete mEnv;
}
