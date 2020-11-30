#include <iostream>
#include <vector>
#include "../experimentValuesStruct.hpp"
#include "../general-functions.hpp"
#include <math.h>
#include <fstream>




void experimentLoop(struct experimentValues experiment_values){
    /* In Epsilon-Greedy and Optimistic Initial Values an epsilon value can be used. */
    if (experiment_values.algorithm == 0 || experiment_values.algorithm == 1){
        /* User is asked to put in a value for epsilon. */
        std::cout << "Please choose a value for epsilon on the range [0, 1]\n";
        std::cin >> experiment_values.epsilon;

        /* In Optimistic Initial Values, an initial action value estimate is required. */
        if (experiment_values.algorithm == 1){
            std::cout << "Please choose an initial action value estimate:\n";
            std::cin >> experiment_values.initial_estimate;
        }
    /* In Upper-Confidence-Bound action selection a value c > 0 denoting the
     * degree of exploration is required.
     */
    } else if (experiment_values.algorithm == 2){
        std::cout << "Please choose the degree of exploration, c > 0:\n";
        std::cin >> experiment_values.c;
    }

    std::cout << "\nLearning phase starts now, please wait for the results!\n";

    int action;
    /* Keeps track of the total_reward after each round. */
    std::vector<double> total_rewards;
    double reward;
    /* The sum of all rewards in one round. */
    double total_reward;
    /* Total sum for each time step if the optimal action was chosen in that time
        step, over all runs. */
    std::vector<double> sum_optimal_action_chosen(experiment_values.T, 0.0);
    /* Vector that keeps track for each time step if the optimal action was chosen in that time step. */
    std::vector<double> optimal_action_chosen;

    /* Average reward for each timestep, over all rounds. */
    std::vector<double> all_rewards_averaged(experiment_values.T, 0.0);
    /* Reward for each timestep. */
    std::vector<double> all_rewards;

    std::vector<double> bandit;
    std::vector<double> action_value_estimates;
    std::vector<int> action_counter;

    
    for (int round = 0; round < experiment_values.N; round++){
        total_reward = 0;

        /* Initialise the K arms/the true action values, with the index of
         * the optimal action at the end.
         */
        bandit = initialiseBandit(experiment_values.K, experiment_values.distribution);

        /* Initialise the K action value estimates, each starting at initial_estimate. 
         * Also initialise a vector that counts how many times each action
         * was chosen. 
         */
        for (int i = 0; i < experiment_values.K; i++){
            action_value_estimates.push_back(experiment_values.initial_estimate);
            action_counter.push_back(0);
        }


        for (int step = 0; step < experiment_values.T; step++){
            action = selectAction(experiment_values, action_value_estimates, action_counter, step);

            /* Calculate the reward. Use the reward to update the estimate
             * for that action. Add the reward to the total_reward sum for
             * this round, and save it in the all_rewards vector. */
            reward = getReward(bandit[action], experiment_values);
            updateValueEstimate(action_value_estimates, action_counter, action, reward);
            all_rewards.push_back(reward);
            total_reward += reward;

            /* Check if the chosen action was the optimal action.
             *  If so, store 1 for this time step, else store 0. */
            if (action == bandit[experiment_values.K]){
                optimal_action_chosen.push_back(1);
            } else {
                optimal_action_chosen.push_back(0);
            }
        }

        /* Add the total_rewards sum for the last round to the vector
         * that stores the total_rewards sums for all rounds. */
        total_rewards.push_back(total_reward);

        /* Update the vector that keeps track of sum of having chosen 
         * the true optimal action at each time step over all rounds. */
        for (int i = 0; i < experiment_values.T; i++){
            sum_optimal_action_chosen[i] += optimal_action_chosen[i];
        }
        optimal_action_chosen.clear();

        /* Update the vector that keeps track of the average reward
         * at each time step. */
        integrateAllRewards(all_rewards_averaged, all_rewards, round);
        all_rewards.clear();

        action_value_estimates.clear();
        action_counter.clear();

    }

    for (int i = 0; i < experiment_values.T; i++){
        sum_optimal_action_chosen[i] /= experiment_values.N;
        sum_optimal_action_chosen[i] *= 100;
    }

    for (int i = 0; i < experiment_values.T; i += 100){
        std::cout << i << ": \n";
        std::cout << "    average reward: " << all_rewards_averaged[i] << "\n";
        std::cout << "    percentage optimal action: " << sum_optimal_action_chosen[i] << "\n";
    }

    printMeanAndStandardDeviation(total_rewards);

    createOutputFile(experiment_values, all_rewards_averaged, sum_optimal_action_chosen);

}