/*
 * Copyright (C) 2014-2016  Yaochuang Ding - <ych_ding@163.com>
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

#include <cstdlib>
#include <cassert>
#include <vector>

#include<bitset>

#include <glog/logging.h>
#include "thirdparty/gtest/gtest.h"

/*! \brief add one to an integer signed and unsigned */
#define ADD_ONE(n) (-(~n)) // 2's complement code

/*! \brief fetch bit[i] of an integer, i's range assured by user. */
#define BIT(n,i) ((n & (1 << i)) >> i)

/*! \brief an array of number, all number occur twice except one, find that number. */
//int unpairNumber(const std::vector<int> &nums);

/*! \brief an array of number, all number occur twice except two, find them. */
//void unpairNumbers(const std::vector<int> &nums, int &x, int &y);

/*! \brief how to only keep right most bit */
inline unsigned int bitSeted(unsigned int n)
{
    assert(!(n & (n - 1))); //user should assure only bit set.
    int bits = sizeof(n) * 8, i;
    for (i = 0; i < bits && !(n & (1 << i)); ++i);
    return i;
}

inline unsigned int lowestBitSet(int n)
{
    int bits = sizeof(n) * 8, i;
    for (i = 0; i < bits && !(n & (1 << i)); ++i);
    return i;
}

/*! \brief test an integer, is it 4's power */
inline bool isPowerOf4(unsigned int n)
{
    if (n > 0 && !(n & (n - 1)))
    {
        for (unsigned int i = 0; i < sizeof(n) * 8; i += 2)
        {
            if ((n >> i) & 0x1) return true;
        }
    }
    return false;
}

/*! \brief return min by bit operation.
 * http://graphics.stanford.edu/~seander/bithacks.html#IntegerMinOrMax
 * -1 = ~0; b ^ a ^ b = a; a ^ a = 0; a ^ 0 = a;
 */
inline bool minByBit(int a, int b)
{
    // -1 = 0xFFFF -0 = 0x0000
    return b ^ ((a ^ b) & -(a < b));
}

    class Poly
    {
    public:
        virtual void Render() = 0;
    };
    void Poly::Render()
    {
        std::cout << "Poly" << std::endl;
    }

    class Rectangle : public Poly
    {
    public:
        virtual void Render() override
        {
            std::cout << "Rectangle" << std::endl;
        }
    };
    class Square : public Rectangle
    {
    public:
        virtual void Render() override
        {
            std::cout << "Square" << std::endl;
        }
    };

TEST(misc, arrayOperate)
{
    class A { int a;  };
    A(*arrayptr)[100] = nullptr;

    {
        printf("%u \n", sizeof(A));
        printf("%u \n", sizeof(arrayptr));
        printf("%u \n", arrayptr);
        printf("%u \n", (arrayptr+5));
	    //EXPECT_EQ(6, NumberOfStrictlyIncreasedSubarrays(a));
    }

    class Shape
    {
    public:
        void Render()
        {
            std::cout << "Shape" << std::endl;
        }
    };

    class Circle 
    {
    public:
        void Render()
        {
            std::cout << "Circle" << std::endl;
        }
    };

    {

        Shape *sp = reinterpret_cast<Shape*>(new Circle);
        sp->Render(); // should output Shape
    }


    {
        // reinterpret_cast<> is Not good idea
        Rectangle *sp = reinterpret_cast<Rectangle*>(new Square);
        sp->Render(); // should output Square 
    }
    {
        int x = 0; 
        int y = 0;
        if (x++ || y++)
        {
            y += 2;
        }
        std::cout << x + y << std::endl;
    }
}

struct Node
{
    int dat;
    Node *next;
};

Node* reverseList(Node* head)
{
    if (!head || !head->next) 
    {
        return head;
    }
    Node *newHead = nullptr;
    while (head)
    {
        Node *cur = head;
        head = head->next;
        cur->next = newHead;
        newHead = cur;
    }
    return newHead;
}

