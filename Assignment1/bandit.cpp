#include <iostream>
#include <vector>
#include "general-functions.hpp"
#include "epsilon-greedy/epsilon-greedy.hpp"
#include "experimentValuesStruct.hpp"
#include "experiment-loop/experiment-loop.hpp"



/* Main, central function of the experiment program. */
int main(){
    /* Get the experiment values. */
    struct experimentValues experiment_values;
    initialiseExperiment(experiment_values);

    experimentLoop(experiment_values);
    // switch(experiment_values.algorithm){
    //     case 0:
    //         epsilonGreedyMain(experiment_values);
    //         break;
    //     case 1:
    //         /* Same function, but extra choice of initial values will be triggered. */
    //         epsilonGreedyMain(experiment_values);
    //         break;
    //     case 2:
    //         //Third algorithm
    //         std::cout << "not yet implemented\n"; 
    //         break;
    //     case 3:
    //         //Fourth algorithm
    //         std::cout << "not yet implemented\n"; 
    //         break;
    //     default:
    //         break;
    // }


    return 0;
}