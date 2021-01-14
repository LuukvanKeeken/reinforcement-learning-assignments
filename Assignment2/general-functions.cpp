#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "parameterValuesStruct.hpp"
#include "general-functions.hpp"
#include <random>
#include <fstream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

std::default_random_engine generator(time(0));

/* Function that selects the action by the opponent.
    Minimax is used with probability 0.8, random selection
    is used with probability 0.2. */
std::vector<std::string> actionByO(std::vector<std::string> currentBoard){
    /* With probability 0.2, the opponent chooses a random action.
        Otherwise, it uses the minimax algorithm. */
    if (getRandomNumberFromRangeUniform(0, 1) < 0.4){
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
        int selectedPlace = (int)getRandomNumberFromRangeUniform(0, freePlaces.size());
        currentBoard[freePlaces[selectedPlace]] = "O";
        return currentBoard;
    } else {
        int bestScore = 10000;
        int newScore = 0;
        std::vector<std::string> bestNewBoard;
        std::vector<std::string> possibleNewBoard;
        for (int i = 0; i < 9; i++){
            if (currentBoard[i] == "e"){
                possibleNewBoard = currentBoard;
                possibleNewBoard[i] = "O";
                newScore = minimax(possibleNewBoard, 0, true);
                if (newScore < bestScore){
                    bestScore = newScore;
                    bestNewBoard = possibleNewBoard;
                }
            }
        }
        return bestNewBoard;
    }
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
std::vector<std::string> chooseNewAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, std::vector<double>> qValueTableXAfterStates, int t, struct parameterValues parameter_values, int gameCounter){
    std::vector<std::string> bestAfterstate;
    std::vector<std::vector<std::string>> candidateBoards;
    double bestQValue = -10000;
    double newQValue = 0;
    int count = 0;
    std::vector<std::string> possibleNewBoard;

    /* When the run has come to a point where the user wants to
        switch to greedy selection, select the afterstate with
        the highest q-value. */
    if (gameCounter >= (parameter_values.gamesPerRun - parameter_values.lastGreedyGames)){
        bestAfterstate = findBestAfterstate(currentBoard, qValueTableXAfterStates);
        return bestAfterstate;
    }

    /* If the selected exploration algorithm is Epsilon-greedy, select
        an afterstate randomly out of the current possibilities, with
        probability parameter_values.eValue. Else, continue to find 
        the best/greedy afterstate. */
    if (parameter_values.explorationAlg == 2 && getRandomNumberFromRangeUniform(0, 1) < parameter_values.eValue){
        std::vector<int> freePlaces;
        for (int i = 0; i < 9; i++){
            if (currentBoard[i] == "e"){
                freePlaces.push_back(i);
            }
        }
        int selectedPlace = (int)getRandomNumberFromRangeUniform(0, freePlaces.size());
        currentBoard[freePlaces[selectedPlace]] = "X";
        return currentBoard;
    }
    
    /* Go through all board positions, and find the ones that are
        still free. Then, find which of the possible afterstates
        has the highest Q-value currently. If multiple afterstates
        have the highest Q-value, select from them randomly. */
    for (int i = 0; i < 9; i++){
    
        /* If the position is still free, look up the Q-value
            for the afterstate that is reached when placing
            an X there. */
        if (currentBoard[i] == "e"){
            possibleNewBoard = currentBoard;
            possibleNewBoard[i] = "X";
            newQValue = getQValue(possibleNewBoard, qValueTableXAfterStates);
            
            /* If UCB is used, add the uncertainty to the Q-value, which is calculated
                based on how many times the afterstate was previously selected, and the
                amount of steps that have passed. If this afterstate was never selected,
                it should immediately be selected, so it gets a value of 1000. */
            if (parameter_values.explorationAlg == 0){
                count = getQCount(possibleNewBoard, qValueTableXAfterStates);
                if (count == 0){
                    newQValue = 1000;
                } else{
                    newQValue = uCb(newQValue, parameter_values.cValue, count, t);
                }
            } 

            /* If the Q-value for this afterstate is better than
                the previous best, make this afterstate the new best.
                Randomly select among anyboards which share the best values. */
            if (newQValue > bestQValue){
                candidateBoards.clear();
                candidateBoards.push_back(possibleNewBoard);
                bestQValue = newQValue;
            } else if (newQValue == bestQValue){
                candidateBoards.push_back(possibleNewBoard); 
                
            }    
        }
    }

    /* If only one afterstate had the highest value, return it.
        Else, select randomly from the multiple possibilities. */
    if (candidateBoards.size() == 1){
        return candidateBoards[0];
    } else {
        return boardSelection(candidateBoards);
    }
}

