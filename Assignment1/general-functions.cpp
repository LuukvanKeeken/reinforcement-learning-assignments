#include <vector>
#include <random>
#include <iostream>
#include "experimentValuesStruct.hpp"
#include <math.h>

std::default_random_engine generator(time(0));

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

    std::cout << "Please choose the algorithm:\n    (0) Epsilon-greedy\n    (1) Optimistic Initial Values\n    (2) Upper-Confidence-Bound" << "\n";
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

/* Function that calculates the new action value estimate for the last
 * chosen action, with an equation of the form newEstimate = oldEstimate +
 * 1/(number of times action was chosen)*(newReward - oldEstimate). */
void updateValueEstimate(std::vector<double> &action_value_estimates, std::vector<int> &action_counter, int action, double reward){
    action_counter[action] += 1;
    action_value_estimates[action] = action_value_estimates[action] + (1.0/action_counter[action])*(reward - action_value_estimates[action]);
}
