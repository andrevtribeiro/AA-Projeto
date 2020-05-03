import sys, time, os
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import math


OUTPUT_FILE = "out"
EPSILON = 0.01
DELTA = 0.1
C = 0.5

def main(main_dir, graph_dir):
    graph_name="{}/{}".format(graph_dir,"graph-5000.txt")
    sizes = [0.01,0.02,0.04,0.06,0.08,0.1]
    timesVC_dic = {}
    timesBP_dic = {}
    average_error=[]
    average_stddev_error=[]
    average_max_error=[]
    with open(graph_name, "r") as f:
        edges = f.readlines()
    
    edges = edges[1:]
    edges = [(int(e.split(" ")[0]), int(e.split(" ")[1])) for e in edges]
    print("exact bc")
    graph_nx = nx.Graph(edges, nodetype=int)
    bc_exact=nx.betweenness_centrality(graph_nx)

    for size in sizes:
        print(size)
        t1 = time.time()
        print("{}/main {} {} {} < {} > {}".format(main_dir, size, DELTA, C, graph_name, OUTPUT_FILE))
        os.system("{}/main {} {} {} < {} > {}".format(main_dir, size, DELTA, C, graph_name, OUTPUT_FILE))
        t2 = time.time()
        timesVC_dic[size] = t2-t1

        print("networkx")


        # print(edges)
        
        t1 = time.time()
        
        r=math.ceil((1/(2*pow(size,2)))*(math.log(graph_nx.number_of_nodes())+math.log(2)+math.log(1/0.1)))
        bc={}
        if r>graph_nx.number_of_nodes():
            r=graph_nx.number_of_nodes()
        nx.betweenness_centrality(graph_nx,k=r,normalized=True)
        t2 = time.time()
        timesBP_dic[size] = t2-t1

        with open(OUTPUT_FILE, "r") as f:
            data = f.read()

        soma=float(data.split("\n")[1].split(":")[1])
        data=data.split("\n")[0][:-1]
        
        data = data.split(" ")
        err=0
        stddev=0
        max_error = -1
        for i in sorted(bc_exact.keys()):
            dif=abs(float(data[i])/soma-bc_exact[i])
            err+=dif
            max_error = max(max_error, dif)
            stddev += pow(dif, 2)
        print("Error->",err/len(data))
        stddev /= len(data)
        average_error+=[err/len(data)]
        average_stddev_error+=[math.sqrt(stddev)]
        average_max_error+=[max_error]
    
    timesVC = [timesVC_dic[i] for i in sorted(timesVC_dic.keys())]
    timesBP = [timesBP_dic[i] for i in sorted(timesBP_dic.keys())]

    print(timesVC)
    print(timesBP)

    plt.title('Average error by epsilon')
    plt.ylabel('Error')
    plt.xlabel('Epsilon')

    df=pd.DataFrame({'Epsilon': sizes, 'Avg': average_error,'Avg + Stddev':average_stddev_error,'Max Error':average_max_error })
    plt.scatter( 'Epsilon', 'Avg', data=df, marker='o', color='skyblue')
    plt.scatter( 'Epsilon', 'Avg + Stddev', data=df, marker='s', color='red')
    plt.scatter( 'Epsilon', 'Max Error', data=df, marker='^', color='gray')
    plt.legend()

    plt.savefig('average_error_epsilon.png')
    plt.close()

    plt.title('Running Time by epsilon')
    plt.ylabel('Running Time')
    plt.xlabel('Epsilon')
    df=pd.DataFrame({'Epsilon': sizes, 'VC': timesVC,'BP': timesBP })
    plt.plot( 'Epsilon', 'VC', data=df, marker='o', markerfacecolor='blue', markersize=12, color='skyblue', linewidth=4)
    plt.plot( 'Epsilon', 'BP', data=df, marker='s', markerfacecolor='red', markersize=12, color='red', linewidth=4,linestyle='dashed')
    plt.legend()
    plt.savefig('running_time_epsilon.png')


    

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])