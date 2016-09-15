#ifndef GRAPH_H
#define GRAPH_H

#include <dataload.h>
#include <vector>
#include <cstring>
#include <queue>
#include <algorithm>
#include <unordered_map>
using namespace std;

const int maxn_edge = 600000;
const int maxn = 300000;

struct edge
{
    int nxt, v, u;
    double w;
};

struct point_t
{
    double x, y;
    int id;
    point_t() {}
    point_t(double x, double y) : x(x), y(y) {}
    point_t operator -(const point_t &b) const
    {
        return point_t(x - b.x, y - b.y);
    }
    double length()
    {
        return sqrt(x * x + y * y);
    }
};

double dis_point_segment(const point_t p, const point_t s, const point_t t);

//struct Graph
namespace Graph
{
    extern edge E[maxn_edge];
    extern int head[maxn], tot;
    extern int n, m, n1;
    extern unordered_map<int, int> NodeFilter;
    extern int relabel[maxn * 2];
    extern point_t P[maxn], P1[maxn * 2];
    struct edge1
    {
        int u, v;
        double w;
    };
    extern edge1 E1[maxn];
    void add(int a, int b, double c);
    void build();
    struct edge2
    {
        int v, nxt;
        double w;
    };
    extern int headR[maxn], totR;
    extern edge2 ER[maxn_edge];
    void addR(int a, int b, double c);
}

extern bool cmpx(point_t a, point_t b);
//extern Graph G;
#endif
