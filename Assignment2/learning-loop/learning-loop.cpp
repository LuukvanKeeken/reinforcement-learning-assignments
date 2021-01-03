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
        if (step > 0){
            //Update q-value of previous board/afterstate using this afterstate's value.
        }


        /* Check whether X has won, lost, if there's a draw, or
            if the game hasn't ended yet. */
        gameResult = getGameResult(currentBoard);

        
        if (gameResult == "not ended"){
            /* Save the currentBoard in previousBoard, so that its
            Q-value can be updated in the next step, i.e. after
            the opponent has reacted and X has chosen a new
            afterstate. */
            previousBoard = currentBoard;

            /* Let the opponent select his action and update the board. */
            currentBoard = actionByO(currentBoard);
            std::cout << "O PLAYS:\n";
            printBoard(currentBoard);
            std::cout << "\n";

            gameResult = getGameResult(currentBoard);
            if (gameResult == "X"){
                std::cout << "WIN\n";
                exit(0);
            } else if (gameResult == "O"){
                std::cout << "LOSE\n";
                exit(0);
            }
        } else if (gameResult == "X"){
            //won, (update with +1? and) start new run
            std::cout << "WIN\n";
            exit(0);
        } else if (gameResult == "O"){
            //lost, (update with -1? and) start new run
            std::cout << "LOSE\n";
            exit(0);
        } else if (gameResult == "draw"){
            //draw, (update with 0? and) start new run
            std::cout << "DRAW\n";
            exit(0);
        }

    }

    // std::string gameResult = getGameResult(currentBoard);
    // std::cout << "result: " << gameResult << "\n";

    currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates);
    printBoard(currentBoard);


}