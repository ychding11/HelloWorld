/*
 * HuffmanTree.hpp
 * HuffmanTree definition.
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
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <map>
#include <stack>
#include <fstream>
#include "HuffmanNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

/*************************************************
 *  A 'Function class' used as the Compare class for
 *  priority_queue<HuffmanNode*>
 *  WARNING:    operator< must be overloaded in 
 *  class  HuffmanNode.
*************************************************/
class HuffmanNodePtrComp 
{
  public:
    bool operator()(HuffmanNode*& lhs, HuffmanNode*& rhs) const 
    {
      return *lhs < *rhs;
    }
};

/*************************************************
 *  A Huffman Code Tree class.
 *  used for Huffman encode and decode.
*************************************************/
class HuffmanTree 
{
  private:
    HuffmanNode* mpRoot;
    vector<HuffmanNode*> mLeaves;
    map<byte, stack<int> > mDictionary; 

    /*
     * This function is used to post-order traverse the tree and
     * delete every node in the tree from memory.
     */
    void deleteAllNodes(HuffmanNode * node) 
    {
      // If the tree is empty already, then return
      if(node == nullptr) 
      {
        return;
      }
      if(node->c0 != nullptr) 
      {
        deleteAllNodes(node->c0);
      }
      if(node->c1 != nullptr) 
      {
        deleteAllNodes(node->c1);
      }
      delete node;
    }

  public:
    explicit HuffmanTree() 
            : mpRoot(0)
            , mLeaves(256, (HuffmanNode*) 0) 
            , mDictionary()
    {
      //mLeaves = vector<HuffmanNode*>(256, (HuffmanNode*) 0);
    }

    ~HuffmanTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  mpRoot points to the mpRoot of the trie,
     *  and mLeaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize mpRoot pointer and mLeaves vector.
     */
    int encode(byte symbol, BitOutputStream& out) const;

    /** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize mpRoot pointer and mLeaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
     *  BE USED IN THE FINAL SUBMISSION.
     */
    void encode(byte symbol, ofstream& out) const;


    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize mpRoot pointer and mLeaves vector.
     */
    int decode(BitInputStream& in) const;

    /** Return the symbol coded in the next sequence of bits (represented as 
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize mpRoot pointer and mLeaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
    int decode(ifstream& in) const;
    
    friend ostream& operator<<(ostream &os, HuffmanTree &root);

private:
    /*! \brief convert huffman tree into code dictionary */
    map<byte, stack<int> >  generateDictionary(void);
};

#endif // HCTREE_HPP
