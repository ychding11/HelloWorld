/*
 * hmm.cpp
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

#include "hmm.h"

const size_t DIGIT_NUM = 5;

/*************************************************
 * Function(s)
*************************************************/
std::vector<Sequence> load_sequence(const std::string &filename)
{
    std::vector<Sequence> sequences;
    std::ifstream fin(filename.c_str());
    if (!fin.is_open())
    {
        throw "Couldn't load sequence: '" + filename + "'";
    }
    
    logger << "Load sequence from file : " << filename << std::endl;
    
    std::string line;
    while (std::getline(fin, line)) // 
    {
        size_t length = line.size();
        Sequence sequence(length);
        for (size_t t = 0; t < length; ++t)
        {
            sequence[t] = line[t] - 'A';
        }

        sequences.push_back(sequence);
    }

    fin.close();
    return sequences;
}

/*************************************************
 * HMM - Static Method(s)
*************************************************/
HMM HMM::load(const std::string &filename)
{
    size_t state_num, observ_num;
    double *init_prob;
    double **trans_prob;
    double **observ_prob;

    std::ifstream fin(filename.c_str());
    if (!fin.is_open())
    {
        throw "Couldn't load HMM: '" + filename + "'";
    }
    logger << " Load HMM initial state from file : " << filename << std::endl;
    std::string token;
    while (fin >> token, !fin.eof())
    {
        if (token == "initial:")
        {
            fin >> state_num;
            init_prob = new double[state_num];
            for (size_t i = 0; i < state_num; ++i)
            {
                fin >> init_prob[i];
            }
        }
        else if (token == "transition:")
        {
            fin >> state_num;
            trans_prob = new double*[state_num];
            for (size_t i = 0; i < state_num; ++i)
            {
                trans_prob[i] = new double[state_num];
                for (size_t j = 0; j < state_num; ++j)
                {
                    fin >> trans_prob[i][j];
                }
            }
        }
        else if (token == "observation:")
        {
            fin >> observ_num;
            observ_prob = new double*[observ_num];
            for (size_t o = 0; o < observ_num; ++o)
            {
                observ_prob[o] = new double[state_num];
                for (size_t i = 0; i < state_num; ++i)
                {
                    fin >> observ_prob[o][i];
                }
            }
        }
    }

    fin.close();

    // create model
    HMM model(filename, state_num, observ_num, init_prob, trans_prob, observ_prob);

    // release variables
    for (size_t i = 0; i < state_num; ++i)
    {
        delete trans_prob[i];
    }

    for (size_t o = 0; o < observ_num; ++o)
    {
        delete observ_prob[o];
    }

    delete init_prob;
    delete trans_prob;
    delete observ_prob;

    return model;
}

/*************************************************
 * HMM - Static Method(s)
*************************************************/
void HMM::dump(const std::string &filename, const HMM &model)
{
    std::ofstream fout(filename.c_str());
    if (!fout.is_open())
    {
        throw "Couldn't dump model: '" + filename + "'";
    }
    
    logger << "Save HMM Object to file : " << filename << std::endl;
    
    fout << std::fixed << std::setprecision(DIGIT_NUM);
    fout << "initial: " << model.mStateNum << std::endl;
    for (size_t i = 0; i < model.mStateNum - 1; ++i)
    {
        fout << model.getmpInitPr(i) << " ";
    }

    fout << model.getmpInitPr(model.mStateNum - 1) << std::endl;

    fout << "\ntransition: " << model.mStateNum << std::endl;
    for (size_t i = 0; i < model.mStateNum; ++i)
    {
        for (size_t j = 0; j < model.mStateNum - 1; ++j)
        {
            fout << model.getmppTransPr(i, j) << " ";
        }

        fout << model.getmppTransPr(i, model.mStateNum - 1) << std::endl;
    }

    fout << "\nobservation: " << model.mObservNum << std::endl;
    for (size_t o = 0; o < model.mObservNum; ++o)
    {
        for (size_t i = 0; i < model.mStateNum - 1; ++i)
        {
            fout << model.getmppObservPr(o, i) << " ";
        }

        fout << model.getmppObservPr(o, model.mStateNum - 1) << std::endl;
    }

    fout.close();
}

/*************************************************
 * HMM - Constructor(s) & Destructor(s)
*************************************************/
HMM::HMM(const std::string &name, size_t state_num, size_t observ_num,
         double *init_prob, double **trans_prob, double **observ_prob)
      : _name(name)
      , mStateNum(state_num)
      , mObservNum(observ_num)
{
    logger << "Construct HMM." << std::endl;
    _malloc();
    setmpInitPr(init_prob);
    setmppTransPr(trans_prob);
    setmppObservPr(observ_prob);
    logger << "Construct HMM Finished." << std::endl;
}

HMM::HMM(const HMM &model) // can access private member
      : _name(model._name)
      , mStateNum(model.mStateNum)
      , mObservNum(model.mObservNum)
{
    _malloc();

    setmpInitPr(model.mpInitPr);
    setmppTransPr(model.mppTransPr);
    setmppObservPr(model.mppObservPr);
}

