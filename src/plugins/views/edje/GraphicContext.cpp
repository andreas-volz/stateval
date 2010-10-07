#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/GraphicContext.h"

using namespace std;

GraphicContext &GraphicContext::instance ()
{
  static GraphicContext _instance;
  return _instance;
}

GraphicContext::~GraphicContext ()
{
  cout << "~GraphicContext" << endl;
}

void GraphicContext::init (Evasxx::Canvas &evas)
{
  mEvas = &evas;
}

Evasxx::Canvas &GraphicContext::getCanvas ()
{
  return *mEvas;
}

void GraphicContext::setResolution (const Eflxx::Size &resolution)
{
  mResolution = resolution;
}

const Eflxx::Size GraphicContext::getResolution ()
{
  return mResolution;
}
