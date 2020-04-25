#include <iostream>

int main(){
    uint64_t x = 0xdeadbeefcafebabe;
    int b = 12;
    uint64_t j = (x >> (64-b));
    printf("%lx\n", j);
    printf("%lx\n", x & ((uint64_t)-1 >> b));

    return 0;
}
