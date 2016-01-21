#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

/* stateval */
#include "stateval/private/View.h"

/* STD */
#include <string>
#include <list>

/* forward declarations */
class Context;

class TextView : public View
{
public:
  TextView(const std::map <std::string, std::string> &params);
  virtual ~TextView() {};

  void createWidget(const std::string &name, const Variable *value);

  void realize();
  void unrealize();

  void updateContent();

private:
  std::string mFileName;
};

#endif // TEXT_VIEW_H
