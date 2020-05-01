#pragma once
#include "structures/graph/compressed_sparse_rows.hpp"
#include <random>
#include <math.h>
#include <algorithm>    // std::random_shuffle
#include <string.h>

double _epsilon;
double _delta;

uint64_t BFS(CSRGraph& graph, NodeId v, vector<list<NodeId> >& prev, int* dist, int* max1, int* max2){
    uint64_t iterations = 0;
    uint64_t source = v;
    vector<NodeId>* neighbours = new vector<NodeId>(graph.GetNeighboors(v));
    vector<NodeId>* rev_neighbours = new vector<NodeId>(graph.GetReverseNeighboors(v));
  
    list<int> queue; 
  
    queue.push_back(v); 

    neighbours->insert(neighbours->end(), rev_neighbours->begin(), rev_neighbours->end());
    dist[v] = 0;
    while(!queue.empty()) { 
        
        // Dequeue a vertex from queue and print it 
        v = queue.front(); 
        cout << v << "->"; 
        cout << dist[v] << " "; 
        queue.pop_front(); 
        
        // Get all adjacent vertices of the dequeued 
        // vertex s. If a adjacent has not been visited,  
        // then mark it visited and enqueue it 
        delete neighbours;
        delete rev_neighbours;

        neighbours = new vector<NodeId>(graph.GetNeighboors(v));
        rev_neighbours = new vector<NodeId>(graph.GetReverseNeighboors(v));
        neighbours->insert(neighbours->end(), rev_neighbours->begin(), rev_neighbours->end());

        for (NodeId i : *neighbours) { 
            if (i != source && dist[i] == 0) { 
                iterations++;
                dist[i] = dist[v]+1;
                prev[i].push_front(v);

                if(dist[i] >= *max1){
                    *max2 = *max1;
                    *max1 = dist[i];
                }
                queue.push_back(i); 
            } else if(i != source && dist[i] == dist[v]+1){
                prev[i].push_front(v);
            }
        } 
    } 

    0 -> 1 -> 3
      -> 2 /

    013
    123

    0
    1 2
    3
    3

    std::cout << std::endl;
    return iterations;
}

int diameter(CSRGraph& graph){
    int d = 0;



    return 0;
}

void all_paths(list<NodeId> &prev, vector<list<NodeId> >&big_prev, vector<NodeId> current , list<vector<NodeId>>& n_sps){

    if(prev.empty()){
        n_sps.push_front(current);
    }

    while(!prev.empty()){
        NodeId node = prev.front();
        vector<NodeId> tmp = vector<NodeId>(current);
        tmp.push_back(node);
  
        prev.pop_front();
        all_paths(big_prev[node], big_prev, tmp, n_sps);
    }
}

void bc(CSRGraph& graph, double epsilon, double delta, double c) {
    _epsilon = epsilon;
    _delta = delta;
    int N = graph.N;
    int max1 = -1;
    int max2 = -1;
    uint64_t diameter, r;
    uint64_t n_sp;

    vector<list<NodeId> > prev = vector<list<NodeId> >(N);
    int* dist = (int*) calloc(1, sizeof(int)*N);

    list<vector<NodeId>> sps;
    std::vector<uint64_t>* access_vector = new std::vector<uint64_t>(N);

    // set some values:
    for (int i=0; i<N; ++i) (*access_vector)[i] = i; 

    // using built-in random generator:
    std::random_shuffle ( access_vector->begin(), access_vector->end() );

    
    std::mt19937 generator(time(0));
    std::uniform_int_distribution<int> distribution(0, N-1);

    uint64_t v = distribution(generator);
    
    n_sp = BFS(graph, v, prev, dist, &max1, &max2);
    
    for(uint64_t i : *access_vector){
        std::cout << "i: " << i << std::endl;
        vector<NodeId> vec = vector<NodeId>();
        vec.push_back(i);
        all_paths(prev[i], prev, vec, sps);

    
        for(vector<NodeId> p : sps){
            for(NodeId v : p){
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }

    }
    exit(0);


    diameter = max1 + max2;

    r = (c/pow(epsilon, 2))*(log2(diameter-2) + log(1/delta));

    // for(uint64_t i = 1; i <= r; i++){
    //     // sample

    //     if(!sps.empty())




    //     // compute shortest paths
    //     // while

    // }


    std::cout << std::endl << max1 << " " << max2 << std::endl;

    for (int i = 0; i < N; i++){
        std::cout << i << " " << dist[i] << std::endl;
    }
    
    return;

}


// 12.0191
// 37