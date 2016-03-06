#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Eflxx */
#include <elementaryxx/Elementaryxx.h>

/* local */
#include "EdjeWidget.h"
#include "stringUtil.h"

using namespace std;

EdjeWidget::EdjeWidget(View &view, const std::string &name) :
  mLogger("stateval.plugins.viewmanager.edje.EdjeWidget"),
  Widget(name),
  mView(dynamic_cast<EdjeView*>(&view))
{
  //mUpdateDataDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeWidget::updateDataDispatched));
  mSetPropertyDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeWidget::setPropertyDispatched));
  mGetPropertyDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeWidget::getPropertyDispatched));
}

void EdjeWidget::setProperty(const std::string &name, const Variable &property)
{
  LOG4CXX_TRACE(mLogger, "+setProperty()");

  mActiveSetPropertyName = name;
  // FIXME: borken memory handling
  //delete mProperties[mActiveSetPropertyName];
  mProperties[mActiveSetPropertyName] = property.copy();
  
  mSetPropertyDispatcher.emit();

  mMutexUpdateData.lock();
  mCondUpdateData.wait(mMutexUpdateData);
  mMutexUpdateData.unlock();
  
  LOG4CXX_TRACE(mLogger, "-setProperty()");
}

void EdjeWidget::setPropertyDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "setPropertyDispatched: " << getName() << " / " << mActiveSetPropertyName);

  Elmxx::Layout *layout = mView->getLayout();

  if(layout)
  {
    Eflxx::CountedPtr <Edjexx::Object> edjeObj(layout->getEdje());

    try
    {
      Edjexx::Part &part = edjeObj->getPart(getName());

      Variable *var = mProperties[mActiveSetPropertyName];

      switch(var->getType ())
      {
        case Variable::TYPE_INTEGER:
        {
          Integer *intObj = dynamic_cast<Integer*>(var);
          LOG4CXX_TRACE(mLogger, "TYPE_INTEGER: " << intObj->getData());
          Edjexx::ExternalParam param (mActiveSetPropertyName, intObj->getData());
          bool validParam = part.setParam (&param);        
          break;
        }
        case Variable::TYPE_STRING:
        {
          String *strObj = dynamic_cast<String*>(var);
          LOG4CXX_TRACE(mLogger, "TYPE_STRING: " << strObj->getData());
          Edjexx::ExternalParam param (mActiveSetPropertyName, strObj->getData());
          bool validParam = part.setParam (&param);
          LOG4CXX_TRACE(mLogger, "validParam: " << validParam);
          //part.setText(strObj->getData());
          break;
        }
        default:
          LOG4CXX_DEBUG(mLogger, "unknown type");
      }
    }
    catch (Edjexx::ExternalNotExistingException ene)
    {
      LOG4CXX_ERROR(mLogger, ene.what());
    }
  }
  
  mCondUpdateData.signal();
}


Variable *EdjeWidget::getProperty(const std::string &name)
{
  LOG4CXX_TRACE(mLogger, "+getProperty()");

  mActiveGetPropertyName = name;
  
  mGetPropertyDispatcher.emit();

  mMutexUpdateData.lock();
  mCondUpdateData.wait(mMutexUpdateData);
  mMutexUpdateData.unlock();
  
  LOG4CXX_TRACE(mLogger, "-getProperty()");

  Variable *var = mProperties[name];
  
  return var;
}

