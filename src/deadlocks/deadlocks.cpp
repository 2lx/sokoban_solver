#include "deadlocks1x2.h"
#include "deadlocks2x2.h"
#include "manual_deadlocks.h"

using namespace std;

namespace Sokoban
{

vector<vector<Sokoban::DeadlockInfo>> generated_deadlocks = {
    /* deadlocks1x2, */
    deadlocks2x2,
    manual_deadlocks,
};

}
