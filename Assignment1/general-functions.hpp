#include <vector>

int getIndexOfLargestValue(std::vector<double> vector);
int getRandomIntFromRangeUniform(int lower_limit, int upper_limit);
double getRandomNumberFromRangeNormal(double mean, double standard_deviation);
double getRandomNumberFromRangeUniform(int lower_limit, int upper_limit);
double getReward(double true_action_value, struct experimentValues experiment_values);
std::vector<double> initialiseBandit(int K, int distribution);
void initialiseExperiment(struct experimentValues &experiment_values);
void integrateAllRewards(std::vector<double> &all_rewards_averaged, std::vector<double> all_rewards, int round);
void integratePercentageOptimalActionChosen(std::vector<double> &averaged_p_o_a_c, std::vector<double> p_o_a_c, int round);
int selectAction(struct experimentValues experiment_values, std::vector<double> action_value_estimates, std::vector<int> action_counter, int step);
void updateValueEstimate(std::vector<double> &action_value_estimates, std::vector<int> &action_counter, int action, double reward);