HMM::~HMM(void)
{
    _release();
}

/*************************************************
 * = Operator(s) overload
*************************************************/
HMM &HMM::operator=(const HMM &model)
{
    //check this == model?
    // resize
    if (model.mStateNum != mStateNum || model.mObservNum != mObservNum)
    {
        _release();

        mStateNum = model.mStateNum;
        mObservNum = model.mObservNum;

        _malloc();
    }

    // copy probability
    setmpInitPr(model.mpInitPr);
    setmppTransPr(model.mppTransPr);
    setmppObservPr(model.mppObservPr);

    return *this;
}

/*************************************************
 * HMM - Operate Method(s)
*************************************************/
double HMM::evaluate(const Sequence &sequence) const
{
    size_t length = sequence.size();

    // calculate delta
    double *delta = new double[mStateNum];
    double *new_delta = new double[mStateNum];
    for (size_t i = 0; i < mStateNum; ++i)
    {
        delta[i] = mpInitPr[i] * mppObservPr[sequence[0]][i];
    }

    for (size_t t = 1; t < length; ++t)
    {
        for (size_t j = 0; j < mStateNum; ++j)
        {
            double max_prob = delta[0] * mppTransPr[0][j];
            for (size_t i = 1; i < mStateNum; ++i)
            {
                double prob = delta[i] * mppTransPr[i][j];
                if (prob > max_prob)
                {
                    max_prob = prob;
                }
            }

            new_delta[j] = max_prob * mppObservPr[sequence[t]][j];
        }

        memcpy(delta, new_delta, sizeof(double) * mStateNum);
    }

    // calculate probability
    double prob = delta[0];
    for (size_t i = 1; i < mStateNum; ++i)
    {
        if (delta[i] > prob)
        {
            prob = delta[i];
        }
    }

    delete delta;
    return prob;
}

void HMM::learn(const std::vector<Sequence> &sequences, size_t iteration)
{
    size_t number = sequences.size();
    if (number == 0) { return; }
    
    logger << "Training HMM..." << std::endl;
    
    size_t length = sequences[0].size();

    // allocate alpha, beta, gamma, and epsilon
    double **alpha = new double*[length];
    double **beta = new double*[length];
    double **gamma = new double*[length];
    double ***epsilon = new double**[length];

    double **sigma_gamma = new double*[length];
    double ***sigma_epsilon = new double**[length];

    for (size_t t = 0; t < length; ++t)
    {
        alpha[t] = new double[mStateNum];
        beta[t]  = new double[mStateNum];
        gamma[t]  = new double[mStateNum];
        epsilon[t]  = new double*[mStateNum];

        sigma_gamma[t]  = new double[mStateNum];
        sigma_epsilon[t]  = new double*[mStateNum];

        for (size_t s = 0; s < mStateNum; ++s)
        {
            epsilon[t][s] = new double[mStateNum];
            sigma_epsilon[t][s] = new double[mStateNum];
        }
    }

    // allocate observ_gamma_sum
    double **observ_gamma_sum = new double*[mObservNum];
    for (size_t o = 0; o < mObservNum; ++o)
    {
        observ_gamma_sum[o] = new double[mStateNum];
    }

    // perform learning
    for (size_t count = 0; count < iteration; ++count)
    {
        // initialize observ_gamma_sum, sigma_gamma and sigma_epsilon
        for (size_t o = 0; o < mObservNum; ++o)
        {
            memset(observ_gamma_sum[o], 0, sizeof(double) * mStateNum);
        }

        for (size_t t = 0; t < length; ++t)
        {
            memset(sigma_gamma[t], 0, sizeof(double) * mStateNum);
            for (size_t s = 0; s < mStateNum; ++s)
            {
                memset(sigma_epsilon[t][s], 0, sizeof(double) * mStateNum);
            }
        }
        
        //loop all sequence
        for (std::vector<Sequence>::const_iterator iter = sequences.begin(); iter != sequences.end(); ++iter)
        {
            const Sequence &sequence = *iter;

            // calculate alpha and beta
            _forward(alpha, sequence);
            _backward(beta, sequence);

            // calculate gamma
            for (size_t t = 0; t < length; ++t)
            {
                double prob_sum = 0;
                for (size_t i = 0; i < mStateNum; ++i)
                {
                    gamma[t][i] = alpha[t][i] * beta[t][i];
                    prob_sum += gamma[t][i];
                }

                if (prob_sum == 0) { continue; }

                for (size_t i = 0; i < mStateNum; ++i)
                {
                    gamma[t][i] /= prob_sum;
                    sigma_gamma[t][i] += gamma[t][i];
                    observ_gamma_sum[sequence[t]][i] += gamma[t][i];
                }
            }

            // calculate epsilon
            for (size_t t = 0; t < length - 1; ++t)
            {
                double prob_sum = 0;
                for (size_t i = 0; i < mStateNum; ++i)
                {
                    for (size_t j = 0; j < mStateNum; ++j)
                    {
                        epsilon[t][i][j] = alpha[t][i] \
                            * mppTransPr[i][j] \
                            * mppObservPr[sequence[t + 1]][j] \
                            * beta[t + 1][j];
                        prob_sum += epsilon[t][i][j];
                    }
                }

                if (prob_sum == 0) { continue; }

                for (size_t i = 0; i < mStateNum; ++i)
                {
                    for (size_t j = 0; j < mStateNum; ++j)
                    {
                        sigma_epsilon[t][i][j] += epsilon[t][i][j] / prob_sum;
                    }
                }
            }
        }

        for (size_t i = 0; i < mStateNum; ++i)
        {
            // update initialize probability
            mpInitPr[i] = sigma_gamma[0][i] / number;

            // update transition probability
            double gamma_sum = 0;
            for (size_t t = 0; t < length - 1; ++t)
            {
                gamma_sum += sigma_gamma[t][i];
            }

            if (gamma_sum > 0)
            {
                for (size_t j = 0; j < mStateNum; ++j)
                {
                    double epsilon_sum = 0;
                    for (size_t t = 0; t < length - 1; ++t)
                    {
                        epsilon_sum += sigma_epsilon[t][i][j];
                    }

                    mppTransPr[i][j] = epsilon_sum / gamma_sum;
                }
            }
            else
            {
                memset(mppTransPr[i], 0, sizeof(double) * mStateNum);
            }

            // update observation probability
            gamma_sum += sigma_gamma[length - 1][i];

            if (gamma_sum > 0)
            {
                for (size_t o = 0; o < mObservNum; ++o)
                {
                    mppObservPr[o][i] = observ_gamma_sum[o][i] / gamma_sum;
                }
            }
            else
            {
                for (size_t o = 0; o < mObservNum; ++o)
                {
                    mppObservPr[o][i] = 0;
                }
            }
        }
    }

    // release variables
    for (size_t o = 0; o < mObservNum; ++o)
    {
        delete observ_gamma_sum[o];
    }

    delete observ_gamma_sum;

    for (size_t t = 0; t < length; ++t)
    {
        for (size_t s = 0; s < mStateNum; ++s)
        {
            delete sigma_epsilon[t][s];
            delete epsilon[t][s];
        }

        delete sigma_epsilon[t];
        delete sigma_gamma[t];

        delete epsilon[t];
        delete gamma[t];
        delete beta[t];
        delete alpha[t];
    }

    delete sigma_epsilon;
    delete sigma_gamma;

    delete epsilon;
    delete gamma;
    delete beta;
    delete alpha;
}

