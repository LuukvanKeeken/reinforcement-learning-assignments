#ifndef EXPERIMENT_VALUES
#define EXPERIMENT_VALUES

struct experimentValues{
    int distribution;
    int K;
    int N;
    int T;
    int algorithm;
    double epsilon;
    double initial_estimate = 0.0;
    double c;
    double alpha;
};

#endif