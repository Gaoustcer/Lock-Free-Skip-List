#include "config.hpp"
#include <cstdlib>
int getlevel(int maxlevel,int seed){
    srand(seed);
    for(int i = 0;i < maxlevel;i++){
        if(rand()/(double)RAND_MAX > 0.5){
            return i;
        }
    }
    return maxlevel;
}