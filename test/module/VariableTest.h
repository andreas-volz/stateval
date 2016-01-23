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
  CPPUNIT_TEST (testBool6);

  CPPUNIT_TEST (testList1);
  CPPUNIT_TEST (testList2);
  CPPUNIT_TEST (testList3);

  CPPUNIT_TEST_SUITE_END ();

public:
  void setUp ();
  void tearDown ();

protected:
  /**
   * bool constructors and getData() function
   */
  void testBool1 ();

  /**
   * Check Bool false as default
   */
  void testBool2 ();

  /**
   * test equal()
   */
  void testBool3 ();

  /**
   * test copy() Variable{Bool} factory
   */
  void testBool4 ();

  /**
   * test copy(Variable{Bool})
   */
  void testBool5 ();

  /**
   * test = operator
   */
  void testBool6 ();

  /**
   * List constructor
   */
  void testList1 ();

  /**
   * test copy() Variable{List} factory
   */
  void testList2 ();

  /**
   * test copy(Variable{List})
   */
  void testList3 ();

private:

};

#endif // VARIABLE_TEST_H
