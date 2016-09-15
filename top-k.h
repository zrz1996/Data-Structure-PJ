#ifndef TOPK_H
#define TOPK_H

#include <queue>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cassert>
#include "graph.h"
#include "dataload.h"

namespace TOPKHEAP
{
    double query(int s, int t, int k);
}
namespace TOPKASTAR
{
    double query(int s, int t, int k);
}
#endif // TOPK_H

