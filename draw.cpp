#include "draw.h"

using namespace cv;
using namespace std;

#define MaxX 1000
#define MaxY 800

int baseThickness;
double Zoom;
double midx, midy, shiftx, shifty;

void MyLine( Mat img, Point2d start, Point2d end, int thickness, uint32_t color)
{
    if (start.x < 0 || start.x > MaxX || start.y < 0 || start.y > MaxY)
        if (end.x < 0 || end.x > MaxX || end.y < 0 || end.y > MaxY)
            return;
    int a = color & 0xff;
    int b = (color >> 8) & 0xff;
    int c = (color >> 16) & 0xff;
    thickness = min(thickness, 255);
    line( img, start, end, Scalar( a, b, c ), thickness, CV_AA);
}

void MyPolygon(Mat img, Point p[], int n, int color)
{
    int a = color & 0xff;
    int b = (color >> 8) & 0xff;
    int c = (color >> 16) & 0xff;
    const Point* ppt[1] = { &p[0] };
    int npt[1] = { n };
    fillPoly( img, ppt, npt, 1, Scalar( a, b, c ) );
}

Mat mymap, showmap;

void drawInit()
{
    mymap = Mat( MaxY, MaxX, CV_8UC3, Scalar(254, 226, 206));
}

inline void changeXY(int id)
{
    Node[id].curx = Node[id].x * Zoom + shiftx;
    Node[id].cury = Node[id].y * Zoom + shifty;
}

inline Point2d getPoint(int id)
{
    return Point2d(Node[id].curx, Node[id].cury);
}

bool checkWay(int id)
{
    if (Zoom < 7 && Way[id].thickness < 6)
        return 0;
    for (unsigned j : (Way[id].wayNode))
    {
        node *i = &Node[labelNode[j]];
        if (i->curx >= 0 && i->curx <= MaxX && i->cury >= 0 && i->cury <= MaxY)
            return 1;
    }
    return 0;
}

bool checkBuilding(int id)
{
    for (unsigned j : (Building[id].buildingNode))
    {
        node *i = &Node[labelNode[j]];
        if (i->curx >= 0 && i->curx <= MaxX && i->cury >= 0 && i->cury <= MaxY)
            return 1;
    }
    return 0;
}

/*
unordered_set<unsigned> Building2draw, Way2draw;
void drawWay_RTree()
{
    vector<int> tmp;
    for (auto i : Way2draw)
        tmp.push_back(i);
    sort(tmp.begin(), tmp.end());
    for (auto i : tmp)
    {
        int thickness = min(Way[i].thickness, (int)(Way[i].thickness * Zoom / 10)) + (int)(Zoom / 5) + 1;
        for (int j = 0; j < (int)Way[i].wayNode.size() - 1; j++)
        {
            int p1 = labelNode[Way[i].wayNode[j]], p2 = labelNode[Way[i].wayNode[j + 1]];
            if (Way[i].color == 0)
            {
                continue;
            }
            if (Way[i].color == 1)
            {
                continue;
            }
            MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0);
        }
    }
    for (auto i : tmp)
    {
        int thickness = min(Way[i].thickness, (int)(Way[i].thickness * Zoom / 10));
        for (int j = 0; j < (int)Way[i].wayNode.size() - 1; j++)
        {
            int p1 = labelNode[Way[i].wayNode[j]], p2 = labelNode[Way[i].wayNode[j + 1]];
            if (Way[i].color == 0)
            {
                MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0xf6d1cc);
                continue;
            }
            if (Way[i].color == 1)
            {
                MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0xffffff);
                continue;
            }
            MyLine(mymap, getPoint(p1), getPoint(p2), thickness, Way[i].color);
        }
    }
}
*/

