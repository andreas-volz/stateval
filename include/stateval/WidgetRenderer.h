#ifndef WIDGET_RENDERER_H
#define WIDGET_RENDERER_H

/* stateval */
#include "stateval/Widget.h"

/* STD */
#include <string>

class WidgetRenderer
{
public:
  //WidgetRenderer(const std::string &name);
  //virtual ~WidgetRenderer();

  virtual void render(void *renderObject, const std::string &varName, Variable *var) = 0;

  virtual Variable *getRenderedValue(void *renderObject, const std::string &varName) = 0;
  
protected:
  
};

#endif // WIDGET_RENDERER_H
