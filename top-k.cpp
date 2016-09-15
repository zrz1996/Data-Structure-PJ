#include "top-k.h"
using namespace std;
using namespace Graph;
namespace TOPKHEAP
{
    const double inf = 1e10;
    namespace LHEAP
    {
        struct HeapNode
        {
            pair<double, int> val;
            int dis;
            HeapNode *ls, *rs;
        };

        HeapNode *null, *H[maxn], *h[maxn];
        HeapNode pool[5000000], *tail = pool;
        inline HeapNode *newnode()
        {
            return tail++;
        }

        void init()
        {
            tail = pool;
            null = newnode();
            null -> val = make_pair(inf, 0);
            null -> dis = -1;
            null -> ls = null -> rs = null;
        }

        HeapNode *marge(HeapNode *a, HeapNode *b)
        {
            if (a == null) return b;
            if (b == null) return a;
            if (a -> val > b -> val)
                swap(a, b);
            HeapNode *c = newnode();
            *c = *a;
            c -> rs = marge(a -> rs, b);
            if (c -> rs -> dis > c -> ls -> dis)
                swap(c -> ls, c -> rs);
            c -> dis = c -> rs -> dis + 1;
            return c;
        }

        HeapNode *join(HeapNode *a, pair<double, int> b)
        {
            HeapNode *c = newnode();
            c -> val = b, c -> dis = 0;
            c -> ls = c -> rs = null;
            return marge(a, c);
        }
    }
    using namespace LHEAP;

    struct edge3
    {
        int v, nxt;
    };
    edge3 ET[maxn];
    int headT[maxn], totT;


    void addT(int u, int v)
    {
        totT++; ET[totT].nxt = headT[u]; headT[u] = totT;
        ET[totT].v = v;
    }

    int fa[maxn];
    double delta[maxn_edge];
    void make_Heap(int u)
    {
        queue<HeapNode*> Q;
        h[u] = H[u] = null;
        for (int i = head[u]; i; i = E[i].nxt)
        {
            pair<double, int> v = make_pair(delta[i], i);
            if (E[i].v == fa[u] && fabs(delta[i]) < 1e-6)
                fa[u] = 0;
            else
                Q.push(join(null, v));
        }
        while (!Q.empty())
        {
            h[u] = Q.front();
            Q.pop();
            if (Q.empty())
                return;
            HeapNode *p = Q.front();
            h[u] = marge(h[u], p);
            Q.pop();
            Q.push(h[u]);
        }
    }
    int S, T, K;
    double dis[maxn];
    void dijsktra()
    {
        for (int i = 1; i <= n; i++)
            dis[i] = inf, fa[i] = 0;
        dis[T] = 0;
        priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > Q;
        Q.push(make_pair(0, T));
        while (!Q.empty())
        {
            pair<double, int> p = Q.top();
            Q.pop();
            int u = p.second;
            if (p.first > dis[u])
                continue;
            for (int i = headR[u]; i; i = ER[i].nxt)
            {
                int v = ER[i].v;
                if (dis[u] + ER[i].w < dis[v])
                {
                    dis[v] = dis[u] + ER[i].w;
                    fa[v] = u;
                    Q.push(make_pair(dis[v], v));
                }
            }
        }
        for (int i = 1; i <= n; i++)
            if (i != T)
                addT(fa[i], i);
        for (int i = 1; i <= tot; i++)
        {
            delta[i] = E[i].w + dis[E[i].v] - dis[E[i].u];
        }
        for (int i = 1; i <= n; i++)
            make_Heap(i);
    }

    void dfs(int u)
    {
        for (int i = headT[u]; i; i = ET[i].nxt)
        {
            int v = ET[i].v;
            H[v] = marge(H[u], h[v]);
            dfs(v);
        }
    }

    typedef pair<double, pair<int, HeapNode*> > qnode;
    priority_queue<qnode, vector<qnode>, greater<qnode> > Q;
    double ans;
    void Kshort()
    {
        while (!Q.empty())
            Q.pop();
        Q.push(make_pair(0, make_pair(S, null)));

        while (dis[S] != inf && !Q.empty() && K)
        {
            qnode _top = Q.top();
            Q.pop();
            ans = _top.first;
            K -= 1;
            int u = _top.second.first;
            HeapNode *p = _top.second.second;
            double _delta = p->val.first;

            if (p->ls != null)
            {
                HeapNode *pl = p->ls;
                int v = E[pl->val.second].v;
                double w = pl->val.first - _delta;
                Q.push(make_pair(ans + w, make_pair(v, pl)));
            }
            if (p->rs != null)
            {
                HeapNode *pr = p->rs;
                int v = E[pr->val.second].v;
                double w = pr->val.first - _delta;
                Q.push(make_pair(ans + w, make_pair(v, pr)));
            }
            if (H[u] != null)
            {
                int v = E[H[u]->val.second].v;
                double w = H[u]->val.first;
                Q.push(make_pair(ans + w, make_pair(v, H[u])));
            }
        }
    }
    double query(int s, int t, int k)
    {
        init();
        totT = 0;
        memset(headT, 0, sizeof(headT));
        S = s, T = t, K = k;
        dijsktra();
        H[T] = h[T];
        dfs(T);
        Kshort();
        return K ? -1 : ans + dis[S];
    }
}
namespace TOPKASTAR
{
    const double inf = 1e8;
    double h[maxn];
    void dijsktra(int T)
    {
        for (int i = 1; i <= n; i++)
            h[i] = inf;
        h[T] = 0;
        priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > Q;
        Q.push(make_pair(0, T));
        while (!Q.empty())
        {
            pair<double, int> p = Q.top();
            Q.pop();
            int u = p.second;
            if (p.first > h[u])
                continue;
            for (int i = headR[u]; i; i = ER[i].nxt)
            {
                int v = ER[i].v;
                if (h[u] + ER[i].w < h[v])
                {
                    h[v] = h[u] + ER[i].w;
                    Q.push(make_pair(h[v], v));
                }
            }
        }
    }
    int pre[maxn];
    typedef pair<double, int> qnode;
    priority_queue<qnode, vector<qnode>, greater<qnode> > Q;
    int outq[maxn];
    double Astar(int S, int T, int K)
    {
        if (h[S] == inf)
            return -1;
        while (!Q.empty())
            Q.pop();
        for (int i = 1; i <= n; i++)
            outq[i] = 0;
        Q.push(make_pair(h[S], S));
        while (!Q.empty())
        {
            qnode q = Q.top(); Q.pop();
            int u = q.second;
            outq[u]++;
            double d = q.first - h[u];
            if (outq[T] == K)
                return d;
            if (outq[u] > K)
                continue;
            for (int i = head[u]; i; i = E[i].nxt)
            {
                int v = E[i].v;
                double f = h[v] + E[i].w + d;
                Q.push(make_pair(f, v));

            }
        }
        return -1;
    }
    double query(int s, int t, int k)
    {
        dijsktra(t);
        return Astar(s, t, k);
    }
}