/* Function that randomly selects an afterstate out of multiple
    afterstates in a vector. */
std::vector<std::string> boardSelection(std::vector<std::vector<std::string>> candidateBoards){
    double randNum = getRandomNumberFromRangeUniform(0, candidateBoards.size());
    int index = (int)randNum;
    return candidateBoards[index];
}

/* Function that returns the Count of the input state.*/
double getQCount(std::vector<std::string> afterState, std::map<std::vector<std::string>, std::vector<double>> qValueTable){
    return qValueTable[afterState][1];
}


double uCb(double qValue, double c, int count, double steps) {
    double uCbValue;
    uCbValue = qValue + c*sqrt((log(steps))/count);
    return uCbValue;
}

/* Function that checks if a directory exists. If it doesn't, a new
 * directory with the desired name is created. */
void directoryCheck(const char* directory_name, std::string dir_name){
    struct stat info;
    if (stat(directory_name, &info) != 0){
        if (mkdir(directory_name, 0777) != 0){
            std::cout << "Could not create directory \"" + dir_name + "\", please create it manually.\n";
        } else {
            std::cout << "Created directory \"" + dir_name + "\".\n";
        }
    } else if (!(info.st_mode & S_IFDIR)){
        std::cout << "\"" + dir_name + "\" is not an existing directory\n";
        if (mkdir(directory_name, 0777) != 0){
            std::cout << "Could not create directory \"" + dir_name + "\", please create it manually with that exact name, and restart the experiment.\n";
        } else {
            std::cout << "Created directory \"" + dir_name + "\".\n";
        }
    }
}

/* Function that outputs the won/lost/draw percentages for each game
    to a csv file. */
