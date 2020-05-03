#pragma once
#include "structures/graph/compressed_sparse_rows.hpp"
#include <random>
#include <math.h>
#include <algorithm>    // std::random_shuffle
#include <string.h>
#include <set> 

double _epsilon;
double _delta;

uint64_t BFS(CSRGraph& graph, NodeId v, NodeId t, vector<list<NodeId> >& prev, int* dist,int* sigma, int* max1, int* max2){
    uint64_t iterations = 0;
    uint64_t source = v;
    vector<NodeId>* neighbours;
  
    list<int> queue; 
  
    queue.push_back(v); 
    dist[v] = 0;
    while(!queue.empty() && dist[t]==0) { 
        
        // Dequeue a vertex from queue and print it 
        v = queue.front(); 
        queue.pop_front(); 
        
        // Get all adjacent vertices of the dequeued 
        // vertex s. If a adjacent has not been visited,  
        // then mark it visited and enqueue it 
        
        neighbours = new vector<NodeId>(graph.GetNeighboors(v));

        for (NodeId i : *neighbours) { 
            if (i != source && dist[i] == 0) { 
                iterations++;
                dist[i] = dist[v]+1;
                prev[i].push_front(v);
                sigma[i]+=sigma[v];

                if(dist[i] >= *max1){
                    *max2 = *max1;
                    *max1 = dist[i];
                }
                queue.push_back(i); 
            } else if(i != source && dist[i] == dist[v]+1){
                prev[i].push_front(v);
                sigma[i]+=sigma[v];
            }
        }
        delete neighbours; 
    }

    return iterations;
}

void all_paths(list<NodeId>prev, vector<list<NodeId> >&big_prev, list<vector<NodeId>> current , set<vector<NodeId>>& n_sps){

    if(prev.empty()){
        for(vector<NodeId> vet: current){
            if(vet.size()!=1)n_sps.insert(vet);
        }
        
    }

    while(!prev.empty()){
        NodeId node = prev.front();
        list<vector<NodeId>> tmp = list<vector<NodeId>>();
        for(vector<NodeId> vet: current){
            tmp.push_front(vet);
            tmp.front().push_back(node);
        }    
        tmp.push_front(vector<NodeId>({node}));
        prev.pop_front();
        all_paths(big_prev[node], big_prev, tmp, n_sps);
    }
}

void bc(CSRGraph& graph, double epsilon, double delta, double c) {
    int N = graph.N;
    int max1 = -1;
    int max2 = -1;
    uint64_t diameter;
    double r;
    uint64_t n_sp;
    uint64_t u,v;

    vector<list<NodeId> > prev = vector<list<NodeId> >(N);
    int* dist = (int*) calloc(1, sizeof(int)*N);
    int* sigma = (int*) calloc(1, sizeof(int)*N);

    std::vector<uint64_t>* access_vector = new std::vector<uint64_t>(N);
    std::vector<double>* bcs = new std::vector<double>(N);
    

    // set some values:
    for (int i=0; i<N; ++i){
        (*access_vector)[i] = i;
        (*bcs)[i]=0;
    }

    // using built-in random generator:
    std::random_shuffle ( access_vector->begin(), access_vector->end() );

    std::mt19937 generator(time(0));
    std::uniform_int_distribution<int> distribution(0, N-1);

    u = distribution(generator);
    n_sp = BFS(graph, u, u,prev, dist,sigma, &max1, &max2);
    diameter = max1 + max2;
    r = (c/pow(epsilon, 2))*(floor(log2(diameter-2)) + log(1/delta));
    // std::cerr<<r<<std::endl;
    // std::cerr << "r: " << r << std::endl;
    double normalize = 0;
    for(uint64_t i = 1; i <= r;i++){
        // std::cerr<<i<<std::endl;
        // if(i % 1000 == 0) std::cerr << i << std::endl;
        u = distribution(generator);
        v = distribution(generator);
        if(u==v){
            continue;
        }

        prev.clear();
        prev.resize(N);
        memset(dist,0,sizeof(int)*N);
        memset(sigma,0,sizeof(int)*N);
        sigma[u]=1;
        n_sp = BFS(graph, u, v, prev, dist,sigma, &max1, &max2);
        

        NodeId j=v;
        NodeId s=v;
        NodeId t=v;
        bool stop=false;
        if(prev[s].empty()){
            // std::cerr << "emptyyy!" << std::endl;
            stop = true;
        }
        while(!stop){
            vector<int> vetor(N,0);

            for(NodeId p:prev[s]){
                vetor[p]=sigma[p];
            }
            std::discrete_distribution<int> dist(vetor.begin(),vetor.end());
            NodeId z=dist(generator);
            if(z!=u){
                (*bcs)[z]=(*bcs)[z]+((double)1)/r;
                normalize+=((double)1)/r;
                s=t;
                t=z;
            } else {
                stop = true;
            }
        }
        // std::cerr<<"Ola de fora "<<std::endl;

    }

    
    int maxNode=-1;
    double val = -1;
    for(uint64_t i=0;i<N;i++){
        if((*bcs)[i]/normalize > val){
            maxNode = i;
            val = (*bcs)[i]/normalize;
        }
        std::cout<<(*bcs)[i]/normalize<<" ";
    }

    std::cout << std::endl << "normalize: "<< normalize << std::endl;

    return;
}
