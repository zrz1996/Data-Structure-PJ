#ifndef NODEFILTER_H
#define NODEFILTER_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <iostream>
#include "dataload.h"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double, 2, bg::cs::cartesian> bpoint;
typedef bg::model::box<bpoint> box;
typedef std::pair<bpoint, unsigned> value;

extern std::vector<value> Node2draw;

extern bgi::rtree< value, bgi::quadratic<16> > rtree;

void filterInit();
void filter(double x1, double y1, double x2, double y2);

#endif // NODEFILTER_H

