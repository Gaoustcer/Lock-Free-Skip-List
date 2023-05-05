#include <vector>
#include <functional>
#include <string>
#include <iostream>
using namespace std;
// template<typename key,typename values>
int main(){
    hash<string> func;
    string str("hello world");
    size_t n = func(str);
    cout << "n " << n << endl;
}