/*************************************************
 * HMM - Helper(s)
*************************************************/
void HMM::_malloc(void)
{
    // unsafe
    mpInitPr   = new double [mStateNum];
    mppTransPr  = new double*[mStateNum];
    mppObservPr = new double*[mObservNum];

    for (size_t i = 0; i < mStateNum; ++i)
    {
        mppTransPr[i] = new double[mStateNum];
    }

    for (size_t o = 0; o < mObservNum; ++o)
    {
        mppObservPr[o] = new double[mStateNum];
    }
}

void HMM::_release(void)
{
    // unsafe
    for (size_t i = 0; i < mStateNum; ++i)
    {
        delete mppTransPr[i];
    }

    for (size_t o = 0; o < mObservNum; ++o)
    {
        delete mppObservPr[o];
    }

    delete mpInitPr;
    delete mppTransPr;
    delete mppObservPr;
}

/*************************************************
 * HMM - Forward algorithms
*************************************************/
void HMM::_forward(double **alpha, const Sequence &sequence) const
{
    size_t length = sequence.size();
    for (size_t i = 0; i < mStateNum; ++i)
    {
        alpha[0][i] = mpInitPr[i] * mppObservPr[sequence[0]][i];
    }

    for (size_t t = 0; t < length - 1; ++t)
    {
        for (size_t j = 0; j < mStateNum; ++j)
        {
            double prob = 0;
            for (size_t i = 0; i < mStateNum; ++i)
            {
                prob += alpha[t][i] * mppTransPr[i][j];
            }

            alpha[t + 1][j] = prob * mppObservPr[sequence[t + 1]][j];
        }
    }
}

void HMM::_backward(double **beta, const Sequence &sequence) const
{
    size_t length = sequence.size();
    for (size_t i = 0; i < mStateNum; ++i)
    {
        beta[length - 1][i] = 1;
    }

    for (size_t t = length - 1; t > 0; --t)
    {
        for (size_t i = 0; i < mStateNum; ++i)
        {
            double prob = 0;
            for (size_t j = 0; j < mStateNum; ++j)
            {
                prob += beta[t][j] * mppTransPr[i][j] * mppObservPr[sequence[t]][j];
            }

            beta[t - 1][i] = prob;
        }
    }
}

