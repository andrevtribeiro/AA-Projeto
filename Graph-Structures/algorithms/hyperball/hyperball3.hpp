#pragma once
#include "structures/graph/compressed_sparse_rows.hpp"
#include "lookup3.hpp"
#include <math.h>
#include <string.h>
#include <mutex>

#define ROT(x,k) (((x)<<(k)) | ((x)>>(32-(k))))
#define MAX(x,y) (x > y ? x : y)

void hashlittle2( 
  const void *key,       /* the key to hash */
  size_t      length,    /* length of the key */
  uint32_t   *pc,        /* IN: primary initval, OUT: primary hash */
  uint32_t   *pb);        /* IN: secondary initval, OUT: secondary hash */

// 0.7213/(1+(1.079)/m) -> m = 4096
double alpha =0.673 ;
int B = 4;
uint64_t m = 16;
uint64_t m_square = 256;
std::mutex mtx;

using namespace std;

uint64_t ro(uint64_t w) {

    // Keep shifting x by one until leftmost bit 
    // does not become 1. 
    int total_bits = sizeof(w)*8; 
    int res = 0; 
    while ( !(w & (((uint64_t)1) << (total_bits - 1)))) 
    { 
        w = (w << 1); 
        res++; 
    } 
    return res-B+1; 	
}


double size(uint64_t *counter,double *fraction) {
	double res = 0,res2=0;
	uint64_t v=0;
	for(uint64_t i = 0; i < m; i++) {
		//if(counter[i]==0)v++;
		res += 1/pow(2, counter[i]);
	}
    *fraction=res;
	res=1/res;
	res=alpha*m_square*res;
	/*cout<<"SIZE-> "<<res;
	if(res<=5/2*m){
		if(v!=0){
			res=m*log2((double)m/v);
			cout<<" if-> "<<res;
		}
	}
	cout<<endl;*/
	return res;
}


void add(uint64_t *counter, uint64_t v) {
	uint64_t x = 0, j, w;
	hashlittle2(&v, sizeof(v), (uint32_t*)&x, ((uint32_t*)&x) + 1);
	j = (x >> (64-B));
	w = x & ((uint64_t)-1 >> B);
	counter[j] = MAX(counter[j], ro(w));
}

void union_op(uint64_t *M, uint64_t *N,double *fraction){
	uint64_t new_value=0;
    for(uint64_t i = 0; i <  m; i++) {
    	//M[i] = MAX(M[i], N[i]);

        new_value = M[i] ^ ((M[i] ^ N[i]) & -(M[i] < N[i])); 
		if(new_value!=M[i]){
            *fraction+=1/pow(2,new_value)-1/pow(2,M[i]);
        }
		M[i]=new_value;
    }
} 
void initialize(uint64_t parameter){
	m=parameter;
	B=log2(parameter);
	m_square=pow(m,2);
	alpha=0.7213/(1+1.079/m);
	cout<<m<<" "<<B<<" "<<m_square<<" "<<alpha<<" "<<endl;
}
void hyperball(CSRGraph& graph,uint64_t parameter) {
    uint64_t& N = graph.N;
	uint64_t t;
	double s, s_new;
	uint64_t test_array[1039] = {0};
	initialize(parameter);

    uint64_t *c = (uint64_t*) calloc(1, sizeof(uint64_t)*N*m);
	uint64_t *c_copy = (uint64_t*) calloc(1, sizeof(uint64_t)*N*m);
	double *fractions = (double*) calloc(1, sizeof(double)*N);

	if(!c){
		cout << "error allocating counters" << endl;
		exit(0);
	}

	#pragma omp parallel for
	for(uint64_t v = 0; v < N; v++) {
		add(&c[v*m], v);
	}

	

	s = 0;
	s_new = 0;
	#pragma omp parallel for reduction(+ : s_new)
	for(uint64_t v = 0; v < N; v++){
		s_new+=size(&c[v*m],&fractions[v]);
	}
	
	memcpy(c_copy, c, sizeof(uint64_t)*N*m);


	//cout << s_new << endl;

	t = 0;
	do {
		s = s_new;
        s_new=0;
		
		#pragma omp parallel for reduction(+ : s_new)
		for(uint64_t v = 0; v < N; v++){
			bool control=false;
			double size_node;
			// memcpy(c, c_copy, sizeof((uint64_t*)*N)m
			for(NodeId w : graph.GetNeighboors((NodeId)v)) {
				union_op(&c_copy[v*m], &c[w*m],&fractions[v]);
			}
			s_new += alpha*m_square*(1/fractions[v]);
		}
		
		memcpy(c, c_copy, sizeof(uint64_t)*N*m);

		/*#pragma omp parallel for reduction(+ : s_new)
		for(uint64_t v = 0; v < N; v++){
			s_new += size(&c[v*m]);
		}*/
		
	
		cout << s_new<<" "<<s_new - s << endl;
		test_array[++t] = s_new - s;
	} while(abs(s_new-s)>0.00001);
	
	double apl = 0;
	uint64_t sum = 0;
	for(uint64_t i = 1; i < t; i++){
		apl += (i)*test_array[i];
		sum += test_array[i];
		i++;
	}
	apl = apl/sum;

	cout << apl << endl;
	cout << t-1 << endl;
}


// 12.0191
// 37