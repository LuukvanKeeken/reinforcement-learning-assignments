#include <iostream>
#include <vector>
#include "../parameterValuesStruct.hpp"
#include "../general-functions.hpp"


void learningLoop(struct parameterValues parameter_values){
    /* Board is initialised with each cell empty ("e"). */
    std::vector<std::string> currentBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};


    std::string gameResult = getGameResult(currentBoard);
    std::cout << "result: " << gameResult << "\n";

}