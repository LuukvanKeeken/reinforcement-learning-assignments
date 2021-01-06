#ifndef PARAMETER_VALUES
#define PARAMETER_VALUES

/* Struct used for storing all the parameter values used
 * in learning. */
struct parameterValues{
    int RLAlg;
    int explorationAlg;
    double cValue;
    double oValue;
    int ammOfRuns;
    int gamesPerRun;
    double alpha;
    double gamma;
};

#endif