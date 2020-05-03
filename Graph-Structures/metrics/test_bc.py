import sys, time, os
import networkx as nx
import matplotlib as plt
import pandas as pd


OUTPUT_FILE = "/dev/null"
EPSILON = 0.01
DELTA = 0.1
C = 0.5

def main(main_dir, graph_dir):
    files = os.listdir(graph_dir)
    files=["{}/graph-5000.txt".format(graph_dir), "{}/graph-10000.txt".format(graph_dir)]
    sizes = sorted([int(gn.split("-")[1].split(".")[0]) for gn in files])
    timesVC_dic = {}
    timesNX_dic = {}
    
    for graph_name in files:
        t1 = time.time()
        print("{}/main {} {} {} < {} > {}".format(main_dir, EPSILON, DELTA, C, graph_name, OUTPUT_FILE))
        os.system("{}/main {} {} {} < {} > {}".format(main_dir, EPSILON, DELTA, C, graph_name, OUTPUT_FILE))
        t2 = time.time()

        timesVC_dic[graph_name] = t2-t1

    for graph_name in files:
        t1 = time.time()
        graph_nx = nx.read_edgelist(graph_name, create_using=nx.Graph, nodetype=int)
        nx.betweenness_centrality(graph_nx)
        t2 = time.time()

        timesNX_dic[graph_name] = t2-t1
    
    timesVC = [timesVC_dic[i] for i in sorted(timesVC_dic.keys())]
    timesNX = [timesNX_dic[i] for i in sorted(timesNX_dic.keys())]
    
    plt.title('Time of BC calculations by number of nodes')
    plt.ylabel('Time in seconds')
    plt.xlabel('Number of Nodes')

    df=pd.DataFrame({'x': sizes, 'VC': timesVC, 'Brandes': timesNX})
    plt.plot( 'x', 'VC', data=df, marker='o', markerfacecolor='blue', markersize=12, color='skyblue', linewidth=4)
    plt.plot( 'x', 'Brandes', data=df, marker='s', markerfacecolor='red', markersize=12, color='red', linewidth=4,linestyle='--')
    # plt.show()
    plt.savefig('foo.png')

    

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])