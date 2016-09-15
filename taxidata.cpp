#include "taxidata.h"
using namespace std;
extern pair<double, double> lonLat2Mercator(double lat, double lon);
namespace TAXI
{
    vector<tnode> Taxi[32000];
    vector<pnode> TaxiP;
    point_t P[1000020];
    vector<int> TaxiID;
    vector<int> TaxiState;
    template<class T>
    string to_string(const T& t)
    {
         stringstream ss;
         ss << t;
         return ss.str();
    }
    string getFileName(int id)
    {
        string f = "./taxi/";
        f += to_string(id) + "taxi.out";
        return f;
    }
    int n;
    void getIDlist()
    {
        for (int i = 2; i <= 40000; i++)
            if (access(getFileName(i).c_str(), 0) != -1)
                TaxiID.push_back(i);
    }

    bool parser(int id)
    {
        if (Taxi[id].size() != 0)
            return 1;
        string fname = getFileName(id);
        if (access(fname.c_str(), 0) == -1)
            return 0;
        FILE *f = fopen(fname.c_str(), "r");
        char str[65536], str1[1000];
        while (fgets(str, 65535, f))
        {
            tnode cur;
            int id, year, mon, day, h, m, s;
            char warn;
            sscanf(str, "%d,%d-%d-%d %d:%d:%d,%c,%s", &id, &year, &mon, &day, &h, &m, &s, &warn, str1);
            if (mon == 3)
                day = 0;
            cur.time = day * 24 * 3600 + h * 3600 + m * 60 + s;
            sscanf(str1, "%lf,%lf,%c", &cur.lon, &cur.lat, &cur.customer);
            Taxi[id].push_back(cur);
        }
        return 1;
    }

    pair<int, int> getTime(int id)
    {
        int t1 = 0;
        for (int i = 0; i < (int)Taxi[id].size() - 1; i++)
            if (Taxi[id][i].customer == '0' && Taxi[id][i + 1].customer == '0')
                t1 += Taxi[id][i + 1].time - Taxi[id][i].time;
        int t2 = Taxi[id][Taxi[id].size() - 1].time - Taxi[id][0].time;
        return make_pair(t2, t1);
    }

    void getpath(int id)
    {
        TaxiP.clear();
        TaxiState.clear();
        for (int i = 0; i < (int)Taxi[id].size(); i++)
        {
            pnode p;
            pair<double, double> p1 = lonLat2Mercator(Taxi[id][i].lat, Taxi[id][i].lon);
            p.x = p1.first, p.y = p1.second;
            TaxiP.push_back(p);
            TaxiState.push_back(Taxi[id][i].customer == '1' ? 0 : 1);
        }
    }
    vector<keynode> Key;
    double gauss[1000000];
    void parserAll()
    {
        FILE *f = fopen("./taxi/keypoint2.in", "r");
        int num;
        fscanf(f, "%d", &num);
        TaxiP.clear();
        for (int i = 0; i < num; i++)
        {
            keynode k;
            fscanf(f, "%d %d %lf %lf %d\n", &k.id, &k.time, &k.lat, &k.lon, &k.customer);
            Key.push_back(k);
            pnode p;
            pair<double, double> p1 = lonLat2Mercator(k.lat, k.lon);
            p.x = p1.first, p.y = p1.second;
            TaxiP.push_back(p);
            P[i].x = p.x, P[i].y = p.y, P[i].id = i;
        }
        n = TaxiP.size();
        KDTT.build(P, 0, n);
        /*
        char ps = 0;
        for (int i = 0; i < TaxiID.size(); i++)
        {
            string fname = getFileName(TaxiID[i]);
            FILE *f = fopen(fname.c_str(), "r");
            char str[65536], str1[1000];
            ps = 0;
            while (fgets(str, 65535, f))
            {
                tnode cur;
                int id, year, mon, day, h, m, s;
                char warn;
                sscanf(str, "%d,%d-%d-%d %d:%d:%d,%c,%s", &id, &year, &mon, &day, &h, &m, &s, &warn, str1);
                if (mon == 3)
                    day = 0;
                cur.time = day * 24 * 3600 + h * 3600 + m * 60 + s;
                sscanf(str1, "%lf,%lf,%c", &cur.lon, &cur.lat, &cur.customer);
                if (cur.customer != ps)
                {
                    ps = cur.customer;
                    keynode k;
                    k.id = id, k.time = cur.time, k.lat = cur.lat, k.lon = cur.lon;
                    k.customer = ps == '0' ? 1 : 0;
                    Key.push_back(k);
                }
                else
                    if (cur.customer == '1')
                    {
                        keynode k;
                        k.id = id, k.time = cur.time, k.lat = cur.lat, k.lon = cur.lon;
                        k.customer = ps == '0' ? 1 : 0;
                        //Key.push_back(k);
                    }
            }
        }
        FILE *f = fopen("./taxi/keypoint1.in", "w");
        fprintf(f, "%d\n", Key.size());
        for (auto i : Key)
            fprintf(f, "%d %d %.6f %.6f %d\n", i.id, i.time, i.lat, i.lon, i.customer);
        FILE *f = fopen("./taxi/keypoint1.in", "r");
        FILE *f1 = fopen("./taxi/keypoint2.in", "w");
        fscanf(f, "%d", &n);
        printf("%d\n", n);
        fflush(stdout);
        int pid = 0, pc = -1;
        double plat = 0, plon = 0;
        for (int i = 0; i < n; i++)
        {
            double lat, lon;
            int c, t, id;
            fscanf(f, "%d%d%lf%lf%d", &id, &t, &lat, &lon, &c);
            if (pid == id && pc == c)
            {
                double diff = fabs(lat - plat) + fabs(lon - plon);
                diff *= 124000;
                if (diff < 200)
                    continue;
            }
                pid = id, plat = lat, plon = lon, pc = c;
                fprintf(f1, "%d %d %.6f %.6f %d\n", id, t, lat, lon, c);
        }
        */
    }
    point_t meanshift(point_t p)
    {
        for (int step = 0; step < 10000; step++)
        {
            point_t p1 = point_t(p.x - 5, p.y - 5);
            point_t p2 = point_t(p.x + 5, p.y + 5);
            KDTT.query(p1, p2);
            if (KDTT.vec.size() == 0)
            {
                p.x += rand() % 2 - 2, p.y += rand() % 2 - 2;
            }
            double vx = 0, vy = 0, v = 0;
            for (int i = 0; i < (int)KDTT.vec.size(); i++)
            {
                int pid = KDTT.vec[i];
                //vx += TaxiP[pid].x - p.x, vy += TaxiP[pid].y - p.y;
                double dx = (TaxiP[pid].x - p.x) / 5, dy = (TaxiP[pid].y - p.y) / 5;
                double d = dx * dx + dy * dy;
                int pos = d * 20000;
                if (gauss[pos] != 0)
                    d = gauss[pos];
                else
                {
                    d = exp(-(dx * dx + dy * dy) / 2);
                    gauss[pos] = d;
                }
                vx += TaxiP[pid].x * d, vy += TaxiP[pid].y * d;
                v += d;
            }
            vx /= v, vy /= v;
            vx = (vx - p.x) * 0.1, vy = (vy - p.y) * 0.1;
            double d = (vx - p.x) * (vx - p.x) + (vy - p.y) * (vy - p.y);
            p.x += vx, p.y += vy;
            if (d < 1e-2)
                break;
        }
        return p;
    }
}
