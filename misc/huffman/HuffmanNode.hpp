#ifndef HCNODE_HPP
#define HCNODE_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** A class, instances of which are nodes in an HuffmanTree.
 */
class HuffmanNode 
{
  friend bool comp(HuffmanNode* one, HuffmanNode* other);

public:
  int count;
  byte symbol; // byte in the file we're keeping track of
  HuffmanNode* c0;  // pointer to '0' child
  HuffmanNode* c1;  // pointer to '1' child
  HuffmanNode* p;   // pointer to parent

  HuffmanNode(int count, byte symbol, HuffmanNode* c0 = 0, HuffmanNode* c1 = 0, HuffmanNode* p = 0)
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

/** For printing an HuffmanNode to an ostream
 *  Possibly useful for debugging.
 */
ostream& operator<<(ostream&, const HuffmanNode&) __attribute__((weak)); // shut the linker up
ostream& operator<<(ostream& stm, const HuffmanNode& n) 
{
    stm << "[" << n.count << "," << (int) (n.symbol) << "]";
    return stm;
}

bool comp(HuffmanNode* one, HuffmanNode* other);


#endif // HCNODE_HPP
