/*
 * Utils.cpp
 *
 * Source File
 *
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

#ifndef _UTILS_H_
#include <cmath>

float ln(float n);

/*
 * The natural logarithm in C++, that is, function 'log' defined in cmatch, is the base-e logarithm.
 * Since logN(x) = logY(x)/logY(N), we can calculate any base logarithm using the base-e logrithm.
 * For example, log2(x) = log(x)/log(2).
 */
float logN(float n, int base);

/*
 * e^z = (2^logN(e, 2))^z = 2^(z / logN(e, 2));
 */
float pow(int z);
float pow(float z);
long double pow(long double z);

float min(float x, float y);
float max(float x, float y);


float ln(float n) {
	return log(n);
}

float logN(float n, int base) {
	return log(n)/log(base);
}

float pow(int z) {
	return pow(2, z / log(2));
}

float pow(float z) {
	return pow(2, z / log(2));
}

long double pow(long double z) {
	return pow(2, z / log(2));
}

float min(float x, float y) {
	return x < y ? x : y;
}

float max(float x, float y) {
	return min(y, x);
}
#endif
