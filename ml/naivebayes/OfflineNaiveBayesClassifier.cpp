/*
 * OfflineNaiveBayesClassifier.cpp
 * .
 *
 * Source Header
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

#include <iostream>
#include <vector>
#include "OfflineNaiveBayesClassifier.h"
#include "FileTokenizer.h"
#include "Utils.h"

OfflineNaiveBayesClassifier::OfflineNaiveBayesClassifier(InputDataSet& inputDataSet, Logger &log)
                            : mCategoryNum(inputDataSet.getCategoryNum())
                            , mlog(log) 
{
#ifdef DEBUG
	mlog << "Constructing  OfflineNaiveBayesClassifier..." << std::endl;
#endif
	if(mCategoryNum <= 0) return;	
	int documentNum = 0;
	mpDocumentNames = new std::vector<std::string>[mCategoryNum];
	for(int category = 0; category < mCategoryNum; ++category) 
	{
		mpDocumentNames[category] = inputDataSet.getDocuments(category);
		documentNum += mpDocumentNames[category].size();
	}

	// construct mVocabulary 
	std::string dirName = inputDataSet.getDir() + "//";
	mVocabularyNum = 0;//__
#ifdef DEBUG
	mlog << "Vocabulary List " << std::endl;
	mlog << "---------------------------------" << std::endl;
#endif
	for(int i = 0; i < mCategoryNum; ++i) 
	{
		auto & cateDocuments = mpDocumentNames[i];
		for(auto p = cateDocuments.begin(); p != cateDocuments.end(); ++p) 
		{
			FileTokenizer tokenizer(dirName + *p);
			while(tokenizer.hasNext()) 
			{
				std::string token = tokenizer.nextToken();
				auto pos = mVocabulary.find(token);
				if(pos == mVocabulary.end())
				{
				#ifdef DEBUG
	                mlog << "[  " << token << ", "
	                       << mVocabularyNum << " ]" << std::endl;
                #endif
					mVocabulary[token] = mVocabularyNum++;
				}
			}
		}
	}
	
#ifdef DEBUG
	mlog << "---------------------------------"<< std::endl;
#endif
	
	/*
	 * Here we calculate vectors for all the docuemts, one vector per document.
	 * Actually if we only need to collect how many times of each token in all the mpDocumentNames, 
	 * we only need to calculate vectors based on category, one vector per category.
	 */
#ifdef DEBUG
	mlog << ">>>> Build mVocabulary map for each document."<< std::endl;
#endif
	mpDocumentVectors = new std::map<std::string, std::vector<int> >[mCategoryNum];
	mpDocumentTokens  = new std::map<std::string, int>[mCategoryNum];
	for(int i = 0; i < mCategoryNum; ++i) 
	{
		auto & cateDocuments = mpDocumentNames[i];
		for(auto p = cateDocuments.begin(); p != cateDocuments.end(); ++p) 
		{
		    #ifdef DEBUG
			    mlog << "Document Name: " << *p << std::endl;
            #endif
			std::vector<int> docVector(mVocabularyNum, 0);
			std::string docName = *p;
			FileTokenizer tokenizer(dirName + docName);
			int docTokens = 0;
			while(tokenizer.hasNext()) 
			{
				std::string token = tokenizer.nextToken();			
				docVector[mVocabulary[token]] = 1;
				++docTokens;
			}
			mpDocumentVectors[i][docName] = docVector;
			mpDocumentTokens[i][docName]  = docTokens;
			
        #ifdef DEBUG
			for(int j = 0; j < mVocabularyNum; ++j)	
			{
			    mlog << mpDocumentVectors[i][docName][j] << " ";
			}
			mlog << std::endl;
			mlog <<"Document token number: " << mpDocumentTokens[i][docName] << std::endl;;
        #endif	
		}
	}		
	#ifdef DEBUG
	mlog << "<<<< Build mVocabulary map for each document Finished."<< std::endl;
    #endif
    
    mTotalTokens = 0;
	for(int i = 0; i < mCategoryNum; ++i) 
	{
		auto & cateTokens = mpDocumentTokens[i]; //std::map<std::string, int>
		for(auto p = cateTokens.begin(); p != cateTokens.end(); ++p)
			mTotalTokens += (*p).second;
	}
#ifdef DEBUG
	mlog << "mTotalTokens = " << mTotalTokens << std::endl;
#endif
}

OfflineNaiveBayesClassifier:: ~OfflineNaiveBayesClassifier() 
{
	if(mCategoryNum > 0) 
	{
		delete[] mpDocumentNames;
		
#ifndef USE_VECTOR		
		for(int category = 0; category < mCategoryNum; ++category) 
		{
			std::map<std::string, int*>& cateVectors = mpDocumentVectors[category];
			for(std::map<std::string, int*>::iterator iter = cateVectors.begin(); iter != cateVectors.end(); ++iter) 
			{
				int* value = (*iter).second;
				delete[] value;
			}
		}
#endif
		delete[] mpDocumentVectors;
		delete[] mpDocumentTokens;
	}
}

