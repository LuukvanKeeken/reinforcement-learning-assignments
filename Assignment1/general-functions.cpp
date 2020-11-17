#include <vector>
#include <random>
#include <iostream>
#include "experimentValuesStruct.hpp"
#include <math.h>
#include <fstream>

std::default_random_engine generator(time(0));


/* Function that creates a .csv file with as a file name all the relevant values,
 * such as the used algorithm, the amount of rounds, and any relevant parameters
 * such as epsilon and alpha. This file contains the averaged reward received at
 * each step, and average percentage of the optimal action having been chosen at
 * each step, over all runs. */
void createOutputFile(struct experimentValues experiment_values, std::vector<double> all_rewards_averaged, std::vector<double> averaged_percentage_optimal_action_chosen){
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
            file_name = "dist-" + std::to_string(experiment_values.distribution) + "_K-" + std::to_string(experiment_values.K) +
                "_runs-" + std::to_string(experiment_values.N) + "_steps-" + std::to_string(experiment_values.T) + "_algorithm-" + std::to_string(experiment_values.algorithm) +
                "_alpha-" + std::to_string(experiment_values.alpha);
            file.open("gradient-based-algorithm/" + file_name + ".csv");
            break;
    }

    file << "average reward,percentage optimal action\n";
    for (int i = 0; i < experiment_values.T; i++){
        file << all_rewards_averaged[i] << "," << averaged_percentage_optimal_action_chosen[i] << "\n";
    }

    file.close();
}


/* Function that finds and returns the index of the largest
 * value in a double vector.
 */
int getIndexOfLargestValue(std::vector<double> vector){
    int largest_value_index = 0;
    double largest_value = vector[0];
    for (int i = 1; i < vector.size(); i++){
        if (largest_value < vector[i]){
            largest_value = vector[i];
            largest_value_index = i;
        }
    }
    return largest_value_index;
}


/* Function that returns a random integer on the range [lower_limit, upper_limit]. */
int getRandomIntFromRangeUniform(int lower_limit, int upper_limit){
    std::uniform_int_distribution<int> distribution(lower_limit, upper_limit);
    return distribution(generator);
}

/* Function that returns a random decimal number [lower_limit, upper_limit]. */
double getRandomNumberFromRangeUniform(int lower_limit, int upper_limit){
    std::uniform_real_distribution<double> distribution(lower_limit, upper_limit);
    return distribution(generator);
}

/* Function that returns a random decimal number from a normal distribution
 * with a certain mean and standard_deviation.
 */ 
double getRandomNumberFromRangeNormal(double mean, double standard_deviation){
    std::normal_distribution<double> distribution(mean, standard_deviation);
    return distribution(generator);
}

double getReward(double true_action_value, struct experimentValues experiment_values){
    double reward = 0;

    /* When the user wants the rewards to be sampled from a 
     * normal distribution with as the mean the true action
     * value, and a unit standard deviation. */
    if (experiment_values.distribution == 0){
        reward = getRandomNumberFromRangeNormal(true_action_value, 1);
    } else if (experiment_values.distribution == 1){
        std::bernoulli_distribution distribution(true_action_value);
        reward = distribution(generator);
    }

    return reward;

}


/* Function that gets the basic experiment values from the user,
 * i.e. the used reward distribution for each arm, the amount of
 * arms K, the amount of round N, the amount of steps per round T,
 * and the algorithm to use. These values are stored in an 
 * experimentValues struct, which was passed by reference.
 */ 
void initialiseExperiment(struct experimentValues &experiment_values){
    std::cout << "Please choose a distribution:\n    (0) Gaussian/Normal\n    (1) Bernoulli" << "\n";
    std::cin >> experiment_values.distribution ;

    std::cout << "Please choose K" << "\n";
    std::cin >> experiment_values.K;

    std::cout << "Please choose amount of rounds:" << "\n";
    std::cin >> experiment_values.N;

    std::cout << "Please choose amount of steps in each round:" << "\n";
    std::cin >> experiment_values.T;

    std::cout << "Please choose the algorithm:\n    (0) Epsilon-greedy\n    (1) Optimistic Initial Values\n    (2) Upper-Confidence-Bound\n    (3) Gradient-Based Algorithm" << "\n";
    std::cin >> experiment_values.algorithm;
}


/* Function that initialises a chosen number K of arms/actions,
 * i.e. K values are sampled from a normal distribution with
 * mean 0 and standard deviation 1, to denote the K true action
 * values. The function also keeps track of which action value
 * is the highest, i.e. which action is the optimal action. At
 * the end of initialisation, this value is included in the return
 * vector.
 */ 
std::vector<double> initialiseBandit(int K, int distribution){
    std::vector<double> bandit;

    /* For a Gaussian/Normal distribution. */
    
    int max_index = 0;
    for (int i = 0; i < K; i++){
        if (distribution == 0){ /* For a Gaussian/Normal distribution of action values and rewards. */
            bandit.push_back(getRandomNumberFromRangeNormal(0.0, 1.0));
        } else if (distribution == 1){ /* For a Bernoulli distribution of rewards. */
            bandit.push_back(getRandomNumberFromRangeUniform(0, 1));
        }

        if (bandit[i] > bandit[max_index]){
            max_index = i;
        }
    }
    bandit.push_back(max_index);
    
    

    return bandit;
}

