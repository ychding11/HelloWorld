/*
 * uncompress.cpp
 * demostrate how to use huffman decode.
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
#include "HuffmanTree.hpp"
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

/*************************************************
 *  demo how to decode.
*************************************************/
int decode(const char* enFile, const char *deFile)
{
    if (enFile == NULL || deFile == NULL)
    {
        printf("ERROR, invalide params. line: %d\n", __LINE__);
        return -1;
    }
    HuffmanTree tree;             // Huffman tree
    vector<int> counts(256, 0);   // Keeps track of each character's frequency.
    ifstream infile(enFile, ios::binary); //how to check?     
    ofstream outfile(deFile, ios::binary);
    BitInputStream in(infile);
    BitOutputStream out(outfile);
    
    //read file header of the encoded && rebuild the Huffman Tree.
    int total = in.readInt();
    for(unsigned long i = 0; i < counts.size(); i++) 
    {
        counts[i] = in.readInt();
    } 
    tree.build(counts);

    //loop encoded file to decode and write the result to outfile.
    for(int i = 0; i < total; i++) 
    {
        out.writeByte(tree.decode(in));
    }
    return 0;
}

/*************************************************
 *  tester
*************************************************/
int main(int argc, char** argv) 
{
  if(argc != 3) 
  {
    std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
    return 0;
  }
  decode(argv[1], argv[2]);
  return 0;
}

