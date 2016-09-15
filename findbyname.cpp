#include "findbyname.h"
using namespace std;
namespace FIND
{
    vector<int> foundWay, foundBuilding;
    char sname[100];
    int cur;
    void find(string s)
    {
        foundWay.clear();
        foundBuilding.clear();
        strcpy(sname, s.c_str());
        for (int i = 0; i < numWay; i++)
            if (strstr(Way[i].name, sname) != NULL)
                foundWay.push_back(i);
        for (int i = 0; i < numBuilding; i++)
            if (strstr(Building[i].name, sname) != NULL)
                foundBuilding.push_back(i);
    }
}
