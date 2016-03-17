#ifndef EDJE_DEFAULT_WIDGET_RENDERER_H
#define EDJE_DEFAULT_WIDGET_RENDERER_H

/* stateval */
#include "stateval/private/Logger.h"
#include "stateval/Variable.h"
#include "stateval/Widget.h"
#include "stateval/WidgetRenderer.h"

/* STD */
#include <string>

class EdjeDefaultWidgetRenderer : public WidgetRenderer
{
public:
  EdjeDefaultWidgetRenderer();
  virtual ~EdjeDefaultWidgetRenderer();

  void render(void *renderObject, const std::string &varName, Variable *var);

  Variable *getRenderedValue(void *renderObject, const std::string &varName);
  
protected:

private:
  Logger mLogger;
};

#endif // EDJE_DEFAULT_WIDGET_RENDERER_H
