#include <iostream>
#include <vector>
#include "general-functions.hpp"
#include "epsilon-greedy/epsilon-greedy.hpp"
#include "experimentValuesStruct.hpp"
#include "experiment-loop/experiment-loop.hpp"
#include "experiment-loop/experiment-loop-gradient.hpp"



/* Main, central function of the experiment program. */
int main(){
    /* Get the experiment values. */
    struct experimentValues experiment_values;
    initialiseExperiment(experiment_values);

    if (experiment_values.algorithm == 3){
        experimentLoopGradient(experiment_values);
    } else {
        experimentLoop(experiment_values);
    }
    return 0;
}