#include <vector>
#include <cassert>
void printMatrix(int n)
{
    assert(n > 0);
    n = sqrt(n + 1);
    std::vector<std::vector<int> > a(n, std::vector<int>(n, 0));
    int top = 0, down = n - 1, left = 0, right = n - 1, v = 0;

    //build the matrix, then output the matrix
    while (true)
    {
        //top 
        for (int col = left; col <= right; ++col) a[top][col] = v++;
        if (++top > down) break;
        //right
        for (int row = top; row <= down; ++row) a[row][right] = v++;
        if (--right < left) break;
        //down
        for (int col = right; col >= left; --col) a[down][col] = v++;
        if (--down < top) break;
        //left
        for (int row = down; row >= top; --row) a[row][left] = v++;
        if (++left > right) break;
    }
    assert(v == n * n);
    // print matrix.
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            printf("%3i ", a[i][j]);
        printf("\n");
    }
}

TEST(misc, printMatirx)
{
   // printMatrix(24);
}

TEST(misc, castOperator)
{
    char *pb = new char[100];
    //unsigned int pp = (unsigned int) pb;
    unsigned int pp = reinterpret_cast<unsigned int>(pb);
    //unsigned int pp = static_cast<unsigned int>(pb); // cannot compile

}

//
//< There are circles in matrix.
//< 1. precompute the first value of the circle and store it
//< 2. split matrix into two parts:
//<     up triangle(value increase from first value of current cirlce)
//<     down triangle(value decrease from first value of next cirlce)
//<     for elements in last circle, it needs a dummy next circle 
//< 3. identify the circle index of current element by formula. 
//<    then caculate the corresponding value.
//<
//< Complexity Analysis
//< Time Complexity is O(n)
//< Memory Complexity is O(sqrt(n))
//<
#include <algorithm>
void printMatrix_2(int n)
{
    assert(n > 0);
    n = sqrt(n + 1);

    int nCircles = n / 2 + n % 2;
    std::vector<int> a(nCircles, 0); // extra memory required
    for (int i = 1; i < a.size(); ++i)
    {
        a[i] = a[i-1] + 4 * (n -1 - 2 * (i-1));
    }
    a.push_back(n * n); // dummy value for latst circle

    // print matrix.
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            // circle index
            int c = std::min(std::min(i, j), std::min(n - i - 1, n - j - 1));
            int v = 0;
            
            if (i <= j) // up triagle: increase
            {
                v = a[c] + (i - c) + (j - c);
            }
            else // down triagle: decrease
            {
                v = a[c + 1] - (i - c) - (j - c);
            }
            printf("%4i ", v);
        }
        printf("\n");
    }
}

TEST(misc, printMatirx_2)
{
    //printMatrix_2(24);

    //printf("\n");
    //
    //printMatrix_2(15);

    //printf("\n");
    //
    //printMatrix_2(35);
}

//
//< There are circles in matrix.
//< 1. derive the formula { 4c(n-c) } to caculate first value of the circle 
//< 2. split matrix into two parts:
//<     up triangle(value increase from first value of current cirlce)
//<     down triangle(value decrease from first value of next cirlce)
//< 3. identify the circle index of current element by formula. 
//<    then caculate the corresponding value.
//<
//< Complexity Analysis
//< Time Complexity is O(n)
//< Memory Complexity is O(1)
//<
#include <algorithm>
void printMatrix_3(int n)
{
    assert(n > 0);
    n = sqrt(n + 1);

    // print matrix.
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            // circle index
            int c = std::min(std::min(i, j), std::min(n - i - 1, n - j - 1));
            int v = 0;
            
            if (i <= j) // up triagle: increase
            {
                v = 4 * c * (n - c) + (i - c) + (j - c);
            }
            else // down triagle: decrease
            {
                v = 4 * (c+1) * (n - c - 1) - (i - c) - (j - c);
            }
            printf("%4i ", v);
        }
        printf("\n");
    }
}

TEST(misc, printMatirx_3)
{
    printf("\n");
    printMatrix_3(15);

    printf("\n");
    printMatrix_3(24);

    printf("\n");
    printMatrix_3(35);
}

