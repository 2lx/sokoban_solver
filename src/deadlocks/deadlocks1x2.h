#ifndef DEADLOCKS1x2_H
#define DEADLOCKS1x2_H

#include "deadlock_info.h"

const std::vector<Sokoban::DeadlockInfo> deadlocks1x2 = {

//
//    #
//   #?
  {
    { { 0, -1}, {-1,  0} },
    {  },
    {  },
    { { { 0,  0} } },
    1
  },
};

#endif
