# reinforcement-learning-assignments
This program can be used to analyse the performance of four different algorithms that try to balance exploration and exploitation in K-armed bandit problems.
The four algorithms are:

  - Epsilon-Greedy
  - Optimistic Initial Values
  - Upper Confidence Bound Action Selection
  - A Gradient-Based algorithm
  
The user has a number of choices about how to initialise an experiment. First the user is asked from which distribution the rewards should be sampled (Gaussian/Normal or 
Bernoulli), after which the user has to specify values for K, the amount of runs, the amount of steps per run, and the used algorithm. Depending on the algorithm
choice, the user is also asked to specify certain parameter values (epsilon for EG and OIV, an initial estimate for OIV, c for UCBAS, and alpha for the GBA).
 
The program prints the mean and standard deviation of the total reward to the terminal. It also creates a .csv file, with the file name containing the used parameters
and parameter values, and puts it in a directory corresponding to the used algorithm (to keep everything ordered). If that directory doesn't exist yet, it is created
by the program (or if something goes wrong, the program asks the user to create the directory manually).
 
Compile with "g++ bandit.cpp general-functions.cpp experiment-loop/experiment-loop.cpp experiment-loop/experiment-loop-gradient.cpp", while being in the "Assignment1" directory.
Execute with "./a.out". You can optionally create a text file, e.g. "example.in", containing the parameter values you want to use, and use "./a.out < example.in".
