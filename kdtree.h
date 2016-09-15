#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <unordered_set>
#include "graph.h"
#include "dataload.h"
#include "mysignal.h"
#define maxn_bc 100010
#define alpha 0.75
struct KDNode
{
    point_t e;
    int size, count;
    KDNode *lc, *rc;
    bool split;
    bool exist;
    bool isBad()
    {
        return lc->count > alpha * count + 5 || rc->count > alpha * count + 5;
    }
    inline void push_up()
    {
        size = exist + lc->size + rc->size;
        count = 1 + lc->count + rc->count;
    }
};

struct KDTREE
{
    KDNode *root,*null;
    KDNode *bc[maxn_bc];
    int bc_top;
    void init()
    {
        null = new KDNode;
        null->lc = null->rc = null;
        null->size = null->count = 0;
        root = null;
        bc_top = 0;
    }
    KDNode *newKDNode(point_t e)
    {
        KDNode *p;
        if (bc_top)
            p = bc[--bc_top];
        else
            p = new KDNode;
        p->e = e;
        p->lc = p->rc = null;
        p->size = p->count = 1;
        p->exist = true;
        return p;
    }
    static bool cmpX(const point_t &a, const point_t &b)
    {
        return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.id < b.id);
    }
    static bool cmpY(const point_t &a, const point_t &b)
    {
        return a.y < b.y || (a.y == b.y && a.x < b.x) || (a.y == b.y && a.x == b.x && a.id < b.id);
    }
    KDNode *build(point_t *a, int l, int r, bool split)
    {
        if (l >= r)
            return null;
        int mid = (l + r) / 2;
        nth_element(a + l, a + mid, a + r, split ? cmpY : cmpX);
        KDNode *p = newKDNode(a[mid]);
        p->split = split;
        p->lc = build(a, l, mid, !split);
        p->rc = build(a, mid + 1, r, !split);
        p->push_up();
        return p;
    }
    void build(point_t *a, int l, int r)
    {
        init();
        root = build(a, l, r, 0);
    }
    void Travel(KDNode *p, vector<point_t>&v)
    {
        if (p == null)
            return;
        Travel(p->lc, v);
        if (p->exist)
            v.push_back(p->e);
        bc[bc_top++] = p;
        Travel(p->rc, v);
    }
    KDNode *splitide(vector<point_t>&v, int l, int r, bool split)
    {
        if (l >= r)return null;
        int mid = (l + r) / 2;
        nth_element(v.begin() + l, v.begin() + mid, v.begin() + r, split ? cmpY : cmpX);
        KDNode *p = newKDNode(v[mid]);
        p->split = split;
        p->lc = splitide(v, l, mid, !split);
        p->rc = splitide(v, mid + 1, r, !split);
        p->push_up();
        return p;
    }
    inline void rebuild(KDNode *&p)
    {
        vector<point_t>v;
        Travel(p, v);
        p = splitide(v, 0, v.size(), p->split);
    }
    KDNode **insert(KDNode *&p, point_t a, bool split)
    {
        if (p == null)
        {
            p = newKDNode(a);
            p->split = split;
            return &null;
        }
        else
        {
            p->count++;
            p->size++;
            KDNode **res;
            if (split ? cmpY(a, p->e) : cmpX(a, p->e))
                res = insert(p->lc, a, !split);
            else
                res = insert(p->rc, a, !split);
            if (p->isBad())
                res = &p;
            return res;
        }
    }
    void insert(point_t e)
    {
        KDNode **p = insert(root, e, 0);
        if (*p != null)
            rebuild(*p);
    }
    void Delete(KDNode *p, point_t a)
    {
        if(p->size == 0)
            return;
        if(p->exist && p->e.x == a.x && p->e.y == a.y)
        {
            p->exist = 0;
            p->size--;
        }
        if(p->split ? cmpY(a, p->e) : cmpX(a, p->e))
            Delete(p->lc, a);
        else
            Delete(p->rc, a);
        p->push_up();
    }
    void Delete(point_t a)
    {
        Delete(root, a);
    }
    vector<int>vec;
    void getvec(KDNode *p, double minx, double maxx, double miny, double maxy)
    {
        if (p->size == 0)
            return;
        if (p->exist && minx <= p->e.x && p->e.x <= maxx && miny <= p->e.y && p->e.y <= maxy)
        {
            vec.push_back(p->e.id);
        }
        if (p->split ? miny <= p->e.y : minx <= p->e.x)
            getvec(p->lc, minx, maxx, miny, maxy);
        if (p->split ? maxy >= p->e.y : maxx >= p->e.x)
            getvec(p->rc, minx, maxx, miny, maxy);
    }
    void query(point_t p1, point_t p2)
    {
        vec.clear();
        double minx = min(p1.x, p2.x), miny = min(p1.y, p2.y);
        double maxx = max(p1.x, p2.x), maxy = max(p1.y, p2.y);
        getvec(root, minx, maxx, miny, maxy);
    }

    inline double sdis(point_t a, point_t b)
    {
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    }
    typedef pair<double, int> qnode;
    priority_queue<qnode> Q;
    void query(point_t p, KDNode *x, bool split, int k)
    {
        if (x->size == 0)
            return;
        if (split ? cmpY(p, x->e) : cmpX(p, x->e))
        {
            query(p, x->lc, !split, k);
            if ((int)Q.size() < k)
            {
                if (x->exist)
                    Q.push(make_pair(sdis(p, x->e), x->e.id));
                query(p, x->rc, !split, k);
            }
            else
            {
                if (sdis(p, x->e) <= Q.top().first)
                {
                    if (x->exist)
                        Q.push(make_pair(sdis(p, x->e), x->e.id));
                    Q.pop();
                }
                if (!split)
                {
                    if ((x->e.x - p.x) * (x->e.x - p.x) <= Q.top().first)
                        query(p, x->rc, !split, k);
                }
                else
                {
                    if ((x->e.y - p.y) * (x->e.y - p.y) <= Q.top().first)
                        query(p, x->rc, !split, k);
                }
            }
        }
        else
        {
            query(p, x->rc, !split, k);
            if ((int)Q.size() < k)
            {
                if (x->exist)
                    Q.push(make_pair(sdis(p, x->e), x->e.id));
                query(p, x->lc, !split, k);
            }
            else
            {
                if (sdis(p, x->e) <= Q.top().first)
                {
                    if (x->exist)
                        Q.push(make_pair(sdis(p, x->e), x->e.id));
                    Q.pop();
                }
                if (!split)
                {
                    if ((x->e.x - p.x) * (x->e.x - p.x) <= Q.top().first)
                        query(p, x->lc, !split, k);
                }
                else
                {
                    if ((x->e.y - p.y) * (x->e.y - p.y) <= Q.top().first)
                        query(p, x->lc, !split, k);
                }
            }
        }
    }
    vector<int> kans;
    point_t qpoint;
    int qk;
    void query(point_t p, int k)
    {
        qpoint = p;
        qk = k / 10;
        kans.clear();
        if (k == 0)
            return;
        query(p, root, 0, k);
        while (!Q.empty())
        {
            kans.push_back(Q.top().second);
            Q.pop();
        }
    }
    unordered_set <int> kansWay, kansBuilding;
    int selectedB, selectedW;
    void getPointData(int type)
    {
        kansWay.clear();
        kansBuilding.clear();
        if (type == 1)
        {
            for (int i = 0; i < (int)kans.size(); i++)
            {
                int pid = kans[i];
                for (int j = 0; j < (int)Node[pid].nWay.size(); j++)
                    if ((int)kansWay.size() < qk)
                        kansWay.insert(Node[pid].nWay[j]);
                for (int j = 0; j < (int)Node[pid].nBuilding.size(); j++)
                    if ((int)kansBuilding.size() < qk)
                        kansBuilding.insert(Node[pid].nBuilding[j]);
            }
        }
        if (type == 0)
        {
            for (int i = 0; i < (int)vec.size(); i++)
            {
                int pid = vec[i];
                for (int j = 0; j < (int)Node[pid].nWay.size(); j++)
                    kansWay.insert(Node[pid].nWay[j]);
                for (int j = 0; j < (int)Node[pid].nBuilding.size(); j++)
                    kansBuilding.insert(Node[pid].nBuilding[j]);
            }
        }
        if (type == 2)
        {
            selectedB = -1, selectedW = -1;
            int posb;
            for (int i = (int)kans.size() - 1; i >= 0; i--)
            {
                int pid = kans[i];
                if (Node[pid].nBuilding.size() != 0)
                {
                    selectedB = Node[pid].nBuilding[0];
                    posb = pid;
                    break;
                }
            }
            double minsd = 1e10;
            for (int i = 0; i < (int)kans.size(); i++)
                for (int j = i + 1; j < (int)kans.size(); j++)
                {
                    bool pd = 0;
                    int cW;
                    int pid1 = kans[i], pid2 = kans[j];
                    for (int w1 = 0; w1 < (int)Node[pid1].nWay.size(); w1++)
                        for (int w2 = 0; w2 < (int)Node[pid2].nWay.size(); w2++)
                            if (Node[pid1].nWay[w1] == Node[pid2].nWay[w2])
                            {
                                pd = 1;
                                cW = Node[pid1].nWay[w1];
                                break;
                            }
                    if (!pd)
                        continue;
                    point_t p1 = point_t(Node[pid1].x, Node[pid1].y);
                    point_t p2 = point_t(Node[pid2].x, Node[pid2].y);
                    double d = dis_point_segment(qpoint, p1, p2);
                    if (d < minsd)
                    {
                        minsd = d, selectedW = cW;
                    }
                }
            if (selectedB != -1 && selectedW != -1)
            {
                double minbd = sqrt((Node[posb].x - qpoint.x) * (Node[posb].x - qpoint.x) + (Node[posb].y - qpoint.y) * (Node[posb].y - qpoint.y));
                if (minbd < minbd)
                    selectedW = -1;
                else
                    selectedB = -1;
            }
            if (selectedB == -1)
                MS.setValue(0, selectedW);
            else
                MS.setValue(1, selectedB);
        }
    }
};
extern KDTREE KDTW, KDTB, KDTT;
#endif // KDTREE_H

