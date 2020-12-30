#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "parameterValuesStruct.hpp"
#include "general-functions.hpp"


std::vector<std::string> chooseNewAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, double> qValueTable){
    std::vector<std::string> bestAfterstate = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
    double bestQValue = -10000;
    double newQValue = 0;
    std::vector<std::string> possibleNewBoard;
    /* Basic selection, without any exploration. Go through all
        positions to check if an X can be placed there. */
    for (int i = 0; i < 9; i++){
        possibleNewBoard = currentBoard;
        /* If the position is still free, look up the Q-value
            for the afterstate that is reached when placing
            an X there. */
        if (currentBoard[i] == "e"){
            possibleNewBoard[i] = "X";
            newQValue = getQValue(possibleNewBoard, qValueTable);
            /* If the Q-value for this afterstate is better than
                the previous best, make this afterstate the new best. */
            if (newQValue > bestQValue){
                bestQValue = newQValue;
                bestAfterstate = possibleNewBoard;
            }
        }
    }

    return possibleNewBoard;
}

/* Function that creates a q-value table for each possible board
    state. Can probably be used to update afterstates for both
    player X and player O, as those don't overlap. */
std::map<std::vector<std::string>, double> generateQValueTable(){
    std::map<std::vector<std::string>, double> qValueTable;
    std::vector<std::string> newBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
    std::vector<std::string> possibleSymbols = {"e", "X", "O"};

    /* There's probably a better way to do this ...
        If needed, don't even use the newBoards that
        e.g. have more Os than Xs, or other impossible
        board states (or even only afterstates for X.*/
    for (int i = 0; i < 3; i++){
        newBoard[8] = possibleSymbols[i];
        for (int j = 0; j < 3; j++){
            newBoard[7] = possibleSymbols[j];
            for (int k = 0; k < 3; k++){
                newBoard[6] = possibleSymbols[k];
                for (int l = 0; l < 3; l++){
                    newBoard[5] = possibleSymbols[l];
                    for (int m = 0; m < 3; m++){
                        newBoard[4] = possibleSymbols[m];
                        for (int n = 0; n < 3; n++){
                            newBoard[3] = possibleSymbols[n];
                            for (int o = 0; o < 3; o++){
                                newBoard[2] = possibleSymbols[o];
                                for (int p = 0; p < 3; p++){
                                    newBoard[1] = possibleSymbols[p];
                                    for (int q = 0; q < 3; q++){
                                        newBoard[0] = possibleSymbols[q];
                                        qValueTable[newBoard] = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return qValueTable;
} 


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

/* Function that returns the Q-value of the input afterstate.*/
double getQValue(std::vector<std::string> afterState, std::map<std::vector<std::string>, double> qValueTable){
    return qValueTable[afterState];
}


void initialiseExperiment(struct parameterValues &parameter_values){
    std::cout << "Please choose the reinforcement learning algorithm you want to use:\n    0) Q-learning\n    1) Sarsa\n";
    std::cin >> parameter_values.RLAlg;

    std::cout << "\nPlease choose the exploration/exploitation algorithm you want to use:\n    0) Upper-Confidence-Bound\n    1) Gradient-based\n";
    std::cin >> parameter_values.explorationAlg;

    std::cout << "\nPlease declare the amount of steps:\n";
    std::cin >> parameter_values.steps;
}
