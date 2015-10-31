/*
 * compress.cpp
 * Demo how to use Huffman encode interface.
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

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

void usage(const char *app)
{
    printf("Usage: %s InputFile OutputFile\n", app);
}

int main(int argc, char** argv) 
{
  HuffmanTree tree;      // Huffman tree
  ifstream infile;  // Input file
  ofstream outfile; // Output file
  vector<int> counts(256, 0); // Keeps track of each character's frequency
  int nextChar;   // Used to store the character read from the input file    
  int total = 0;

  // If the number of command-line arguments is not 3
  if(argc != 3) 
  {
    cerr << "ERROR: Incorrect number of arguments" << endl;
    usage(argv[0]);
    exit(0);
  }
  
  infile.open(argv[1], ios::binary); 
  outfile.open(argv[2], ios::binary);  

  // If the file is good and has something
  if(!infile.good() || infile.peek() == EOF) 
  {
    cerr << "ERROR: Input file is corrupted or empty." << endl;
    exit(0);
  }

  // Loop Input file to caculte 'byte frequency'
  while((nextChar = infile.get()) != EOF) 
  {
      counts[nextChar]++;
      total++;
  }
 
  tree.build(counts);
  
  infile.clear(); // Required to rewind the file, since it already reached EOF
  infile.seekg(0, ios::beg); // Rewind the file to beginning
  
  //Construct Bit Stream
  BitInputStream  in(infile);
  BitOutputStream out(outfile);

  out.writeInt(total); // Write the total number of bytes in input file.

  // Write the entire counts vector at the header of the output file 
  for(unsigned long i=0; i<counts.size(); i++) 
  {
    out.writeInt(counts[i]);
  }

  // Encode every byte in the input file and write to the output file.
  while((nextChar = infile.get()) != EOF) 
  {
    tree.encode(nextChar, out);
  }
  out.flush();
}