void createOutputFile(std::vector<std::vector<double>> averagesWonLostDraw, struct parameterValues parameter_values){
    std::ofstream file;
    std::string fileName;
    std::string RLAlg;

    if (parameter_values.RLAlg == 0){
        if (parameter_values.explorationAlg == 0){
            directoryCheck("output-files/q-learning/upper-confidence-bound", "output-files/q-learning/upper-confidence-bound");
            fileName = "runs-" + std::to_string(parameter_values.ammOfRuns) + "_games-" + std::to_string(parameter_values.gamesPerRun) +
                "_alpha-" + std::to_string(parameter_values.alpha) + "_gamma-" + std::to_string(parameter_values.gamma) +
                "_c-" + std::to_string(parameter_values.cValue);
            file.open("output-files/q-learning/upper-confidence-bound/" + fileName + ".csv");
        } else if (parameter_values.explorationAlg == 1){
            directoryCheck("output-files/q-learning/optimistic-initial-values", "output-files/q-learning/optimistic-initial-values");
            fileName = "runs-" + std::to_string(parameter_values.ammOfRuns) + "_games-" + std::to_string(parameter_values.gamesPerRun) +
                "_alpha-" + std::to_string(parameter_values.alpha) + "_gamma-" + std::to_string(parameter_values.gamma) +
                "_initEst-" + std::to_string(parameter_values.oValue);
            file.open("output-files/q-learning/optimistic-initial-values/" + fileName + ".csv");
        } else if (parameter_values.explorationAlg == 2){
            directoryCheck("output-files/q-learning/epsilon-greedy", "output-files/q-learning/epsilon-greedy");
            fileName = "runs-" + std::to_string(parameter_values.ammOfRuns) + "_games-" + std::to_string(parameter_values.gamesPerRun) +
                "_alpha-" + std::to_string(parameter_values.alpha) + "_gamma-" + std::to_string(parameter_values.gamma) +
                "_epsilon-" + std::to_string(parameter_values.eValue);
            file.open("output-files/q-learning/epsilon-greedy/" + fileName + ".csv");
        }
    } else if (parameter_values.RLAlg == 1){
        directoryCheck("output-files/sarsa", "output-files/sarsa");
        if (parameter_values.explorationAlg == 0){
            directoryCheck("output-files/sarsa/upper-confidence-bound", "output-files/q-learning/upper-confidence-bound");
            fileName = "runs-" + std::to_string(parameter_values.ammOfRuns) + "_games-" + std::to_string(parameter_values.gamesPerRun) +
                "_alpha-" + std::to_string(parameter_values.alpha) + "_gamma-" + std::to_string(parameter_values.gamma) +
                "_c-" + std::to_string(parameter_values.cValue);
            file.open("output-files/sarsa/upper-confidence-bound/" + fileName + ".csv");
        } else if (parameter_values.explorationAlg == 1){
            directoryCheck("output-files/sarsa/optimistic-initial-values", "output-files/q-learning/optimistic-initial-values");
            fileName = "runs-" + std::to_string(parameter_values.ammOfRuns) + "_games-" + std::to_string(parameter_values.gamesPerRun) +
                "_alpha-" + std::to_string(parameter_values.alpha) + "_gamma-" + std::to_string(parameter_values.gamma) +
                "_initEst-" + std::to_string(parameter_values.oValue);
            file.open("output-files/sarsa/optimistic-initial-values/" + fileName + ".csv");
        } else if (parameter_values.explorationAlg == 2){
            directoryCheck("output-files/sarsa/epsilon-greedy", "output-files/q-learning/epsilon-greedy");
            fileName = "runs-" + std::to_string(parameter_values.ammOfRuns) + "_games-" + std::to_string(parameter_values.gamesPerRun) +
                "_alpha-" + std::to_string(parameter_values.alpha) + "_gamma-" + std::to_string(parameter_values.gamma) +
                "_epsilon-" + std::to_string(parameter_values.eValue);
            file.open("output-files/sarsa/epsilon-greedy/" + fileName + ".csv");    
        }
    }

    file << "game number,percentage won,percentage lost,percentage draw\n";
    for (int i = 0; i < parameter_values.gamesPerRun; i++){
        file << i+1 << "," << averagesWonLostDraw[i][0] << "," << averagesWonLostDraw[i][1] << "," << averagesWonLostDraw[i][2] << "\n";
    }

    file.close();
}


