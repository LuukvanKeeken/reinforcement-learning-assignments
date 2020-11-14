#include <iostream>
#include <vector>
#include "general-functions.hpp"


int main(){
    std::vector<int> experimentValues = initialiseExperiment();
    int distribution = experimentValues[0];
    int K = experimentValues[1];
    int N = experimentValues[2];
    int T = experimentValues[3];
    int algorithm = experimentValues[4];

    return 0;
}