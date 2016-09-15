#ifndef DATALOAD_H
#define DATALOAD_H

#include <unordered_map>
#include <iostream>
#include <cassert>
#include "data.h"
#include "pugixml.hpp"
using namespace std;

const int maxn_node = 505000;
extern node Node[maxn_node];
extern int numNode;
extern unordered_map<unsigned, int> labelNode;

const int maxn_way = 73000;
extern way Way[maxn_way];
extern int numWay;

const int maxn_building = 73000;
extern building Building[maxn_building];
extern int numBuilding;

const int maxn_coastline = 1000;
extern coastline Coastline[maxn_coastline];
extern int numCoastline;

const int maxn_island = 20;
extern island Island[maxn_island];
extern int numIsland;

void load_data();

#endif // DATALOAD_H

