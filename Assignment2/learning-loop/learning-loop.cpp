#include <iostream>
#include <vector>
#include "../parameterValuesStruct.hpp"
#include "../general-functions.hpp"

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
    


    std::map<std::vector<std::string>, double> qValueTableXAfterStates;
    std::vector<std::string> currentBoard;
    std::vector<std::string> previousBoard;
    std::string gameResult;
    int gameCounter = 0;

    for (int run = 0; run < parameter_values.ammOfRuns; run++){
        /* Initialise the q-value table for X's afterstates. */
        qValueTableXAfterStates = generateQValueTableXAfterStates();
        
        /* Board is initialised with each cell empty ("e"). */
        currentBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
        
        std::cout << "Start of run " << run << "\n";
        while (gameCounter < parameter_values.gamesPerRun){
            /* Select the next board from the possible afterstates. */
            currentBoard = chooseNewAfterstate(currentBoard, qValueTableXAfterStates);
            // std::cout << "X PLAYS:\n";
            // printBoard(currentBoard);
            // std::cout << "\n";


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
                // std::cout << "O PLAYS:\n";
                // printBoard(currentBoard);
                // std::cout << "\n";

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
                    //std::cout << "LOSS\n\n";
                    updateAfterstateQValue(parameter_values, previousBoard, qValueTableXAfterStates, -1);
                    currentBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
                    gameCounter += 1;
                    wonLostDraw.push_back({0.0, 1.0, 0.0});
                    wonLostDrawCount[1] += 1;
                }
            } else if (gameResult == "X"){
                /* Won, afterstate already has value 1, start new run. */
                //std::cout << "WIN\n\n";
                currentBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
                gameCounter += 1;
                wonLostDraw.push_back({1.0, 0.0, 0.0});
                wonLostDrawCount[0] += 1;
            } else if (gameResult == "draw"){
                /* Draw, afterstate already has value 0, start new run. */
                //std::cout << "DRAW\n\n";
                currentBoard = {"e", "e", "e", "e", "e", "e", "e", "e", "e"};
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

        /* Clear the q-value table for the next run. */
        qValueTableXAfterStates.clear();

        /* Reset gameCounter for the next run. */
        gameCounter = 0;
    }

    /* Calculate the percentages of having won/lost/drawn in a game,
        averaged over all runs. */
    for (int i = 0; i < parameter_values.gamesPerRun; i++){
        sumsWonLostDraw[i][0] = (sumsWonLostDraw[i][0]/parameter_values.ammOfRuns)*100;
        sumsWonLostDraw[i][1] = (sumsWonLostDraw[i][1]/parameter_values.ammOfRuns)*100;
        sumsWonLostDraw[i][2] = (sumsWonLostDraw[i][2]/parameter_values.ammOfRuns)*100;
    }

    printMeanAndStandardDeviation(sumWonLostDrawCount);
}