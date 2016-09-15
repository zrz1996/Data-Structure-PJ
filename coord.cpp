#include "coord.h"
using namespace std;
const double PI = acos(-1.0);
pair<double, double> lonLat2Mercator(double lat, double lon)
{
	double x = lon * 20037508.34 / 180;
	double y = log(tan((90 + lat) * PI / 360)) / (PI / 180);
	y = y * 20037508.34 / 180;
	x -= 13427723.263268; 
	y -= 3581423.765453;
	x -= 57742.488534;
	y -= 36036.236168; 
    x /= 70, y /= 70;
    //cout << x << ' ' << y << endl;
    //return make_pair(x, y);
    return make_pair(x, 1100 - y);
}
