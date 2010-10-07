#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

/* stateval */
#include "stateval/View.h"

/* STD */
#include <string>
#include <list>

class TextView : public View
{
public:
  virtual ~TextView () {};
  TextView (const std::list <std::string> &params);

  const std::string getType ();
  
  const unsigned int getMajorVersion ();

  const unsigned int getMinorVersion ();
  
  void realize ();
  void unrealize ();
  
private:
  std::string mFileName;
};

#endif // TEXT_VIEW_H