/*
 * Here calculate the probabilities of document 'docToClassify' belonging to class 'Ci' one by one, 
 * and choose the class with maximum probability.
 * P(Ci|'docToClassify') = P(Ci|W0) * ... * P(Ci|Wn), W0...Wn are words in document 'docToClassify'.
 *
 * P(Ci|Wi) = P(Wi|Ci) * P(Ci) / P(Wi).
 * P(Wi|Ci) = (number of mpDocumentNames belonging to class 'Ci' containing word 'Wi') 
 * 			/ (number of words in mpDocumentNames belonging to class 'Ci').
 * P(Ci) = (number of words in mpDocumentNames belonging to class 'Ci') 
 * 		 / (number of words in all mpDocumentNames of all classes).
 * P(Wi) = (number of mpDocumentNames of all classes containing word 'Wi') 
 * 		 / (number of words in all mpDocumentNames of all classes).
 *
 * N.B. Above we plus 1 to the nominator when word 'Wi' appears in some document no matter 
 * how many times of occurrences of it in the document when calculating P(Wi|Ci) and P(Wi). 
 * We can take the occurrences of word 'Wi' in a document into account, 
 * which induces the following formulas:
 * P(Wi|Ci) = (number of occurrences of word 'Wi' in mpDocumentNames belonging to class 'Ci') 
 * 			/ (number of words in mpDocumentNames belonging to class 'Ci').
 * P(Wi) = (number of occurrences of word 'Wi' in mpDocumentNames of all classes) 
 * 		 / (number of words in all mpDocumentNames of all classes).
 */ 
int OfflineNaiveBayesClassifier::classify(const std::string& docToClassify) 
{
    std::vector<float> cateProb(mCategoryNum, 1.0);
	FileTokenizer tokenizer(docToClassify);
	#ifdef DEBUG
		mlog << ">>>> Begin Classify :"<< docToClassify << std::endl;
    #endif
	while(tokenizer.hasNext()) 
	{
	    int totalFreq = 0;
	    std::vector<int> cateFreq(mCategoryNum, 0);
	    std::vector<int> cateTokens(mCategoryNum, 0);
		std::string token = tokenizer.nextToken();	
			
#ifdef DEBUG
		mlog << "Current Token : "<< token << std::endl;
#endif
		for(int i = 0; i < mCategoryNum; ++i) 
		{
			auto & cateDocuments = mpDocumentNames[i];
			for(auto iter = cateDocuments.begin(); iter != cateDocuments.end(); ++iter) 
			{
				std::string docName = *iter;
				cateTokens[i] += mpDocumentTokens[i][docName];
				if(mpDocumentVectors[i][docName][mVocabulary[token]] == 1) 
				{
					++cateFreq[i];
					++totalFreq;
				}
			}
			#ifdef DEBUG
		        mlog << "Occurs : "<< cateFreq[i] << " times in Category " 
		               << i << std::endl;
            #endif
		}
        #ifdef DEBUG
		        mlog << "Occurs : "<< totalFreq << " times in All Category " 
		               << std::endl;
        #endif
		for(int i = 0; i < mCategoryNum; ++i) 
		{
			float prob;
			if(cateFreq[i] == 0)
				prob = static_cast<float>(cateTokens[i]) / mTotalTokens;
			else
				prob = (static_cast<float>(cateFreq[i]) / cateTokens[i]) *
					   (static_cast<float>(cateTokens[i]) / mTotalTokens) /
					   (static_cast<float>(totalFreq) / mTotalTokens);
			cateProb[i] *= prob;
	
        #ifdef DEBUG
			mlog << "cateFreq[" << i << "] = " << cateFreq[i] 
			       << ", cateTokens[" << i << "] = " << cateTokens[i]
				   << ", prob = " << prob << ", cateProb[" << i << "] = "
				   << cateProb[i] << std::endl;
        #endif
		}
	}

	int category  = 0;
	float maxProb = cateProb[0];
	for(int i = 1; i < mCategoryNum; ++i) 
	{
		if(maxProb < cateProb[i]) 
		{
			category = i;
			maxProb = cateProb[i];
		}
	}
#ifdef DEBUG
	std::cout << "MaxProb = " << maxProb << ", Category = " << category << std::endl;
#endif
#ifdef DEBUG
	mlog << "<<<< Finish Classify :"<< docToClassify << std::endl;
#endif
	return category;
}

