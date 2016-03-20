#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Eflxx */
#include <elementaryxx/Elementaryxx.h>

/* local */
#include "EdjeWidget.h"
#include "stringUtil.h"

using namespace std;

EdjeWidget::EdjeWidget(View &view, WidgetRenderer &widgetRenderer, const std::string &name) :
  mLogger("stateval.plugins.viewmanager.edje.EdjeWidget"),
  Widget(widgetRenderer, name),
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
  // FIXME: broken memory handling
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

      if(mWidgetRenderer && var)
      {
        // call the default renderer
        mWidgetRenderer->render(&part, mActiveSetPropertyName, var);
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

      // get value back from default renderer
      Variable *var = mWidgetRenderer->getRenderedValue(&part, mActiveGetPropertyName);
      if(var)
      {
        mProperties[mActiveGetPropertyName] = var;
      }

    }
    catch (Edjexx::ExternalNotExistingException ene)
    {
      LOG4CXX_ERROR(mLogger, ene.what());
    }
  }
  
  mCondUpdateData.signal();
}

void EdjeWidget::updateContent()
{
  setPropertyDispatched(0);
}

