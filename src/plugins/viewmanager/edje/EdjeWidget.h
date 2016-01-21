#ifndef EDJE_WIDGET_H
#define EDJE_WIDGET_H

/* STD */
#include <string>

/* local */
#include "stateval/private/Logger.h"
#include "stateval/private/Widget.h"
#include "stateval/Variable.h"
#include "EdjeView.h"

class EdjeWidget : public Widget
{
public:
  EdjeWidget(View &view, const std::string &name, const Variable *value);

  void updateContent();
  
  //virtual void setValue(const Variable &val);

private:
  EdjeView *mView;
};

#endif // EDJE_WIDGET_H
