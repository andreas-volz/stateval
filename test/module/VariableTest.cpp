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
}

void VariableTest::testBool4 ()
{
  Bool b1(true);
  Variable *b2 = b1.copy();

  CPPUNIT_ASSERT_EQUAL (b1.equals(b2), true);
  delete b2;
}

void VariableTest::testBool5 ()
{
  Bool b1(true);
  Bool b2(false);

  b2.copy(&b1);

  CPPUNIT_ASSERT_EQUAL (b1.equals(&b2), true);
}

void VariableTest::testBool6 ()
{
  Bool b1(true);
  Bool b2(false);

  b2 = b1;

  CPPUNIT_ASSERT_EQUAL (b1.equals(&b2), true);
}


void VariableTest::testList1 ()
{
  List l1;
  String s1("str1");
  String s2("str2");
  String s3("str3");
  l1.pushBack (&s1);
  l1.pushBack (&s2);
  l1.pushBack (&s3);

  List l2(l1);

  /*for(List::Iterator li_it = l2.begin();
       li_it != l2.end();
       ++li_it)
  {
    Variable *var = *li_it;

    String *str = dynamic_cast<String*>(var);

    cout << "str: " << str->getData () << endl; 
  }*/

  CPPUNIT_ASSERT_EQUAL (l1.equals(&l2), true);  
}

void VariableTest::testList2 ()
{

}

void VariableTest::testList3 ()
{

}