void drawWay_bruteForce()
{
    for (int i = 0; i < numWay; i++)
    {
        if (!checkWay(i))
            continue;
        int thickness = min(Way[i].thickness, (int)(Way[i].thickness * Zoom / 10)) + (int)(Zoom / 5) + 1;
        for (int j = 0; j < (int)Way[i].wayNode.size() - 1; j++)
        {
            int p1 = labelNode[Way[i].wayNode[j]], p2 = labelNode[Way[i].wayNode[j + 1]];
            if (Way[i].color == 0)
            {
                continue;
            }
            if (Way[i].color == 1)
            {
                continue;
            }
            if (Way[i].thickness == 11 || Way[i].thickness == 5)
            {
                continue;
            }
            MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0);
        }
    }
    for (int i = 0; i < numWay; i++)
    {
        if (!checkWay(i))
            continue;
        int thickness = min(Way[i].thickness, (int)(Way[i].thickness * Zoom / 10));
        for (int j = 0; j < (int)Way[i].wayNode.size() - 1; j++)
        {
            int p1 = labelNode[Way[i].wayNode[j]], p2 = labelNode[Way[i].wayNode[j + 1]];
            if (Way[i].color == 0)
            {
                MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0xf6d1cc);
                continue;
            }
            if (Way[i].color == 1)
            {
                MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0xffffff);
                continue;
            }
            MyLine(mymap, getPoint(p1), getPoint(p2), thickness, Way[i].color);
        }
    }
}

Point pt1 = Point(0, 0), pt2 = Point(0, 0), curpt1, curpt2;
point_t p1, p2;
void drawPath_bruteforce()
{
    int thickness = min(15, (int)(15 * Zoom / 10));
    for (int i = 0; i < (int)path.size() - 1; i++)
    {
        int u = path[i], v = path[i + 1];
        MyLine(mymap, getPoint(u), getPoint(v), thickness, 0xff0000);
    }
}

void drawTaxiPath_bruteforce()
{
    for (int i = 0; i < (int)TAXI::TaxiP.size() - 1; i++)
    {
        Point p1, p2;
        p1.x = TAXI::TaxiP[i].curx, p1.y = TAXI::TaxiP[i].cury;
        p2.x = TAXI::TaxiP[i + 1].curx, p2.y = TAXI::TaxiP[i + 1].cury;
        int thickness = min(20, (int)(20 * Zoom / 10));
        int color = 0x00ff00;
        if (TAXI::TaxiState[i] && TAXI::TaxiState[i + 1])
            color = 0xff0000;
        MyLine(mymap, p1, p2, thickness, color);
    }
}

void drawHighlightPath_bruteforce(int id = -1)
{
    int pid = id == -1 ? FIND::foundWay[FIND::cur] : id;
    for (int j = 0; j < (int)Way[pid].wayNode.size() - 1; j++)
    {
        int p1 = labelNode[Way[pid].wayNode[j]], p2 = labelNode[Way[pid].wayNode[j + 1]];
        int thickness = min(20, (int)(20 * Zoom / 10));
        MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0x0000ff);
    }
}

void drawHighlightBuilding_bruteforce(int id = -1)
{
    int thickness = min(15, (int)(15 * Zoom / 10));
    int pid = id == -1 ? FIND::foundBuilding[FIND::cur] : id;
    for (int j = 0; j < (int)Building[pid].buildingNode.size(); j++)
    {
        int j1 = (j == (int)Building[pid].buildingNode.size() -1 ? 0 : j + 1);
        int p1 = labelNode[Building[pid].buildingNode[j]], p2 = labelNode[Building[pid].buildingNode[j1]];
        MyLine(mymap, getPoint(p1), getPoint(p2), thickness, 0x0000ff);
    }
}

Point P[2000];

/*
void drawBuilding_RTree()
{
    vector<int> tmp;
    for (auto i : Building2draw)
        tmp.push_back(i);
    sort(tmp.begin(), tmp.end());
    for (auto i : tmp)
    {
        for (int j = 0; j < (int)Building[i].buildingNode.size(); j++)
        {
            int pid = labelNode[Building[i].buildingNode[j]];
            P[j] = getPoint(pid);
        }
        MyPolygon(mymap, P, Building[i].buildingNode.size(), Building[i].color);
    }
}
*/

void drawBuilding_bruteForce()
{
    for (int i = 0; i < numBuilding; i++)
    {
        if (!checkBuilding(i))
            continue;
        for (int j = 0; j < (int)Building[i].buildingNode.size(); j++)
        {
            int pid = labelNode[Building[i].buildingNode[j]];
            P[j] = getPoint(pid);
        }
        MyPolygon(mymap, P, Building[i].buildingNode.size(), Building[i].color);
    }
}

