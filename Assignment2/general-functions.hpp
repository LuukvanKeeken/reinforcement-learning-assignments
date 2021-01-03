#include <vector>
#include <map>

std::vector<std::string> actionByO(std::vector<std::string> currentBoard);
bool checkIfXAfterState(std::vector<std::string> newBoard);
std::vector<std::string> chooseNewAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, double> qValueTable);
std::map<std::vector<std::string>, double> generateQValueTableXAfterStates();
std::string getGameResult(std::vector<std::string> currentBoard);
double getQValue(std::vector<std::string> afterState, std::map<std::vector<std::string>, double> qValueTable);
void initialiseExperiment(struct parameterValues &parameter_values);
void printBoard(std::vector<std::string> board);