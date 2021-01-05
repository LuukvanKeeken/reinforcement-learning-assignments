#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import pandas as pd

def main(args):
    dfTestRun = pd.read_csv("output-files/testFile.csv")

    testFig = plt.figure(1)
    plt.plot(dfTestRun['game number'], dfTestRun['percentage won'], label = 'won')
    plt.plot(dfTestRun['game number'], dfTestRun['percentage lost'], label = 'lost')
    plt.plot(dfTestRun['game number'], dfTestRun['percentage draw'], label = 'draw')
    plt.ylabel('% won/lost/drawn')
    plt.xlabel('game number')
    plt.title("Win/lose/draw percentages for each game in sequence")
    leg = plt.legend()
    for line in leg.get_lines():
        line.set_linewidth(2)


    plt.show()




if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))


