#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* STD */
#include <string>
#include <map>

/* local */
#include "stateval/private/Thread.h"

using std::map;
using std::string;

/* forward declarations */
class AbstractVariable;

/*
 * Singleton 
 */
class GlobalVariables
{
public:  
  static GlobalVariables& instance();
  
  void addVariable (const std::string &str, AbstractVariable &av);
  AbstractVariable *getVariable (const std::string &str);
  void changeVariable (const std::string &str, AbstractVariable &av);
    
  void init ();
  
private: // Singleton
  GlobalVariables () {}
  GlobalVariables (const GlobalVariables&);
  //GlobalVariables(GlobalVariables&);
  virtual ~GlobalVariables ();
  //void operator = (GlobalVariables&);

  std::map <std::string, AbstractVariable*> mVariableList; // is cleaned
  Threading::Mutex mutex;
};

#endif // GLOBAL_VARIABLES_H
