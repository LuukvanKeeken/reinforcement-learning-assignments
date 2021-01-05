#include <vector>
#include <map>
#include "parameterValuesStruct.hpp"

std::vector<std::string> actionByO(std::vector<std::string> currentBoard);
bool checkIfXAfterState(std::vector<std::string> newBoard);
std::vector<std::string> chooseNewAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, double> qValueTableXAfterStates);
void createOutputFile(std::vector<std::vector<double>> averagesWonLostDraw, struct parameterValues parameter_values);
std::vector<std::string> findBestAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, double> qValueTableXAfterStates);
std::map<std::vector<std::string>, double> generateQValueTableXAfterStates();
std::string getGameResult(std::vector<std::string> currentBoard);
double getQValue(std::vector<std::string> afterState, std::map<std::vector<std::string>, double> qValueTable);
double getRandomNumberFromRangeUniform(int lower_limit, int upper_limit);
void initialiseExperiment(struct parameterValues &parameter_values);
int minimax(std::vector<std::string> board, int depth, bool isMaximising);
void printBoard(std::vector<std::string> board);
void printMeanAndStandardDeviation(std::vector<std::vector<int>> sumWonLostDrawCount);
void updateAfterstateQValue(struct parameterValues parameter_values, std::vector<std::string> afterState, std::map<std::vector<std::string>, double> &qValueTableXAfterStates, double bestAfterAfterstateQValue);