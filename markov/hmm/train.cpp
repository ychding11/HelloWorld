#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "hmm.h"

void error(const std::string &msg)
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    ENTER_FUNCTION;
    
    // argv[1]: iteration
    // argv[2]: initialize model path
    // argv[3]: model sequences path
    // argv[4]: output model path
    if (argc != 5)
    {
        error("Usage: ./train iteration init_model seq_model output_model");
    }

    try
    {
        size_t iteration = (size_t)atoi(argv[1]);
        
        //construct HMM object from file
        HMM model = HMM::load(argv[2]);
        
        //load sequence from file
        std::vector<Sequence> sequences = load_sequence(argv[3]);

        model.learn(sequences, iteration); //train the HMM
        
        HMM::dump(argv[4], model); //save the HMM object to disk file
    }
    catch (const std::string &msg)
    {
        error(msg);
    }
    EXIT_FUNCTION;
    return 0;
}

