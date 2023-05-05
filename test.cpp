// #include "node.h"
#include "skiplist.h"
#include <string>
#include <iostream>
using namespace std;
int main(){
    SkipList<string,int> skiplist;
    cout << skiplist.depth() << endl;
    string str("hello");
    skiplist.insert(str,10);
    auto value = skiplist.find(str);
    cout << "find value "  << value.value() << endl;
    return 0;
}