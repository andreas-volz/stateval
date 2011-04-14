#ifndef VIEW_CACHE_H
#define VIEW_CACHE_H

/* local */
#include "View.h"
#include "Logger.h"

/* STD */
#include <list>

struct ViewSpec
{
  View *view;
  int layer;
};

// Define two ViewSpecs as equal if they contain same View. Layer isn't important.
bool operator == (const ViewSpec &viewSpec1, const ViewSpec &viewSpec2);

/*
 TODO: Current ViewCache implementation supports only easy case that old and new
 ViewState contains same views. If needed more advanced uses cases could be implemented.
 */

class ViewCache
{
public:
  ViewCache ();
  
  void setUnrealizeViewList (std::list <ViewSpec> unrealizeViewList);
  void setRealizeViewList (std::list <ViewSpec> realizeViewList);

private:
  std::list <ViewSpec> mUnrealizeViewList;

  Logger mLogger;
};

#endif // VIEW_CACHE_H
