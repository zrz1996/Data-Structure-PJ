#include "nodefilter.h"
using namespace std;
bgi::rtree< value, bgi::quadratic<16> > rtree;
vector<value> Node2draw;
void filterInit()
{
    for (int  i = 0; i < numNode; i++)
    {
        rtree.insert(make_pair(bpoint(Node[i].x, Node[i].y), i));
    }
}

void filter(double x1, double y1, double x2, double y2)
{
    box query_box(bpoint(x1, y1), bpoint(x2, y2));
    rtree.query(bgi::intersects(query_box), back_inserter(Node2draw));
}
