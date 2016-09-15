#include "spath.h"
using namespace Graph;
using namespace std;
const double inf = 1e10;
double dist[maxn];
int pre[maxn];
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
#define QLIMIT 1000000
namespace Dijkstra
{
	typedef pair<double, int> qnode;
	priority_queue<qnode, vector<qnode>, greater<qnode> > Q;
    void dijsktra(int S, int T)
	{
		for (int i = 1; i <= n; i++)
            dist[i] = inf, pre[i] = 0;
		dist[S] = 0;
        while (!Q.empty())
            Q.pop();
		Q.push(make_pair(0, S));
		while (!Q.empty())
		{
            if (Q.size() > QLIMIT)
                return;
			qnode q = Q.top(); Q.pop();
			int u = q.second;
            if (u == T)
                return;
			if (q.first > dist[u])
				continue;
			for (int i = head[u]; i; i = E[i].nxt)
			{
				int v = E[i].v;
				if (dist[u] + E[i].w < dist[v])
				{
					dist[v] = dist[u] + E[i].w;
                    pre[v] = u;
					Q.push(make_pair(dist[v], v));
				}
			}
		}
	}
	double query(int s, int t)
	{
        dijsktra(s, t);
		return dist[t];
	}
}
namespace SPFA
{
	bool flag[maxn];
	queue<int> Q;
    int T;
    double dis[maxn];
	void spfa(int S)
	{
		for (int i = 1; i <= n; i++)
        {
            dist[i] = inf, flag[i] = 0, pre[i] = 0;
            dis[i] = disOnEarth(relabel[i], relabel[T]);
        }
		dist[S] = 0;
        Q.push(S); flag[S] = 1;
		while (!Q.empty())
		{
            if (Q.size() > QLIMIT)
                return;
			int u = Q.front(); Q.pop();
			for (int i = head[u]; i; i = E[i].nxt)
			{
				int v = E[i].v;
				if (dist[u] + E[i].w < dist[v])
				{
                    dist[v] = dist[u] + E[i].w;
                    pre[v] = u;
                    if (!flag[v])
                        if (dist[v] + dis[v] < dist[T])
                        {
                            flag[v] = 1;
                            Q.push(v);
                        }
				}
			}
			flag[u] = 0;
		}
	}
	double query(int s, int t)
	{
        T = t;
        spfa(s);
		return dist[t];
	}
}
namespace Astar
{
	typedef pair<double, int> qnode;
	priority_queue<qnode, vector<qnode>, greater<qnode> > Q;
    bool outq[maxn];
	inline double calcH(int s, int t)
	{
        int u = relabel[s], v = relabel[t];
        return disOnEarth(u, v);
	}
	void Astar(int S, int T)
	{
		for (int i = 1; i <= n; i++)
            dist[i] = inf, outq[i] = 0, pre[i] = 0;
		dist[S] = 0;
        while (!Q.empty())
            Q.pop();
		Q.push(make_pair(calcH(S, T), S));
		while (!Q.empty())
		{
            if (Q.size() > QLIMIT)
                return;
            qnode q = Q.top(); Q.pop();
            int u = q.second;
            if (u == T)
                return;
            if (outq[u])
                continue;
            outq[u] = 1;
			for (int i = head[u]; i; i = E[i].nxt)
			{
				int v = E[i].v;
				if (dist[u] + E[i].w < dist[v])
				{
					dist[v] = dist[u] + E[i].w;
                    pre[v] = u;
                    Q.push(make_pair(calcH(v, T) + dist[v], v));
				}
			}
		}
	}
	double query(int s, int t)
	{
		Astar(s, t);
		return dist[t];
	}
}
vector<int> path;
void getpath(int s, int t)
{
    if (dist[t] >= inf)
        return;
    path.clear();
    path.push_back(relabel[t]);
    int p = t;
    while (p != s)
    {
        p = pre[p];
        path.push_back(relabel[p]);
    }
}
