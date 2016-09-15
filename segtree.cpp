#include "segtree.h"
namespace SEGMENTTREE
{
	struct dnode
	{
		point_t *val;
		int lpt, rpt;
		bool operator <(const dnode b) const
		{
			return val->y < b.val->y;
		}
	};
	struct snode
	{
		snode *l, *r;
		vector<dnode> D;	
	};
	snode *root;
	vector<point_t *> vis;
	void pushUP(snode *rt)
	{
		if (rt->l == NULL || rt->r == NULL)
		{
			if (rt->l != NULL)
			{
				rt->D = rt->l->D;
                for (int i = 0; i < int(rt->l->D.size()); i++)
					rt->D[i].lpt = i;
				return;
			}
			if (rt->r != NULL)
			{
				rt->D = rt->r->D;
                for (int i = 0; i < int(rt->r->D.size()); i++)
					rt->D[i].rpt = i;
				return;
			}
			return;
		}
		int sl = rt->l->D.size(), sr = rt->r->D.size();
		int posl = 0, posr = 0;
		while (posl < sl && posr < sr)
		{
			double yl = rt->l->D[posl].val->y, yr = rt->r->D[posr].val->y;
			if (yl <= yr)
			{
				dnode td;
				td.val = rt->l->D[posl].val;
				posl++;
				rt->D.push_back(td);
			}
			else
			{
				dnode td;
				td.val = rt->r->D[posr].val;
				posr++;
				rt->D.push_back(td);
			}
		}
		if (posl < sl)
		{
			for (; posl < sl; posl++)
			{
				dnode td;
				td.val = rt->l->D[posl].val;
				rt->D.push_back(td);
			}
		}
		if (posr < sr)
		{
			for (; posr < sr; posr++)
			{
				dnode td;
				td.val = rt->r->D[posr].val;
				rt->D.push_back(td);
			}
		}
		posl = 0, posr = 0;
        for (int i = 0; i < (int)(rt->D.size()); i++)
		{
			while (posl + 1 < sl && rt->l->D[posl].val->y < rt->D[i].val->y)
				posl++;
			rt->D[i].lpt = posl;
			if (rt->l->D[posl].val->y == rt->D[i].val->y)
				posl = min(posl + 1, sl - 1);
			while (posr + 1 < sr && rt->r->D[posr].val->y < rt->D[i].val->y)
				posr++;
			rt->D[i].rpt = posr;
			if (rt->r->D[posr].val->y == rt->D[i].val->y)
				posr = min(posr + 1, sr - 1);
		}
	}
	snode *newnode()
	{
		snode *p = new snode;
		p->l = p->r = NULL;
		p->D.clear();
		return p;
	}
	snode *build(int l, int r)
	{
		snode *rt = newnode();
		if (l == r)
		{
			dnode d;
            d.val = &Graph::P1[l];
			d.lpt = d.rpt = -1;
			rt->D.push_back(d);
			return rt;
		}
		int mid = (l + r) >> 1;
		rt->l = build(l, mid);
		rt->r = build(mid + 1, r);
		pushUP(rt);
		return rt;
	}
    void init()
    {
        sort(Graph::P1 + 1, Graph::P1 + Graph::n1 + 1, cmpx);
        root = build(1, Graph::n1);
	}
	double x1, x2, y1, y2;
	void Tquery(snode *rt, int l, int r, int pos1, int pos2)
	{
		if (rt == NULL)
			return;
		while (pos1 <= pos2)
		{
			double y = rt->D[pos1].val->y;
			if (y < y1)
				pos1++;
			else
				break;
		}
		while (pos2 >= pos1)
		{
			double y = rt->D[pos2].val->y;
			if (y > y2)
				pos2--;
			else
				break;
		}
		if (pos1 > pos2) 
			return;
        if (Graph::P1[l].x >= x1 && Graph::P1[r].x <= x2)
		{
			for (int i = pos1; i <= pos2; i++)
				vis.push_back(rt->D[i].val);
			return;
		}
		int mid = (l + r) >> 1;
        if (x1 <= Graph::P1[mid].x && rt->l != NULL)
		{
			Tquery(rt->l, l, mid, rt->D[pos1].lpt, rt->D[pos2].lpt);
		}
        if (x2 >= Graph::P1[mid + 1].x && rt->r != NULL)
		{
			Tquery(rt->r, mid + 1, r, rt->D[pos1].rpt, rt->D[pos2].rpt);
		}
	}
	void travel(snode *rt, int l, int r)
	{
		if (rt == NULL)
			return;
		cout << l << ' ' << r << endl;
		for (auto i : rt->D)
			cout << i.val->id << ' ' << i.lpt << ' ' << i.rpt << endl;
		int mid = (l + r) >> 1;
		travel(rt->l, l, mid);
		travel(rt->r, mid + 1, r);
	}
    vector<int> vec;
	void query(point_t p1, point_t p2)
	{
		vis.clear();
        double minx = min(p1.x, p2.x), miny = min(p1.y, p2.y);
        double maxx = max(p1.x, p2.x), maxy = max(p1.y, p2.y);
        p1.x = minx, p1.y = miny, p2.x = maxx, p2.y = maxy;
        dnode y;
        y.val = new point_t(p1.x, p1.y - 1e-8);
		int pos1 = lower_bound(root->D.begin(), root->D.end(), y) - root->D.begin();
        if (pos1 == (int)root->D.size())
			return;
        y.val->y = p2.y + 1e-8;
		int pos2 = upper_bound(root->D.begin(), root->D.end(), y) - root->D.begin();
        if (pos2 == (int)root->D.size())
			pos2--;
		x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
        Tquery(root, 1, Graph::n1, pos1, pos2);
        vec.clear();
        for (int i = 0; i < (int)vis.size(); i++)
            vec.push_back(vis[i]->id);
	}
}

