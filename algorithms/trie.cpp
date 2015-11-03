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
#include <string>
#include <iostream>

using namespace std;

class TrieNode 
{
public:
    TrieNode* table[26]; //letter is case insenstive.
    bool isword;
/*************************************************
 *  default Constructor.
 *  
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
 *  
 *  TODO: try catch throw.
*************************************************/
Trie() 
{
    root = new TrieNode();
}
    
/*************************************************
 *  Inserts a word into the trie.
 *  
 *  TODO: .
*************************************************/
void insert(string word) 
{
    int n = word.length();
    TrieNode *cur = root;
    for (int i = 0; i < n; i++)
    {
        char ch = word[i];
        if (NULL == cur->table[ch-'a'])
        {
            cur->table[ch - 'a'] = new TrieNode();
        }
        cur = cur->table[ch-'a'];
    }
    cur->isword = true; //marking ending here is a word.
}
    
/*************************************************
 *  Search word in trie.
 *  Returns if the word is in the trie.
 *  TODO: .
*************************************************/
    bool search(string word) 
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
    
    // Returns 
    // that starts with the given prefix.
/*************************************************
 *  Search if there is any word in the trie starting
 *  with prefix.
 *  
 *  TODO: .
*************************************************/
bool startsWith(string prefix)
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

int main()
{
	Trie trie;
	char buf[128];
	while ((scanf("%s", buf)) != EOF)
	{
	    std::string word(buf);
	    trie.insert(word);
	}
	// how to analyze the memory usage after while loop?
	printf("search word, startwith Enter your cmd>");
	//scanf("%s",)
	cout << trie.search("a");
	cout << trie.startsWith("a");
	cout << endl;
}
