#include <iostream>
#include <vector>
#include "../experimentValuesStruct.hpp"
#include "../general-functions.hpp"
#include <math.h>

/* This file contains the code of the experiment loop specifically
 * for the gradient-based algorithm, as it is a bit different from
 * the other three algorithms. */



void experimentLoopGradient(struct experimentValues experiment_values){
    std::cout << "Please choose a value for the step-size parameter alpha on the range [0, 1]:\n";
    std::cin >> experiment_values.alpha;

    std::cout << "\nLearning phase starts now, please wait for the results!\n";

    int action;

    /* Keeps track of the total_reward after each round. */
    std::vector<double> total_rewards;
    double reward;
    /* The sum of all rewards in one round. */
    double total_reward;
    /* The average of all awards received until the current round. */
    double average_reward;

    /* Average percentage of having chosen the optimal action at each 
     * time step, over all rounds, initialised at 0 for each time step. */
    std::vector<double> averaged_percentage_optimal_action_chosen(experiment_values.T, 0.0);
    /* Percentage of having chosen the optimal action at each time step. */
    std::vector<double> percentage_optimal_action_chosen;

    /* Average reward for each timestep, over all rounds. */
    std::vector<double> all_rewards_averaged(experiment_values.T, 0.0);
    /* Reward for each timestep. */
    std::vector<double> all_rewards;

    std::vector<double> bandit;
    std::vector<double> action_preferences;
    std::vector<int> action_counter;
    std::vector<double> action_probabilities;
    

    for (int round = 0; round < experiment_values.N; round++){
        total_reward = 0;
        average_reward = 0;

        /* Initialise the K arms/the true action values, with the index of
         * the optimal action at the end. */
        bandit = initialiseBandit(experiment_values.K, experiment_values.distribution);

        /* Initialise the K action preferences, each starting at 0.
         * Also initialise a vector that counts how many times each 
         * action was chosen, and a vector containing the probabilities
         * of each action (all 1/K at the start). */
        for (int i = 0; i < experiment_values.K; i++){
            action_preferences.push_back(0.0);
            action_counter.push_back(0);
            action_probabilities.push_back(1/experiment_values.K);
        }

        for (int step = 0; step < experiment_values.T; step++){
            action = selectAction(experiment_values, action_preferences, action_probabilities);
            action_counter[action] += 1;

            /* Calculate the reward. Use the reward to update the estimate
             * for that action. Add the reward to the total_reward sum for
             * this round, and save it in the all_rewards vector. */
            reward = getReward(bandit[action], experiment_values);
            updateAverageReward(average_reward, reward, step);
            updateActionPreferences(action, reward, action_preferences, action_probabilities, average_reward, experiment_values.alpha);
            all_rewards.push_back(reward);
            total_reward += reward;

            /* Calculate the current percentage of having chosen the true optimal
             * action out of the total number of chosen actions. */
            percentage_optimal_action_chosen.push_back(100*((double)action_counter[bandit[experiment_values.K]]/(step+1)));
        }

        /* Add the total_rewards sum for the last round to the vector
         * that stores the total_rewards sums for all rounds. */
        total_rewards.push_back(total_reward);

        /* Update the vector that keeps track of the average percentage
         * of having chosen the true optimal action at each time step
         * over all rounds. */
        integratePercentageOptimalActionChosen(averaged_percentage_optimal_action_chosen, percentage_optimal_action_chosen, round);
        percentage_optimal_action_chosen.clear();

        /* Update the vector that keeps track of the average reward
         * at each time step. */
        integrateAllRewards(all_rewards_averaged, all_rewards, round);
        all_rewards.clear();

        action_preferences.clear();
        action_counter.clear();
        action_probabilities.clear();
    }

    for (int i = 0; i < experiment_values.T; i += 100){
        std::cout << i << ": \n";
        std::cout << "    average reward: " << all_rewards_averaged[i] << "\n";
        std::cout << "    percentage optimal action: " << averaged_percentage_optimal_action_chosen[i] << "\n";
    }

    printMeanAndStandardDeviation(total_rewards);

    createOutputFile(experiment_values, all_rewards_averaged, averaged_percentage_optimal_action_chosen);


    


}