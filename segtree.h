#ifndef SEGTREE_H
#define SEGTREE_H

#include "graph.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>

namespace SEGMENTTREE
{
    void init();
    extern vector<int> vec;
    void query(point_t p1, point_t p2);
}


namespace SEGMENTTREE1
{
    void init();
    extern vector<int> vec;
    void query(point_t p1, point_t p2);
}

#endif // SEGTREE_H

