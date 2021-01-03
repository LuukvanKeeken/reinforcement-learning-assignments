#include <iostream>
#include <vector>
#include "../parameterValuesStruct.hpp"
#include "../general-functions.hpp"


void learningLoop(struct parameterValues parameter_values){
    srand(time(NULL));
    std::map<std::vector<std::string>, double> qValueTableXAfterStates = generateQValueTableXAfterStates();

    /* Board is initialised with each cell empty ("e"). */
    std::vector<std::string> currentBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
    std::vector<std::string> previousBoard;
    //printBoard(currentBoard);

    double alpha = 0.1;
    double gamma = 0.9;
    std::string gameResult;
    for (int step = 0; step < parameter_values.steps; step++){
        /* Select the next board from the possible afterstates. */
        currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates);
        std::cout << "X PLAYS:\n";
        printBoard(currentBoard);
        std::cout << "\n";


        /* Check whether X has won, if there's a draw, or
            if the game hasn't ended yet. X cannot have
            lost after its own move. */
        gameResult = getGameResult(currentBoard);
        if (gameResult == "not ended"){
            /* Save the currentBoard in previousBoard, so that its
                Q-value can be updated after O has made a move. */
            previousBoard = currentBoard;

            /* Let the opponent select his action and update the board. */
            currentBoard = actionByO(currentBoard);
            std::cout << "O PLAYS:\n";
            printBoard(currentBoard);
            std::cout << "\n";

            /* Check if after O's move X has now lost, or if
                the game hasn't ended. A win for X or a draw
                aren't possible after O's move. */
            gameResult = getGameResult(currentBoard);

            if (gameResult == "not ended"){
                /* Game hasn't ended, update previous board/afterstate q-value with the 
                    q-value of the best possible next afterstate as the target. */
                updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, getQValue(findBestAfterstate(currentBoard, qValueTableXAfterStates), qValueTableXAfterStates));
            } else if (gameResult == "O"){
                /* Lost, update previous board/afterstate q-value with -1 as target, start new run. */
                std::cout << "LOSS\n";
                updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, -1);
                exit(0);
            }
        } else if (gameResult == "X"){
            /* Won, afterstate already has value 1, start new run. */
            std::cout << "WIN\n";
            std::cout << qValueTableXAfterStates[currentBoard] << "\n";
            exit(0);
        } else if (gameResult == "draw"){
            /* Draw, afterstate already has value 0, start new run. */
            std::cout << "DRAW\n";
            std::cout << qValueTableXAfterStates[currentBoard] << "\n";
            exit(0);
        }

    }

    // std::string gameResult = getGameResult(currentBoard);
    // std::cout << "result: " << gameResult << "\n";

    currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates);
    printBoard(currentBoard);


}