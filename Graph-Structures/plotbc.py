#!/usr/bin/env python3

import matplotlib.pyplot as plt
import sys


def parse(fname):
    with open(fname, "r") as f:
        data = f.read()
    
    return data.split("\n")[0][:-1]
    

def main(fname):
    data = parse(fname)

    data = data.split(" ")
    graph = {}

    for e in data:
        if e in graph:
            graph[e] += 1
        else:
            graph[e] = 1

    keys = []
    vals = []
    for k in graph.keys():
        keys += [float(k)]
        vals += [graph[k]]

    print(keys)
    print(vals)

    plt.title('BC Distribution')
    plt.ylabel('Count')
    plt.xlabel('BC')
    plt.scatter(keys, vals)
    # plt.show()
    plt.savefig('foo.png')



if __name__ == '__main__':
    main(sys.argv[1])


