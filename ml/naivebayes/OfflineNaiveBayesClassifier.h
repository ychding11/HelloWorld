/*
 * OfflineNaiveBayesClassifier.h
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

#ifndef _OFFLINE_NAIVE_BAYES_CLASSIFIER_H_
#define _OFFLINE_NAIVE_BAYES_CLASSIFIER_H_


#include <string>
#include <vector>
#include <map>
#include "InputDataSet.h"
#include "Logger.h"
/*
 * We assume here the tokens in the document to classify have already been added
 * to Vocabulary. This is an offline version. The training set and test set is 
 * fixed, so the Vocabulary is fixed.
 * While the practical scenario is always online, while the 'document to classify'
 * come in real-time and in such a case.
 * The Vocabulary will be changed each time when a new document is handled.
 */
class OfflineNaiveBayesClassifier 
{
private:
	int mCategoryNum;
	int mVocabularyNum;
	int mTotalTokens;
	Logger &mlog;
	/* [document name] one vector for each category */
	std::vector<std::string>* mpDocumentNames;
	
	/* [token : tokens index] one map */
	std::map<std::string, int> mVocabulary;
	
	/* [document : tokens number] one map for each category */
    std::map<std::string, int>* mpDocumentTokens; 
    
    /* [document : tokens bitmap] one map for each category */
	std::map<std::string, std::vector<int> >* mpDocumentVectors;


public:
	OfflineNaiveBayesClassifier(InputDataSet& inputDataSet, Logger &log);
   ~OfflineNaiveBayesClassifier();
	int classify(const std::string& docToClassify);
	friend std::ostream& operator<<(std::ostream& out, OfflineNaiveBayesClassifier& kmeans);
	
};

#endif
