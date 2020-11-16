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
    /* Average percentage of having chosen the optimal action at each 
     * time step, over all rounds, initialised at 0 for each time step. 
     */
    std::vector<double> averaged_percentage_optimal_action_chosen(experiment_values.T, 0.0);
    /* Percentage of having chosen the optimal action at each time step. */
    std::vector<double> percentage_optimal_action_chosen;

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

        action_value_estimates.clear();
        action_counter.clear();

    }

    for (int i = 0; i < experiment_values.T; i += 100){
        std::cout << i << ": \n";
        std::cout << "    average reward: " << all_rewards_averaged[i] << "\n";
        std::cout << "    percentage optimal action: " << averaged_percentage_optimal_action_chosen[i] << "\n";
    }

    double mean = 0;
    for (int i = 0; i < total_rewards.size(); i++){
        mean += total_rewards[i];
    }
    mean /= total_rewards.size();
    std::cout << "mean total reward: " << mean << "\n";

    double sum_of_squared_differences = 0;
    for (int i = 0; i < total_rewards.size(); i++){
        sum_of_squared_differences += pow(total_rewards[i] - mean, 2);
    }
    sum_of_squared_differences /= total_rewards.size();

    double standard_deviation = sqrt(sum_of_squared_differences);
    std::cout << "standard deviation total reward: " << standard_deviation << "\n";

    std::ofstream file;
    std::string file_name;
    switch(experiment_values.algorithm){
        case 0:
            file_name = "dist-" + std::to_string(experiment_values.distribution) + "_K-" + std::to_string(experiment_values.K) +
                "_runs-" + std::to_string(experiment_values.N) + "_steps-" + std::to_string(experiment_values.T) + "_algorithm-" + std::to_string(experiment_values.algorithm) +
                "_epsilon-" + std::to_string(experiment_values.epsilon);
            file.open("epsilon-greedy/" + file_name + ".csv");
            break;
        case 1:
            file_name = "dist-" + std::to_string(experiment_values.distribution) + "_K-" + std::to_string(experiment_values.K) +
                "_runs-" + std::to_string(experiment_values.N) + "_steps-" + std::to_string(experiment_values.T) + "_algorithm-" + std::to_string(experiment_values.algorithm) +
                "_epsilon-" + std::to_string(experiment_values.epsilon) + "_initEst-" + std::to_string(experiment_values.initial_estimate);
            file.open("optimistic-initial-values/" + file_name + ".csv");
            break;
        case 2: 
            file_name = "dist-" + std::to_string(experiment_values.distribution) + "_K-" + std::to_string(experiment_values.K) +
                "_runs-" + std::to_string(experiment_values.N) + "_steps-" + std::to_string(experiment_values.T) + "_algorithm-" + std::to_string(experiment_values.algorithm) +
                "_c-" + std::to_string(experiment_values.c);
            file.open("upper-confidence-bound/" + file_name + ".csv");
            break;
        case 3:
            //something
            break;
    }

    file << "average reward,percentage optimal action\n";
    for (int i = 0; i < experiment_values.T; i++){
        file << all_rewards_averaged[i] << "," << averaged_percentage_optimal_action_chosen[i] << "\n";
    }

    file.close();

}