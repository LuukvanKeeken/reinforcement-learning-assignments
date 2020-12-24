#include <iostream>
#include "parameterValuesStruct.hpp"
#include "general-functions.hpp"
#include "learning-loop/learning-loop.hpp"

int main(){
    struct parameterValues parameter_values;
    initialiseExperiment(parameter_values);

    learningLoop(parameter_values);
}