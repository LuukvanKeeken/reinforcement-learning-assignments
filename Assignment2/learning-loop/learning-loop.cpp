#include <iostream>
#include <vector>
#include "../parameterValuesStruct.hpp"
#include "../BoardHasherStruct.hpp"
#include "../general-functions.hpp"
#include "../boardStateStruct.hpp"
#include <ctime>
#include <cmath>
#include <unordered_map>

/* Function that lets the agent play a certain amount of games
    (parameter_values.gamesPerRun), and repeats this for a certain
    number of runs (parameter_values.ammOfRuns). For each run, 
    the program keeps track of the result of each game. At the end,
    averages are calculated for whether the agent won/lost/drew a 
    certain game, over all runs. Also, the total amount of games
    won/lost/drawn are kept track of for each run, and at the end
    the means and standard deviations of those totals are calculated. */
void learningLoop(struct parameterValues parameter_values){
    srand(time(NULL));

    /* Vector of vectors that keeps track of whether player X won, lost, or
        drew the game corresponding to its index. */
    std::vector<std::vector<double>> wonLostDraw;
    /* Vector of vectors that sums the wonLostDraw vectors over
        all runs. In the end its averaged over all runs. Initialised
        with gamesPerRun amount of {0.0, 0.0, 0.0} vectors. */
    std::vector<std::vector<double>> sumsWonLostDraw(parameter_values.gamesPerRun, {0.0, 0.0, 0.0});
    
    /* Vector keeping track of the total amount of wins,
        losses, and draws for X for one run. */
    std::vector<int> wonLostDrawCount = {0, 0, 0};
    /* Vector of vectors keeping track of the wonLostDrawCount
        for all runs. Can later be used to calculate the total 
        wins/losses/draws over all runs. */
    std::vector<std::vector<int>> sumWonLostDrawCount;
    
    
    std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> qValueTableXAfterStates;
    struct boardState currentBoard;
    struct boardState previousBoard;
    char gameResult;
    int gameCounter = 0;
    int stepsTotal = 0;
    int steps = 0;

    /* Initialise the q-value table for X's afterstates. */
    qValueTableXAfterStates = generateQValueTableXAfterStates(parameter_values);

    for (int run = 0; run < parameter_values.ammOfRuns; run++){
        
        /* Board is initialised with each cell empty ("e"). */
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                currentBoard.board[i][j] = 'e';
            }
        }
        
        std::cout << "Start of run " << run << "\n";
        while (gameCounter < parameter_values.gamesPerRun){
            // std::cout << "Game number: " << gameCounter << "\n";
            steps += 1;
            /* Select the next board from the possible afterstates and adds to the count: number of times that board has been selected. */
            currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates, steps, parameter_values, gameCounter);
            // std::cout << "X plays:\n";
            // printBoard(currentBoard);
            // std::cout << "test7\n";
            qValueTableXAfterStates[currentBoard][1]+=1;
            // std::cout << "test8\n";

            /* Check whether X has won, if there's a draw, or
                if the game hasn't ended yet. X cannot have
                lost after its own move. */
            gameResult = getGameResult(currentBoard);
            if (gameResult == 'n'){
                // std::cout << "not ended X\n";
                /* If the RL algorithm is Sarsa, the Q-value of the previous afterState can be
                    updated, now that X has chosen a new afterState. */
                if (steps > 1 && parameter_values.RLAlg == 1){
                    updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, getQValue(currentBoard, qValueTableXAfterStates));
                }

                /* Save the currentBoard in previousBoard, so that its
                    Q-value can be updated after O has made a move. */
                previousBoard = currentBoard;

                /* Let the opponent select his action and update the board. */
                currentBoard = actionByO(currentBoard);
                // std::cout << "O plays:\n";
                // printBoard(currentBoard);
                // std::cout << "after actionByO\n";
                steps += 1;

                /* Check if after O's move X has now lost, or if
                    the game hasn't ended. A win for X or a draw
                    aren't possible after O's move. */
                gameResult = getGameResult(currentBoard);
                // std::cout << "after getGameResult\n";
                if (gameResult == 'n'){
                    // std::cout << "not ended O\n";
                    /* Game hasn't ended, update previous board/afterstate q-value with the 
                        q-value of the best possible next afterstate as the target. Only
                        for Q-learning.*/
                    if (parameter_values.RLAlg == 0){
                        updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, getQValue(findBestAfterstate(currentBoard, qValueTableXAfterStates), qValueTableXAfterStates));
                    }
                } else if (gameResult == 'O'){
                    // std::cout << "O won\n";
                    /* Lost, update previous board/afterstate q-value with -1 as target, start new run.
                        This works the same for both Q-learning and Sarsa, i.e. update the Q-value of the
                        last afterstate chosen by X with -1 as the target. */
                    updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, -1);
                    
                    
                    for (int i = 0; i < 3; i++){
                        for (int j = 0; j < 3; j++){
                            currentBoard.board[i][j] = 'e';
                        }
                    }
                    gameCounter += 1;
                    wonLostDraw.push_back({0.0, 1.0, 0.0});
                    wonLostDrawCount[1] += 1;
                }
            } else if (gameResult == 'X'){
                /* Won, afterstate already has value 1, start new run. */
                /* If the RL algorithm is Sarsa, the Q-value of the previous afterState can be
                    updated, now that X has chosen a new afterState. As X has won with this
                    new afterState, its value is 1.*/
                if (steps > 1 && parameter_values.RLAlg == 1){
                    updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, 1);
                }
                for (int i = 0; i < 3; i++){
                    for (int j = 0; j < 3; j++){
                        currentBoard.board[i][j] = 'e';
                    }
                }
                gameCounter += 1;
                wonLostDraw.push_back({1.0, 0.0, 0.0});
                wonLostDrawCount[0] += 1;
            } else if (gameResult == 'd'){
                /* Draw, afterstate already has value 0, start new run. */
                /* If the RL algorithm is Sarsa, the Q-value of the previous afterState can be
                    updated, now that X has chosen a new afterState. As there is a draw with
                    this new afterState, the value is 0.*/
                if (steps > 1 && parameter_values.RLAlg == 1){
                    updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, 0);
                }
                for (int i = 0; i < 3; i++){
                    for (int j = 0; j < 3; j++){
                        currentBoard.board[i][j] = 'e';
                    }
                }
                gameCounter += 1;
                wonLostDraw.push_back({0.0, 0.0, 1.0});
                wonLostDrawCount[2] += 1;
            }
        }

        /* Update the sumWonLostDraw vector. */
        for (int i = 0; i < parameter_values.gamesPerRun; i++){
            sumsWonLostDraw[i][0] += wonLostDraw[i][0];
            sumsWonLostDraw[i][1] += wonLostDraw[i][1];
            sumsWonLostDraw[i][2] += wonLostDraw[i][2];
        }

        /* Add the vector keeping track of wins/losses/draws
            in the past run to the vector that saves all those
            vectors. */
        sumWonLostDrawCount.push_back(wonLostDrawCount);

        /* Clear the wonLostDraw vector for the next run. */
        wonLostDraw.clear();

        /* Clear the wonLostDrawCount vector keeping track
            of the total wins/losses/draws for the past run. */
        wonLostDrawCount = {0, 0, 0};

        /* Reset the afterstates in the map to the correct
            values, if there is a next run. */
        if (run != parameter_values.ammOfRuns - 1){
            resetQValueTable(qValueTableXAfterStates, parameter_values);
        }

        /* Reset gameCounter for the next run. */
        gameCounter = 0;
        stepsTotal += steps;
        steps = 0;
    }

    /* Calculate the percentages of having won/lost/drawn in a game,
        averaged over all runs. */
    for (int i = 0; i < parameter_values.gamesPerRun; i++){
        sumsWonLostDraw[i][0] = (sumsWonLostDraw[i][0]/parameter_values.ammOfRuns)*100;
        sumsWonLostDraw[i][1] = (sumsWonLostDraw[i][1]/parameter_values.ammOfRuns)*100;
        sumsWonLostDraw[i][2] = (sumsWonLostDraw[i][2]/parameter_values.ammOfRuns)*100;
    }

    if (parameter_values.lastGreedyGames > 0){
        double greedyScoreSum = 0;
        for (int i = (sumsWonLostDraw.size() - 1);  i > (sumsWonLostDraw.size() - 1 - parameter_values.lastGreedyGames); i--){
            greedyScoreSum += (sumsWonLostDraw[i][0] - sumsWonLostDraw[i][1])/100;
        }

        greedyScoreSum /= parameter_values.lastGreedyGames;

        double sumOfSquaredDifferences = 0;
        for (int i = (sumsWonLostDraw.size() - 1);  i > (sumsWonLostDraw.size() - 1 - parameter_values.lastGreedyGames); i--){
            sumOfSquaredDifferences += pow((sumsWonLostDraw[i][0] - sumsWonLostDraw[i][1])/100 - greedyScoreSum, 2);
        }

        sumOfSquaredDifferences /= parameter_values.lastGreedyGames;
        sumOfSquaredDifferences = sqrt(sumOfSquaredDifferences);


        std::cout << "Average score over last 20 greedy games, averaged over all runs: " << greedyScoreSum << "\n";
        std::cout << "Standard deviation of average score over last 20 greedy games, averaged over all runs: " << sumOfSquaredDifferences << "\n";
    }
    

    printMeanAndStandardDeviation(sumWonLostDrawCount);

    createOutputFile(sumsWonLostDraw, parameter_values);    



    std::cout << "Total amount of steps: " << stepsTotal << "\n";
    std::cout << "Average amount of steps per run: " << (double)stepsTotal/(double)parameter_values.ammOfRuns << "\n";
    std::cout << "Average amount of steps per game: " << ((double)stepsTotal/(double)parameter_values.ammOfRuns)/(double)parameter_values.gamesPerRun << "\n";
}