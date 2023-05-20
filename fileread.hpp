#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <ctime>
#include "skiplist.hpp"
#define VALIDATE 
using namespace std;
#define SEARCHPROCESS 2
class operations{
public:
    string searchkey;
    int value;
    operations(string _key,int _value):value(_value),searchkey(_key){}
};
void multiprocessvalidate(SkipList<string,int> &skiplist,string filename,string outputfile);

void validate(SkipList<string,int> &skiplist,string filename = "./data/operations.txt");