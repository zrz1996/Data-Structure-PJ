#ifndef DATA_H
#define DATA_H
#include <vector>

using namespace std;

struct node
{
    unsigned id;
    double x, y, curx, cury, lat, lon;
    vector<unsigned> nBuilding, nWay;
    node()
    {
        id = x = y = curx = cury = 0;
        nBuilding.clear();
        nWay.clear();
    }
    //node(node const&) = default;
};
struct way
{
    unsigned id;
    int color, thickness, layer;
    vector<unsigned> wayNode;
    char name[100];
    double len;
    bool operator <(const way &b) const
    {
        return layer < b.layer;
    }
};
struct building
{
    unsigned id;
    int color, layer;
    vector<unsigned> buildingNode;
    char name[100];
    double cx, cy;
    bool operator <(const building &b) const
    {
        return layer < b.layer;
    }
};
struct coastline
{
    unsigned id;
    vector<unsigned> coastlineNode;
};
typedef vector<unsigned> island;


#endif // DATA_H

