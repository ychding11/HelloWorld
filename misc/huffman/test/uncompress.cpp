// uncompress.cpp

#include "HuffmanTree.hpp"
#include <iostream>
#include <fstream>

#define ARG_COUNT 3

using namespace std;

int main(int argc, char** argv) 
{
  HuffmanTree tree;                  // Huffman tree
  vector<int> counts(256, 0);   // Keeps track of each character's frequency.
  ifstream infile;              // Input file
  ofstream outfile;             // Output file

  int total = 0;

  // If the number of command-line arguments is not 3
  if(argc != ARG_COUNT) 
  {
    cerr << "ERROR: Incorrect number of arguments" << endl;
    exit(0);
  }

  infile.open(argv[1], ios::binary);
  outfile.open(argv[2], ios::binary);

  BitInputStream in(infile);
  BitOutputStream out(outfile);

  // Read the header of the file. rebuild the Huffman Tree to decode.
  total = in.readInt();
  for(unsigned long i = 0; i < counts.size(); i++) 
  {
    counts[i] = in.readInt();
  } 
  tree.build(counts);
  
  // Loop Coded file to decode and write the result to outfile.
  for(int i=0; i<total; i++) 
  {
    out.writeByte(tree.decode(in));
  }
}

