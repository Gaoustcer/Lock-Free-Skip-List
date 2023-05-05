#ifndef CONFIG
#define CONFIG
#define MAXHEIGHT 32 // Max Index level
#include <cstdlib>
using namespace std;
int level(int seed = 1024){
    srand(seed);
    for(int i = 0;i < MAXHEIGHT;i++){
        if(rand()/(double)RAND_MAX > 0.5){
            return i;
        }
    }
    return MAXHEIGHT;
}

#endif