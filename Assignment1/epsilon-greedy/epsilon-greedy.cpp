#include <iostream>
#include <vector>
#include "../experimentValuesStruct.hpp"
#include "../general-functions.hpp"

/* Function that calculates the new action value estimate for the last
 * chosen action, with an equation of the form newEstimate = oldEstimate +
 * 1/(number of times action was chosen)*(newReward - oldEstimate).
 */
void updateEstimate(std::vector<double> &action_value_estimates, std::vector<int> &action_counter, int action, double reward){
    action_counter[action] += 1;
    action_value_estimates[action] = action_value_estimates[action] + (1.0/action_counter[action])*(reward - action_value_estimates[action]);
}

double getReward(double true_action_value){
    return getRandomNumberFromRangeNormal(true_action_value, 1);
}

void epsilonGreedyMain(struct experimentValues experiment_values){
    srand(time(0));
    double epsilon;
    std::cout << "Please choose a value for epsilon on the range [0, 1]:\n";
    std::cin >> epsilon;
    int action;

    for (int round = 0; round < /*experiment_values.N*/ 1; round++){
        /* Initialise the K arms/the true action values, with the index of
         * the optimal action at the end.
         */
        std::vector<double> bandit = initialiseBandit(experiment_values.K);

        /* Initialise the K action value estimates, each starting at 0. 
         * Also initialise a vector that counts how many times each action
         * was chosen. 
         */
        std::vector<double> action_value_estimates;
        std::vector<int> action_counter;
        for (int i = 0; i < experiment_values.K; i++){
            action_value_estimates.push_back(0.0);
            action_counter.push_back(0);
        }

        for (int step = 0; step < experiment_values.T; step++){
            if (getRandomNumberFromRangeUniform(0, 1) <= epsilon){
                /* Choose a random action. */
                action = getRandomIntFromRangeUniform(0, experiment_values.K);
            } else {
                /* Find the action that has the highest action value estimate. */
                action = getIndexOfLargestValue(action_value_estimates);
            }
            updateEstimate(action_value_estimates, action_counter, action, getReward(bandit[action]));
        }

        std::cout << "true optimal action: " << bandit[experiment_values.K] << "\n";
        for (int i = 0; i < experiment_values.K; i++){
            std::cout << i << ": \n";
            std::cout << "    actual: " << bandit[i] << "\n";
            std::cout << "    estimate: " << action_value_estimates[i] << "\n";
            std::cout << "    amount of times chosen: " << action_counter[i] << "\n";
        }
    }

}