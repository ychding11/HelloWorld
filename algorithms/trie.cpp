/*
 * trie.cpp
 * an implementation of prefix tree.
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
 
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include "Logger.h"
//using namespace std;

/*************************************************
 *  TrieNode.  
 *  NOTE: only LOWCASE is supported.
*************************************************/
class TrieNode 
{
public:
    TrieNode* table[26]; //letter is case insenstive.
    bool isword;
    /*************************************************
     *  default Constructor.
     *  TODO: try catch throw.
    *************************************************/
    TrieNode() : isword(false)
    {
        for (int i = 0; i < 26; i++)
        {
            table[i] = NULL;
        }
    }    
};

class Trie 
{
private:
    TrieNode* root;
    
public:
    
    /*************************************************
     *  default Constructor.
     *  TODO: try catch throw.
    *************************************************/
    Trie() 
    {
        root = new TrieNode();
    }
        
    /*************************************************
     *  Inserts a word into the trie. 
     *  NOTE: only lowcase letter is supported.
    *************************************************/
    bool insert(const std::string& word) 
    {
        int n = word.length();
        TrieNode *cur = root;
        for (int i = 0; i < n; i++)
        {
            char ch = word[i];
            int index = ch-'a';
            if (index > 25 || index < 0)
            {
                printf("Error, invalid character[%d]. line: %d\n", ch, __LINE__);
                return false;
            }
            if (NULL == cur->table[index])
            {
                cur->table[index] = new TrieNode();
            }
            cur = cur->table[index];
        }
        cur->isword = true; //marking ending here is a word.
        return true;
    }
    
    /*************************************************
     *  Search word in trie.
     *  Return true if the word is in the trie.
     *  TODO: .
    *************************************************/
    bool search(std::string word) 
    {
        int n = word.length();
        TrieNode *cur = root;
        for (int i = 0; i < n; i++)
        {
            char ch = word[i];
            if (NULL == cur->table[ch-'a'])
            {
               return false;
            }
            cur = cur->table[ch-'a'];
        }
        return cur->isword;
    }
    
    /*************************************************
     *  Search if there are any words in the trie starting
     *  with prefix, if so return true.
    *************************************************/
    bool startsWith(std::string prefix)
    {
        int n = prefix.length();
        TrieNode *cur = root;
        for (int i = 0; i < n; i++)
        {
            char ch = prefix[i];
            if (NULL == cur->table[ch-'a'])
            {
               return false;
            }
            cur = cur->table[ch-'a'];
        }      
        return true; //don't care cur is word or not.
    }

};

// Your Trie object will be instantiated and called as such:
// Trie trie;
// trie.insert("somestring");
// trie.search("key");

void trieTester(int n = 10)
{
    assert(n > 0);
    Trie trie;
	char buf[128];
	std::vector<std::string> pool;
	int words = 0;
	while ((scanf("%s", buf)) != EOF) //read word from stdin
	{
	    ++words;
	    std::string word(buf);
	    logger << "Read <" << word << ">" << std::endl;
	    pool.push_back(word);
	    if (true != trie.insert(word))
	    {
	        printf("Test failed, insert <%s> error. \n", word.c_str());
	        return;
	    }
	}
	srand(time(NULL));
	for (int i = 0; i < n; ++i)
	{
	    int index = rand() % words;
	    std::string str = pool[index];
	    if ( true != trie.search(str)) //test search()
	    {
	        printf("Test failed. while search <%s>\n", str.c_str());
	        return;
	    }
	    str.pop_back();
	    if ( true != trie.startsWith(str)) //test startsWith()
	    {
	        printf("Test failed. while startsWith <%s>\n", str.c_str());
	        return;
	    }
	}
    printf("Algorithms correct.\n");
}

int main(int argc, char** argv)
{
  logger.setLevel(DEBUG);
  logger.setLineLevel(DEBUG);
  ENTER_FUNCTION;
  if (argc == 2)
  {
    int n = atoi(argv[1]);
    logger << "n = " << n << std::endl;
    trieTester(n);
  }
  else
  {
    trieTester();
  }
  EXIT_FUNCTION;
  return 0;
	
}
