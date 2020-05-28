#pragma once
#include "structures/graph/compressed_sparse_rows.hpp"
#include <random>
#include <math.h>
#include <algorithm>    // std::random_shuffle
#include <string.h>
#include <set> 

double _epsilon;
double _delta;

uint64_t BFS(CSRGraph& graph, NodeId v, NodeId t, vector<list<NodeId> >& prev, int* dist, int* max1, int* max2,bool undirected){
    uint64_t iterations = 0;
    uint64_t source = v;
    vector<NodeId>* neighbours = new vector<NodeId>(graph.GetNeighboors(v));
    vector<NodeId>* rev_neighbours;
  
    list<int> queue; 
  
    queue.push_back(v); 
    if(undirected){       
        rev_neighbours = new vector<NodeId>(graph.GetReverseNeighboors(v));
        neighbours->insert(neighbours->end(), rev_neighbours->begin(), rev_neighbours->end());
    }
    dist[v] = 0;
    while(!queue.empty() && (undirected || dist[t]==0)) { 
        
        // Dequeue a vertex from queue and print it 
        v = queue.front(); 
        queue.pop_front(); 
        
        // Get all adjacent vertices of the dequeued 
        // vertex s. If a adjacent has not been visited,  
        // then mark it visited and enqueue it 
        delete neighbours;

        neighbours = new vector<NodeId>(graph.GetNeighboors(v));
        if(undirected){       
            delete rev_neighbours;
            rev_neighbours = new vector<NodeId>(graph.GetReverseNeighboors(v));
            neighbours->insert(neighbours->end(), rev_neighbours->begin(), rev_neighbours->end());
        }

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
    _epsilon = epsilon;
    _delta = delta;
    int N = graph.N;
    int max1 = -1;
    int max2 = -1;
    uint64_t diameter;
    double r;
    uint64_t n_sp;
    uint64_t u,v;

    vector<list<NodeId> > prev = vector<list<NodeId> >(N);
    int* dist = (int*) calloc(1, sizeof(int)*N);

    set<vector<NodeId>> sps;
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
    n_sp = BFS(graph, u, -1,prev, dist, &max1, &max2,true);

    diameter = max1 + max2;

    r = (c/pow(epsilon, 2))*(floor(log2(diameter-2)) + log(1/delta));
    
    vector<list<vector<NodeId>>> Suv(N);

    std::cerr << "r: " << r << std::endl;
    for(uint64_t i = 1; i <= r; i++){

        if(i % 1000 == 0) std::cerr << i << std::endl;
        u = distribution(generator);
        v = distribution(generator);
    
        if(u==v){
            continue;
        }

        prev.clear();
        prev.resize(N);
        memset(dist,0,sizeof(int)*N);
        n_sp = BFS(graph, u, v, prev, dist, &max1, &max2,false);

        list<vector<NodeId>> vec = list<vector<NodeId>>();

        vec.push_back(vector<NodeId>({v}));
        sps.clear();

        all_paths(prev[v], prev, vec, sps);

        Suv.clear();
        Suv.resize(N);

        for(vector<NodeId> p: sps){
            Suv[p[0]].push_front(p);
        }

        NodeId j=v;
        NodeId s=v;
        NodeId t=v;
        
        while(t!=u){
            vector<int> vetor(N,0);
            int c=0;
            for(vector<NodeId> ve : Suv[s]){
                bool flag=false;
                for(int i=0; i<(ve.size()-1);i++){          
                    if(flag){
                        vetor[ve[i]]=Suv[ve[i]].size();
                        c++;
                    }
                    if(ve[i]==t)flag=true;
                }
            }
            if(!c){
                break;
            }
            std::discrete_distribution<int> dist(vetor.begin(),vetor.end());
            NodeId z=dist(generator);
            if(z!=u){   
                (*bcs)[z]=(*bcs)[z]+((double)1)/r;
                s=t;
                t=z;
            }
        }
    }

    double a = 0;

    for(uint64_t i=0;i<N;i++){
        a +=(*bcs)[i];
        std::cout<<(*bcs)[i]<<" ";
    }

    std::cout << std::endl << "sum: " << a << std::endl;

    return;
}
