#include <vector>
#include <random>
#include <iostream>

std::vector<int> initialiseExperiment(){
    std::vector<int> experimentValues;
    int value;

    std::cout << "Please choose a distribution:\n    (0) Gaussian/Normal\n    (1) Bernoulli" << std::endl;
    std::cin >> value;
    experimentValues.push_back(value);

    std::cout << "Please choose K" << std::endl;
    std::cin >> value;
    experimentValues.push_back(value);

    std::cout << "Please choose amount of rounds:" << std::endl;
    std::cin >> value;
    experimentValues.push_back(value);

    std::cout << "Please choose amount of steps in each round:" << std::endl;
    std::cin >> value;
    experimentValues.push_back(value);

    std::cout << "Please choose the algorithm:\n    (0) Epsilon-greedy\n    (1) Optimistic Initial Values" << std::endl;
    std::cin >> value;
    experimentValues.push_back(value);

    return experimentValues;
}


std::vector<double> initialiseBandits(int K){
    std::vector<double> bandits;
    std::default_random_engine generator(time(0));
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < K; i++){
        bandits.push_back(distribution(generator));
    }

    return bandits;
}