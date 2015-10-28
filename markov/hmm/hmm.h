/*
 * .cpp
 * This a InputDataSet data type for handwriting recognise.
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
 
#ifndef HMM_H_INCLUDED_
#define HMM_H_INCLUDED_

#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include "Logger.h"

/*************************************************
    Function(s)
*************************************************/
typedef std::vector<size_t> Sequence;
std::vector<Sequence> load_sequence(const std::string &filename);

/*************************************************
 * HMM - Class Declaration
*************************************************/
class HMM
{
    public:
        // Static Method(s)
        static HMM load(const std::string &filename);
        static void dump(const std::string &filename, const HMM &model);

        // Constructor(s) & Destructor(s)
        HMM(const std::string &name, size_t state_num, size_t observ_num,
              double *init_prob, double **trans_prob, double **observ_prob);
        HMM(const HMM &model);
        ~HMM(void);

        // assignment Operator(s) overload
        HMM & operator=(const HMM &model);

        // Setter(s) / Getter(s)
        std::string get_name(void) const;
        void        set_name(const std::string &name);
        size_t      getmStateNum(void) const;
        size_t      getmObservNum(void) const;
        double      getmpInitPr(size_t state) const;
        void        setmpInitPr(double *prob);
        double      getmppTransPr(size_t state_from, size_t state_to) const;
        void        setmppTransPr(double **prob);
        double      getmppObservPr(size_t observ, size_t state) const;
        void        setmppObservPr(double **prob);

        // Operate Method(s)
        double      evaluate(const Sequence &sequence) const;
        void        learn(const std::vector<Sequence> &sequences, size_t iteration);

    private:
        // Helper(s)
        void        _malloc(void);
        void        _release(void);
        void        _forward(double **alpha, const Sequence &sequence) const;
        void        _backward(double **beta, const Sequence &sequence) const;

        // Data Member(s)
        std::string _name;
        size_t      mStateNum;
        size_t      mObservNum;
        double      *mpInitPr;
        double      **mppTransPr;
        double      **mppObservPr;
};

/*************************************************
 * HMM - Setter(s) / Getter(s)
*************************************************/
inline std::string HMM::get_name(void) const
{
    return _name;
}

inline void HMM::set_name(const std::string &name)
{
    _name = name;
}

inline size_t HMM::getmStateNum(void) const
{
    return mStateNum;
}

inline size_t HMM::getmObservNum(void) const
{
    return mObservNum;
}

inline double HMM::getmpInitPr(size_t state) const
{
    if (state >= mStateNum) { return 0.0; }
    return mpInitPr[state];
}

inline void HMM::setmpInitPr(double *prob)
{
    memcpy(mpInitPr, prob, sizeof(double) * mStateNum);
}

inline double HMM::getmppTransPr(size_t state_from, size_t state_to) const
{
    /* input parameter check is not good, should improve */
    if (state_from >= mStateNum || state_to >= mStateNum) { return 0; }
    return mppTransPr[state_from][state_to];
}

inline void HMM::setmppTransPr(double **prob)
{
    for (size_t s = 0; s < mStateNum; ++s)
    {
        memcpy(mppTransPr[s], prob[s], sizeof(double) * mStateNum);
    }
}

inline double HMM::getmppObservPr(size_t observ, size_t state) const
{
    /* input parameter check is not good, should improve */
    if (state >= mStateNum || observ >= mObservNum) { return 0; }
    return mppObservPr[observ][state];
}

inline void HMM::setmppObservPr(double **prob)
{
    for (size_t o = 0; o < mObservNum; ++o)
    {
        memcpy(mppObservPr[o], prob[o], sizeof(double) * mStateNum);
    }
}

#endif

