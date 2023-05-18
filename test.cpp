// #include "node.h"
#include "skiplist.h"
// #include "fileread.h"
#include "config.h"
#include <string>
#include <iostream>
#include <iostream>
#include <ctime>
// #define VALIDATE
using namespace std;
string filename = "./data/operations.txt";
string outputfile = "./data/skiplist.log";
void multiprocessvalidate(SkipList<string,int> &skiplist,string filename,string outputfile);
int main(){
    clock_t start,end;
    SkipList<string,int> skiplist(32);
    start = clock();
    // validate(skiplist,filename);
    multiprocessvalidate(skiplist,filename,outputfile);
    end = clock();
    return 0;
    cout << "Time for 32 index " << end - start << endl;
    SkipList<string,int> ski(0);
    start = clock();
    multiprocessvalidate(ski,filename,outputfile);
    end = clock();
    cout << "Time for 0 index " << end - start << endl; 
    return 0;
}