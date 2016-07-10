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
#include <cassert>
#include <ctime>

using namespace std;

void printDuration(time_t start, time_t stop)
{
    double diff = difftime(stop, start);
    int hrs = (int)diff/3600;
    int mins = ((int)diff/60)-(hrs*60);
    int secs = (int)diff-(hrs*3600)-(mins*60);
    printf("- Complete!\tTime Taken: %i hrs, %i mins, %i secs.\n", hrs, mins, secs);
}

int encode(const char *rawFile, const char *enFile)
{
    if (enFile == NULL || rawFile == NULL)
    {
        printf("ERROR, invalide params. line: %d\n", __LINE__);
        return -1;
    }   
    time_t start, stop;
    time(&start);
    HuffmanTree tree; // Huffman tree   
    ifstream infile(rawFile, ios::binary);  //input file
    ofstream outfile(enFile, ios::binary);  //output file
    if(!infile.good() || infile.peek() == EOF) 
    {
        printf("ERROR: Input file is corrupted or empty.""line: %d\n", __LINE__);
        return -2;
    }
     
    vector<int> counts(256, 0); //keep track of each byte's frequency
    int nextChar;   //store the character read from the input file
    int total = 0;
    int bits = 0;
    //loop raw file to caculte 'byte frequency'
    while((nextChar = infile.get()) != EOF) 
    {
      counts[nextChar]++;
      total++;
    } 
    tree.build(counts);
    //std::cout << tree;
    
    infile.clear(); //required to rewind the file, since it already reached EOF
    infile.seekg(0, ios::beg); //rewind the file to beginning
    
    //construct bit Stream
    BitInputStream  in(infile);
    BitOutputStream out(outfile);

    out.writeInt(total); // Write the total number of bytes to output file.
    // Write the entire frequency vector to the header of the output file 
    for(unsigned long i=0; i<counts.size(); i++) 
    {
        out.writeInt(counts[i]);
    }

    // Encode every byte in the input file and write to the output file.
    while((nextChar = infile.get()) != EOF) 
    {
        bits += tree.encode(nextChar, out);
    }
    out.flush();
    time(&stop);
    printf("Original size = %d, Encoded size = %d, %lf\n", total, bits >> 3, (double)(bits) / (total << 3));
    printDuration(start, stop);
    return 0;
}



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
    time_t start, stop;
    time(&start);
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
    time(&stop);
    printDuration(start, stop);
    return 0;
}
