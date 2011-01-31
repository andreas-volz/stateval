#ifndef STATE_MACHINE_ACCESSOR_H
#define STATE_MACHINE_ACCESSOR_H

/* STD */
#include <string>

/* local */
#include "Context.h"
#include "Variable.h"

/* SIGC */
// TODO: this needs HAVE_XML defined and so libxml++2-6 installed (and so libglibmm)
// think about not using it or defining it as minimum requirement...
#include <sigc++/sigc++.h>

typedef sigc::signal<void, int> SignalSignal;
typedef sigc::slot1<void, int> SignalSlot;

/* forward declarations */

/* TODO:
 * Think about how to handle multible running statemachines. New design needed!
 */

// Singleton class
class StateMachineAccessor
{
public:
  static StateMachineAccessor& getInstance ();

  static void destroy ();
  
  void load (const std::string &loader, const std::string &file, Context *context);

  void start ();
  
  void pushEvent (int event);
  void pushEvent (const std::string &event);

  void addVariable (const std::string &var, AbstractVariable &av);

  AbstractVariable *getVariable (const std::string &var);
  
  void changeVariable (const std::string &var, AbstractVariable &av);
  
  int findMapingEvent (const std::string &event);
  std::string findMapingEvent (int event);

  void connect (int event, const SignalSlot& slot);
  void connect (const std::string &event, const SignalSlot& slot);
  void connect (const SignalSlot& slot);
  void disconnect (int event);
  void disconnectAll ();

  bool isInitialized ();
  
private:
  StateMachineAccessor ();
  StateMachineAccessor (const StateMachineAccessor&);
  virtual ~StateMachineAccessor ();

  static StateMachineAccessor *mInstance;

  struct StateMachineAccessorPImpl *mPImpl;
};

#endif // STATE_MACHINE_ACCESSOR_H
