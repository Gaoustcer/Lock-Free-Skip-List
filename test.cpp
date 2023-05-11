// #include "node.h"
#include "skiplist.h"
#include "fileread.h"
#include "config.h"
#include <string>
#include <iostream>
#include <iostream>
using namespace std;
string filename = "./data/operations.txt";
int main(){
    // validate();
    SkipList<string,int> skiplist(3);
    validate(skiplist,filename);
    // cout << skiplist.depth() << endl;
    // cout << skiplist.getlength() << endl;
    // string str("hello");
    // skiplist.insert(str,10);
    // auto value = skiplist.find(str);
    // cout << "find value "  << value.value() << " " << skiplist.getlength()  << endl;
    // skiplist.deletekey(str);
    // cout << skiplist.getlength() << endl;
    return 0;
}