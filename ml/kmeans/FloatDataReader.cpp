/*
 * FloatDataReader.cpp
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

#include "FloatDataReader.h"

bool FloatDataReader::operator()(const std::string& digitStr, float& value)const 
{
	float digit = 0.0;
	bool minus = false;
    size_t dotPos = digitStr.find_first_of('.', 0);
    if(dotPos == std::string::npos) 
    {
		size_t i = 0;
		if(digitStr[i] == '-') {
			minus = true;
			++i;
		} else if(digitStr[i] == '+')
			++i;
		
        while(i < digitStr.length())
            digit += digit*10 + digitStr[i++] - '0';
    } 
    else 
    {
        if(dotPos == digitStr.length()) return false;

        std::string integralStr = digitStr.substr(0, dotPos);
        std::string decimalStr  = digitStr.substr(dotPos+1, digitStr.length()-(dotPos+1));

        int integral = 0.0;
		size_t i = 0;
		if(integralStr[i] == '-') {
			minus = true;
			++i;
		} else if(integralStr[i] == '+')
			++i;
	
        while(i < integralStr.length())
            integral = integral*10 + integralStr[i++] - '0';

        float decimal = 0.0;
        int denominator = 10;
        for(size_t j = 0; j < decimalStr.length(); ++j) {
            decimal = decimal + static_cast<float>(decimalStr[j] - '0') / denominator;
            denominator *= 10;
        }

        digit = integral + decimal;
    }
    value = (minus == true) ? -digit : digit;

    return true;
}
