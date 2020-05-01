#!/usr/bin/env python3
import numpy as np
import networkx as nx
import sys
# import math


graph = nx.DiGraph()
l = 5
p = 128
c = 3
# h0 = (128, 3)
hns = []
W0 = np.ones((p, c))
W1 = np.ones((p, p))
W2 = np.ones((p, p)) 
W3 = np.ones((p, p))
U1 = np.ones((p, p))
U2 = np.ones((p, p))
U3 = np.ones((p, p))
W4 = np.ones((l, p))
W5 = np.ones((1, l))

def relu(X):
    return np.maximum(0,X)

def sigmoid(x):
    return (1 / (1 + np.exp(-x)))

def hN(v, l, h):
    neighbours = graph.neighbors(v)
    res = np.zeros_like(h[0][l])

    for j in neighbours:
        res += 1/(np.sqrt(graph.degree(v)+1)*np.sqrt(graph.degree(j)+1))*h[j][l]

    return res

def ul(_h, _hns):
    return sigmoid(W1.dot(_hns) + U1.dot(_h))

def rl(_h, _hns):
    return sigmoid(W2.dot(_hns) + U2.dot(_h))

def fl(_h, _hns):
    return sigmoid(W3.dot(_hns) + U3.dot(np.multiply(rl(_h, _hns), _h)))

def hl(_h, _hns):
    return np.add(np.multiply(ul(_h, _hns), fl(_h, _hns)), np.matrix(np.multiply(1-ul(_h, _hns), _h)))


def main(graph):
    global l
    h = [[np.matrix([[graph.degree(v)], [1], [1]])] for v in graph.nodes]

    # h[v] = [[relu(W0.dot(h[v][0])) for v in graph.nodes]]

    for v in graph.nodes:
        prod = relu(W0.dot(h[v][0]))
        h[v].append(prod/np.linalg.norm(prod))
    
    for lt in range(2, l+1):
        for v in graph.nodes:
            hns[v][lt-2] = hN(v, lt-1, h)
            h[v].append(hl(h[v][lt-1], hns[v][lt-2]))
        
        for v in graph.nodes:
            h[v][lt] = h[v][lt]/np.linalg.norm(h[v][lt])

    z = [np.zeros_like(h[v][-1]) for v in graph.nodes]
    
    for v in graph.nodes:
        for lt in range(1, l+1):
            z[v] = np.maximum(z[v], h[v][lt])

    print(z[0].shape)

    # decode 
    z2 = [relu(W4.dot(zv)) for zv in z]

    y = W5.dot(z2)

    


if __name__ == '__main__':
    graph = nx.read_edgelist(sys.argv[1], create_using=nx.DiGraph, nodetype=int)

    hns = [[0 for lt in range(l)] for v in graph.nodes]    # nx.read
    
    # print(graph.nodes)
    # print(graph.neighbors(0))
    main(graph)