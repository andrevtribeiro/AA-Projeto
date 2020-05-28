import sys, time, os
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import math


ITERATIONS = 5
OUTPUT_FILE = "out"


def main(main_dir, graph_name):
    ms = [8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
    times = []
    errors=[]
    average_stddev_error=[]
    average_max_error=[]
    with open(graph_name, "r") as f:
        edges = f.readlines() 

    N = int(edges[0].split()[0])
    E = int(edges[0].split()[1])
    exact = 11.7163298
    
    for m in ms:
        print(m)
        
        print("{}/apl {} < {} > {}".format(main_dir, m, graph_name, OUTPUT_FILE))
        ts = 0
        for i in range(ITERATIONS):
            print(i)
            t1 = time.time()    
            os.system("{}/apl {} < {} > {}".format(main_dir, m, graph_name, OUTPUT_FILE))
            t2 = time.time()

            ts += (t2-t1)/ITERATIONS
            
        with open(OUTPUT_FILE, "r") as f:
            data = float(f.read().split("\n")[1])
                
        errors += [abs(exact - data)]
        times += [ts]
        

    plt.title('Error by number of registers')
    plt.ylabel('Error')
    plt.xlabel('p')

    df=pd.DataFrame({'p': ms, 'Error': errors, 'time': times})
    plt.scatter( 'p', 'Error', data=df, marker='o', color='skyblue')
    plt.legend()

    plt.savefig('error_p.png')
    plt.close()

    plt.title('Running Time by m')
    plt.ylabel('Running Time')
    plt.xlabel('p')
    plt.plot( 'p', 'time', data=df, marker='x', markerfacecolor='blue', markersize=12, color='red', linewidth=4)
    plt.legend()
    plt.savefig('running_time_p.png')
    plt.close()

    

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])