void drawIsland_bruteForce()
{
    for (int i = 0; i < numIsland; i++)
    {
        for (int j = 0; j < (int)Island[i].size(); j++)
        {
            int pid = labelNode[Island[i][j]];
            P[j] = getPoint(pid);
        }
        MyPolygon(mymap, P, Island[i].size(), 0xf6f4ed);
    }
}


void drawSave()
{
    imwrite("map.bmp", mymap);
}

/*
void RTreeProcessInit()
{
    double borderX1 = -shiftx / Zoom - 10, borderY1 = -shifty / Zoom - 10;
    double borderX2 = (MaxX - shiftx) / Zoom + 10, borderY2 = (MaxY - shifty) / Zoom + 10;
    filter(borderX1, borderY1, borderX2, borderY2);
    Building2draw.clear();
    Way2draw.clear();
    for (auto i : Node2draw)
    {
        int pid = i.second;
        changeXY(pid);
        for (auto j : Node[pid].nBuilding)
            Building2draw.insert(j);
        for (auto j : Node[pid].nWay)
            Way2draw.insert(j);
    }
}
*/

bool selected1 = false, selected2 = false;
void calcShift()
{
    double tmp_midx = midx * Zoom, tmp_midy = midy * Zoom;
    shiftx = MaxX / 2 - tmp_midx, shifty = MaxY / 2 - tmp_midy;
}
void nodeCoordinateChange(int type)
{
    for (int i = 0; i < numNode; i++)
        changeXY(i);
    if (type == 3)
    {
        curpt1.x = pt1.x * Zoom + shiftx, curpt1.y = pt1.y * Zoom + shifty;
        curpt2.x = pt2.x * Zoom + shiftx, curpt2.y = pt2.y * Zoom + shifty;
    }
    if (type == 4)
    {
        curpt1.x = pt1.x * Zoom + shiftx, curpt1.y = pt1.y * Zoom + shifty;
    }
    if (type == 5 || type == 8)
    {
        for (int i = 0; i < (int)TAXI::TaxiP.size(); i++)
        {
            TAXI::TaxiP[i].curx = TAXI::TaxiP[i].x * Zoom + shiftx;
            TAXI::TaxiP[i].cury = TAXI::TaxiP[i].y * Zoom + shifty;
        }
    }
}

void cvMouseCallback(int mouseEvent,int x,int y,int flags,void* param);
void cvMouseCallback1(int mouseEvent,int x,int y,int flags,void* param);
void cvMouseCallback2(int mouseEvent,int x,int y,int flags,void* param);

void showInit(int type)
{
    namedWindow("MyMap",CV_WINDOW_AUTOSIZE );
    if (type == 2)
        setMouseCallback("MyMap", cvMouseCallback);
    if (type == 0)
        setMouseCallback("MyMap", cvMouseCallback1);
    if (type == 9)
        setMouseCallback("MyMap", cvMouseCallback2);
}

void showClean()
{
    destroyWindow("MyMap");
}

void showMap()
{
    //showmap = Mat(Size(1000, 800), CV_8UC3);
    //resize(mymap, showmap, Size(1000, 800));
    //imshow("MyMap", showmap);
    imshow("MyMap", mymap);
}

void drawCircle(Point2d p, int color = 0xff0000)
{
    int a = color & 0xff;
    int b = (color >> 8) & 0xff;
    int c = (color >> 16) & 0xff;
    circle(mymap, p, 5, Scalar(a, b, c), -1, 8);
}

void drawPoint()
{
    drawCircle(curpt1, 0);
    if ((int)KDTB.kans.size() == 0)
        return;
    for (int i = 0; i < (int)KDTB.kans.size(); i++)
    {
        int pid = KDTB.kans[i];
        drawCircle(Point2d(Node[pid].curx, Node[pid].cury));
    }
}
void drawTaxiKeyPoint()
{
    printf("%d\n", TAXI::TaxiP.size());
    fflush(stdout);
    for (int i = 0; i < (int)TAXI::TaxiP.size(); i++)
    {
        drawCircle(Point2d(TAXI::TaxiP[i].curx, TAXI::TaxiP[i].cury));
    }
}

