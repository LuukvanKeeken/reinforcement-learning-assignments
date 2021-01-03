#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "parameterValuesStruct.hpp"
#include "general-functions.hpp"
#include <random>

/* Function that selects the action by the opponent.
    For now, only random selection is implemented. */
std::vector<std::string> actionByO(std::vector<std::string> currentBoard){
    /* Random action by O. Find all places that are
        still empty and put them in one vector. Then
        get a random index of that vector, the value 
        at that index is the location of the new O. */
    std::vector<int> freePlaces;
    for (int i = 0; i < 9; i++){
        if (currentBoard[i] == "e"){
            freePlaces.push_back(i);
        }
    }
    int selectedPlace = rand() % freePlaces.size();
    currentBoard[freePlaces[selectedPlace]] = "O";
    return currentBoard;

}



/* Function that checks if the input board state is an afterstate
    from the perspective of X, i.e. one more Xs than Os should be
    on the board. */ 
bool checkIfXAfterState(std::vector<std::string> newBoard){
    int xCount = 0;
    int oCount = 0;
    for (int i = 0; i < 9; i++){
        if (newBoard[i] == "X"){
            xCount += 1;
        } else if (newBoard[i] == "O"){
            oCount += 1;
        }
    }

    if (xCount == (oCount + 1)){
        return true;
    }
    return false;
}

/* Function that selects the next afterstate. Doesn't necessarily select the best afterstate,
    as there is a balance between exploration and exploitation. */
std::vector<std::string> chooseNewAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, double> qValueTableXAfterStates){
    std::vector<std::string> bestAfterstate;
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
            newQValue = getQValue(possibleNewBoard, qValueTableXAfterStates);
            /* If the Q-value for this afterstate is better than
                the previous best, make this afterstate the new best. */
            if (newQValue > bestQValue){
                bestQValue = newQValue;
                bestAfterstate = possibleNewBoard;
            }
        }
    }

    return bestAfterstate;
}

/* Function that finds the best possible afterstate from the current board. */
std::vector<std::string> findBestAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, double> qValueTableXAfterStates){
    std::vector<std::string> bestAfterstate;
    double bestQValue = -10000;
    double newQValue = 0;
    std::vector<std::string> possibleNewBoard;
    /* Go through all positions to check if an X can be placed there. */
    for (int i = 0; i < 9; i++){
        possibleNewBoard = currentBoard;
        /* If the position is still free, look up the Q-value
            for the afterstate that is reached when placing
            an X there. */
        if (currentBoard[i] == "e"){
            possibleNewBoard[i] = "X";
            newQValue = getQValue(possibleNewBoard, qValueTableXAfterStates);
            /* If the Q-value for this afterstate is better than
                the previous best, make this afterstate the new best. */
            if (newQValue > bestQValue){
                bestQValue = newQValue;
                bestAfterstate = possibleNewBoard;
            }
        }
    }

    return bestAfterstate;
}



/* Function that creates a q-value table for each possible board
    state. Can probably be used to update afterstates for both
    player X and player O, as those don't overlap. */
std::map<std::vector<std::string>, double> generateQValueTableXAfterStates(){
    std::map<std::vector<std::string>, double> qValueTableXAfterStates;
    std::vector<std::string> newBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
    std::vector<std::string> possibleSymbols = {"e", "X", "O"};

    int xCount = 0;
    int oCount = 0;
    /* There's probably a better way to do this ...
        Only boards that have exactly one more X than
        the number of Os are put into the table, as
        only those are afterstates for X. */
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
                                        /* Only add the board if it is an afterstate from
                                            X's perspective. */
                                        if (checkIfXAfterState(newBoard)){
                                            /* If in this board state X has won, give it
                                                value 1. It doesn't matter too much if it
                                                is invalid, as those won't be reached in 
                                                the game. */
                                            if (getGameResult(newBoard) == "X"){
                                                qValueTableXAfterStates[newBoard] = 1;
                                            } else {
                                                qValueTableXAfterStates[newBoard] = 0; 
                                            }       
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return qValueTableXAfterStates;
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

/* Function that asks the user to provide the parameter values before
    starting the learning phase. */
void initialiseExperiment(struct parameterValues &parameter_values){
    std::cout << "Please choose the reinforcement learning algorithm you want to use:\n    0) Q-learning\n    1) Sarsa\n";
    std::cin >> parameter_values.RLAlg;

    std::cout << "\nPlease choose the exploration/exploitation algorithm you want to use:\n    0) Upper-Confidence-Bound\n    1) Gradient-based\n";
    std::cin >> parameter_values.explorationAlg;

    std::cout << "\nPlease declare the amount of steps:\n";
    std::cin >> parameter_values.steps;

    std::cout << "\nPlease declare the value for alpha:\n";
    std::cin >> parameter_values.alpha;

    std::cout << "\nPlease declare the value for gamma:\n";
    std::cin >> parameter_values.gamma;
}



/* Function that prints a board. */
void printBoard(std::vector<std::string> board){
    std::cout << " " << board[0] << "|";
    std::cout << board[1] << "|";
    std::cout << board[2] << "\n";
    std::cout << "------\n";
    std::cout << " " << board[3] << "|";
    std::cout << board[4] << "|";
    std::cout << board[5] << "\n";
    std::cout << "------\n";
    std::cout << " " << board[6] << "|";
    std::cout << board[7] << "|";
    std::cout << board[8] << "\n";
}

/* Function that updates the Q-value for the given afterState, based on the
    the Q-value of the best afterState than can be reached after O has made 
    a move. In tic-tac-toe there are no rewards associated with single moves,
    only +1, -1, or 0 at the end. That's why there is no reward factor in this
    update equation. */
void updateAfterstateQValue(struct parameterValues parameter_values, std::vector<std::string> afterState, std::map<std::vector<std::string>, double> &qValueTableXAfterStates, double bestAfterAfterstateQValue){
    qValueTableXAfterStates[afterState] += parameter_values.alpha*(parameter_values.gamma*bestAfterAfterstateQValue - qValueTableXAfterStates[afterState]);
}