#include <iostream>
#include <fstream>
#include <string>

void usage() {
    std::cout << "placeholder" << std::endl;
}

int main(int argc, char** argv, char** envp) {
    std::ifstream input_file; 
    std::string line;
    std::string token;
    uint32_t v, e = 0;
    uint32_t *c_idx, *offset, e_ctr = 0, v_ctr = 0;
    size_t pos;

    if(argc < 2) {
        usage();
        return 1;
    }

    input_file.open(argv[1]);
    if(input_file.is_open()) {
        getline(input_file, line);
        size_t token_idx = line.find(' ');
    
        v = std::stoi(line.substr(0, token_idx));
        e = std::stoi(line.substr(token_idx));

        c_idx  = (uint32_t *) malloc(sizeof(uint32_t)*e);
        memset(c_idx, 0, e);
        offset = (uint32_t *) malloc(sizeof(uint32_t)*v);
        memset(offset, 0, v);

        while(getline(input_file, line)){
            offset[v_ctr++] = e_ctr;
            while((pos = line.find(' ')) != std::string::npos){ 
                token = line.substr(0, pos);
                c_idx[e_ctr++] = std::stoi(token);
                line.erase(0, pos+1);
            }
            c_idx[e_ctr++] = std::stoi(line);
        }
    }

    for(int i = 0; i < v; i++){
        std::cout << offset[i] << " ";
    }
    std::cout << std::endl;
    for(int i = 0; i < e; i++){
        std::cout << c_idx[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

// bitvec libraries
// SDSL-lite -> Succinct DataStructure Library

// [0, 1, 1, 1, 1, 1, 1]
// 1 2 3 4 5 6 
// 1 0 5

