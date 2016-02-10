
/*
 * Copyright (C) 2014-2015  Yaochuang Ding - <ych_ding@163.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution, and in the same 
 *    place and form as other copyright, license and disclaimer information.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */

#include<cstdio>
#include<iostream>
#include<vector>
#include<climits>
#include<cfloat>

using namespace std;

struct Point 
{
	int x;
	int y;
};

/*************************************************
 * Function: 
 *   
 * Param[in]:  lt1, left top point of first rectangle 
 * Param[in]:  rb1, right bottom point of first rectangle 
 * Param[in]:  lt2, left top point of second rectangle 
 * Param[in]:  rb2, right bottom point of second rectangle 
 *   
 * Retrun: bool, indicating whether overlapped or not. 
 *   
 * Notice:   
 * Ideas:  two retangles do not overlapped if and 
 * only if one above another or one at left side of
 * another.
*************************************************/
bool isTwoRectangleOverlapped(Point &lt1, Point &rb1, Point &lt2, Point &rb2)
{
	if (lt1.y < rb2.y || lt2.y < rb1.y) return false;
	if (lt1.x > rb2.x || lt2.x > rb1.x) return false;
	return true;
}

double triangleCost(vector<Point> &points, int i, int k, int j)
{
	double cost;
}

/*************************************************
 * Function: 
 *   
 * Param[in]:  points, array of 2D points 
 * Param[in]:  i, starting index of point array, included 
 * Param[in]:  j, ending index of point array, included 
 *   
 * Retrun: int, min cost of triangulation 
 *   
 * Notice:   
 * Ideas: recursive   
*************************************************/
double minTriangleCut(vector<Point> &points, int i, int j)
{
	if (j < i + 2) return 0; //less than 3 points
	double ret = DBL_MAX;
	for (int k = i + 1; k < j; ++k)
	{
		ret = (ret, minTriangleCut(points, i, k) + minTriangleCut(points, k, j) + triangleCost(points, i, k, j));
	}
	return ret;	
}

int main()
{
	return 0;
}
