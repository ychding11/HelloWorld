#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

class TrieNode {
public:
    // Initialize your data structure here.
    TrieNode() 
    {
        for (int i = 0; i < 26; i++)
        {
            table[i] = NULL;
        }
    }
    TrieNode* table[26];
};
class Trie {
public:
    Trie() 
    {
        root = new TrieNode();
    }
    // Inserts a word into the trie.
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
    }
    // Returns if the word is in the trie.
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
        return true;
    }
    // Returns if there is any word in the trie
    // that starts with the given prefix.
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
        
        #if 0
        for (int i = 0; i < 26; i++)
        {
            if (cur->table[i]) return true;
        }
        #endif
        
        return true;
    }
private:
    TrieNode* root;
};
// Your Trie object will be instantiated and called as such:
// Trie trie;
// trie.insert("somestring");
// trie.search("key");

int main()
{
	Trie trie;
	trie.insert("ab");
	cout << trie.search("a");
	cout << trie.startsWith("a");
	cout << endl;
}
