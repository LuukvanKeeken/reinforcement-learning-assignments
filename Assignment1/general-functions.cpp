#include <vector>
#include <random>
#include <iostream>
#include "experimentValuesStruct.hpp"

std::default_random_engine generator(time(0));

/* Function that finds and returns the index of the largest
 * value in a double vector.
 */
int getIndexOfLargestValue(std::vector<double> vector){
    int largest_value_index = 0;
    double largest_value = vector[0];
    for (int i = 1; i < vector.size(); i++){
        if (largest_value < vector[i]){
            largest_value = vector[i];
            largest_value_index = i;
        }
    }
    return largest_value_index;
}


/* Function that returns a random integer on the range [lower_limit, upper_limit]. */
int getRandomIntFromRangeUniform(int lower_limit, int upper_limit){
    std::uniform_int_distribution<int> distribution(lower_limit, upper_limit);
    return distribution(generator);
}

/* Function that returns a random decimal number [lower_limit, upper_limit]. */
double getRandomNumberFromRangeUniform(int lower_limit, int upper_limit){
    std::uniform_real_distribution<double> distribution(lower_limit, upper_limit);
    return distribution(generator);
}

/* Function that returns a random decimal number from a normal distribution
 * with a certain mean and standard_deviation.
 */ 
double getRandomNumberFromRangeNormal(double mean, double standard_deviation){
    std::normal_distribution<double> distribution(mean, standard_deviation);
    return distribution(generator);
}

/* Function that gets the basic experiment values from the user,
 * i.e. the used reward distribution for each arm, the amount of
 * arms K, the amount of round N, the amount of steps per round T,
 * and the algorithm to use. These values are stored in an 
 * experimentValues struct, which was passed by reference.
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


/* Function that initialises a chosen number K of arms/actions,
 * i.e. K values are sampled from a normal distribution with
 * mean 0 and standard deviation 1, to denote the K true action
 * values. The function also keeps track of which action value
 * is the highest, i.e. which action is the optimal action. At
 * the end of initialisation, this value is included in the return
 * vector.
 */ 
std::vector<double> initialiseBandit(int K){
    std::vector<double> bandit;

    int max_index = 0;
    for (int i = 0; i < K; i++){
        bandit.push_back(getRandomNumberFromRangeNormal(0.0, 1.0));
        if (i > 0){
            if (bandit[i] > bandit[max_index]){
                max_index = i;
            }
        }
    }

    bandit.push_back(max_index);

    return bandit;
}