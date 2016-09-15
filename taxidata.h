#ifndef TAXIDATA_H
#define TAXIDATA_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <unistd.h>
#include "dataload.h"
#include "graph.h"
#include "kdtree.h"
using namespace std;

struct tnode
{
    double lat, lon;
    int time;
    char customer;
};
struct pnode
{
    double x, y, curx, cury;
};
struct keynode
{
    double lat, lon;
    int id, time;
    bool customer;
};

namespace TAXI
{
    extern vector<tnode> Taxi[32000];
    bool parser(int id);
    pair<int, int> getTime(int id);
    extern vector<pnode> TaxiP;
    void getpath(int id);
    extern vector<int> TaxiID;
    extern vector<int> TaxiState;
    void getIDlist();
    void parserAll();
    extern vector<keynode> Key;
    extern point_t P[1000020];
    extern int n;
    point_t meanshift(point_t p);
}
#endif // TAXIDATA_H

