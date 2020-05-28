import sys, time, os
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import math


ITERATIONS = 5
OUTPUT_FILE = "out"


def main():
    ms = [8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
    
    mem = [17.16, 599.869, 1246.642]
    mem2 = [151.059, 599.869, 2399.459]
    ps = [256, 1024, 4096]
    N = [1039, 36595, 75887]
    E = [5802, 91826, 508837]
    VALS_g = [N[i]*1024 + math.log2(N[i]) for i in range(len(N))]
    VALS_p = [36595*ps[i] + math.log2(36595) for i in range(len(ps))]
    

    # plt.title('Memory usage by graph size')
    # plt.ylabel('Memory in Mb')
    # plt.xlabel('pN+logN')

    # df=pd.DataFrame({'Complexity': VALS_g, 'mem': mem})
    # plt.plot( 'Complexity', 'mem', data=df, marker='o', color='green')
    # plt.legend()

    # plt.savefig('memory_graphs.png')
    # plt.close()

    plt.title('Memory usage by p')
    plt.ylabel('Memory in Mb')
    plt.xlabel('pN+logN')

    df=pd.DataFrame({'Complexity': VALS_p, 'mem': mem2})
    plt.plot( 'Complexity', 'mem', data=df, marker='o', markersize=12, color='green', linewidth=4)
    plt.legend()
    plt.savefig('memory_ps.png')
    plt.close()

    

if __name__ == '__main__':
    main()
