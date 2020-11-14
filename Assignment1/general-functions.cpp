#include <vector>
#include <random>
#include <iostream>
#include "experimentValuesStruct.hpp"

/* Function that gets the basic experiment values from the user,
 * i.e. the used reward distribution for each arm, the amount of
 * arms K, the amount of round N, the amount of steps per round T,
 * and the algorithm to use. These values are stored in a vector
 * which is then returned.
 */ 
void initialiseExperiment(struct experimentValues &experiment_values){
    std::cout << "Please choose a distribution:\n    (0) Gaussian/Normal\n    (1) Bernoulli" << "\n";
    std::cin >> experiment_values.distribution ;

    std::cout << "Please choose K" << "\n";
    std::cin >> experiment_values.K;

    std::cout << "Please choose amount of rounds:" << "\n";
    std::cin >> experiment_values.N;

    std::cout << "Please choose amount of steps in each round:" << "\n";
    std::cin >> experiment_values.T;

    std::cout << "Please choose the algorithm:\n    (0) Epsilon-greedy\n    (1) Optimistic Initial Values" << "\n";
    std::cin >> experiment_values.algorithm;
}


/* Function that initialises a chosen number K of bandits,
 * i.e. K values are sampled from a normal distribution with
 * mean 0 and standard deviation 1, to denote the K true action
 * values.
 */ 
std::vector<double> initialiseBandits(int K){
    std::vector<double> bandits;
    std::default_random_engine generator(time(0));
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < K; i++){
        bandits.push_back(distribution(generator));
    }

    return bandits;
}