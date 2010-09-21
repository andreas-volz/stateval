#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string>
#include <map>


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
    
  void init ();
  
private: // Singleton
  GlobalVariables () {}
  GlobalVariables (const GlobalVariables&);
  //GlobalVariables(GlobalVariables&);
  virtual ~GlobalVariables ();
  //void operator = (GlobalVariables&);

  std::map <std::string, AbstractVariable*> mVariableList; // is cleaned
  
};

#endif // GLOBAL_VARIABLES_H
