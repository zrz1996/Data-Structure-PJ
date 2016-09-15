#ifndef DRAW_H
#define DRAW_H
#include <vector>
#include <iostream>
#include <map>
#include <ctime>
#include <algorithm>
#include <unordered_set>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "dataload.h"
#include "spath.h"
#include "kdtree.h"
#include "taxidata.h"
#include "findbyname.h"
#include "graph.h"
#include "mysignal.h"
void draw_map(int type);
extern point_t p1, p2;
extern cv::Point pt1, pt2;
void draw_reset();
#endif // DRAW_H

