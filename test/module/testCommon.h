#ifndef _testCommon_H_
#define _testCommon_H_ "2B35D852-C7BF-42DE-B938-CBB68A9EC5A9"

#include <sys/time.h>

static time_t getCurrentTimeInMS()
{
  struct timeval tv;
  if (gettimeofday(&tv, NULL) != 0)
    return 0;
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

#endif // _testCommon_H_
