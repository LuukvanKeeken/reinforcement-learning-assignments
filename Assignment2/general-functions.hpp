#include <vector>
#include <map>

std::vector<std::string> chooseNewAfterstate(std::vector<std::string> currentBoard, std::map<std::vector<std::string>, double> qValueTable);
std::map<std::vector<std::string>, double> generateQValueTable();
std::string getGameResult(std::vector<std::string> currentBoard);
double getQValue(std::vector<std::string> afterState, std::map<std::vector<std::string>, double> qValueTable);
void initialiseExperiment(struct parameterValues &parameter_values);