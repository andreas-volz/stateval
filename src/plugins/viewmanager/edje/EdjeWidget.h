#ifndef EDJE_WIDGET_H
#define EDJE_WIDGET_H

/* STD */
#include <string>

/* local */
#include "stateval/private/Logger.h"
#include "stateval/Widget.h"
#include "stateval/Variable.h"
#include "EdjeView.h"

class EdjeWidget : public Widget
{
public:
  EdjeWidget(View &view, const std::string &name);

  void updateContent(); // TODO: protected and friend?
  void updateData(); // TODO: protected and friend?
  
  Variable *getProperty(const std::string &name);

  void setProperty(const std::string &name, const Variable &property);

private:
  void updateDataDispatched(int missedEvents);

  void setPropertyDispatched(int missedEvents);

  void getPropertyDispatched(int missedEvents);
  
  Logger mLogger;
  EdjeView *mView;

  EcoreDispatcher mUpdateDataDispatcher;
  EcoreDispatcher mSetPropertyDispatcher;
  EcoreDispatcher mGetPropertyDispatcher;
  
  Threading::Condition mCondUpdateData;
  Threading::Mutex mMutexUpdateData;

  std::string mActiveGetPropertyName;
  std::string mActiveSetPropertyName;
};

#endif // EDJE_WIDGET_H
