#include <vector>
#include <map>
#include "parameterValuesStruct.hpp"

std::vector<std::string> actionByO(std::vector<std::string> currentBoard);
bool checkIfXAfterState(std::vector<std::string> newBoard);
std::vector<std::string> chooseNewAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, std::vector<double>> qValueTableXAfterStates, int t, struct parameterValues parameter_values);
void createOutputFile(std::vector<std::vector<double>> averagesWonLostDraw, struct parameterValues parameter_values);
std::vector<std::string> findBestAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, std::vector<double>> qValueTableXAfterStates);
std::map<std::vector<std::string>, std::vector<double>> generateQValueTableXAfterStates(struct parameterValues parameter_values);
std::string getGameResult(std::vector<std::string> currentBoard);
double getQValue(std::vector<std::string> afterState, std::map<std::vector<std::string>, std::vector<double>> qValueTable);
double getRandomNumberFromRangeUniform(int lower_limit, int upper_limit);
void initialiseExperiment(struct parameterValues &parameter_values);
int minimax(std::vector<std::string> board, int depth, bool isMaximising);
void printBoard(std::vector<std::string> board);
void printMeanAndStandardDeviation(std::vector<std::vector<int>> sumWonLostDrawCount);
void updateAfterstateQValue(struct parameterValues parameter_values, std::vector<std::string> afterState, std::map<std::vector<std::string>, std::vector<double>> &qValueTableXAfterStates, double bestAfterAfterstateQValue);
double uCb(double qValue, double c, int count, double steps);
double getQCount(std::vector<std::string> afterState, std::map<std::vector<std::string>, std::vector<double>> qValueTable);
std::vector<std::string> boardSelection (std::vector<std::vector<std::string>> candidateBoards, int counter);