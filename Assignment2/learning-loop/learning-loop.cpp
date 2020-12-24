#include <iostream>
#include <vector>
#include "../parameterValuesStruct.hpp"
#include "../general-functions.hpp"


void learningLoop(struct parameterValues parameter_values){
    std::vector<std::string> currentBoard = {"X", "e", "O", "X", "X", "O", "e", "e", "e"};


    std::string gameResult = getGameResult(currentBoard);
    std::cout << "result: " << gameResult << "\n";

}