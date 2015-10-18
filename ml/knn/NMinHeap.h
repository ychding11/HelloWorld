/*
 * NMinHeap.h
 * .
 *
 * Header Header
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

#ifndef _N_MIN_HEAP_H_
#define _N_MIN_HEAP_H_

//#include <utility>

/*
 * 
 */
template<typename K, typename V>
class NMinHeap 
{
private:
	std::pair<K, V>* pairs;
	int capacity;
	int num;

public:
	typedef std::pair<K, V>* iterator;

	NMinHeap(int n);
	bool insert(const K& k, const V& v);
	// iterate the heap like an array
	int size()const;
	std::pair<K, V> operator[](int index)const;
	~NMinHeap();

private:
	inline void swap(std::pair<K, V>* pair1, std::pair<K, V>* pair2);
};


template<typename K, typename V> 
NMinHeap<K, V>::NMinHeap(int cap) : num(0), capacity(cap)
{
	pairs = new std::pair<K, V>[capacity];
}

template<typename K, typename V> NMinHeap<K, V>::~NMinHeap() 
{
	delete[] pairs;
}

template<typename K, typename V>
void NMinHeap<K, V>::swap(std::pair<K, V>* pair1, std::pair<K, V>* pair2) 
{
	std::pair<K, V> tmp = *pair1;
	*pair1 = *pair2;
	*pair2 = tmp;
}

template<typename K, typename V>
bool NMinHeap<K, V>::insert(const K& k, const V& v) 
{
	if(num == capacity) 
	{
		if(k > pairs[0].first) return false;

		// place (k, v) in pairs[0] and adjust it top-bottom
		pairs[0] = std::make_pair(k, v);
		int curr = 0;
		int child = 1; // 2*0+1
		while(child < capacity) 
		{
			if((child + 1 < capacity) && (pairs[child].first < pairs[child + 1].first))	child += 1;
			if(pairs[curr].first >= pairs[child].first) break;

			// swap pair in 'curr' and 'child'
			swap(&pairs[curr], &pairs[child]);

			curr = child;
			child = 2 * curr + 1;
		}
	}
	else 
	{
		// place (k, v) in pairs[num](the bottom of the heap), then adjust heap bottom-up
		pairs[num] = std::make_pair(k, v);
		int curr = num;
		int parent = (curr-1) / 2; // heap from index 0
		while(curr > 0) 
		{
			if(pairs[curr].first <= pairs[parent].first) break;

			// swap pair in 'curr' and 'parent'
			swap(&pairs[curr], &pairs[parent]);
			curr = parent;
			parent = (curr-1)/2;
		}
		++num;
	}
	return true;
}

template<typename K, typename V>
int NMinHeap<K, V>::size()const 
{
	return num;
}

template<typename K, typename V>
std::pair<K, V> NMinHeap<K, V>::operator[](int index)const 
{
	return pairs[index];
}
#endif
