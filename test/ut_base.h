
#ifndef __FAP5_UT_BASE_H
#define __FAP5_UT_BASE_H

#include <cppunit/extensions/HelperMacros.h>

#include "env.h"

/** @brief UT base fixture
*/
class Ut_fixture : public CPPUNIT_NS::TestFixture
{
    protected:
    MNode* constructSystem(const string& aFname);
    protected:
    Env* mEnv;
};


#endif