#define UseRTree 0

void draw_process(double zoom = Zoom, double x = midx, double y = midy, int type = 0)
{
    //time_t t1 = clock();
    Zoom = zoom;
    midx = x, midy = y;
    calcShift();
    drawInit();
    if (UseRTree)
    {
        /*
        RTreeProcessInit();
        drawBuilding_RTree();
        drawWay_RTree();
        */
    }
    else
    {
        nodeCoordinateChange(type);
        drawIsland_bruteForce();
        drawBuilding_bruteForce();
        drawWay_bruteForce();
        if (type == 1)
            drawPath_bruteforce();
        if (type == 3)
        {
            drawPath_bruteforce();
            drawCircle(curpt1);
            drawCircle(curpt2);
            int thickness = min(15, (int)(15 * Zoom / 10));
            MyLine(mymap, curpt2, getPoint(path[0]), thickness, 0xff0000);
            MyLine(mymap, curpt1, getPoint(path[path.size() - 1]), thickness, 0xff0000);
        }
        if (type == 4)
        {
            drawPoint();
        }
        if (type == 5)
        {
            drawTaxiPath_bruteforce();
        }
        if (type == 6)
        {
            drawHighlightPath_bruteforce();
        }
        if (type == 7)
        {
            drawHighlightBuilding_bruteforce();
        }
        if (type == 8)
        {
            drawTaxiKeyPoint();
        }
    }
}

void cvMouseCallback(int mouseEvent,int x,int y,int flags,void* para)
{
    switch(mouseEvent)
    {
    case CV_EVENT_LBUTTONDOWN:
        if (!selected1)
        {
            pt1 = Point(x,y);
            drawCircle(pt1);
            pt1.x = (pt1.x - shiftx) / Zoom, pt1.y = (pt1.y - shifty) / Zoom;
            p1.x = pt1.x, p1.y = pt1.y;
            selected1 = 1;
            showMap();
        }
        else
            if (!selected2)
            {
                pt2 = Point(x, y);
                drawCircle(pt2);
                pt2.x = (pt2.x - shiftx) / Zoom, pt2.y = (pt2.y - shifty) / Zoom;
                p2.x = pt2.x, p2.y = pt2.y;
                selected2 = 1;
                showMap();
            }
        break;
    case CV_EVENT_RBUTTONDOWN:
        selected1 = selected2 = 0;
        draw_process();
        showMap();
        break;
    }
    return;
}

void cvMouseCallback1(int mouseEvent,int x,int y,int flags,void* para)
{
    if (mouseEvent == CV_EVENT_LBUTTONDOWN)
    {
        point_t p = point_t(x, y);
        p.x = (p.x - shiftx) / Zoom, p.y = (p.y - shifty) / Zoom;
        KDTB.query(p, 20);
        KDTB.getPointData(2);
        draw_process();
        if (KDTB.selectedB != -1)
            drawHighlightBuilding_bruteforce(KDTB.selectedB);
        else
            drawHighlightPath_bruteforce(KDTB.selectedW);
        showMap();
    }
    return;
}

