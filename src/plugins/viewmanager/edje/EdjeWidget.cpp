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

  try
  {
    Edjexx::Part &part = edjeObj->getPart(mName);
  
    if(mValue)
    {
      if(mValue->getType() == Variable::TYPE_STRING)
      {
        String *str = static_cast <String *>(mValue);

        part.setText(str->getData());
      }
      else if (mValue->getType() == Variable::TYPE_LIST)
      {
        List *ls = static_cast <List *>(mValue);

        Evasxx::Object &ext_eo3 = part.getExternalObject();
        Evasxx::Object &eo3 = part.getSwallow();
        LOG4CXX_DEBUG(mLogger, "Edje External Widget type: " << ext_eo3.getType());
        LOG4CXX_DEBUG(mLogger, "Edje Part Widget type: " << eo3.getType());

        if (ext_eo3.getType() == "elm_list")
        {
          Elmxx::Object &elm_object = *(static_cast <Elmxx::Object *>(&ext_eo3));

          LOG4CXX_DEBUG(mLogger, "Elm Widget type: " << elm_object.getWidgetType());

          if (elm_object.getWidgetType() == "Elm_List")
          {
            Elmxx::List &list = *(static_cast <Elmxx::List *>(&elm_object));

            // TODO: I think until the edited/merge feature is implemented it's the
            // best to clear the list before adding new elements...
            list.clear();
            for (List::Iterator ls_it = ls->begin();
                 ls_it != ls->end();
                 ++ls_it)
            {
              Variable *av = *ls_it;

              if (av->getType() == Variable::TYPE_STRING)
              {
                String *str = static_cast <String *>(av);
                list.append(str->getData(), NULL, NULL);
              }
              list.go();
            }
          }
        }
      }
      else
      {
        LOG4CXX_WARN(mLogger, "Currently not supported Variable Type!");
      }
    }
  }
  catch (Edjexx::ExternalNotExistingException ene)
  {
    LOG4CXX_ERROR(mLogger, ene.what());
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