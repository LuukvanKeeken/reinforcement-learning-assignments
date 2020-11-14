#include <iostream>
#include <vector>
#include "general-functions.hpp"
#include "epsilon-greedy/epsilon-greedy.hpp"
#include "experimentValuesStruct.hpp"



/* Main, central function of the experiment program. */
int main(){
    /* Get the experiment values. */
    struct experimentValues experiment_values;
    initialiseExperiment(experiment_values);


    /* Initialise the K bandits/the true action values. */
    std::vector<double> bandits = initialiseBandits(experiment_values.K);

    switch(experiment_values.algorithm){
        case 0:
            epsilonGreedyMain(experiment_values, bandits);
            break;
        case 1:
            //Optimistic Initial Values
            std::cout << "not yet implemented\n"; 
            break;
        case 2:
            //Third algorithm
            std::cout << "not yet implemented\n"; 
            break;
        case 3:
            //Fourth algorithm
            std::cout << "not yet implemented\n"; 
            break;
        default:
            break;
    }


    return 0;
}