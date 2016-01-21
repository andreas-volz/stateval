#ifndef VARIABLE_TEST_H
#define VARIABLE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

// STD
#include <string.h>

class VariableTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE (VariableTest);

  CPPUNIT_TEST (testBool1);
  CPPUNIT_TEST (testBool2);
  CPPUNIT_TEST (testBool3);
  CPPUNIT_TEST (testBool4);
  CPPUNIT_TEST (testBool5);

  CPPUNIT_TEST (testInteger);

  CPPUNIT_TEST_SUITE_END ();

public:
  void setUp ();
  void tearDown ();

protected:
  /**
   * Constructors
   */
  void testBool1 ();

  /**
   * Check false as default
   */
  void testBool2 ();
  
  void testBool3 ();
  
  void testBool4 ();
  
  void testBool5 ();
  
  void testInteger ();

private:

};

#endif // VARIABLE_TEST_H
