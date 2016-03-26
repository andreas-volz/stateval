#ifndef WIDGET_H
#define WIDGET_H

/* STD */
#include <string>

/* local */
#include "stateval/Variable.h"
#include "stateval/WidgetRenderer.h"

class Widget
{
public:
  Widget(WidgetRenderer &widgetRenderer, const std::string &name);
  virtual ~Widget();

  const std::string getName() const;

  /* returns a copy of the widget variable => needs to be deleted afterwards */  
  virtual Variable *getProperty(const std::string &name) = 0;

  virtual void setProperty(const std::string &name, const Variable &property);

  // only allowed to call from a GUI dispatched client!! 
  virtual void updateContent();

  // only allowed to call from a GUI dispatched client!! 
  virtual void freeContent();

  void setWidgetRender(WidgetRenderer &widgetRenderer);

protected:
  std::string mName;
  std::map<std::string, Variable*> mProperties;
  WidgetRenderer *mWidgetRenderer;
};

#endif // WIDGET_H
