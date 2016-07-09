/*
 * HuffmanNode.hpp
 * HuffmanNode definition.
 *
 * Header File
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
#ifndef HCNODE_HPP
#define HCNODE_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;


/*************************************************
 *  HuffmanNode of HuffmanTree.
 *  WARNING:    operator< must be overloaded in 
 *  class  HuffmanNode.
*************************************************/
class HuffmanNode 
{
  friend bool comp(HuffmanNode* one, HuffmanNode* other);

public:
  int count;
  byte symbol;       // byte in the file we're keeping track of
  HuffmanNode* c0;  // pointer to '0' child
  HuffmanNode* c1;  // pointer to '1' child
  HuffmanNode* p;   // pointer to parent

  HuffmanNode(int count, byte symbol, HuffmanNode* c0 = NULL, HuffmanNode* c1 = NULL, HuffmanNode* p = NULL)
  : count(count)
  , symbol(symbol)
  , c0(c0)
  , c1(c1)
  , p(p) 
  { }

  /** Less-than comparison, so HuffmanNodes will work in std::priority_queue
   *  We want small counts to have high priority.
   *  And we want to break ties deterministically.
   */
  bool operator<(const HuffmanNode& other);
};

/*************************************************
 *  Overload operator<<.
 *  It it useful for debugging.
*************************************************/
ostream& operator<<(ostream&, const HuffmanNode&) __attribute__((weak)); // shut the linker up
ostream& operator<<(ostream& stm, const HuffmanNode& n) 
{
    stm << "[" << n.count << "," << (int) (n.symbol) << "] ";
    return stm;
}

bool comp(HuffmanNode* one, HuffmanNode* other);


#endif // HCNODE_HPP
