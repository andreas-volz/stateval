#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "testCommon.h"
#include "stateval/Thread.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

/* ========================================================================== *
 *
 * ========================================================================== */
class TestCase : public CppUnit::TestCase
{
public:
    TestCase() : CppUnit::TestCase(){}
    
    virtual ~TestCase(){}
    
    class TestThread : public Threading::Thread
    {
    public:
        bool mThreadRun;
        bool mKeepRunning;
        bool mSignalCancelCalled;
        
        /** Create this test thread.
          *
          * \param inKeepRunning false if you just want the thread enter run() and leave immediately.
          *        If this param is true then the test thread remains in run until cancel will be called.
          */
        TestThread(bool inKeepRunning) 
        : mThreadRun(false)
        , mKeepRunning(inKeepRunning) 
        , mSignalCancelCalled(false)
        {}

        virtual ~TestThread(){}
        
    protected:
        virtual void run()
        {
            mThreadRun = true;
            if (mKeepRunning)
            {
                for(;isRunning();)
                {
                    sleepMS(1);
                }
            }
        }
        
        virtual void signal_cancel() 
        {
            mSignalCancelCalled = true;
        }
    }; // class TestThread : Threading::Thread


    void testThreadRun()
    {
        TestThread thread(false);
        CPPUNIT_ASSERT_EQUAL(false, thread.isRunning());
        thread.start();
        Threading::Thread::sleepMS(10);
        
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("The thread did not run!", true, thread.mThreadRun);
        CPPUNIT_ASSERT_EQUAL(false, thread.isRunning());
    }

    void testThreadCancel()
    {
        TestThread thread(true);
        CPPUNIT_ASSERT_EQUAL(false, thread.isRunning());
        thread.start();
        
        // Give the thread some time to run...
        for (int i=0; i<10; ++i)
        {
            if (thread.isRunning())
            {
                break;
            }
            Threading::Thread::sleepMS(1);
        }
        CPPUNIT_ASSERT_EQUAL(true, thread.isRunning());
        thread.cancel();
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("The thread did not run!", thread.mThreadRun, true);
        CPPUNIT_ASSERT_EQUAL(false, thread.isRunning());
        CPPUNIT_ASSERT_EQUAL(true, thread.mSignalCancelCalled);
    }

    virtual void setUp() 
    {
    }
    
    //! Clean up after the test run.
    virtual void tearDown() 
    {
    }
    
    static Test *suite()
    {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestCase");
        
        suiteOfTests->addTest(new CppUnit::TestCaller<TestCase>( 
                                "testThreadRun", 
                                &TestCase::testThreadRun));

        suiteOfTests->addTest(new CppUnit::TestCaller<TestCase>( 
                                "testThreadCancel", 
                                &TestCase::testThreadCancel));

        return suiteOfTests;
    }
}; // class MyTestCase : public CppUnit::TestCase


int main(int argc, char *argv[])
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(TestCase::suite());
    runner.run();
    return EXIT_SUCCESS;
}

