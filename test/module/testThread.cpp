#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <zlib.h>

#include "testCommon.h"
#include "stateval/Thread.h"
#include "stateval/StateMachine.h"
#include "stateval/StateMachineThread.h"
#include "stateval/SimpleState.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

class StringCollector
{
    std::string mString;
    
public:
    StringCollector() : mString(){}
    void append(const std::string& inString)
    {
        mString.append(inString);
    }
    
    uint32_t getHash() const
    {
        uLong initCRC = ::crc32(0, NULL, 0);
        return ::crc32(initCRC, (const Bytef*)mString.c_str(), mString.length());
    }
};

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
        
        // Try to start the thread.
        const Threading::Thread::EError err(thread.start());
        CPPUNIT_ASSERT_EQUAL(Threading::Thread::eErrorOK, err);

        Threading::Thread::sleepMS(10);
        
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("The thread did not run!", true, thread.mThreadRun);
        CPPUNIT_ASSERT_EQUAL(false, thread.isRunning());
    }

    void testThreadCancel()
    {
        TestThread thread(true);
        CPPUNIT_ASSERT_EQUAL(false, thread.isRunning());

        // Try to start the thread.
        Threading::Thread::EError err(thread.start());
        CPPUNIT_ASSERT_EQUAL(Threading::Thread::eErrorOK, err);
        
        // Give the thread some time to run...
        for (int i=0; i<10; ++i)
        {
            if (thread.isRunning())
            {
                break;
            }
            Threading::Thread::sleepMS(1); // sleep a while if not running...
        }
        
        // The thread shoul be in running state.
        CPPUNIT_ASSERT_EQUAL(true, thread.isRunning());
        
        // Cancel the thread!
        err = thread.cancel();

        // Check if cancel returned successfully.
        CPPUNIT_ASSERT_EQUAL(Threading::Thread::eErrorOK, err);
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("The thread did not run!", thread.mThreadRun, true);
        
        // The thread should state to run any longer.
        CPPUNIT_ASSERT_EQUAL(false, thread.isRunning());
        
        // Check if the method signal_cancel() has really called!
        CPPUNIT_ASSERT_EQUAL(true, thread.mSignalCancelCalled);
    }

    void testSimpleSM()
    {
        StringCollector sc;
        
        class MyAction : public Action
        {
            StringCollector& mCollector;
            const char *mName;
            
        public:
            MyAction(const char *inName, StringCollector& inSC)
            : Action(), mCollector(inSC), mName(inName){}
            
            virtual void run () const
            {
//                printf("**** run() of action '%s' called!\n", mName);
                mCollector.append("->Action: ");
                mCollector.append(mName);
            }
        };
        
        enum EEvents {eEventA, eEventB, eEventC};
        
        StateMachine sm ("simple");
        //Loader &loader(sm.getLoader());
        SimpleState *initState = new SimpleState(NULL);

        SimpleState *stateA = new SimpleState(NULL);
        MyAction *entryActionStateA = new MyAction("entryActionStateA", sc);
        MyAction *exitActionStateA = new MyAction("exitActionStateA", sc);
        stateA->addEntryAction(entryActionStateA);
        stateA->addExitAction(exitActionStateA);
        
        SimpleState *stateB = new SimpleState(NULL);
        MyAction *entryActionStateB = new MyAction("entryActionStateB", sc);
        MyAction *exitActionStateB = new MyAction("exitActionStateB", sc);
        stateB->addEntryAction(entryActionStateB);
        stateB->addExitAction(exitActionStateB);

        SimpleState *stateC = new SimpleState(NULL);
        MyAction *entryActionStateC = new MyAction("entryActionStateC", sc);
        MyAction *exitActionStateC = new MyAction("exitActionStateC", sc);
        stateC->addEntryAction(entryActionStateC);
        stateC->addExitAction(exitActionStateC);
        
        Transition *tr1 = new Transition(stateA, eEventA);
        initState->addLeaveTransition(*tr1);

        Transition *tr2 = new Transition(stateB, eEventB);
        stateA->addLeaveTransition(*tr2);

        Transition *tr3 = new Transition(stateC, eEventC);
        stateB->addLeaveTransition(*tr3);

        Transition *tr4 = new Transition(stateA, eEventA);
        stateC->addLeaveTransition(*tr4);

        // Hint: new interface doesn't longer allow this!
        // write a test loader plugin to directly add states!!
        /*loader.addState(initState);
        loader.addState(stateA);
        loader.addState(stateB);
        loader.addState(stateC);*/

      
        sm.start();       
        StateMachineThread smt(sm);
        smt.start();
        
        // hdusel: This is weird - when you increas the loop count then the results 
        // differ from run to run...
        //
        // TODO Check this out!
        for (int i=0; i< 1; ++i)
        {
            smt.pushEvent(eEventA);
            sc.append("EventA");

            smt.pushEvent(eEventA);
            sc.append("EventA");

            smt.pushEvent(eEventC);
            sc.append("EventC");

            smt.pushEvent(eEventB);
            sc.append("EventB");

            smt.pushEvent(eEventB);
            sc.append("EventB");
            
            smt.pushEvent(eEventA);
            sc.append("EventA");

            smt.pushEvent(eEventB);
            sc.append("EventB");

            smt.pushEvent(eEventC);
            sc.append("EventC");

            smt.pushEvent(eEventC);
            sc.append("EventC");

            smt.pushEvent(eEventA);
            sc.append("EventA");
            
            smt.pushEvent(eEventC);
            sc.append("EventC");

            smt.pushEvent(eEventA);
            sc.append("EventA");

            smt.pushEvent(eEventB);
            sc.append("EventB");

            smt.pushEvent(eEventC);
            sc.append("EventC");

            smt.pushEvent(eEventB);
            sc.append("EventB");
        }
        
        Threading::Thread::sleepMS(10);

        smt.cancel();
        
//        printf("The Checksum is 0x%08x\n", sc.getHash());
        CPPUNIT_ASSERT_EQUAL((uint32_t)2558933020UL,  sc.getHash());
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

        suiteOfTests->addTest(new CppUnit::TestCaller<TestCase>( 
                                "testSimpleSM", 
                                &TestCase::testSimpleSM));
        
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