void EdjeWidget::getPropertyDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "getPropertyDispatched: " << getName() << " / " << mActiveGetPropertyName);

  Elmxx::Layout *layout = mView->getLayout();

  if(layout)
  {
    Eflxx::CountedPtr <Edjexx::Object> edjeObj(layout->getEdje());

    try
    {
      Edjexx::Part &part = edjeObj->getPart(getName());

      Edje_External_Param_Type paramType = part.getParamType(mActiveGetPropertyName);
      switch(paramType)
      {
        case EDJE_EXTERNAL_PARAM_TYPE_INT:
        {
          Edjexx::ExternalParam param (mActiveGetPropertyName, (int) 0);    
          bool validParam = part.getParam (param);
          LOG4CXX_DEBUG(mLogger, "INT: " << param.mParam.i);
          // FIXME: borken memory handling
          //delete mProperties[mActiveGetPropertyName];
          Integer *intObj = new Integer(param.mParam.i);
          mProperties[mActiveGetPropertyName] = intObj;
          break;
        }
        case EDJE_EXTERNAL_PARAM_TYPE_DOUBLE:
          LOG4CXX_DEBUG(mLogger, "DOUBLE");
          break;
        case EDJE_EXTERNAL_PARAM_TYPE_STRING:
        {
          Edjexx::ExternalParam param (mActiveGetPropertyName, "");    
          bool validParam = part.getParam (param);
          LOG4CXX_DEBUG(mLogger, "STRING" << param.mParam.s);
          //delete mProperties[mActiveGetPropertyName];
          String *strObj = new String(param.mParam.s);
          mProperties[mActiveGetPropertyName] = strObj;
          break;
        }
        case EDJE_EXTERNAL_PARAM_TYPE_BOOL:
          LOG4CXX_DEBUG(mLogger, "BOOL");
          break;
        case EDJE_EXTERNAL_PARAM_TYPE_CHOICE:
          LOG4CXX_DEBUG(mLogger, "CHOICE");
          break;
        case EDJE_EXTERNAL_PARAM_TYPE_MAX:
          LOG4CXX_DEBUG(mLogger, "MAX");
          break;  
        default:
          LOG4CXX_DEBUG(mLogger, "unknown type");
      }

    }
    catch (Edjexx::ExternalNotExistingException ene)
    {
      LOG4CXX_ERROR(mLogger, ene.what());
    }
  }
  
  mCondUpdateData.signal();
}

void EdjeWidget::updateData()
{

}

void EdjeWidget::updateDataDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "updateData: " << mName);

  // TODO: this will crash if calling a not visible view => fix later 
  Eflxx::CountedPtr <Edjexx::Object> edjeObj(mView->getLayout()->getEdje());

  try
  {
    Edjexx::Part &part = edjeObj->getPart(getName());
    
    // TODO: data type introspection by definion in XML?
    Edjexx::ExternalParam paramHours ("hours", (int) 0); 
    Edjexx::ExternalParam paramMinues ("minutes", (int) 0);
    Edjexx::ExternalParam paramSeconds ("seconds", (int) 0);
    bool validParamHours = part.getParam (paramHours);
    bool validParamMinutes = part.getParam (paramMinues);
    bool validParamSeconds = part.getParam (paramSeconds);


    Edje_External_Param_Type paramType = part.getParamType("hours");
    switch(paramType)
    {
      case EDJE_EXTERNAL_PARAM_TYPE_INT:
        LOG4CXX_DEBUG(mLogger, "INT");
        break;
      case EDJE_EXTERNAL_PARAM_TYPE_DOUBLE:
        LOG4CXX_DEBUG(mLogger, "DOUBLE");
        break;
      case EDJE_EXTERNAL_PARAM_TYPE_STRING:
        LOG4CXX_DEBUG(mLogger, "STRING");
        break;
      case EDJE_EXTERNAL_PARAM_TYPE_BOOL:
        LOG4CXX_DEBUG(mLogger, "BOOL");
        break;
      case EDJE_EXTERNAL_PARAM_TYPE_CHOICE:
        LOG4CXX_DEBUG(mLogger, "CHOICE");
        break;
      case EDJE_EXTERNAL_PARAM_TYPE_MAX:
        LOG4CXX_DEBUG(mLogger, "MAX");
        break;  
      default:
        LOG4CXX_DEBUG(mLogger, "unknown type");
    }

    
    
    string str = toString(paramHours.mParam.i) + string(":") + toString(paramMinues.mParam.i) + string("h");
    LOG4CXX_TRACE(mLogger, "Clock: " << str);
#if 0
    if(mValue)
    {
      delete mValue; 
    }

    mValue = new String(str);
#endif
  }
  catch (Edjexx::ExternalNotExistingException ene)
  {
    LOG4CXX_ERROR(mLogger, ene.what());
  }

  mCondUpdateData.signal();
}


void EdjeWidget::updateContent()
{
#if 0
  LOG4CXX_TRACE(mLogger, "updateContent: " << mName);

  Eflxx::CountedPtr <Edjexx::Object> edjeObj(mView->getLayout()->getEdje());

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
#endif
}
