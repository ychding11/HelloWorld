/*
 * list.cpp
 * demostrate single linked list algorithms.
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
 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
#include "Logger.h"

using namespace std;

#define PERFORMANCE_METER

/*************************************************
 *  ListNode struct.
 *
 *  NOTE: member is public.
*************************************************/
struct ListNode 
{
    int val;
    ListNode *next;
    
    ListNode(int x) 
    : val(x)
    , next(NULL)
    {}
};

/*************************************************
 *  ListAlgorithms class -- a collection of classic
 *  list algorithms.
 *
 *  NOTE: .
*************************************************/
class ListAlgorithms
{
public:
    ListNode* reverseList(ListNode* head)
    {
        if (!head || !head->next) //check param
        {
            return head;
        }
        ListNode tempHead(0), *hd = &tempHead;
        hd->next = NULL;
        while (head)
        {
            ListNode *cur = head;
            head = head->next;
            cur->next = hd->next;
            hd->next = cur;
        }
        return tempHead.next;
    }
};

/*************************************************
 *  Helper function
 *  contruct a linked list from a vector.
 *  NOTE: list order is reversed.
*************************************************/
ListNode* constructList(const vector<int> &nums)
{
    ListNode tempHead(0), *head = &tempHead;
    for (auto e : nums)
    {
        ListNode *temp = new ListNode(e);
        if (temp)
        {
            temp->next = head->next;
            head->next = temp;
        }
        else
        {
            cout << "Out of memory." << std::endl;
            return tempHead.next;
        }
    }
    return tempHead.next;
}

/*************************************************
 *  Helper function
 *  destruct a linked list from a vector.
 *  NOTE: list order is reversed.
*************************************************/
void destructList(ListNode *head)
{
    while (head)
    {
        ListNode *cur = head;
        head = head->next;
        delete cur;
    }
}

/*************************************************
 *  Tester function
 *  reverseList algorithm tester.
 *  param: n number of elements
*************************************************/
void reverseListTester(int n)
{
    assert(n >= 0);
    vector<int> nums(n, 0);
    srand(time(NULL)); //rand seed
    for (int i = 0; i < n; i++)
    {
        nums[i] =  rand() % n;
    }
    ListNode *head = constructList(nums);
    int c = n - 1;
    ListNode *tempHead = head;
    while (tempHead)
    {
        if (tempHead->val != nums[c])
        {
            cout << "Test data error." << std::endl;
            return;
        }
        tempHead = tempHead->next;
        --c;
    }
    
    ListAlgorithms la;
    ListNode *result = la.reverseList(head); 
    c = 0; tempHead = result;
    while (tempHead)
    {
        if (tempHead->val != nums[c])
        {
            cout << "Element mismatch. index = " << c << std::endl;
            return;
        }
        tempHead = tempHead->next;
        ++c;
    }
    cout << "Algorithm is correct." << std::endl;
}

/*************************************************
 * list algorithm tester.
*************************************************/
int main(int argc, char** argv)
{
  logger.setLevel(DEBUG);
  logger.setLineLevel(DEBUG);
  ENTER_FUNCTION;
  int n;
  while (true)
  {
    cout << "Enter elements number:";
    cin >> n;
    if (-1 == n)
    {
        cout << "Game Over.";
        return 0;
    }
    reverseListTester(n);
  }
  EXIT_FUNCTION;
  return 0;
}

