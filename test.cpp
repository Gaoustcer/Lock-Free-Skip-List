// #include "node.h"
#include "skiplist.hpp"
#include "fileread.hpp"
// #include "fileread.h"
// #include "config.h"
#include <string>
#include <iostream>
#include <iostream>
#include <ctime>
#include <vector> 
// #define VALIDATE
// void multiprocessvalidate(SkipList<string,int> &,string ,string);
using namespace std;
string filename = "./data/operations.txt";
string outputfile = "./data/insertdense/skiplist.log";
SkipList<string,int> skiplist(32);
// void multiprocessvalidate(SkipList<string,int> &skiplist,string filename,string outputfile);
int main(){
    clock_t start,end;
    vector<string> modifiedoperations;
    for(int i = 0;i < 4;i++){
        modifiedoperations.push_back("./data/insertdense/modified_operation_"+to_string(i)+".txt");
    }

    string searchoperations("./data/insertdense/searchoperations.txt");
    
    // SkipList<string,int> skiplist(32);
    start = clock();
    cout << "enter the insert\n";
    parallelinsert(modifiedoperations,searchoperations,outputfile);
    // validate(skiplist,filename);
    // multiprocessvalidate(filename,outputfile);
    end = clock();
    // return 0;
    cout << "Time for 32 index " << end - start << endl;
    return 0;
}