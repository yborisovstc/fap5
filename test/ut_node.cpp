#include <stdlib.h>


#include "ut_base.h"
#include "mchromo.h"
#include "melem.h"
#include "ifu.h"
#include "mecont.h"

/*
 * This test of MNode interface main operations
 */


class Ut_node : public Ut_fixture
{
    CPPUNIT_TEST_SUITE(Ut_node);
    CPPUNIT_TEST(test_cre_1);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();
private:
    void test_cre_1();
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_node );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_node, "Ut_node");


void Ut_node::setUp()
{
}

void Ut_node::tearDown()
{
//    delete iEnv;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}

/** @brief Test of creating node based model
 * */
void Ut_node::test_cre_1()
{
    cout << endl << "=== Test of creating node based model ===" << endl;

    string specn = "ut_node_cre_1";
    string ext = "chs";
    constructSystem(specn);
    MNode* root = mEnv->Root();
    MElem* eroot = root ? root->lIf(eroot) : nullptr;
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", eroot);
    GUri ruri;
    root->getUri(ruri);
    string ruris = ruri.toString();
    root->dump(Ifu::EDM_Base | Ifu::EDM_Comps | Ifu::EDM_Recursive,0);
    // Save root chromo
    eroot->Chromos().Save(specn + "_saved." + ext);
    // Verify c12 content
    MNode* n1 = root->getNode("n1");
    CPPUNIT_ASSERT_MESSAGE("Fail to get n1", n1);
    MContentOwner* n1co = n1->lIf(n1co);
    CPPUNIT_ASSERT_MESSAGE("Fail to get n1co", n1co);
    // Verify Debug.LogLevel content
    string dl;
    bool res = n1co->getContent("LogLevel", dl);
    CPPUNIT_ASSERT_MESSAGE("Wrong LogLevel content", dl == "Dbg");
    // Verify getting node by absolute URI
    MNode* nn = n1->getNode(".MyRoot.n1.n1_1.n1_1_2.n1_1_2_1");
    CPPUNIT_ASSERT_MESSAGE("Fail to get node by absolute URI", nn);
    // Verify not getting not owned node
    nn = n1->getNode(".MyRoot.n2.n2_1");
    CPPUNIT_ASSERT_MESSAGE("Fail to deny getting not owned node", !nn);
    string n1About;
    bool cres = n1co->getContent("About", n1About);
    CPPUNIT_ASSERT_MESSAGE("Wrong n1 About content", n1About == "Test node 1");
    MNode* n1_1_2_1 = root->getNode("n1.n1_1.n1_1_2.n1_1_2_1");
    CPPUNIT_ASSERT_MESSAGE("Missing n1_1_2_1", n1_1_2_1);

    // Test Uri
    GUri uri1 = GUri("");
    MNode* nt = n1->getNode(uri1);
    CPPUNIT_ASSERT_MESSAGE("getNode fail with empty URI", nt);
    // Test Uri
    GUri uri2 = GUri("_$");
    MNode* nt2 = n1->getNode(uri2);
    CPPUNIT_ASSERT_MESSAGE("getNode fail with _$ URI", nt2);

    delete mEnv;
}
