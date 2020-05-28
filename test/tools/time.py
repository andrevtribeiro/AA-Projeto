import sys, time, os
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import math


ITERATIONS = 5
M = 4096
OUTPUT_FILE = "out"

def main(main_dir, graph_dir):
    files = os.listdir(graph_dir)
    files = ["{}/{}".format(graph_dir, f) for f in files]
    
    complexity = []
    timesP_dic = {}
    # timesVC = [19.830432844161987, 44.879074811935425, 90.34010257720948, 315.59038548469545]

    for graph in files:
        
        if "facebook" in graph:
            continue

        with open(graph, "r") as f:
            edges = f.readlines() 
        
        N = int(edges[0].split()[0])
        E = int(edges[0].split()[1])
        edges = edges[1:]
        edges = [(int(e.split(" ")[0]), int(e.split(" ")[1])) for e in edges]

        graph_sz = M * math.log2(N) * (E + N)
        print(N, E)
        complexity += [M * math.log2(N) * (E + N)] # ElogN  

        print("{}/apl {} < {} > {}".format(main_dir, M, graph, OUTPUT_FILE))
        for i in range(ITERATIONS):
            print("iteration ", i)
            t1 = time.time()    
            os.system("{}/apl {} < {} > {}".format(main_dir, M, graph, OUTPUT_FILE))
            t2 = time.time()

            if graph_sz not in timesP_dic:
                timesP_dic[graph_sz] = (t2-t1)
            else:
                timesP_dic[graph_sz] += (t2-t1)

        timesP_dic[graph_sz] /= ITERATIONS


    print(sorted(timesP_dic.keys()))
    timesP = [timesP_dic[i] for i in sorted(timesP_dic.keys())]
    # complexity = [complexity_dic[i] for i in sorted(complexity_dic.keys())]
    complexity = sorted(complexity)
    print(timesP)
    print(complexity)
    
    plt.title('Running Time by complexity')
    plt.ylabel('Running Time')
    plt.xlabel('p*logN(E+N)')
    df=pd.DataFrame({'p*logN(E+N)': complexity, 'Parallel': timesP})
    plt.plot( 'p*logN(E+N)', 'Parallel', data=df, marker='o', markerfacecolor='blue', markersize=12, color='blue', linewidth=4)
    plt.legend()
    plt.savefig('running_time_complexity.png')

    

if __name__ == '__main__':
    print(sys.argv)
    main(sys.argv[1], sys.argv[2])


# phonecalls 11,7163298