/* Function that finds the best possible afterstate from the current board. */
std::vector<std::string> findBestAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, std::vector<double>> qValueTableXAfterStates){
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

/* Function that goes through all the boards in the map
    that was already generated, and reassigns it the 
    correct starting values for the next run. */
void resetQValueTable(std::map<std::vector<std::string>, std::vector<double>> &qValueTableXAfterstates, struct parameterValues parameter_values){
    std::string gameResult;
    for (auto const& x : qValueTableXAfterstates){
        gameResult = getGameResult(x.first);

        /* If in this board state X has won, give it
            value 1. If the game state is a draw,
            give it value 0. If OIV is being used,
            give any other afterstates the requested
            parameter value. Else, give it value 0. */
        if (gameResult == "X"){
            qValueTableXAfterstates[x.first] = {1,0};
        } else if (gameResult == "draw"){
            qValueTableXAfterstates[x.first] = {0,0};
        } else if (parameter_values.explorationAlg == 1){
            qValueTableXAfterstates[x.first] = {parameter_values.oValue,0};
        } else {
            qValueTableXAfterstates[x.first] = {0,0};
        }
    }
}


/* Function that creates a q-value table for each possible board
    state. Can probably be used to update afterstates for both
    player X and player O, as those don't overlap. */
std::map<std::vector<std::string>, std::vector<double>> generateQValueTableXAfterStates(struct parameterValues parameter_values){
    std::map<std::vector<std::string>, std::vector<double>> qValueTableXAfterStates;
    std::vector<std::string> newBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
    std::vector<std::string> possibleSymbols = {"e", "X", "O"};
    int xCount = 0;
    int oCount = 0;
    std::string gameResult;
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
                                                value 1. If the game state is a draw,
                                                give it value 0. If OIV is being used,
                                                give any other afterstates the requested
                                                parameter value. Else, give it value 0. 
                                                It doesn't matter too much if it
                                                is invalid, as those won't be reached in 
                                                the game. */
                                            gameResult = getGameResult(newBoard);
                                            if (gameResult == "X"){
                                                qValueTableXAfterStates[newBoard] = {1,0};
                                            } else if (gameResult == "draw"){
                                                qValueTableXAfterStates[newBoard] = {0,0};
                                            } else if (parameter_values.explorationAlg == 1){
                                                qValueTableXAfterStates[newBoard] = {parameter_values.oValue,0}; 
                                            } else{
                                                qValueTableXAfterStates[newBoard] = {0,0};
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
double getQValue(std::vector<std::string> afterState, std::map<std::vector<std::string>, std::vector<double>> qValueTable){
    return qValueTable[afterState][0];
}


/* Function that returns a random decimal number [lower_limit, upper_limit]. */
double getRandomNumberFromRangeUniform(int lower_limit, int upper_limit){
    std::uniform_real_distribution<double> distribution(lower_limit, upper_limit);
    return distribution(generator);
}

/* Function that asks the user to provide the parameter values before
    starting the learning phase. */
void initialiseExperiment(struct parameterValues &parameter_values){
    std::cout << "Please choose the reinforcement learning algorithm you want to use:\n    0) Q-learning\n    1) Sarsa\n";
    std::cin >> parameter_values.RLAlg;

    std::cout << "\nPlease choose the exploration/exploitation algorithm you want to use:\n    0) Upper-Confidence-Bound\n    1) Optimistic initial values\n    2) Epsilon-greedy\n";
    std::cin >> parameter_values.explorationAlg;

    if (parameter_values.explorationAlg == 0){
        std::cout << "\nPlease choose the value for c (UCB Algorithm) you want to use:\n";
        std::cin >> parameter_values.cValue;
    } else{
        parameter_values.cValue = 5;
    }

    if (parameter_values.explorationAlg == 1){
        std::cout << "\nPlease choose the optimistic initial value you want to use (between 0 and 1):\n";
        std::cin >> parameter_values.oValue;
    } else{
        parameter_values.oValue = 5;
    }

    if (parameter_values.explorationAlg == 2){
        std::cout << "\n Please choose the epsilon-value you want to use (between 0 and 1):\n";
        std::cin >> parameter_values.eValue;
    }
    
    std::cout << "\nPlease declare the amount of runs:\n";
    std::cin >> parameter_values.ammOfRuns;

    std::cout << "\nPlease declare the total amount of games to be played (including the greedy ones at the end, if relevant):\n";
    std::cin >> parameter_values.gamesPerRun;

    std::cout << "\nPlease declare how many games at the end should be played greedily:\n";
    std::cin >> parameter_values.lastGreedyGames;

    std::cout << "\nPlease declare the value for alpha:\n";
    std::cin >> parameter_values.alpha;

    std::cout << "\nPlease declare the value for gamma:\n";
    std::cin >> parameter_values.gamma;
}

/* Minimax algorithm. */
int minimax(std::vector<std::string> board, int depth, bool isMaximising){
    std::string gameResult = getGameResult(board);
    if (gameResult != "not ended"){
        if (gameResult == "X"){
            return 1;
        } else if (gameResult == "O"){
            return -1;
        } else if (gameResult == "draw"){
            return 0;
        }
    }

    if (isMaximising){
        int bestScore = -10000;
        int newScore = 0;
        std::vector<std::string> possibleNewBoard;
        for (int i = 0; i < 9; i++){
            if (board[i] == "e"){
                possibleNewBoard = board;
                possibleNewBoard[i] = "X";
                newScore = minimax(possibleNewBoard, depth+1, false);
                if (newScore > bestScore){
                    bestScore = newScore;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 10000;
        int newScore = 0;
        std::vector<std::string> possibleNewBoard;
        for (int i = 0; i < 9; i++){
            if (board[i] == "e"){
                possibleNewBoard = board;
                possibleNewBoard[i] = "O";
                newScore = minimax(possibleNewBoard, depth+1, true);
                if (newScore < bestScore){
                    bestScore = newScore;
                }
            }
        }
        return bestScore;
    }
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

/* Function that calculates and prints the mean and standard deviation
 * of the amount of games won/lost/drawn for all runs. */
void printMeanAndStandardDeviation(std::vector<std::vector<int>> sumWonLostDrawCount){
    double meanWon = 0;
    double meanLost = 0;
    double meanDraw = 0;
    for (int i = 0; i < sumWonLostDrawCount.size(); i++){
        meanWon += sumWonLostDrawCount[i][0];
        meanLost += sumWonLostDrawCount[i][1];
        meanDraw += sumWonLostDrawCount[i][2];
    }
    meanWon /= sumWonLostDrawCount.size();
    meanLost /= sumWonLostDrawCount.size();
    meanDraw /= sumWonLostDrawCount.size();

    std::cout << "Mean amount of games won: " << meanWon << "\n";
    std::cout << "Mean amount of games lost: " << meanLost << "\n";
    std::cout << "Mean amount of games draw: " << meanDraw << "\n";

    double sumOfSquaredDifferencesWon = 0;
    double sumOfSquaredDifferencesLost = 0;
    double sumOfSquaredDifferencesDraw = 0;
    for (int i = 0; i < sumWonLostDrawCount.size(); i++){
        sumOfSquaredDifferencesWon += pow(sumWonLostDrawCount[i][0] - meanWon, 2);
        sumOfSquaredDifferencesLost += pow(sumWonLostDrawCount[i][1] - meanLost, 2);
        sumOfSquaredDifferencesDraw += pow(sumWonLostDrawCount[i][2] - meanDraw, 2);
    }
    sumOfSquaredDifferencesWon /= sumWonLostDrawCount.size();
    sumOfSquaredDifferencesLost /= sumWonLostDrawCount.size();
    sumOfSquaredDifferencesDraw /= sumWonLostDrawCount.size();

    double standardDeviationWon = sqrt(sumOfSquaredDifferencesWon);
    double standardDeviationLost = sqrt(sumOfSquaredDifferencesLost);
    double standardDeviationDraw = sqrt(sumOfSquaredDifferencesDraw);
    std::cout << "Standard deviation amount of games won: " << standardDeviationWon << "\n";
    std::cout << "Standard deviation amount of games lost: " << standardDeviationLost << "\n";
    std::cout << "Standard deviation amount of games drawn: " << standardDeviationDraw << "\n";
}

/* Function that updates the Q-value for the given afterState, based on the
    the Q-value of the best afterState than can be reached after O has made 
    a move (Q-learning). If Sarsa is chosen, the Q-value of the 
    afterstate isn't updated with the best possible afterstate after O has
    made a move, but the actual next afterstate chosen by X.
    In tic-tac-toe there are no rewards associated with single moves,
    only +1, -1, or 0 at the end. That's why there is no reward factor in this
    update equation. */
void updateAfterstateQValue(struct parameterValues parameter_values, std::vector<std::string> afterState, std::map<std::vector<std::string>, std::vector<double>> &qValueTableXAfterStates, double bestAfterAfterstateQValue){
    qValueTableXAfterStates[afterState][0] += parameter_values.alpha*(parameter_values.gamma*bestAfterAfterstateQValue - qValueTableXAfterStates[afterState][0]);
}