void cvMouseCallback2(int mouseEvent,int x,int y,int flags,void* para)
{
    if (mouseEvent == CV_EVENT_LBUTTONDOWN)
    {
        p1 = point_t(x, y);
        draw_process();
        drawCircle(Point(x, y));
        showMap();
        selected1 = 1;
        p1.x = (p1.x - shiftx) / Zoom, p1.y = (p1.y - shifty) / Zoom;
        /*
        KDTT.query(p1, 50);
        int pid = KDTT.kans[0];
        double dis = sqrt((TAXI::TaxiP[pid].x - p1.x) * (TAXI::TaxiP[pid].x - p1.x) + (TAXI::TaxiP[pid].y - p1.y) * (TAXI::TaxiP[pid].y - p1.y));
        */
        KDTT.query(point_t(p1.x - 5, p1.y - 5), point_t(p1.x + 5, p1.y + 5));
        int dis = KDTT.vec.size();
        MS.setValue(dis);
    }
    if (mouseEvent == CV_EVENT_RBUTTONDOWN)
    {
        point_t pp = TAXI::meanshift(p1);
        /*
        KDTT.query(pp, 50);
        int pid = KDTT.kans[0];
        double dis = sqrt((TAXI::TaxiP[pid].x - pp.x) * (TAXI::TaxiP[pid].x - pp.x) + (TAXI::TaxiP[pid].y - pp.y) * (TAXI::TaxiP[pid].y - pp.y));
        */
        KDTT.query(point_t(pp.x - 5, pp.y - 5), point_t(pp.x + 5, pp.y + 5));
        int dis = KDTT.vec.size();
        MS.setValue(dis);
        draw_process();
        p1.x = p1.x * Zoom + shiftx, p1.y = p1.y * Zoom + shifty;
        pp.x = pp.x * Zoom + shiftx, pp.y = pp.y * Zoom + shifty;
        drawCircle(Point2d(p1.x, p1.y));
        drawCircle(Point2d(pp.x, pp.y));
        showMap();
    }
    return;
}

void draw_map(int type)
{
    double scale, x, y;
    if (type == 4)
    {
        scale = Zoom, x = p1.x, y = p1.y;
    }
    if (type == 0)
    {
        scale = 1, x = 600, y = 550;
    }
    if (type == 2)
    {
        selected1 = selected2 = 0;
        if (Zoom == 0)
            scale = 1, x = 600, y = 550;
        else
        {
            scale = Zoom, x = midx , y = midy;
        }
    }
    if (type == 1)
    {
        if (path.size() == 0)
            type = 0, scale = 1, x = 600, y = 550;
        else
            scale = 1, x = Node[path[0]].x, y = Node[path[0]].y;
    }
    if (type == 3)
    {
        if (path.size() == 0)
            type = 0, scale = Zoom, x = midx, y = midy;
        else
            scale = Zoom, x = Node[path[0]].x, y = Node[path[0]].y;
    }
    if (type == 5)
    {
        scale = 1, x = TAXI::TaxiP[0].x, y = TAXI::TaxiP[0].y;
    }
    if (type == 6)
    {
        int pid = labelNode[Way[FIND::foundWay[FIND::cur]].wayNode[0]];
        scale = 5, x = Node[pid].x, y = Node[pid].y;
    }
    if (type == 7)
    {
        int pid = FIND::foundBuilding[FIND::cur];
        scale = 5, x = Building[pid].cx, y = Building[pid].cy;
    }
    if (type == 8)
    {
        scale = 1, x = 600, y = 550;
    }
    if (type == 9)
    {
        if (Zoom == 0)
            scale = 1, x = 600, y = 550;
        else
            scale = Zoom, x = midx, y = midy;
    }
    showInit(type);
    draw_process(scale, x, y, type);
    showMap();
    char c;
    do
    {
        c = waitKey(0);
        if (c == 'q')
            break;
        if (c == '+')
        {
            scale += 4;
            draw_process(scale, x, y, type);
            showMap();
            continue;
        }
        if (c == '-')
        {
            scale -= 4;
            scale = max(scale, 1.0);
            draw_process(scale, x, y, type);
            showMap();
            continue;
        }
        if (c == 'w' || c == 0)
        {
            y -= 10;
            y = max(y, -100.0);
            draw_process(scale, x, y, type);
            showMap();
        }
        if (c == 's' || c == 1)
        {
            y += 10;
            y = min(y, (double)MaxY + 1000);
            draw_process(scale, x, y, type);
            showMap();
        }
        if (c == 'a' || c == 2)
        {
            x -= 10;
            x = max(x, -100.0);
            draw_process(scale, x, y, type);
            showMap();
        }
        if (c == 'd' || c == 3)
        {
            x += 10;
            x = min(x, (double)MaxX + 1000);
            draw_process(scale, x, y, type);
            showMap();
        }
        //cout << x << ' ' << y << ' ' << scale << endl;
    }while (1);
    showClean();
}
void draw_reset()
{
    Zoom = 0;
}
