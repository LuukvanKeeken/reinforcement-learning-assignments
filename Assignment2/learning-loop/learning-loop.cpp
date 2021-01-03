#include <iostream>
#include <vector>
#include "../parameterValuesStruct.hpp"
#include "../general-functions.hpp"


void learningLoop(struct parameterValues parameter_values){
    std::map<std::vector<std::string>, double> qValueTableXAfterStates = generateQValueTableXAfterStates();
    std::cout << qValueTableXAfterStates.size() << "\n";

    /* Board is initialised with each cell empty ("e"). */
    std::vector<std::string> currentBoard = {"e", "e", "X", "e", "e", "e", "O", "e", "e"};


    std::string gameResult = getGameResult(currentBoard);
    std::cout << "result: " << gameResult << "\n";

    currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates);


}