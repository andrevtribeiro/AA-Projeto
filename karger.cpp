#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h>       /* time */
#include <string.h>

uint32_t **matrix;
uint32_t v, e;

void usage() {
    std::cout << "placeholder" << std::endl;
}

void parse_input(char* filename){
    size_t pos;
    std::ifstream input_file; 
    std::string line;
    std::string token;
    uint32_t v_ctr = 0;

    input_file.open(filename);
    
    if(input_file.is_open()) {
        getline(input_file, line);
        size_t token_idx = line.find(' ');
    
        v = std::stoi(line.substr(0, token_idx));
        e = std::stoi(line.substr(token_idx));

        matrix = (uint32_t **) calloc(1, sizeof(uint32_t*)*v);
        for (uint32_t i = 0; i < v; i++){
            matrix[i] = (uint32_t *) calloc(1, sizeof(uint32_t)*v);
            if(!matrix[i])
                std::cerr << "error allocating matrix" << std::endl;
        }
        
        while(getline(input_file, line)){
            while((pos = line.find(' ')) != std::string::npos){ 
                token = line.substr(0, pos);
                matrix[v_ctr][std::stoi(token)] = 1;
                line.erase(0, pos+1);
            }
            matrix[v_ctr++][std::stoi(line)] = 1;
        }
    }

}

void compress(uint32_t **m, int v1, int v2){

    m[v1][v2] = 0;
    m[v2][v1] = 0;

    for(int i = 0; i < v; i++){
        m[v2][i] += m[v1][i];
    }
    for(int i = 0; i < v; i++){
        m[i][v2] += m[i][v1];
    }
    for(int i = 0; i < v; i++){
        m[v1][i] = 0;
    }
    for(int i = 0; i < v; i++){
        m[i][v1] = 0;
    }
}

void karger(int iter){
    srand(time(NULL));
    int rand_i, rand_j;
    uint32_t ** loop_matrix = (uint32_t **) malloc(sizeof(uint32_t*)*v);

    for (uint32_t i = 0; i < v; i++){
        loop_matrix[i] = (uint32_t *) malloc(sizeof(uint32_t)*v);
        if(!loop_matrix[i])
            std::cerr << "error allocating matrix" << std::endl;
    }
    
    for(int it = 0; it < iter; it++){
        for (uint32_t i = 0; i < v; i++)
            memcpy(loop_matrix[i], matrix[i], v * sizeof(uint32_t));
        // v-1??
        for(int c = 0; c < v-2; c++){
            do{
                rand_i = rand() % v;
                rand_j = rand() % v;
            } while(!loop_matrix[rand_i][rand_j]);
            // printf("%d %d\n", rand_i, rand_j);

            compress(loop_matrix, rand_i, rand_j);
        }
        putchar('\n');
        for(int i = 0; i < v; i++){
            for(int j = 0; j < v; j++)
                std::cout << loop_matrix[i][j] << " ";
            std::cout << std::endl;
        }
    }

    
}

int main(int argc, char** argv, char** envp) {
    

    if(argc < 2) {
        usage();
        return 1;
    }

    parse_input(argv[1]);

    for(int i = 0; i < v; i++){
        for(int j = 0; j < v; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
    karger(v);

    return 0;
}

