#include "dataload.h"
using namespace std;
using namespace pugi;

node Node[maxn_node];
int numNode;
unordered_map<unsigned, int> labelNode;

way Way[maxn_way];
int numWay;

building Building[maxn_building];
int numBuilding;

coastline Coastline[maxn_coastline];
int numCoastline;

island Island[maxn_island];
int numIsland;


extern pair<double, double> lonLat2Mercator(double lat, double lon);

xml_document doc;
xml_node osm;

void init()
{
    xml_parse_result result = doc.load_file("shanghai_map");
    cout << result.description() << endl;
    osm = doc.child("osm");
}

void loadNode()
{
    numNode = 0;
    for (xml_node p = osm.child("node"); p; p = p.next_sibling("node"))
    {
        Node[numNode].id = p.attribute("id").as_uint();
        double lat = p.attribute("lat").as_double();
        double lon = p.attribute("lon").as_double();
        Node[numNode].lat = lat, Node[numNode].lon = lon;
        pair<double, double> txy = lonLat2Mercator(lat, lon);
        Node[numNode].x = txy.first, Node[numNode].y = txy.second;
        labelNode[Node[numNode].id] = numNode;
        Node[numNode].nBuilding.clear();
        Node[numNode].nWay.clear();
        numNode++;
    }
    labelNode[0] = numNode, Node[numNode].id = 0, Node[numNode].x = -100, Node[numNode++].y = -100;
    labelNode[1] = numNode, Node[numNode].id = 1, Node[numNode].x = 1201, Node[numNode++].y = -1;
    labelNode[2] = numNode, Node[numNode].id = 2, Node[numNode].x = -100, Node[numNode++].y = 1201;
    labelNode[3] = numNode, Node[numNode].id = 3, Node[numNode].x = 1201, Node[numNode++].y = 1101;
}
struct color
{
    int r, g, b;
    color() {}
    color(int r, int g, int b) : r(r), g(g), b(b) {}
    int getcolor()
    {
        return b + (g << 8) + (r << 16);
    }
};
void loadBuildingAndWay()
{
    numBuilding = 0;
    numWay = 0;
    numCoastline = 0;

    for (xml_node p = osm.child("way"); p; p = p.next_sibling("way"))
    {
        unsigned id = p.attribute("id").as_uint();
        vector<unsigned> ref;
        for (xml_node tag : p.children("nd"))
            ref.push_back(tag.attribute("ref").as_uint());
        int type = -1, type1 = -1;
        color c, c1;
        int thick = 3 , layer = 0, layer1 = 0;
        char sname[100];
        sname[0] = 0;
        for (xml_node tag : p.children("tag"))
        {
            if(strcmp(tag.attribute("k").value(), "name") == 0)
            {
                strcpy(sname, tag.attribute("v").value());
            }
            if (strcmp(tag.attribute("k").value(), "layer") == 0)
            {
                layer = tag.attribute("v").as_int();
                continue;
            }
            if (strcmp(tag.attribute("k").value(), "highway") == 0)
            {
                type = 1;
                c = color(255, 255, 255);
                if (strcmp(tag.attribute("v").value(), "motorway") == 0 || strcmp(tag.attribute("v").value(), "motorway_link") == 0)
                    c = color(218, 100, 126), thick = 30;
                if (strcmp(tag.attribute("v").value(), "trunk") == 0 || strcmp(tag.attribute("v").value(), "trunk_link") == 0)
                    c = color(245, 143, 118), thick = 20;
                if (strcmp(tag.attribute("v").value(), "primary") == 0 || strcmp(tag.attribute("v").value(), "primary_link") == 0)
                    c = color(250, 196, 127), thick = 15;
                if (strcmp(tag.attribute("v").value(), "secondary") == 0 || strcmp(tag.attribute("v").value(), "secondary_link") == 0)
                    c = color(243, 253, 160), thick = 10;
                if (strcmp(tag.attribute("v").value(), "living_street") == 0)
                    c = color(194, 194, 193), thick = 8;
                if (strcmp(tag.attribute("v").value(), "pedestrian") == 0)
                    c = color(234, 234, 232);
                if (strcmp(tag.attribute("v").value(), "road") == 0)
                    c = color(212, 212, 212);
                if (strcmp(tag.attribute("v").value(), "footway") == 0 ||
                    strcmp(tag.attribute("v").value(), "bridleway") == 0 ||
                    strcmp(tag.attribute("v").value(), "steps") == 0 ||
                    strcmp(tag.attribute("v").value(), "path") == 0)
                    c = color(0, 0, 0);
                if (strcmp(tag.attribute("v").value(), "service") == 0)
                    c = color(0, 0, 1);
                continue;
            }
            if (strcmp(tag.attribute("k").value(), "cycleway") == 0)
            {
                type = 1;
                c = color(212, 212, 212);
            }
            if (strcmp(tag.attribute("k").value(), "natural") == 0)
            {
                if (strcmp(tag.attribute("v").value(), "coastline") == 0)
                {
                    type = 2;
                    continue;
                }
                type1 = 0;
                c1 = color(202, 223, 170);
                if (strcmp(tag.attribute("v").value(), "beach") == 0 ||
                    strcmp(tag.attribute("v").value(), "beach") == 0 ||
                    strcmp(tag.attribute("v").value(), "beach") == 0)
                    c1 = color(238, 232, 186);
                if (strcmp(tag.attribute("k").value(), "water") == 0)
                    c1 = color(186, 208, 209);
                if (strcmp(tag.attribute("k").value(), "wood") == 0 ||
                    strcmp(tag.attribute("k").value(), "scrub") == 0 ||
                    strcmp(tag.attribute("k").value(), "heath") == 0 ||
                    strcmp(tag.attribute("k").value(), "grassland") == 0)
                    c1 = color(189, 229, 183);
                layer1 = 0;
                continue;
            }
            if (strcmp(tag.attribute("k").value(), "building") == 0 || strcmp(tag.attribute("k").value(), "tourism") == 0)
            {
                type1 = 0;
                c1 = color(208, 198, 189);
                layer1 = 2;
                continue;
            }
            if (strcmp(tag.attribute("k").value(), "amenity") == 0)
            {
                type1 = 0;
                c1 = color(208, 198, 189);
                layer1 = 0;
                continue;
            }
            if (strcmp(tag.attribute("k").value(), "landuse") == 0)
            {
                type1 = 0;
                c1 = color(208, 198, 189);
                if (strcmp(tag.attribute("v").value(), "industrial") == 0)
                    c1 = color(235, 196, 197);
                if (strcmp(tag.attribute("v").value(), "farmland") == 0)
                    c1 = color(233, 214, 289);
                if (strcmp(tag.attribute("v").value(), "railway") == 0)
                    c1 = color(224, 199, 222);
                if (strcmp(tag.attribute("v").value(), "residential") == 0)
                    c1 = color(209, 209, 209);
                if (strcmp(tag.attribute("v").value(), "reservoir") == 0)
                    c1 = color(166, 199, 199);
                if (strcmp(tag.attribute("v").value(), "grass") == 0)
                    c1 = color(198, 237, 149);
                if (strcmp(tag.attribute("v").value(), "basin") == 0)
                    c1 = color(186, 208, 209);
                layer1 = 1;
                continue;
            }
            if (strcmp(tag.attribute("k").value(), "leisure") == 0)
            {
                type1 = 0;
                c1 = color(202, 223, 170);
                layer1 = 2;
                continue;
            }

            if (strcmp(tag.attribute("k").value(), "waterway") == 0)
            {
                if (strcmp(tag.attribute("v").value(), "riverbank") == 0)
                {
                    c1 = color(186, 208, 209);
                    layer1 = 0;
                    type1 = 0;
                }
                else
                {
                    c = color(186, 208, 209);
                    layer = 0;
                    if (strcmp(tag.attribute("v").value(), "river") == 0)
                        type = 1, thick = 11;
                }
                continue;
            }
            if (strcmp(tag.attribute("k").value(), "aeroway") == 0)
            {
                if (strcmp(tag.attribute("v").value(), "terminal") == 0)
                {
                    type1 = 0;
                    c1 = color(208, 198, 189);
                    layer1 = 2;
                    continue;
                }
                if (strcmp(tag.attribute("v").value(), "runway") == 0)
                {
                    type1 = 0;
                    c1 = color(193, 202, 208);
                    layer1 = 2;
                    continue;
                }
                type = 1;
                c = color(0, 0, 0);
            }
            /*
            if (strcmp(tag.attribute("k").value(), "man_made") == 0)
            {
                if (strcmp(tag.attribute("v").value(), "pier") == 0)
                {
                    if (type == -1)
                        type = 1, c = color(0, 0, 1);
                }
                else
                {
                    if (type1 == -1)
                        type1 = 0, c1 = color(208, 198, 189), layer1 = 2;
                }
            }
            */
        }
        if (type1 == 0)
        {
            if (layer1 == -1)
                continue;
            if (ref[0] != ref[ref.size() - 1])
                continue;
            Building[numBuilding].id = id, Building[numBuilding].buildingNode = ref;
            Building[numBuilding].color = c1.getcolor();
            Building[numBuilding].layer = layer1;
            strcpy(Building[numBuilding].name, sname);
            numBuilding++;
        }
        if (type == 1)
        {
            Way[numWay].id = id;
            Way[numWay].wayNode = ref;
            Way[numWay].color = c.getcolor();
            Way[numWay].thickness = thick;
            Way[numWay].layer = layer;
            strcpy(Way[numWay].name, sname);
            numWay++;
        }
        if (type == 2)
        {
            Coastline[numCoastline].id = id, Coastline[numCoastline].coastlineNode = ref;
            numCoastline++;
        }
    }
    sort(Way, Way + numWay);
    sort(Building, Building + numBuilding);
    for (int i = 0; i < numWay; i++)
    {
        for (auto pid : Way[i].wayNode)
        {
            int pos = labelNode[pid];
            Node[pos].nWay.push_back(i);
        }
    }
    for (int i = 0; i < numBuilding; i++)
    {
        if (Building[i].layer < 0)
            continue;
        for (auto pid : Building[i].buildingNode)
        {
            int pos = labelNode[pid];
            Node[pos].nBuilding.push_back(i);
        }
    }
    //cout << numCoastline << endl;
}
void getIsland()
{
    numIsland = 0;
    unordered_map<unsigned, int> M1;
    for (int i = 0; i < numCoastline; i++)
        M1[Coastline[i].coastlineNode[0]] = i;
    //cout << M1.size() << endl;
    while (!M1.empty())
    {
        unordered_map<unsigned, int>::iterator it = M1.begin();
        unordered_map<unsigned, int>::iterator it1 = it;
        it1++;
        for (; it1 != M1.end(); it1++)
        {
            int pid = it1->second;
            if (Coastline[pid].coastlineNode[Coastline[pid].coastlineNode.size() - 1] == it->first)
            {
                break;
            }
        }
        if (it1 == M1.end())
        {

            it1 = M1.begin();
            it1++;
            int pid = it->second;
            for (; it1 != M1.end(); it1++)
            {
                if (Coastline[pid].coastlineNode[Coastline[pid].coastlineNode.size() - 1] == it1->first)
                    break;
            }
            if (it1 == M1.end())
            {
                Island[numIsland] = Coastline[it->second].coastlineNode;
                ++numIsland;
                M1.erase(it);
            }
            else
            {
                for (int i = 1; i < (int)Coastline[it1->second].coastlineNode.size(); i++)
                    Coastline[it->second].coastlineNode.push_back(Coastline[it1->second].coastlineNode[i]);
                M1.erase(it1);
            }
        }
        else
        {
            for (int i = 1; i < (int)Coastline[it->second].coastlineNode.size(); i++)
                Coastline[it1->second].coastlineNode.push_back(Coastline[it->second].coastlineNode[i]);
            M1.erase(it);
        }
    }
    Island[0].push_back(0);
    Island[0].push_back(2);
    Island[0].push_back(Island[0][0]);
    //cout << numIsland << endl;
}
void getcenter()
{
    for (int id = 0; id < numBuilding; id++)
    {
        for (int i = 0; i < (int)Building[id].buildingNode.size(); i++)
        {
            int pid = labelNode[Building[id].buildingNode[i]];
            Building[id].cx += Node[pid].x, Building[id].cy += Node[pid].y;
        }
        Building[id].cx /= Building[id].buildingNode.size(), Building[id].cy /= Building[id].buildingNode.size();
    }
}

void load_data()
{
    init();
    loadNode();
    loadBuildingAndWay();
    getIsland();
    getcenter();
    cout << numNode << ' ' << numBuilding << ' ' << numWay << endl;
}
