#include "tbb/task_arena.h"
#include "tbb/task_group.h"


namespace ROOT {

//class ROpaqueNumaNodeId: public tbb::numa_node_id {};
class ROpaqueTaskArena: public tbb::task_arena {};
class ROpaqueTaskGroup: public tbb::task_group {};

}
