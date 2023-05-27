// #include "node.h"
#include "skiplist.hpp"
#include "fileread.hpp"
// #include "fileread.h"
// #include "config.h"
#include <string>
#include <iostream>
#include <iostream>
#include <ctime>
// #define VALIDATE
// void multiprocessvalidate(SkipList<string,int> &,string ,string);
using namespace std;
string filename = "./data/operations.txt";
string outputfile = "./data/skiplist.log";
extern SkipList<string,int> skiplist;
// void multiprocessvalidate(SkipList<string,int> &skiplist,string filename,string outputfile);
int main(){
    clock_t start,end;
    // SkipList<string,int> skiplist(32);
    start = clock();
    // validate(skiplist,filename);
    multiprocessvalidate(filename,outputfile);
    end = clock();
    // return 0;
    cout << "Time for 32 index " << end - start << endl;
    return 0;
}