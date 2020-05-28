import sys, time, os
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import math


ITERATIONS = 1
OUTPUT_FILE = "out"
EPSILON = 0.01
DELTA = 0.1
C = 0.5

def main(main_dir, graph_dir):
    files = os.listdir(graph_dir)
    files = ["{}/{}".format(graph_dir, f) for f in files]
    
    complexity_dic = {}
    complexity = []
    timesVC_dic = {}
    diam_dic = {'metabolic.edgelist.csv' : 8, 'phonecalls.edgelist.txt': 35, 'soc-Epinions1.txt': 15, 
    'email-enorn.csv': 12, 'powergrid.edgelist.csv': 12}
    # timesVC = [19.830432844161987, 44.879074811935425, 90.34010257720948, 315.59038548469545]

    for graph in files:

        print(graph)
        with open(graph, "r") as f:
            edges = f.readlines() 
        

        N = int(edges[0].split()[0])
        E = int(edges[0].split()[1])

        diameter = diam_dic[graph.split("/")[-1]]

        R = (C/math.pow(EPSILON, 2))*((math.log2(diameter - 2)) + math.log(1/DELTA))
        graph_sz = R*(N+E)
        complexity_dic[graph_sz] = R*(N+E)

        print("{}/bc {} {} {} {} < {} > {}".format(main_dir, EPSILON, DELTA, C, 1, graph, OUTPUT_FILE))
        for i in range(ITERATIONS):
            print("iteration ", i)
            t1 = time.time()    
            os.system("{}/bc {} {} {} {} < {} > {}".format(main_dir, EPSILON, DELTA, C, 1, graph, OUTPUT_FILE))
            t2 = time.time()
            # timesVC += [t2-t1]
            # timesVC[-1] += (t2-t1)/ITERATIONS
            if graph_sz not in timesVC_dic:
                timesVC_dic[graph_sz] = (t2-t1)
            else:
                timesVC_dic[graph_sz] += (t2-t1)

        timesVC_dic[graph_sz] /= ITERATIONS


    timesVC = [timesVC_dic[i] for i in sorted(timesVC_dic.keys())]
    complexity = [complexity_dic[i] for i in sorted(complexity_dic.keys())]

    plt.title('Running Time by complexity')
    plt.ylabel('Running Time')
    plt.xlabel('R*(V+E)')
    df=pd.DataFrame({'R*(V+E)': complexity, 'VC': timesVC})
    plt.plot( 'R*(V+E)', 'VC', data=df, marker='o', markerfacecolor='blue', markersize=12, color='skyblue', linewidth=4)
    plt.legend()
    plt.savefig('bc_running_time_complexity.png')

    

if __name__ == '__main__':
    print(sys.argv)
    main(sys.argv[1], sys.argv[2])