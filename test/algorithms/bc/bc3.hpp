#pragma once
#include "../../structures/graph/compressed_sparse_rows.hpp"
#include <random>
#include <math.h>
#include <algorithm>    // std::random_shuffle
#include <string.h>
#include <set> 
#include <unordered_set>


void BFS(CSRGraph& graph, NodeId v, NodeId t, vector<list<NodeId> >* prev, vector<int>* dist, vector<int>* sigma, int* max1, int* max2,bool reverse){
    uint64_t iterations = 0;
    uint64_t source = v;
    vector<NodeId>* neighbours;
    vector<NodeId>* rev_neighbours;
    unordered_set<NodeId> *effective_neighbours = new unordered_set<NodeId>();

    list<int> queue; 
  
    queue.push_back(v); 
    (*dist)[v] = 0;
    while(!queue.empty() && (*dist)[t]==0) { 
        
        v = queue.front(); 
        queue.pop_front(); 
        
        // O(V) space
        neighbours = new vector<NodeId>(graph.GetNeighboors(v));
        effective_neighbours->insert(neighbours->begin(), neighbours->end());
        delete neighbours;

        if(reverse){
            // O(V) space
            rev_neighbours = new vector<NodeId>(graph.GetReverseNeighboors(v));
            effective_neighbours->insert(rev_neighbours->begin(), rev_neighbours->end());
            delete rev_neighbours;
        }
        

        for (NodeId i : *effective_neighbours) { 
            if (i != source && (*dist)[i] == 0) { 
                iterations++;
                (*dist)[i] = (*dist)[v]+1;
                (*prev)[i].push_front(v);
                (*sigma)[i]+=(*sigma)[v];

                if((*dist)[i] >= *max1){
                    *max2 = *max1;
                    *max1 = (*dist)[i];
                }
                queue.push_back(i); 
            } else if(i != source && (*dist)[i] == (*dist)[v]+1){
                (*prev)[i].push_front(v);
                (*sigma)[i]+=(*sigma)[v];
            }
        }
        effective_neighbours->clear(); 
    }

    delete effective_neighbours;
}


// tempo: O(R(V+E)) espaço: O(V+E) (O(6V+E))
void bc(CSRGraph& graph, double epsilon, double delta, double c, int directed) {
    int N = graph.N;
    int max1 = -1;
    int max2 = -1;
    uint64_t diameter;
    double r;
    uint64_t n_sp;
    uint64_t u,v;

    // O(4V+E) space
    vector<list<NodeId>>* prev = new vector<list<NodeId> >(N);
    vector<int> * dist  = new vector<int>(N, 0);
    vector<int> * sigma = new vector<int>(N, 0);
    std::vector<double>* bcs = new std::vector<double>(N, 0);
    // ==========================

    std::mt19937 generator(time(0));
    std::uniform_int_distribution<int> distribution(0, N-1);

    u = distribution(generator);
    // O(V+E) time // O(2V)
    BFS(graph, u, u,prev, dist,sigma, &max1, &max2, directed);

    diameter = max1 + max2; 
    std::cerr << "D: " << diameter << std::endl;
    r = (c/pow(epsilon, 2))*(floor(log2(diameter-2)) + log(1/delta));
    
    double normalize = 0;
    // O(R) (valor no paper)
    for(uint64_t i = 1; i <= r;i++){
        u = distribution(generator);
        v = distribution(generator);
        if(u == v){
            continue;
        }

        // O(3V+E)
        // for(list<NodeId> l:*prev) l.clear(); 
        prev->clear();
        prev->resize(N);
        std::fill( dist->begin(),  dist->end(), 0);
        std::fill(sigma->begin(), sigma->end(), 0);
        // ===================
        (*sigma)[u] = 1;

        // O(V+E) time //O(2V) space
        BFS(graph, u, v, prev, dist,sigma, &max1, &max2,0);

        NodeId j = v;
        NodeId s = v;
        NodeId t = v;
        bool stop = false;
        
        if((*prev)[t].empty()){
            stop = true;
        }
        
        // O(D+E)
        while(!stop){
            //O(N)
            std::fill(dist->begin(), dist->end(), 0);

            for(NodeId p : (*prev)[t]){
                (*dist)[p]=(*sigma)[p];
            }
            // O(V) space
            std::discrete_distribution<int> distr(dist->begin(),dist->end());
            NodeId z=distr(generator);
            if(z != u){
                (*bcs)[z]=(*bcs)[z]+((double)1)/r;
                normalize+=((double)1)/r;
                s = t;
                t = z;
            } else {
                stop = true;
            }
        }
    }

    
    int maxNode=-1;
    double val = -1;
    //O(V)
    for(uint64_t i=0;i<N;i++){
        if((*bcs)[i] > val){
            maxNode = i;
            val = (*bcs)[i];
        }
        std::cout<<(*bcs)[i]<<" ";
    }

    std::cout << std::endl << "normalize: "<< normalize << std::endl;

    delete prev;
    delete dist;
    delete sigma;
    delete bcs;

    return;
}