namespace SEGMENTTREE1
{
struct dnode
{
    point_t *val;
    bool operator <(const dnode b) const
    {
        return val->y < b.val->y;
    }
};
struct snode
{
    snode *l, *r;
    vector<dnode> D;
};
snode *root;
vector<point_t *> vis;
void pushUP(snode *rt)
{
    if (rt->l == NULL || rt->r == NULL)
    {
        if (rt->l != NULL)
        {
            rt->D = rt->l->D;
            return;
        }
        if (rt->r != NULL)
        {
            rt->D = rt->r->D;
            return;
        }
        return;
    }
    int sl = rt->l->D.size(), sr = rt->r->D.size();
    int posl = 0, posr = 0;
    while (posl < sl && posr < sr)
    {
        double yl = rt->l->D[posl].val->y, yr = rt->r->D[posr].val->y;
        if (yl <= yr)
        {
            dnode td;
            td.val = rt->l->D[posl].val;
            posl++;
            rt->D.push_back(td);
        }
        else
        {
            dnode td;
            td.val = rt->r->D[posr].val;
            posr++;
            rt->D.push_back(td);
        }
    }
    if (posl < sl)
    {
        for (; posl < sl; posl++)
        {
            dnode td;
            td.val = rt->l->D[posl].val;
            rt->D.push_back(td);
        }
    }
    if (posr < sr)
    {
        for (; posr < sr; posr++)
        {
            dnode td;
            td.val = rt->r->D[posr].val;
            rt->D.push_back(td);
        }
    }
}
snode *newnode()
{
    snode *p = new snode;
    p->l = p->r = NULL;
    p->D.clear();
    return p;
}
snode *build(int l, int r)
{
    snode *rt = newnode();
    if (l == r)
    {
        dnode d;
        d.val = &Graph::P1[l];
        rt->D.push_back(d);
        return rt;
    }
    int mid = (l + r) >> 1;
    rt->l = build(l, mid);
    rt->r = build(mid + 1, r);
    pushUP(rt);
    return rt;
}
void init()
{
    sort(Graph::P1 + 1, Graph::P1 + Graph::n1 + 1, cmpx);
    root = build(1, Graph::n1);
}
double x1, x2, y1, y2;
void Tquery(snode *rt, int l, int r)
{
    if (rt == NULL)
        return;
    if (Graph::P1[l].x >= x1 && Graph::P1[r].x <= x2)
    {
        point_t p;
        p.y = y1;
        dnode d; d.val = &p;
        int pos1 = lower_bound(rt->D.begin(), rt->D.end(), d) - rt->D.begin();
        p.y = y2;
        int pos2 = upper_bound(rt->D.begin(), rt->D.end(), d) - rt->D.begin();
        if (pos1 == rt->D.size())
            return;
        if (pos2 == rt->D.size() || rt->D[pos2].val->y > y2)
            pos2--;
        for (int i = pos1; i <= pos2; i++)
            vis.push_back(rt->D[i].val);
        return;
    }
    int mid = (l + r) >> 1;
    if (x1 <= Graph::P1[mid].x && rt->l != NULL)
    {
        Tquery(rt->l, l, mid);
    }
    if (x2 >= Graph::P1[mid + 1].x && rt->r != NULL)
    {
        Tquery(rt->r, mid + 1, r);
    }
}
vector<int> vec;
void query(point_t p1, point_t p2)
{
    vis.clear();
    double minx = min(p1.x, p2.x), miny = min(p1.y, p2.y);
    double maxx = max(p1.x, p2.x), maxy = max(p1.y, p2.y);
    p1.x = minx, p1.y = miny, p2.x = maxx, p2.y = maxy;
    x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
    Tquery(root, 1, Graph::n1);
    vec.clear();
    for (int i = 0; i < (int)vis.size(); i++)
        vec.push_back(vis[i]->id);
}
}
