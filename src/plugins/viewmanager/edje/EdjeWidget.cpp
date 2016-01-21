#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Eflxx */
#include <elementaryxx/Elementaryxx.h>

/* local */
#include "EdjeWidget.h"

using namespace std;

EdjeWidget::EdjeWidget(View &view, const std::string &name, const Variable *value) :
  Widget(name, value),
  mView(dynamic_cast<EdjeView*>(&view))
{
  
}

void EdjeWidget::updateContent()
{
  LOG4CXX_TRACE(mLogger, "updateContent: " << mName);

  Eflxx::CountedPtr <Edjexx::Object> edjeObj(mView->mLayout->getEdje());

  Edjexx::Part &part = edjeObj->getPart(mName);

  if(mValue)
  {
    if(mValue->getType() == Variable::TYPE_STRING)
    {
      String *str = static_cast <String *>(mValue);

      part.setText(str->getData());
    }
    else
    {
      LOG4CXX_WARN(mLogger, "Currently not supported Variable Type!");
    }
  }
}

/*void EdjeWidget::setValue(const Variable &val)
{
  // TODO
  
       - cache variable in widget until next time on screen if not on screen
  //   - if on screen do next: 
  // - get Edje object by assigned string
  // - switch Variable type
  // - depend on variable type assign values to native Edje widget
  // - think about to use several EdjeWidget types instead of switch
    
  // - get fetches always from assigned low level widget if available

  // need to model a copy action on entry transition to get a value updated?
}*/