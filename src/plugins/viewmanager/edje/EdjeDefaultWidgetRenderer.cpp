#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* EFL C++ */
// TODO: port EFL C++
/*#include <Evas.hh>
#include <Edje.hh>
#include <Elementary.hh>*/

/* STD */
#include <cassert>

#include "EdjeDefaultWidgetRenderer.h"

EdjeDefaultWidgetRenderer::EdjeDefaultWidgetRenderer() :
  mLogger("stateval.plugins.viewmanager.edje.EdjeDefaultWidgetRenderer")
{

}

EdjeDefaultWidgetRenderer::~EdjeDefaultWidgetRenderer()
{

}

void EdjeDefaultWidgetRenderer::render(void *renderObject, const std::string &varName, Variable *var)
{
  // TODO: port EFL C++
  /*Edjexx::Part *part = static_cast<Edjexx::Part*>(renderObject);

  if(part)
  {
    switch(var->getType ())
    {
      case Variable::TYPE_INTEGER:
      {
        Integer *intObj = dynamic_cast<Integer*>(var);
        LOG4CXX_TRACE(mLogger, "TYPE_INTEGER: " << intObj->getData());

        Edjexx::ExternalParam param (varName, intObj->getData());
        bool validParam = part->setParam (&param);        
        break;
      }
      case Variable::TYPE_STRING:
      {
        String *strObj = dynamic_cast<String*>(var);
        LOG4CXX_TRACE(mLogger, "TYPE_STRING: " << strObj->getData());

        if(part->isExternalObject())
        {
          Edjexx::ExternalParam param (varName, strObj->getData());
          bool validParam = part->setParam (&param);
          LOG4CXX_TRACE(mLogger, "validParam: " << validParam);
        }
        else
        {
          //part->setText(strObj->getData());
        }
        break;
      }
      default:
        LOG4CXX_DEBUG(mLogger, "unknown type");
    }
  }
  else
  {
    assert(false);
  }*/
}

Variable *EdjeDefaultWidgetRenderer::getRenderedValue(void *renderObject, const std::string &varName)
{
  // TODO: port EFL C++
  //Edjexx::Part *part = static_cast<Edjexx::Part*>(renderObject);

  /*if(part)
  {
    Edje_External_Param_Type paramType = part->getParamType(varName);
    switch(paramType)
    {
      case EDJE_EXTERNAL_PARAM_TYPE_INT:
      {
        Edjexx::ExternalParam param (varName, (int) 0);    
        bool validParam = part->getParam (param);
        LOG4CXX_DEBUG(mLogger, "INT: " << param.mParam.i);
        // FIXME: broken memory handling
        //delete mProperties[mActiveGetPropertyName];
        Integer *intObj = new Integer(param.mParam.i);
        return intObj;
        break;
      }
      case EDJE_EXTERNAL_PARAM_TYPE_DOUBLE:
        LOG4CXX_DEBUG(mLogger, "DOUBLE");
        break;
      case EDJE_EXTERNAL_PARAM_TYPE_STRING:
      {
        if(part->isExternalObject())
        {
          Edjexx::ExternalParam param (varName, "");    
          bool validParam = part->getParam (param);
          LOG4CXX_DEBUG(mLogger, "STRING" << param.mParam.s);
          //delete mProperties[mActiveGetPropertyName];
          String *strObj = new String(param.mParam.s);
          return strObj;
        }
        else
        {
          String *strObj = new String(part->getText());
          return strObj;
        }
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
  else
  {
    assert(false);
  }*/
}

