#include <iostream>
#include <vector>
#include "../experimentValuesStruct.hpp"


void epsilonGreedyMain(struct experimentValues experiment_values, std::vector<double> bandits){
    std::vector<double> action_value_estimates = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::cout << experiment_values.distribution << "\n";
    std::cout << experiment_values.K << "\n";
    std::cout << experiment_values.N << "\n";
    std::cout << experiment_values.T << "\n";
    std::cout << experiment_values.algorithm << "\n";
    std::cout << "we're here!\n";
}