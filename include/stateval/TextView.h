#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

/* Project */
#include "View.h"

/* STD */
#include <string>

class TextView : public View
{
public:
  TextView (const std::string &fileName);
    
  void realize ();
  void unrealize ();
  
private:
  std::string mFileName;
};

#endif // TEXT_VIEW_H
