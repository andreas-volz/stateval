#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* EFL */
#include <Efl_Ui.hh>

/* local */
#include "EdjeWidget.h"
#include "stringUtil.h"

using namespace std;

EdjeWidget::EdjeWidget(View &view, WidgetRenderer &widgetRenderer, const std::string &name) :
  mLogger("stateval.plugins.viewmanager.edje.EdjeWidget"),
  Widget(widgetRenderer, name),
  mView(dynamic_cast<EdjeView*>(&view)),
  mGetProperty(false)
{
  //mUpdateDataDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeWidget::updateDataDispatched));
  mSetPropertyDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeWidget::setPropertyDispatched));
  mGetPropertyDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeWidget::getPropertyDispatched));
}

void EdjeWidget::setProperty(const std::string &name, const Variable &property)
{
  LOG4CXX_TRACE(mLogger, "+setProperty()");

  mActiveSetPropertyName = name;
  mProperties[mActiveSetPropertyName] = const_cast<Variable*>(&property);

  mMutexUpdateData.lock();
  mSetPropertyDispatcher.emit();
  
  mCondUpdateData.wait(mMutexUpdateData);
  mMutexUpdateData.unlock();
  
  LOG4CXX_TRACE(mLogger, "-setProperty()");
}

void EdjeWidget::setPropertyDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "setPropertyDispatched: " << getName() << " / " << mActiveSetPropertyName);

  efl::ui::Layout *layout = mView->getLayout();

  if(layout)
  {
    //Eflxx::CountedPtr <Edjexx::Object> edjeObj(layout->getEdje());

    //try
    //{
      //Edjexx::Part &part = edjeObj->getPart(getName());
      efl::Object part = layout->part_get(getName());

      Variable *var = mProperties[mActiveSetPropertyName];

      if(mWidgetRenderer && var)
      {
        // call the default renderer
        mWidgetRenderer->render(&part, mActiveSetPropertyName, var);
      }
    //}
    /*catch (Edjexx::ExternalNotExistingException ene)
    {
      LOG4CXX_ERROR(mLogger, ene.what());
    }*/
  }

  mMutexUpdateData.lock();
  mCondUpdateData.signal();
  mMutexUpdateData.unlock();
}


Variable *EdjeWidget::getProperty(const std::string &name)
{
  LOG4CXX_TRACE(mLogger, "+getProperty()");

  mActiveGetPropertyName = name;

  mMutexUpdateData.lock();
  mGetProperty = false;
  mGetPropertyDispatcher.emit();

  while(!mGetProperty)
  {
    mCondUpdateData.wait(mMutexUpdateData);
  }
  mMutexUpdateData.unlock();
  
  LOG4CXX_TRACE(mLogger, "-getProperty()");

  Variable *var = mProperties[name];
  
  return var;
}

void EdjeWidget::getPropertyDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "getPropertyDispatched: " << getName() << " / " << mActiveGetPropertyName);

  efl::ui::Layout *layout = mView->getLayout();
  
  if(layout)
  {
    //Eflxx::CountedPtr <Edjexx::Object> edjeObj(layout->getEdje());

   // //try
    //{
      //Edjexx::Part &part = edjeObj->getPart(getName());
      efl::Object part = layout->part_get(getName());

      // get value back from default renderer
      Variable *var = mWidgetRenderer->getRenderedValue(&part, mActiveGetPropertyName);
      if(var)
      {
        mProperties[mActiveGetPropertyName] = var;
      }

    /*}
    catch (Edjexx::ExternalNotExistingException ene)
    {
      LOG4CXX_ERROR(mLogger, ene.what());
    }*/
  }

  mGetProperty = true;

  mMutexUpdateData.lock();
  mCondUpdateData.signal();
  mMutexUpdateData.unlock();
}

void EdjeWidget::updateContent()
{
  setPropertyDispatched(0);
}

void EdjeWidget::freeContent()
{
   // TMP: deactivated to free content renderer => was an idea to clear lists
  /*Elmxx::Layout *layout = mView->getLayout();

  if(layout)
  {
    Eflxx::CountedPtr <Edjexx::Object> edjeObj(layout->getEdje());

    try
    {
      Edjexx::Part &part = edjeObj->getPart(getName());

      if(mWidgetRenderer)
      {
        // call the default renderer
        mWidgetRenderer->render(&part, "", NULL);
      }
    }
    catch (Edjexx::ExternalNotExistingException ene)
    {
      LOG4CXX_ERROR(mLogger, ene.what());
    }
  }*/
}
