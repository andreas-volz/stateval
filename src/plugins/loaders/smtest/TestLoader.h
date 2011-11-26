#ifndef TEST_LOADER_H
#define TEST_LOADER_H

/* STD */
#include <iostream>
#include <string>

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"

/* local */
#include "Logger.h"

class TestLoader : public Loader
{
public:
  TestLoader();
  ~TestLoader();

  const std::string getType();

  const unsigned int getMajorVersion();

  const unsigned int getMinorVersion();

  bool load(Context *context, const std::string &test);

protected:

private:
  Logger mLogger;
  Context *mContext;
};

#endif // TEST_LOADER_H
