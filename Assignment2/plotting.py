#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def main(args):
    dfTestRun = pd.read_csv("output-files/q-learning/upper-confidence-bound/runs-200_games-230_alpha-0.300000_gamma-1.000000_c-0.008000.csv")
    trend1 = np.polyfit(dfTestRun['game number'], dfTestRun['percentage won'], 3)
    trend2 = np.polyfit(dfTestRun['game number'], dfTestRun['percentage lost'], 3)
    trend3 = np.polyfit(dfTestRun['game number'], dfTestRun['percentage draw'], 3)
    p = np.poly1d(trend1)
    p2 = np.poly1d(trend2)
    p3 = np.poly1d(trend3)

    testFig = plt.figure(1)
    plt.plot(dfTestRun['game number'], dfTestRun['percentage won'], label = 'won')
    plt.plot(dfTestRun['game number'], p(dfTestRun['game number']), color = 'blue')
    plt.plot(dfTestRun['game number'], dfTestRun['percentage lost'], label = 'lost')
    plt.plot(dfTestRun['game number'], p2(dfTestRun['game number']), color = 'orange')
    plt.plot(dfTestRun['game number'], dfTestRun['percentage draw'], label = 'draw')
    plt.plot(dfTestRun['game number'], p3(dfTestRun['game number']), color = 'green')
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


