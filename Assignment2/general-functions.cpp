#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "parameterValuesStruct.hpp"

/* Function that returns the winner of the game, or else returns that
 * the game has ended in a draw or hasn't ended yet. The game ends 
 * in a draw if there are no open cells left, and there is no winner. */
std::string getGameResult(std::vector<std::string> currentBoard){
    /* Check if anywhere on the board there are three identical
     * symbols (not " ") in a row. If so, the player with that
     * symbol has won, and that symbol is returned. */
    if (currentBoard[0] != "e"){
        /* Top horizontal */
        if (currentBoard[0] == currentBoard[1] && currentBoard[0] == currentBoard[2]){
            return currentBoard[0];
        /* Left vertical */
        } else if (currentBoard[0] == currentBoard[3] && currentBoard[0] == currentBoard[6]){
            return currentBoard[0];
        /* Top-left to bottom-right diagonal */
        } else if (currentBoard[0] == currentBoard[4] && currentBoard[0] == currentBoard[8]){
            return currentBoard[0];
        }
    }
    if (currentBoard[1] != "e"){
        /* Mid vertical */
        if (currentBoard[1] == currentBoard[4] && currentBoard[1] == currentBoard[7]){
            return currentBoard[1];
        }
    } 
    if (currentBoard[2] != "e"){
        /* Top-right to bottom-left diagonal */
        if (currentBoard[2] == currentBoard[4] && currentBoard[2] == currentBoard[6]){
            return currentBoard[2];
        /* Right vertical */
        } else if (currentBoard[2] == currentBoard[5] && currentBoard[2] == currentBoard[8]){
            return currentBoard[2];
        }
    } 
    if (currentBoard[3] != "e"){
        /* Mid horizontal */
        if (currentBoard[3] == currentBoard[4] && currentBoard[3] == currentBoard[5]){
            return currentBoard[3];
        } 
    }
    if (currentBoard[6] != "e"){
        /* Bottom horizontal */
        if (currentBoard[6] == currentBoard[7] && currentBoard[6] == currentBoard[8]){
            return currentBoard[6];
        }
    }
    

    /* As at this point no winner has been found, either the game has ended
     * in a draw or it hasn't ended yet. */
    for (int i = 0; i < currentBoard.size(); i++){
        if (currentBoard[i] == "e"){
            return "not ended";
        }
    }

    /* There is no winner and no open cell left, so the game has ended in a draw. */
    return "draw";
}


void initialiseExperiment(struct parameterValues &parameter_values){
    std::cout << "Please choose the reinforcement learning algorithm you want to use:\n    0) Q-learning\n    1) Sarsa\n";
    std::cin >> parameter_values.RLAlg;

    std::cout << "\nPlease choose the exploration/exploitation algorithm you want to use:\n    0) Upper-Confidence-Bound\n    1) Gradient-based\n";
    std::cin >> parameter_values.explorationAlg;

    std::cout << "\nPlease declare the amount of steps:\n";
    std::cin >> parameter_values.steps;
}