/* Function that is called after each round to calculate the new
 * average reward at each time step. */
void integrateAllRewards(std::vector<double> &all_rewards_averaged, std::vector<double> all_rewards, int round){
    for (int i = 0; i < all_rewards_averaged.size(); i++){
        all_rewards_averaged[i] = all_rewards_averaged[i] + (1.0/((double)round+1))*(all_rewards[i] - all_rewards_averaged[i]);
    }
}


/* Function that is called after each round to calculate the new
 * average percentage of the true optimal action having been chosen
 * at each time step. */
void integratePercentageOptimalActionChosen(std::vector<double> &averaged_p_o_a_c, std::vector<double> p_o_a_c, int round){
    for (int i = 0; i < averaged_p_o_a_c.size(); i++){
        averaged_p_o_a_c[i] = averaged_p_o_a_c[i] + (1.0/((double)round+1)*(p_o_a_c[i] - averaged_p_o_a_c[i]));
    }
}

/* Function that calculates and prints the mean and standard deviation
 * of the total_rewards of all runs. */
void printMeanAndStandardDeviation(std::vector<double> total_rewards){
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
}


/* Function that selects the next action, according to which 
 * algorithm is being used. */
int selectAction(struct experimentValues experiment_values, std::vector<double> action_value_estimates, std::vector<int> action_counter, int step){
    int action = 0;
    /* Epsilon-Greedy and Optimistic Initial Values. */
    if (experiment_values.algorithm == 0 || experiment_values.algorithm == 1){
        /* With a probability of epsilon, choose a random action.
         * Otherwise, choose the action that has the highest 
         * estimated value.
         */
        if (getRandomNumberFromRangeUniform(0, 1) <= experiment_values.epsilon){
            /* Choose a random action. */
            action = getRandomIntFromRangeUniform(0, experiment_values.K);
        } else {
            /* Find the action that has the highest action value estimate. */
            action = getIndexOfLargestValue(action_value_estimates);
        }
    /* Upper-Confidence-Bound Action Selection. */    
    } else if (experiment_values.algorithm == 2){
        double max = 0;
        double estimate_plus_confidence;
        for (int i = 0; i < experiment_values.K; i++){
            /* If this action has never been chosen, it is considered 
             * to be the maximising action. */
            if (action_counter[i] == 0){
                action = i;
                break;
            }
            estimate_plus_confidence = action_value_estimates[i] + experiment_values.c*sqrt(log(step+1)/action_counter[i]);
            if (estimate_plus_confidence > max){
                max = estimate_plus_confidence;
                action = i;
            }
        }
    }
    return action;
}

/* Function that returns an action, specifically for the gradient algorithm.
 * First, the action probabilities are updated. Then, an action is chosen
 * randomly, taking into account the different probabilities. */
int selectAction(struct experimentValues experiment_values, std::vector<double> action_preferences, std::vector<double> &action_probabilities){
    double sum_of_preferences = 0;
    for (int i = 0; i < experiment_values.K; i++){
        sum_of_preferences += exp(action_preferences[i]);
    }

    for (int i = 0; i < experiment_values.K; i++){
        action_probabilities[i] = exp(action_preferences[i])/sum_of_preferences;
    }

    /* In this method of selecting an action, each action is assigned
     * a range somewhere between 0 and 1, e.g. action 0 might be from
     * 0 to 0.147, action 1 from 0.147 to 0.221, etc. A random number
     * between 0 and 1 is sampled from the uniform distribution. The 
     * action whose range the random number falls in is selected. */
    double random_number = getRandomNumberFromRangeUniform(0, 1);
    double sum = 0;
    for (int i = 0; i < experiment_values.K; i++){
        sum += action_probabilities[i];
        if (sum >= random_number){
            return i;
        }
    }

    return 0;
}

/* Function that updates the action preferences for the gradient algorithm. */
void updateActionPreferences(int action, double reward, std::vector<double> &action_preferences, std::vector<double> action_probabilities, double average_reward, double alpha){
    for (int i = 0; i < action_preferences.size(); i++){
        if (i == action){
            action_preferences[i] = action_preferences[i] + alpha*(reward - average_reward)*(1 - action_probabilities[i]);
        } else {
            action_preferences[i] = action_preferences[i] - alpha*(reward - average_reward)*action_probabilities[i];
        }
    }
}

/* Function that calculates the new average of all received rewards. */
void updateAverageReward(double &average_reward, double reward, int step){
    average_reward = average_reward + (1.0/(step+1))*(reward - average_reward);
}


/* Function that calculates the new action value estimate for the last
 * chosen action, with an equation of the form newEstimate = oldEstimate +
 * 1/(number of times action was chosen)*(newReward - oldEstimate). */
void updateValueEstimate(std::vector<double> &action_value_estimates, std::vector<int> &action_counter, int action, double reward){
    action_counter[action] += 1;
    action_value_estimates[action] = action_value_estimates[action] + (1.0/action_counter[action])*(reward - action_value_estimates[action]);
}

