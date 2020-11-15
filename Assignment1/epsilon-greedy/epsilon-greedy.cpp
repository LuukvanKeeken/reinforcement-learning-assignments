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

/* Function that gets a reward from the normal distribution with
 * as the mean the true action value and unit standard deviation.
 */
double getReward(double true_action_value){
    return getRandomNumberFromRangeNormal(true_action_value, 1);
}

/* Function that is called after each round to calculate the new
 * average percentage of the true optimal action having been chosen
 * at each time step.
 */
void integratePercentageOptimalActionChosen(std::vector<double> &averaged_p_o_a_c, std::vector<double> p_o_a_c, int round){
    for (int i = 0; i < averaged_p_o_a_c.size(); i++){
        averaged_p_o_a_c[i] = averaged_p_o_a_c[i] + (1.0/((double)round+1)*(p_o_a_c[i] - averaged_p_o_a_c[i]));
    }
}

/* Function that is called after each round to calculate the new
 * average reward at each time step.
 */
void integrateAllRewards(std::vector<double> &all_rewards_averaged, std::vector<double> all_rewards, int round){
    for (int i = 0; i < all_rewards_averaged.size(); i++){
        all_rewards_averaged[i] = all_rewards_averaged[i] + (1.0/((double)round+1)*(all_rewards[i] - all_rewards_averaged[i]));
    }
}

void epsilonGreedyMain(struct experimentValues experiment_values){
    /* User is asked to put in a value for epsilon. */
    double epsilon;
    std::cout << "Please choose a value for epsilon on the range [0, 1]:\n";
    std::cin >> epsilon;
    std::cout << "Learning phase starts now, please wait for the results!\n";

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
        bandit = initialiseBandit(experiment_values.K);

        /* Initialise the K action value estimates, each starting at 0. 
         * Also initialise a vector that counts how many times each action
         * was chosen. 
         */
        for (int i = 0; i < experiment_values.K; i++){
            action_value_estimates.push_back(0.0);
            action_counter.push_back(0);
        }


        for (int step = 0; step < experiment_values.T; step++){
            /* With a probability of epsilon, choose a random action.
             * Otherwise, choose the action that has the highest 
             * estimated value.
             */
            if (getRandomNumberFromRangeUniform(0, 1) <= epsilon){
                /* Choose a random action. */
                action = getRandomIntFromRangeUniform(0, experiment_values.K);
            } else {
                /* Find the action that has the highest action value estimate. */
                action = getIndexOfLargestValue(action_value_estimates);
            }

            /* Calculate the reward. Use the reward to update the estimate
             * for that action. Add the reward to the total_reward sum for
             * this round, and save it in the all_rewards vector.
             */
            reward = getReward(bandit[action]);
            updateEstimate(action_value_estimates, action_counter, action, reward);
            all_rewards.push_back(reward);
            total_reward += reward;

            /* Calculate the current percentage of having chosen the true optimal
             * action out of the total number of chosen actions.
             */
            percentage_optimal_action_chosen.push_back(100*((double)action_counter[bandit[experiment_values.K]]/(step+1)));
        }

        /* Add the total_rewards sum for the last round to the vector
         * that stores the total_rewards sums for all rounds.
         */
        total_rewards.push_back(total_reward);

        /* Update the vector that keeps track of the average percentage
         * of having chosen the true optimal action at each time step
         * over all rounds.
         */
        integratePercentageOptimalActionChosen(averaged_percentage_optimal_action_chosen, percentage_optimal_action_chosen, round);
        percentage_optimal_action_chosen.clear();

        /* Update the vector that keeps track of the average reward
         * at each time step.
         */
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

}