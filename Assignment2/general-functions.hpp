#include <vector>
#include <map>
#include "parameterValuesStruct.hpp"
#include "boardStateStruct.hpp"
#include <unordered_map>
#include "BoardHasherStruct.hpp"

struct boardState actionByO(struct boardState currentBoard);
bool checkIfXAfterState(std::vector<std::string> newBoard);
struct boardState chooseNewAfterstate(struct boardState currentBoard, std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> qValueTableXAfterStates, int t, struct parameterValues parameter_values, int gameCounter);
void directoryCheck(const char* directory_name, std::string dir_name);
void createOutputFile(std::vector<std::vector<double>> averagesWonLostDraw, struct parameterValues parameter_values);
struct boardState findBestAfterstate(struct boardState currentBoard, std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> qValueTableXAfterStates);
void resetQValueTable(std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> &qValueTableXAfterstates, struct parameterValues parameter_values);
std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> generateQValueTableXAfterStates(struct parameterValues parameter_values);
char getGameResult(struct boardState currentBoard);
double getQValue(struct boardState afterState, std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> qValueTable);
double getRandomNumberFromRangeUniform(int lower_limit, int upper_limit);
void initialiseExperiment(struct parameterValues &parameter_values);
int minimax(struct boardState board, int depth, bool isMaximising);
void printBoard(struct boardState boardState);
void printMeanAndStandardDeviation(std::vector<std::vector<int>> sumWonLostDrawCount);
void updateAfterstateQValue(struct parameterValues parameter_values, struct boardState afterState, std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> &qValueTableXAfterstates, double bestAfterAfterstateQValue);
double uCb(double qValue, double c, int count, double steps);
double getQCount(struct boardState afterState, std::unordered_map<struct boardState, std::array<double, 2>, BoardHasher> qValueTable);
struct boardState boardSelection(std::vector<struct boardState> candidateBoards);