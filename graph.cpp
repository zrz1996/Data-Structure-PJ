#include <graph.h>
using namespace std;

//Graph G;
namespace Graph
{
    edge E[maxn_edge];
    int head[maxn], tot;
    int n, m, n1;
    unordered_map<int, int> NodeFilter;
    int relabel[maxn * 2];
    edge1 E1[maxn];
    point_t P[maxn], P1[maxn * 2];
    void add(int a, int b, double c);
    void build();
    int headR[maxn], totR;
    edge2 ER[maxn_edge];
}

void Graph::add(int a, int b, double c)
{
    tot++; E[tot].nxt = head[a]; head[a] = tot;
    E[tot].v = b, E[tot].w = c, E[tot].u = a;
}

void Graph::addR(int a, int b, double c)
{
    totR++; ER[totR].nxt = headR[a]; headR[a] = totR;
    ER[totR].v = b, ER[totR].w = c;
}

inline double disOnMap(int i, int j)
{
    return sqrt((Node[i].x - Node[j].x) * (Node[i].x - Node[j].x) + (Node[i].y - Node[j].y) * (Node[i].y - Node[j].y));
}
inline double disOnEarth(int i, int j)
{
    double lat1 = Node[i].lat, lat2 = Node[j].lat, lon1 = Node[i].lon, lon2 = Node[j].lon;
    lat1 = lat1 * M_PI / 180, lat2 = lat2 * M_PI / 180;
    lon1 = lon1 * M_PI / 180, lon2 = lon2 * M_PI / 180;
    double a = sin((lat1 - lat2) / 2);
    double b = sin((lon1 - lon2) / 2);
    double s = 2 * asin(sqrt(a * a +cos(lat1) * cos(lat2) * b * b));
    s = s * 6378137;
    return s;
}

inline double dot(point_t a, point_t b)
{
    return a.x * b.x + a.y * b.y;
}
inline double det(point_t a , point_t b)
{
    return a.x * b.y - a.y * b.x;
}
inline int dblcmp(double x)
{
    if (fabs(x < 1e-8))
        return x;
    return x > 0 ? 1 : -1;
}

double dis_point_segment(const point_t p, const point_t s, const point_t t)
{
    if (dblcmp(dot(p - s, t - s)) < 0)
        return (p - s).length();
    if (dblcmp(dot(p - t, s - t)) < 0)
        return (p - t).length();
    return fabs(det(s - p, t - p) / ((s - t).length()));
}

bool cmpx(point_t a, point_t b)
{
    return a.x < b.x;
}
void Graph::build()
{
    m = 0, tot = 0;
    memset(head, 0, sizeof(head));
    NodeFilter.clear();
    int num = 0;
    for (int i = 0; i < numWay; i++)
    {
        Way[i].len = 0;
        for (int j = 0; j < (int)Way[i].wayNode.size() - 1; j++)
        {
            int u = labelNode[Way[i].wayNode[j]], v = labelNode[Way[i].wayNode[j + 1]];
            double w = disOnEarth(u, v);
            E1[m].u = u, E1[m].v = v, E1[m].w = w;
            m++;
            if (NodeFilter.find(u) == NodeFilter.end())
                NodeFilter[u] = ++num, relabel[num] = u;
            if (NodeFilter.find(v) == NodeFilter.end())
                NodeFilter[v] = ++num, relabel[num] = v;
            Way[i].len += w;
        }
    }
    n = NodeFilter.size();
    for (int i = 1; i <= n; i++)
    {
        P[i].id = relabel[i], P[i].x = Node[relabel[i]].x, P[i].y = Node[relabel[i]].y;
    }
    for (int i = 0; i < numBuilding; i++)
    {
        if (Building[i].layer > 0)
        {
            for (int j = 0; j < (int)Building[i].buildingNode.size(); j++)
            {
                int u = labelNode[Building[i].buildingNode[j]];
                if (NodeFilter.find(u) == NodeFilter.end())
                    NodeFilter[u] = ++num, relabel[num] = u;
            }
        }
    }
    n1 = NodeFilter.size();
    printf("%d %d\n", n, n1);
    fflush(stdout);
    for (int i = 1; i <= n1; i++)
    {
        P1[i].id = relabel[i], P1[i].x = Node[relabel[i]].x, P1[i].y = Node[relabel[i]].y;
    }
    //sort(P1 + 1, P1 + n1 + 1, cmpx);
    for (int i = 0; i < m; i++)
    {
        int u = E1[i].u, v = E1[i].v;
        u = NodeFilter[u], v = NodeFilter[v];
        double w = E1[i].w;
        add(u, v, w);
        add(v, u, w);
        addR(u, v, w);
        addR(v, u, w);
    }
    m *= 2;
}
