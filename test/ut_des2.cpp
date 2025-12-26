#include <stdlib.h>
#include <filesystem>

#include "ut_base.h"
#include "des.h"
#include "env.h"
#include "prov.h"
//#include "rmutdata.h"
#include "rdatauri.h"

#include <cppunit/extensions/HelperMacros.h>


/** @brief UT of multithreading DES (Des2)
*/
class Ut_des2 : public Ut_fixture
{
    CPPUNIT_TEST_SUITE(Ut_des2);
    CPPUNIT_TEST(test_des2_2);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    private:
    /** @brief Helper. Get state data string 
     * */
    string getStateDstr(const string& aUri);
    static string getStateDstr(MEnv* aEnv, const string& aUri);
    private:
    void test_des2_2();
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_des2 );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_des2, "Ut_des2");

string Ut_des2::getStateDstr(const string& aUri)
{
    MNode* st = mEnv->Root()->getNode(aUri);
    MDVarGet* stg = st ? st->lIf(stg) : nullptr;
    const DtBase* data = stg ? stg->VDtGet(string()) : nullptr;
    return data ? data->ToString(true) : string();
}

string Ut_des2::getStateDstr(MEnv* aEnv, const string& aUri)
{
    MNode* st = aEnv->Root()->getNode(aUri);
    MDVarGet* stg = st ? st->lIf(stg) : nullptr;
    const DtBase* data = stg ? stg->VDtGet(string()) : nullptr;
    return data ? data->ToString(true) : string();
}




void Ut_des2::setUp()
{
}

void Ut_des2::tearDown()
{
    //    delete iEnv;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}

void Ut_des2::test_des2_2()
{
    cout << endl << "=== Test of simple DES ===" << endl;

    string ssname = "ut_des2_2";
    MNode* root = constructSystem(ssname);
    bool res = mEnv->RunSystem(100, 2);
    if (mEnv->profiler()) mEnv->profiler()->saveMetrics();

    delete mEnv;
}

