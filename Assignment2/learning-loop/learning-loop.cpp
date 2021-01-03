#include <iostream>
#include <vector>
#include "../parameterValuesStruct.hpp"
#include "../general-functions.hpp"


void learningLoop(struct parameterValues parameter_values){
    std::map<std::vector<std::string>, double> qValueTableXAfterStates = generateQValueTableXAfterStates();
    std::cout << qValueTableXAfterStates.size() << "\n";

    /* Board is initialised with each cell empty ("e"). */
    std::vector<std::string> currentBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
    //printBoard(currentBoard);

    double alpha = 0.1;
    double gamma = 0.9;
    std::string gameResult;
    for (int step = 0; step < parameter_values.steps; step++){
        /* Select the next board from the possible afterstates. */
        currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates);
        /* Check whether X has won, lost, if there's a draw, or
            if the game hasn't ended yet. */
        gameResult = getGameResult(currentBoard);


    }

    std::string gameResult = getGameResult(currentBoard);
    std::cout << "result: " << gameResult << "\n";

    currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates);
    printBoard(currentBoard);


}