# reinforcement-learning-assignments
This program can be used to analyse the performance of two different Reinforcement Learning algorithms and three different exploration algorithms in a game of tic-tac-toe. The RL algorithms are SARSA and Q-Learning. The exploration algorithms are epsilon-greedy, Optimistic Initial Values, and Upper-Confidence-Bound.
  
The user has a number of choices about how to initialise an experiment. First the user is asked which RL algorithm should be used, after which an exploration algorithm can be chosen (with the corresponding parameter value). The user then chooses the amount of runs, the total amount of games per run, and the amount of games that should be played greedily at the end (included in the total). Lastly, the values for alpha and gamma are set.
 
The program prints the mean and standard deviation of the total reward to the terminal. It also creates a .csv file, with the file name containing the used parameters and parameter values, and puts it in a directory corresponding to the used algorithms (to keep everything ordered). If that directory doesn't exist yet, it is created by the program (or if something goes wrong, the program asks the user to create the directory manually). The data files used in the report are in directories named 'final', inside the directories corresponding to the used algorithm.

If you're on Linux, compile with "g++ main.cpp general-functions.cpp learning-loop/learning-loop.cpp files-linux.cpp". If you're on Windows, compile with "g++ main.cpp general-functions.cpp learning-loop/learning-loop.cpp files-windows.cpp".
Execute with "./a.out". You can optionally create a text file, e.g. "example.in", containing the parameter values you want to use, and use "./a.out < example.in".

Also included is a python file named 'plotting.py', which was used to create the figures of the report. This can be run using "python3 plotting.py".
