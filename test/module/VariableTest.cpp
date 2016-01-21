#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "VariableTest.h"
#include "stateval/Variable.h"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (VariableTest);

void VariableTest::setUp ()
{
  
}

void VariableTest::tearDown ()
{
  
}

void VariableTest::testBool1 ()
{
  Bool b1(true);
  Bool b2(b1);
  Bool b3 = b1;

  CPPUNIT_ASSERT_EQUAL (b1.getData(), b2.getData());
  CPPUNIT_ASSERT_EQUAL (b1.getData(), b3.getData());
}

void VariableTest::testBool2 ()
{
  Bool b1;
  
  CPPUNIT_ASSERT_EQUAL (b1.getData(), false);
}

void VariableTest::testBool3 ()
{
  Bool b1(true);
  Bool b2(true);
  
  CPPUNIT_ASSERT_EQUAL (b1.equals(&b2), true);
  //CPPUNIT_ASSERT_EQUAL (b1 = b2, true);
}

void VariableTest::testBool4 ()
{
}

void VariableTest::testBool5 ()
{
}

void VariableTest::testInteger ()
{

  //CPPUNIT_ASSERT_EQUAL (alarmExpect.getTimestamp (), mdtReference.getTimestamp () + alarmCalc.timer);


  //CPPUNIT_ASSERT_EQUAL (alarmExpect.getTimestamp (), mdtReference.getTimestamp () + alarmCalc.timer);
}
