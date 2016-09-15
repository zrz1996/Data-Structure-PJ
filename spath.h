#ifndef SPATH_H
#define SPATH_H

#include <queue>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "graph.h"
#include "dataload.h"

extern int pre[maxn];
namespace Dijkstra
{
    double query(int s, int t);
}
namespace SPFA
{
    double query(int s, int t);
}
namespace Astar
{
    double query(int s, int t);
}
extern vector<int> path;
void getpath(int s, int t);
#endif // SPATH_H

